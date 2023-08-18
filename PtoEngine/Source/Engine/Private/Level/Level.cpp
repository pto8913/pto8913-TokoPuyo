
#include "Level/Level.h"

#include "Math/Math.h"

#include "Object/Ground/GroundBase.h"
#include "Object/Event/EventBase.h"
#include "Object/Item/ItemBase.h"
#include "Object/Character/CharacterBase.h"
#include "Object/Character/Player.h"

#include "GameSettings.h"

#include "Engine/World.h"
#include "Controller/PlayerController.h"

#include "GameMode/GameModeBase.h"
#include "GameState/GameStateBase.h"
#include "Controller/PlayerController.h"

#include "UI/HUD.h"

#if _DEBUG
#include <format>
#endif

// ------------------------------------------------------------------------------------------------------------
// Level
// ------------------------------------------------------------------------------------------------------------
Level::Level(DirectX11& dx)
	: pDX(&dx)
{
	SetTickEnabled(true);
}
Level::~Level()
{
	pDX = nullptr;

	pOwningWorld.reset();
	pOwningWorld = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level::SetWorld(std::shared_ptr<World> in)
{
	pOwningWorld = in;
}
std::shared_ptr<World> Level::GetWorld()
{
	return pOwningWorld;
}

// ------------------------------------------------------------------------------------------------------------
// Level 2D
// ------------------------------------------------------------------------------------------------------------
Level2D::Level2D(DirectX11& dx)
	: Level(dx)
{
	pWorldTimer = std::make_unique<WorldTimer>();
}
Level2D::~Level2D()
{
	Clear();

	pWorldTimer.reset();
	pWorldTimer = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level2D::BeginPlay(DirectX11& dx)
{
	GetWorld()->GetGameMode()->SetOuter(shared_from_this());
	GetWorld()->GetGameState()->SetOuter(shared_from_this());
	GetWorld()->GetPlayerController()->SetOuter(shared_from_this());

	Generate(dx);
}

void Level2D::Generate(DirectX11& dx)
{
	Disactivate();

	GenerateGroundLayer();
	GenerateEventLayer();
	GenerateItemLayer();
	GenerateBuildingLayer();
	GenerateCharacterLayer();
	GenerateEffectLayer();

	Activate();
}
void Level2D::SetGroundType(const EGroundType& inGroundType)
{
	mGroundType = inGroundType;
}

void Level2D::Init(const UINT16& x, const UINT16& y)
{
	height = y;
	width = x;

	TArray<std::shared_ptr<GroundBase>> groundRows(x, nullptr);
	GroundLayer.Init(y, groundRows);

	TArray<std::shared_ptr<EventBase>> eventRows(x, nullptr);
	EventLayer.Init(y, eventRows);

	TArray<std::shared_ptr<ItemBase>> itemRows(x, nullptr);
	ItemLayer.Init(y, itemRows);

	TArray<std::shared_ptr<CharacterBase>> characterRows(x, nullptr);
	CharacterLayer.Init(y, characterRows);
}

void Level2D::Activate()
{
#if _DEBUG
	OutputDebugStringA(std::format("start x, y {} {}\n", startX, startY).c_str());
#endif
	const auto& pPlayerController = GetWorld()->GetPlayerController();
	pPlayerController->GetHUD()->ResetMap(this);

	MoveCenter(startX, startY);

	bInitialized = true;
}
void Level2D::Disactivate()
{
	bInitialized = false;
}

bool Level2D::MoveCenter(const int& x, const int& y)
{
#if _DEBUG
	//OutputDebugStringA(std::format("x, y {} {}\n", currX, currY).c_str());
#endif

	const int nextX = Math::Clamp(currX + x, 0, (int)width);
	const int nextY = Math::Clamp(currY + y, 0, (int)height);
	if (IsInWorld(nextX, nextY))
	{
		/* Character */
		const auto& nextCharaData = CharacterLayer[nextY][nextX];
		if (nextCharaData != nullptr)
		{
			return false;
		}

		/* Event */
		{
			const auto& nextEventData = EventLayer[nextY][nextX];
			if (nextEventData != nullptr)
			{
				const EEventId& eventType = nextEventData->GetEventType();
				if (eventType == EEventId::Block)
				{
					return false;
				}
			}

			const auto& currEventData = EventLayer[currY][currX];
			if (currEventData != nullptr)
			{
				currEventData->LeaveVolume(currX, currY);
			}

			if (nextEventData != nullptr)
			{
				nextEventData->EnterVolume(nextX, nextY);
			}
		}

		/* GroundLayer */
		if (GroundLayer[nextY][nextX] != nullptr)
		{
			CharacterLayer[currY][currX] = nullptr;

			currX = nextX;
			currY = nextY;

			screenLeftX = currX - GameSettings::GAMESCREEN_CENTER_X - 1;
			screenLeftY = currY - GameSettings::GAMESCREEN_CENTER_Y - 1;
			screenRightX = currX + GameSettings::GAMESCREEN_CENTER_X + 2;
			screenRightY = currY + GameSettings::GAMESCREEN_CENTER_Y + 2;

			const auto& pPlayerController = GetWorld()->GetPlayerController();
			CharacterLayer[currY][currX] = pPlayerController->GetPlayer();

			pPlayerController->GetHUD()->UpdateMap(this);
			
			return true;
		}
	}
	return false;
}

void Level2D::Tick(DirectX11& dx, float deltaTime)
{
	if (!bInitialized) return;

	for (int y = screenLeftY; y < screenRightY; ++y)
	{
		for (int x = screenLeftX; x < screenRightX; ++x)
		{
			if (IsInWorld(x, y))
			{
				auto& pGroundSprite = GroundLayer[y][x];
				if (pGroundSprite != nullptr)
				{
					pGroundSprite->DoFrame(dx, deltaTime);
					pGroundSprite->SetOffset(WorldToScreen(x, y, pGroundSprite->GetSize()));
				}

				auto& pEventSprite = EventLayer[y][x];
				if (pEventSprite != nullptr)
				{
					pEventSprite->DoFrame(dx, deltaTime);
					pEventSprite->SetOffset(WorldToScreen(x, y, pEventSprite->GetSize()));
				}

				auto& pCharacterSprite = CharacterLayer[y][x];
				if (pCharacterSprite != nullptr)
				{
					pCharacterSprite->DoFrame(dx, deltaTime);
					if (pCharacterSprite->GetCharacterType() == ECharacterId::Player)
					{
						pCharacterSprite->SetOffset(WorldToScreen(currX, currY, pCharacterSprite->GetSize()));
					}
					else
					{
						pCharacterSprite->SetOffset(WorldToScreen(x, y, pCharacterSprite->GetSize()));
					}
				}
			}
		}
	}

	auto pPlayerController = GetWorld()->GetPlayerController();
	if (pPlayerController)
	{
		auto pPlayer = pPlayerController->GetPlayer();
		pPlayer->SetOffset(WorldToScreen(currX, currY, pPlayer->GetSize()));
	}
}

void Level2D::Clear()
{
	Clear(GroundLayer);
	Clear(EventLayer);
	Clear(ItemLayer);
	Clear(CharacterLayer);

	width = 0;
	height = 0;

	startX = 0;
	startY = 0;

	currX = 0;
	currY = 0;
	screenLeftX = 0;
	screenLeftY = 0;
	screenRightX = 0;
	screenRightY = 0;
}

// --------------------------
// Main : Utils
// --------------------------
UINT16 Level2D::GetSize() const noexcept
{
	return (UINT16)GroundLayer.Size();
}
const UINT16& Level2D::GetWidth() const noexcept
{
	return width;
}
const UINT16& Level2D::GetHeight() const noexcept
{
	return height;
}

bool Level2D::IsInScreen(const int& x, const int& y) const noexcept
{
	return (x >= screenLeftX && x < screenRightX) && (y >= screenLeftY && y < screenRightY);
}
bool Level2D::IsInWorld(const int& x, const int& y) const noexcept
{
	return (x >= 0 && x < width) && (y >= 0 && y < height);
}
bool Level2D::IsEmptyGround(const int& x, const int& y) const noexcept
{
	if (IsInWorld(x, y))
	{
		if (GroundLayer[y][x] != nullptr)
		{
			switch (ConvertToGroundTile(GroundLayer[y][x]->GetGroundType()))
			{
			case EGroundTile::Path:
			case EGroundTile::Room:
				break;
			default:
				break;
			}
		}

		if (EventLayer[y][x])
		{
			switch (EventLayer[y][x]->GetEventType())
			{
			case EEventId::Enter:
			case EEventId::Transparent:
				break;
			default:
				break;
			}
		}

		if (ItemLayer[y][x])
		{
			switch (ItemLayer[y][x]->GetItemType())
			{
			case EItemId::None:
				break;
			default:
				return false;
			}
		}
		if (CharacterLayer[y][x])
		{
			switch (CharacterLayer[y][x]->GetCharacterType())
			{
			case ECharacterId::None:
				break;
			default:
				return false;
			}
		}
	}

	return true;
}

void Level2D::SetGroundLayerID(const EGroundId& groundType, const UINT16& x, const UINT16& y)
{
	if (IsInWorld(x, y))
	{
		auto& sprite = GroundLayer[y][x];
		if (sprite != nullptr)
		{
			sprite->SetGroundType(groundType);
		}
		else
		{
			sprite = GetWorld()->SpawnActor<GroundBase>(*pDX, groundType);
		}
	}
}
void Level2D::SetGroundLayerID(const EGroundId& groundType, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal, INT16 inConstantXY2)
{
	if (inConstantXY2 > -1)
	{
		if (bConstantHorizontal)
		{
			if (!IsInWorld(inMinXY, inConstantXY) || !IsInWorld(inMinXY, inConstantXY2) ||
				!IsInWorld(inMaxXY, inConstantXY) || !IsInWorld(inMaxXY, inConstantXY2))
			{
				return;
			}
		}
		else
		{
			if (!IsInWorld(inConstantXY, inMinXY) || !IsInWorld(inConstantXY2, inMinXY) ||
				!IsInWorld(inConstantXY, inMaxXY) || !IsInWorld(inConstantXY2, inMaxXY))
			{
				return;
			}
		}
	}
	else
	{
		if (bConstantHorizontal)
		{
			if (!IsInWorld(inMinXY, inConstantXY) || !IsInWorld(inMaxXY, inConstantXY))
			{
				return;
			}
		}
		else
		{
			if (!IsInWorld(inConstantXY, inMinXY) || !IsInWorld(inConstantXY, inMaxXY))
			{
				return;
			}
		}
	}
	for (UINT16 i = inMinXY; i <= inMaxXY; ++i)
	{
		if (bConstantHorizontal)
		{
			SetGroundLayerID(groundType, i, inConstantXY);
		}
		else
		{
			SetGroundLayerID(groundType, inConstantXY, i);
		}
		if (inConstantXY2 > -1)
		{
			if (bConstantHorizontal)
			{
				SetGroundLayerID(groundType, i, inConstantXY2);
			}
			else
			{
				SetGroundLayerID(groundType, inConstantXY2, i);
			}
		}
	}
}
const std::shared_ptr<GroundBase>& Level2D::GetGroundLayerID(const int& x, const int& y) const
{
	return GroundLayer[y][x];
}

void Level2D::SetEventLayerID(const EEventId& eventType, const UINT16& x, const UINT16& y)
{
	SetEventLayerID(GetWorld()->SpawnActor<EventBase>(*pDX, eventType), x, y);
}
void Level2D::SetEventLayerID(std::shared_ptr<EventBase>& inEventData, const UINT16& x, const UINT16& y)
{
	if (IsInWorld(x, y))
	{
		const auto& eventType = inEventData->GetEventType();
		if (eventType == EEventId::Enter)
		{
			startX = x;
			startY = y;
		}

		auto& sprite = EventLayer[y][x];
		if (sprite != nullptr)
		{
			sprite->SetEventType(eventType);
		}
		else
		{
			sprite = inEventData;
		}
	}
}
void Level2D::SetEventLayerID(std::shared_ptr<EventBase>&& inEventData, const UINT16& x, const UINT16& y)
{
	if (IsInWorld(x, y))
	{
		const auto& eventType = inEventData->GetEventType();
		if (eventType == EEventId::Enter)
		{
			startX = x;
			startY = y;
		}

		auto& sprite = EventLayer[y][x];
		if (sprite != nullptr)
		{
			sprite->SetEventType(eventType);
		}
		else
		{
			sprite = std::move(inEventData);
		}
	}
}
const std::shared_ptr<EventBase>& Level2D::GetEventLayerID(const int& x, const int& y) const
{
	return EventLayer[y][x];
}

void Level2D::SetItemLayerID(const EItemId& itemType, const UINT16& x, const UINT16& y)
{
	if (IsInWorld(x, y))
	{
		auto& sprite = ItemLayer[y][x];
		if (sprite != nullptr)
		{
			sprite->SetItemType(itemType);
		}
		else
		{
			sprite = GetWorld()->SpawnActor<ItemBase>(*pDX, itemType);
		}
	}
}
const std::shared_ptr<ItemBase>& Level2D::GetItemLayerID(const int& x, const int& y) const
{
	return ItemLayer[y][x];
}

void Level2D::SetCharacterLayerID(const ECharacterId& characterType, const UINT16& x, const UINT16& y)
{
	if (IsInWorld(x, y))
	{
		auto& sprite = CharacterLayer[y][x];
		if (sprite != nullptr)
		{
			sprite->SetCharacterType(characterType);
		}
		else
		{
			sprite = GetWorld()->SpawnActor<CharacterBase>(*pDX, characterType);
		}
	}
}
const std::shared_ptr<CharacterBase>& Level2D::GetCharacterLayerID(const int& x, const int& y) const
{
	return CharacterLayer[y][x];
}

DirectX::XMFLOAT2 Level2D::WorldToScreen(const int& x, const int& y, const DirectX::XMFLOAT2& size)
{
	return {
		GameSettings::GAMESCREEN_LEFT_TOP.x + size.x * (x - screenLeftX),
		GameSettings::GAMESCREEN_LEFT_TOP.y * 2.f + 64.f + size.y * (y - screenLeftY)
	};
}

// --------------------------
// Main : Debug
// --------------------------
void Level2D::ShowTiles()
{
	OutputDebugStringA("-------- Show Tiles -------\n");
	for (int y = 0; y < height; ++y)
	{
		std::string str;
		for (int x = 0; x < width; ++x)
		{
			const auto& t = GroundLayer[y][x];
			if (t != nullptr)
			{
				const auto& e = EventLayer[y][x];
				if (e != nullptr)
				{
					switch (e->GetEventType())
					{
					case EEventId::Exit:
						str += "E";
						break;
					case EEventId::Enter:
						str += "S";
						break;
					default:
						break;
					}
					continue;
				}
				switch (ConvertToGroundTile(t->GetGroundType()))
				{
				case EGroundTile::Room:
					str += "R";
					break;
				case EGroundTile::Path:
					str += "P";
					break;
				case EGroundTile::Wall:
					str += " ";
					break;
				default:
					str += "Is not allowed None";
					break;
				}
			}
			else
			{
				str += " ";
			}
		}
		OutputDebugStringA((str + "\n").c_str());
	}
	OutputDebugStringA("-------- Show GroundLayer -------\n");
	OutputDebugStringA("\n\n");
}
