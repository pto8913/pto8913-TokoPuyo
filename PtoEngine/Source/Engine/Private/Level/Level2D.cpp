
#include "Level/Level2D.h"
#include "Level/Layer/Layer.h"

#include "Math/Math.h"

#include "Object/Ground/GroundBase.h"
#include "Object/Event/EventBase.h"
#include "Object/Item/ItemBase.h"
#include "Object/Character/CharacterBase.h"
#include "Object/Character/Player.h"

#include "GameSettings.h"

#include "Engine/World.h"

#if _DEBUG
#include <format>
#endif

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
	Generate(dx);
}
void Level2D::Generate(DirectX11& dx)
{
	Deactivate();

	GenerateGroundLayer();
	GenerateEventLayer();
	GenerateItemLayer();
	GenerateBuildingLayer();
	GenerateCharacterLayer();
	GenerateEffectLayer();

	Activate();
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
					pGroundSprite->Tick(dx, deltaTime);
				}

				auto& pEventSprite = EventLayer[y][x];
				if (pEventSprite != nullptr)
				{
					pEventSprite->Tick(dx, deltaTime);
				}

				auto& pCharacterSprite = CharacterLayer[y][x];
				if (pCharacterSprite != nullptr)
				{
					pCharacterSprite->Tick(dx, deltaTime);
				}
			}
		}
	}
}

void Level2D::SetGroundType(const EGroundType& inGroundType)
{
	mGroundType = inGroundType;
}

void Level2D::Init(const int& x, const int& y)
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

	Character2Layer.Init(y, characterRows);
}

void Level2D::Activate()
{
#if _DEBUG
	OutputDebugStringA(std::format("start {}\n", mStartPosition.ToString()).c_str());
#endif
	MoveCenter(mStartPosition.x, mStartPosition.y);
	GetWorld()->GetPlayer()->SetActorLocation(FVector(GetCenter().x, GetCenter().y, 0));

	bInitialized = true;
}
void Level2D::Deactivate()
{
	bInitialized = false;
}

bool Level2D::MoveCenter(const int& x, const int& y)
{
#if _DEBUG
	//OutputDebugStringA(std::format("x, y {} {}\n", mCenter.x, mCenter.y).c_str());
#endif

	const int nextX = Math::Clamp((int)mCenter.x + x, 0, (int)width);
	const int nextY = Math::Clamp((int)mCenter.y + y, 0, (int)height);
	if (IsInWorld(nextX, nextY))
	{
		/* GroundLayer */
		if (GroundLayer[nextY][nextX] != nullptr)
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

				const auto& currEventData = EventLayer[mCenter.y][mCenter.x];
				if (currEventData != nullptr)
				{
					currEventData->LeaveVolume(mCenter.x, mCenter.y);
				}

				if (nextEventData != nullptr)
				{
					nextEventData->EnterVolume(nextX, nextY);
				}
			}

			Character2Layer[mCenter.y][mCenter.x] = nullptr;

			mCenter.x = nextX;
			mCenter.y = nextY;

			screenLeftX = mCenter.x - GameSettings::GAMESCREEN_CENTER_X - 1;
			screenLeftY = mCenter.y - GameSettings::GAMESCREEN_CENTER_Y - 1;
			screenRightX = mCenter.x + GameSettings::GAMESCREEN_CENTER_X + 2;
			screenRightY = mCenter.y + GameSettings::GAMESCREEN_CENTER_Y + 2;

			auto& pPlayer = GetWorld()->GetPlayer();
			Character2Layer[mCenter.y][mCenter.x] = pPlayer;

			UpdateSpriteOffset();

			return true;
		}
	}
	return false;
}
void Level2D::UpdateSpriteOffset()
{
	for (int y = screenLeftY; y < screenRightY; ++y)
	{
		for (int x = screenLeftX; x < screenRightX; ++x)
		{
			if (IsInWorld(x, y))
			{
				auto& pGroundSprite = GroundLayer[y][x];
				if (pGroundSprite != nullptr)
				{
					pGroundSprite->SetOffset(WorldToScreen(x, y, pGroundSprite->GetActorScale().To2D()));
				}

				auto& pEventSprite = EventLayer[y][x];
				if (pEventSprite != nullptr)
				{
					pEventSprite->SetOffset(WorldToScreen(x, y, pEventSprite->GetActorScale().To2D()));
				}

				auto& pCharacterSprite = CharacterLayer[y][x];
				if (pCharacterSprite != nullptr)
				{
					pCharacterSprite->SetOffset(WorldToScreen(x, y, pCharacterSprite->GetActorScale().To2D()));
				}
			}
		}
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

	mStartPosition = FVector();

	mCenter.x = 0;
	mCenter.y = 0;
	screenLeftX = 0;
	screenLeftY = 0;
	screenRightX = 0;
	screenRightY = 0;
}

// --------------------------
// Main : Utils
// --------------------------
const int& Level2D::GetWidth() const noexcept
{
	return width;
}
const int& Level2D::GetHeight() const noexcept
{
	return height;
}
FVector2D Level2D::GetCenter() const noexcept
{
	return mCenter;
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
bool Level2D::CanMove(const int& x, const int& y) const noexcept
{
	const auto nextX = mCenter.x + x;
	const auto nextY = mCenter.y + y;
	if (IsInWorld(nextX, nextY))
	{
		if (GroundLayer[nextY][nextX] != nullptr)
		{
			switch (ConvertToGroundTile(GroundLayer[nextY][nextX]->GetGroundType()))
			{
			case EGroundTile::Path:
			case EGroundTile::Room:
				break;
			default:
				return false;
			}

			if (EventLayer[nextY][nextX] != nullptr)
			{
				switch (EventLayer[nextY][nextX]->GetEventType())
				{
				case EEventId::Block:
					return false;
				default:
					break;
				}
			}

			if (ItemLayer[nextY][nextX] != nullptr)
			{
				switch (ItemLayer[nextY][nextX]->GetItemType())
				{
				default:
					break;
				}
			}
			if (CharacterLayer[nextY][nextX] != nullptr)
			{
				switch (CharacterLayer[nextY][nextX]->GetCharacterType())
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
	return false;
}
DirectX::XMFLOAT2 Level2D::WorldToScreen(const int& x, const int& y, const FVector2D& size)
{
	return {
		GameSettings::GAMESCREEN_LEFT_TOP.x + size.x * (x - screenLeftX),
		GameSettings::GAMESCREEN_LEFT_TOP.y * 2.f + 64.f + size.y * (y - screenLeftY)
	};
}

// --------------------------
// Main : Utils : Ground
// --------------------------
void Level2D::SetGroundLayerID(const EGroundId& groundType, const FVector2D& pos)
{
	if (IsInWorld(pos.x, pos.y))
	{
		auto& sprite = GroundLayer[pos.y][pos.x];
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
			SetGroundLayerID(groundType, FVector2D(i, inConstantXY));
		}
		else
		{
			SetGroundLayerID(groundType, FVector2D(inConstantXY, i));
		}
		if (inConstantXY2 > -1)
		{
			if (bConstantHorizontal)
			{
				SetGroundLayerID(groundType, FVector2D(i, inConstantXY2));
			}
			else
			{
				SetGroundLayerID(groundType, FVector2D(inConstantXY2, i));
			}
		}
	}
}
const std::shared_ptr<GroundBase>& Level2D::GetGroundLayerID(const int& x, const int& y) const
{
	return GroundLayer[y][x];
}

// --------------------------
// Main : Utils : Event
// --------------------------
void Level2D::SetEventLayerID(const EEventId& eventType, const FVector2D& pos)
{
	SetEventLayerID(GetWorld()->SpawnActor<EventBase>(*pDX, eventType), pos);
}
void Level2D::SetEventLayerID(std::shared_ptr<EventBase>& inEventData, const FVector2D& pos)
{
	if (IsInWorld(pos.x, pos.y))
	{
		const auto& eventType = inEventData->GetEventType();
		auto& sprite = EventLayer[pos.y][pos.x];
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
void Level2D::SetEventLayerID(std::shared_ptr<EventBase>&& inEventData, const FVector2D& pos)
{
	if (IsInWorld(pos.x, pos.y))
	{
		const auto& eventType = inEventData->GetEventType();
		auto& sprite = EventLayer[pos.y][pos.x];
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

// --------------------------
// Main : Utils : Item
// --------------------------
void Level2D::SetItemLayerID(const EItemId& itemType, const FVector2D& pos)
{
	if (IsInWorld(pos.x, pos.y))
	{
		auto& sprite = ItemLayer[pos.y][pos.x];
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

// --------------------------
// Main : Utils : Character
// --------------------------
void Level2D::SetCharacterLayerID(const ECharacterId& characterType, const FVector2D& pos)
{
	if (IsInWorld(pos.x, pos.y))
	{
		auto& sprite = CharacterLayer[pos.y][pos.x];
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
const std::shared_ptr<CharacterBase>& Level2D::GetCharacter2LayerID(const int& x, const int& y) const
{
	return Character2Layer[y][x];
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
