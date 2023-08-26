
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

	Level::Tick(dx, deltaTime);
}

void Level2D::SetGroundType(const EGroundType& inGroundType)
{
	mGroundType = inGroundType;
}

void Level2D::Init(const int& x, const int& y)
{
	height = y;
	width = x;
}

void Level2D::Activate()
{
#if _DEBUG
	OutputDebugStringA(std::format("start {}\n", mStartPosition.ToString()).c_str());
#endif
	MoveCenter(mStartPosition.x, mStartPosition.y);

	auto pos = WorldToScreen(centerX, centerY, GetWorld()->GetPlayer()->GetActorScale());
	GetWorld()->GetPlayer()->SetActorLocation(FVector(pos.x, 0, pos.y));

	bInitialized = true;
}
void Level2D::Deactivate()
{
	bInitialized = false;
}

bool Level2D::MoveCenter(const float& x, const float& y)
{
#if _DEBUG
	OutputDebugStringA(std::format("move center x, y {} {}\n", centerX, centerY).c_str());
#endif

	const int nextX = Math::Clamp(int(centerX + x), 0, width);
	const int nextY = Math::Clamp(int(centerY + y), 0, height);
	if (IsInWorld(nextX, nextY))
	{
		centerX = nextX;
		centerY = nextY;

		screenLeftX = centerX - GameSettings::GAMESCREEN_CENTER_X - 1;
		screenLeftY = centerY - GameSettings::GAMESCREEN_CENTER_Y - 1;
		screenRightX = centerX + GameSettings::GAMESCREEN_CENTER_X + 2;
		screenRightY = centerY + GameSettings::GAMESCREEN_CENTER_Y + 2;

		OutputDebugStringA(std::format("next x, y {} {}\n", centerX, centerY).c_str());
		OutputDebugStringA(std::format("screen Rect {} {} {} {}\n", screenLeftX, screenLeftY, screenRightX, screenRightY).c_str());

		auto pos = WorldToScreen(centerX, centerY, GetWorld()->GetPlayer()->GetActorScale());
		GetWorld()->GetPlayer()->SetActorLocation(FVector(pos.x, 0, pos.y));

		//int sx = 0, sy = 0;
		//for (auto&& layer : mObjectCollection.pActors)
		//{
		//	for (auto&& actor : layer.second)
		//	{
		//		ScreenToWorld(actor->GetActorLocation(), actor->GetActorScale(), sx, sy);
		//		if (IsInScreen(sx, sy))
		//		{
		//			if (actor->GetSortOrder() == Layer::EOrder::Character)
		//			{
		//				if (auto obj = static_pointer_cast<CharacterBase>(actor))
		//				{
		//					if (obj->GetCharacterType() == ECharacterId::Player)
		//					{
		//						continue;
		//					}
		//				}
		//			}
		//			DirectX::XMFLOAT2 pos = WorldToScreen(sx, sy, actor->GetActorScale());
		//			actor->SetActorLocation(FVector(pos.x, 0.f, pos.y));
		//			actor->SetVisibility(true);
		//			//OutputDebugStringA(std::format("screen x, y {} {}\n", pos.x, pos.y).c_str());
		//		}
		//		else
		//		{
		//			actor->SetVisibility(false);
		//		}
		//	}
		//}
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
bool Level2D::IsInScreen(const float& x, const float& y) const noexcept
{
	return (x >= screenLeftX && x < screenRightX) && (y >= screenLeftY && y < screenRightY);
}
bool Level2D::IsInWorld(const int& x, const int& y) const noexcept
{
	return (x >= 0 && x < width) && (y >= 0 && y < height);
}
DirectX::XMFLOAT2 Level2D::WorldToScreen(const int& x, const int& y, const FVector& size)
{
	return {
		GameSettings::GAMESCREEN_LEFT_TOP.x + size.x * (x - screenLeftX),
		GameSettings::GAMESCREEN_LEFT_TOP.y * 2.f + 64.f + size.z * (y - screenLeftY)
	};
}
void Level2D::ScreenToWorld(const FVector& world, const FVector& size, int& x, int& y) const
{
	x = ((world.x - GameSettings::GAMESCREEN_LEFT_TOP.x) / size.x) + screenLeftX;
	y = ((world.z - (GameSettings::GAMESCREEN_LEFT_TOP.y * 2.f) - 64.f) / size.z) + screenLeftY;
}

// --------------------------
// Main : Utils : Ground
// --------------------------
void Level2D::SetGroundLayerID(const EGroundId& groundType, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		auto sprite = GetWorld()->SpawnActor<GroundBase>(*pDX, groundType);
		SetSpriteLocation(sprite, worldX, worldY);
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
		SetSpriteLocation(sprite, worldX, worldY);
	}
}
void Level2D::SetEventLayerID(std::shared_ptr<EventBase>& sprite, const float& worldX, const float& worldY)
{
	if (IsInWorld(worldX, worldY))
	{
		DirectX::XMFLOAT2 pos = WorldToScreen(worldX, worldY, sprite->GetActorScale());
		sprite->SetActorLocation(FVector(pos.x, 0.f, pos.y));
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

void Level2D::SetSpriteLocation(std::shared_ptr<Actor2D> sprite, const float& worldX, const float& worldY)
{
	DirectX::XMFLOAT2 pos = WorldToScreen(worldX, worldY, sprite->GetActorScale());
	sprite->SetActorLocation(FVector(pos.x, 0.f, pos.y));
}
std::shared_ptr<Actor2D> Level2D::GetLayer(const int& worldX, const int& worldY, const Layer::EOrder& inOrder, const EActor2DLayer& inLayer) const
{
	for (auto&& actor : mObjectCollection.pActors.at(inLayer))
	{
		if (actor->GetSortOrder() == inOrder)
		{
			int x = 0, y = 0;
			ScreenToWorld(actor->GetActorLocation(), actor->GetActorScale(), x, y);
			if (worldX == x && worldY == y)
			{
				return actor;
			}
		}
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
			ScreenToWorld(actor->GetActorLocation(), actor->GetActorScale(), x, y);
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
