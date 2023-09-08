
#include "Level/Level2D.h"

#include "Math/Math.h"

#include "Actor/Ground/GroundBase.h"

#include "EngineSettings.h"
#include "GameSettings.h"

#include "Framework/World.h"

#include "Level/ObjectCollection2D.h"


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

void Level2D::SetObjectCollection()
{
	pObjectCollection = std::make_shared<ObjectCollection2D>();
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

	bInitialized = true;
}

void Level2D::Init(const int& x, const int& y)
{
	height = y;
	width = x;
}

void Level2D::Clear()
{
	width = 0;
	height = 0;

	mStartPosition = FVector();
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

bool Level2D::IsInWorld(const float& x, const float& y) const noexcept
{
	return (x >= 0 && x < width) && (y >= 0 && y < height);
}
DirectX::XMFLOAT2 Level2D::WorldToScreen(const float& x, const float& y, const FVector& size)
{
	return {
		GameSettings::GAMESCREEN_PADDING.x + size.x * x,
		GameSettings::GAMESCREEN_PADDING.y + size.y * y
	};
}
void Level2D::SetSpriteLocation(std::shared_ptr<Actor2D> sprite, const float& worldX, const float& worldY)
{
	DirectX::XMFLOAT2 pos = WorldToScreen(worldX, worldY, sprite->GetActorScale());

	sprite->SetActorLocation(FVector(pos.x, pos.y, 0.f));
	sprite->Set2DIdx(FVector2D(worldX, worldY));
}
std::shared_ptr<Actor2D> Level2D::GetLayer(const int& worldX, const int& worldY, const Layer::EOrder& inOrder, const Layer::EActorLayer& inLayer) const
{
	const auto& actors = pObjectCollection->pActors;
	if (actors.contains(inLayer))
	{
		auto elem = actors.at(inLayer);
		if (elem.size() > 0)
		{
			for (const auto& actor : elem)
			{
				const auto actor2d = static_pointer_cast<Actor2D>(actor);
				if (actor2d->GetSortOrder() == inOrder)
				{
					auto idx = actor2d->Get2DIdx();
					if (worldX == idx.x && worldY == idx.y)
					{
						return actor2d;
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
std::shared_ptr<GroundBase> Level2D::SetGroundLayerIDSpe(const EGroundId& groundType, const float& worldX, const float& worldY)
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
		return sprite;
	}
	return nullptr;
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
			SetGroundLayerIDSpe(groundType, i, inConstantXY);
		}
		else
		{
			SetGroundLayerIDSpe(groundType, inConstantXY, i);
		}
		if (inConstantXY2 > -1)
		{
			if (bConstantHorizontal)
			{
				SetGroundLayerIDSpe(groundType, i, inConstantXY2);
			}
			else
			{
				SetGroundLayerIDSpe(groundType, inConstantXY2, i);
			}
		}
	}
}
std::shared_ptr<GroundBase> Level2D::GetGroundLayer(const int& worldX, const int& worldY) const
{
	if (auto obj = GetLayer(worldX, worldY, Layer::EOrder::Ground, Layer::EActorLayer::Background))
	{
		return static_pointer_cast<GroundBase>(obj);
	}
	return nullptr;
}
