
#include "Level/Level2D.h"

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
}
Level2D::~Level2D()
{
	Clear();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level2D::BeginPlay(DirectX11& dx)
{
	Generate(dx);
}
void Level2D::Tick(DirectX11& dx, float deltaTime)
{
	if (!bInitialized) return;

	Level::Tick(dx, deltaTime);
}

void Level2D::Generate(DirectX11& dx)
{
	bInitialized = false;

	GenerateGroundLayer();
	GenerateEventLayer();
	GenerateItemLayer();
	GenerateBuildingLayer();
	GenerateCharacterLayer();
	GenerateEffectLayer();

#if _DEBUG
	OutputDebugStringA(std::format("start {}\n", mStartPosition.ToString()).c_str());
#endif
	MoveCenter(mStartPosition.x, mStartPosition.y);

	bInitialized = true;
}

void Level2D::Init(const int& x, const int& y)
{
	height = y;
	width = x;
}

bool Level2D::MoveCenter(const float& x, const float& y)
{
	const float nextX = Math::Clamp(centerX + x, 0.f, (float)width);
	const float nextY = Math::Clamp(centerY + y, 0.f, (float)height);
	if (IsInWorld(nextX, nextY))
	{
		centerX = nextX;
		centerY = nextY;
#if _DEBUG
		//OutputDebugStringA(std::format("move center x, y {} {}\n", centerX, centerY).c_str());
#endif
		screenLeftX = centerX - GameSettings::GAMESCREEN_CENTER_X - 1;
		screenLeftY = centerY - GameSettings::GAMESCREEN_CENTER_Y - 1;
		screenRightX = centerX + GameSettings::GAMESCREEN_CENTER_X + 2;
		screenRightY = centerY + GameSettings::GAMESCREEN_CENTER_Y + 2;

		SetSpriteLocation(GetWorld()->GetPlayer(), centerX, centerY);

		for (auto&& layer : mObjectCollection.pActors)
		{
			for (auto&& actor : layer.second)
			{
				auto idx = actor->Get2DIdx();
				if (IsInScreen(idx.x, idx.y))
				{
					if (actor->GetSortOrder() == Layer::EOrder::Character)
					{
						if (auto obj = static_pointer_cast<CharacterBase>(actor))
						{
							if (obj->GetCharacterType() == ECharacterId::Player)
							{
								continue;
							}
						}
					}
					SetSpriteLocation(actor, idx.x, idx.y);
					actor->SetVisibility(true);
				}
				else
				{
					SetSpriteLocation(actor, idx.x, idx.y);
					actor->SetVisibility(false);
				}
			}
		}
		return true;
	}
	return false;
}

void Level2D::Clear()
{
	width = 0;
	height = 0;

	mStartPosition = FVector();

	centerX = 0;
	centerY = 0;
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

bool Level2D::IsInScreen(const int& x, const int& y, const int& buffer) const noexcept
{
	return ((screenLeftX - buffer) <= x && x < (screenRightX + buffer)) && ((screenLeftY - buffer) <= y && y < (screenRightY + buffer));
}
bool Level2D::IsInWorld(const int& x, const int& y) const noexcept
{
	return (x >= 0 && x < width) && (y >= 0 && y < height);
}
DirectX::XMFLOAT2 Level2D::WorldToScreen(const int& x, const int& y, const FVector& size)
{
	//return {
	//	GameSettings::GAMESCREEN_LEFT_TOP.x + size.x * (x - screenLeftX),
	//	GameSettings::GAMESCREEN_LEFT_TOP.y * 2.f + 64.f + size.y * (y - screenLeftY)
	//};
	//auto v = GameSettings::GET_CELL(size.x, size.y);
	//return {
	//	v.x * (x - screenLeftX),
	//	GameSettings::CELL + v.y * (y - screenLeftY)
	//};
	return {
		size.x + size.x * (x - screenLeftX),
		size.y + size.y * (y - screenLeftY)
	};
}
void Level2D::SetSpriteLocation(std::shared_ptr<Actor2D> sprite, const float& worldX, const float& worldY)
{
	DirectX::XMFLOAT2 pos = WorldToScreen(worldX, worldY, sprite->GetActorScale());

	sprite->SetActorLocation(FVector(pos.x, pos.y, 0.f));
	sprite->Set2DIdx(FVector2D(worldX, worldY));
}
std::shared_ptr<Actor2D> Level2D::GetLayer(const int& worldX, const int& worldY, const Layer::EOrder& inOrder, const EActor2DLayer& inLayer) const
{
	if (mObjectCollection.pActors.contains(inLayer))
	{
		auto elem = mObjectCollection.pActors.at(inLayer);
		if (elem.size() > 0)
		{
			for (auto&& actor : elem)
			{
				if (actor->GetSortOrder() == inOrder)
				{
					auto idx = actor->Get2DIdx();
					if (worldX == idx.x && worldY == idx.y)
					{
						return actor;
					}
				}
			}
		}
	}
	return nullptr;
}

// --------------------------
// Main : Utils : Ground
// --------------------------
void Level2D::SetGroundLayerID(const EGroundId& groundType, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		auto sprite = GetGroundLayer(worldX, worldY);
		if (sprite != nullptr)
		{
			sprite->SetGroundType(groundType);
		}
		else
		{
			sprite = GetWorld()->SpawnActor<GroundBase>(*pDX, groundType);
			sprite->BeginPlay(*pDX);
			SetSpriteLocation(sprite, worldX, worldY);
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
void Level2D::SetGroundLayerIDChecked(const EGroundId& groundType, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		bool canDraw = false;
		auto sprite = GetGroundLayer(worldX, worldY);
		if (sprite != nullptr)
		{
			switch (ConvertToGroundTile(sprite->GetGroundType()))
			{
			case EGroundTile::None:
			case EGroundTile::Wall:
				canDraw = true;
			default:
				break;
			};
		}
		else
		{
			canDraw = true;
		}
		if (canDraw)
		{
			auto sprite = GetGroundLayer(worldX, worldY);
			if (sprite != nullptr)
			{
				sprite->SetGroundType(groundType);
			}
			else
			{
				sprite = GetWorld()->SpawnActor<GroundBase>(*pDX, groundType);
				sprite->BeginPlay(*pDX);
				SetSpriteLocation(sprite, worldX, worldY);
			}
		}
	}
}
void Level2D::SetGroundLayerIDChecked(const EGroundId& groundType, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal, INT16 inConstantXY2)
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
			SetGroundLayerIDChecked(groundType, i, inConstantXY);
		}
		else
		{
			SetGroundLayerIDChecked(groundType, inConstantXY, i);
		}
		if (inConstantXY2 > -1)
		{
			if (bConstantHorizontal)
			{
				SetGroundLayerIDChecked(groundType, i, inConstantXY2);
			}
			else
			{
				SetGroundLayerIDChecked(groundType, inConstantXY2, i);
			}
		}
	}
}
std::shared_ptr<GroundBase> Level2D::GetGroundLayer(const int& worldX, const int& worldY) const
{
	if (auto obj = GetLayer(worldX, worldY, Layer::EOrder::Ground, EActor2DLayer::Background))
	{
		return static_pointer_cast<GroundBase>(obj);
	}
	return nullptr;
}

// --------------------------
// Main : Utils : Event
// --------------------------
void Level2D::SetEventLayerID(const EEventId& eventType, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		auto sprite = GetWorld()->SpawnActor<EventBase>(*pDX, eventType);
		sprite->BeginPlay(*pDX);
		SetSpriteLocation(sprite, worldX, worldY);
	}
}
void Level2D::SetEventLayerID(std::shared_ptr<EventBase>& sprite, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		SetSpriteLocation(sprite, worldX, worldY);
	}
}
void Level2D::SetEventLayerID(std::shared_ptr<EventBase>&& sprite, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		SetSpriteLocation(sprite, worldX, worldY);
	}
}
std::shared_ptr<EventBase> Level2D::GetEventLayer(const int& worldX, const int& worldY) const
{
	if (auto obj = GetLayer(worldX, worldY, Layer::EOrder::Event, EActor2DLayer::Entities))
	{
		return static_pointer_cast<EventBase>(obj);
	}
	return nullptr;
}

// --------------------------
// Main : Utils : Item
// --------------------------
void Level2D::SetItemLayerID(const EItemId& itemType, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		auto sprite = GetWorld()->SpawnActor<ItemBase>(*pDX, itemType);
		sprite->BeginPlay(*pDX);
		SetSpriteLocation(sprite, worldX, worldY);
	}
}
std::shared_ptr<ItemBase> Level2D::GetItemLayer(const int& worldX, const int& worldY) const
{
	if (auto obj = GetLayer(worldX, worldY, Layer::EOrder::Item, EActor2DLayer::Entities))
	{
		return static_pointer_cast<ItemBase>(obj);
	}
	return nullptr;
}

// --------------------------
// Main : Utils : Character
// --------------------------
void Level2D::SetCharacterLayerID(const ECharacterId& characterType, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		auto sprite = GetWorld()->SpawnActor<CharacterBase>(*pDX, characterType);
		sprite->BeginPlay(*pDX);
		SetSpriteLocation(sprite, worldX, worldY);
	}
}
std::shared_ptr<CharacterBase> Level2D::GetCharacterLayer(const int& worldX, const int& worldY) const
{
	if (auto obj = GetLayer(worldX, worldY, Layer::EOrder::Character, EActor2DLayer::Entities))
	{
		return static_pointer_cast<CharacterBase>(obj);
	}
	return nullptr;
}

// --------------------------
// Main : Debug
// --------------------------
void Level2D::ShowTiles()
{
	OutputDebugStringA("-------- Show Tiles -------\n");

	std::vector<std::string> x(int(width), " ");
	std::vector<std::vector<std::string>> tiles(int(height), x);
	for (const auto& layer : mObjectCollection.pActors)
	{
		for (const auto& actor : layer.second)
		{
			int x = 0, y = 0;
			auto p = actor->Get2DIdx();
			x = p.x; y = p.y;
			switch (actor->GetSortOrder())
			{
			case Layer::EOrder::Ground:
				switch (ConvertToGroundTile(static_pointer_cast<GroundBase>(actor)->GetGroundType()))
				{
				case EGroundTile::Path:
					tiles[y][x] = "P";
					break;
				case EGroundTile::Room:
					tiles[y][x] = "R";
					break;
				case EGroundTile::Wall:
					tiles[y][x] = "W";
					break;
				default:
					break;
				}
				break;
			case Layer::EOrder::Event:
				switch (static_pointer_cast<EventBase>(actor)->GetEventType())
				{
				case EEventId::Enter:
					tiles[y][x] = "S";
					break;
				case EEventId::Exit:
					tiles[y][x] = "E";
					break;
				default:
					break;
				}
				break;
			}
		}
	}

	for (int y = 0; y < height; ++y)
	{
		std::string str;
		for (int x = 0; x < width; ++x)
		{
			str += tiles[y][x];
		}
		OutputDebugStringA((str + "\n").c_str());
	}
	OutputDebugStringA("-------- Show GroundLayer -------\n");
	OutputDebugStringA("\n\n");
}
