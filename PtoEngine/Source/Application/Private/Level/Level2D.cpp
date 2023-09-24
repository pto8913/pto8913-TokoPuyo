
#include "Level/Level2D.h"

#include "Algorithm/Math.h"

#include "Actor/Ground/GroundBase.h"

#include "EngineSettings.h"
#include "GameSettings.h"

#include "Framework/World.h"

#include "Level/ObjectManager2D.h"

// ------------------------------------------------------------------------------------------------------------
// Level 2D
// ------------------------------------------------------------------------------------------------------------
Level2D::Level2D(DirectX11& dx)
	: Level(dx)
{
}
Level2D::~Level2D()
{
}

void Level2D::SetObjectManager()
{
	pObjectManager = std::make_shared<ObjectManager2D>();

	Level::SetObjectManager();
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
void Level2D::SetSpriteLocation(Actor2D* sprite, const float& worldX, const float& worldY)
{
	DirectX::XMFLOAT2 pos = WorldToScreen(worldX, worldY, sprite->GetActorScale());

	sprite->SetActorLocation(FVector(pos.x, pos.y, 0.f));
	sprite->Set2DIdx(FVector2D(worldX, worldY));
}
Actor2D* Level2D::GetLayer(const int& worldX, const int& worldY, const Layer::EOrder& inOrder, const Layer::EActorLayer& inLayer) const
{
	const auto& actors = pObjectManager->pObjects;
	if (actors.contains(inLayer))
	{
		const auto& elem = actors.at(inLayer);
		if (elem.size() > 0)
		{
			for (const auto& actor : elem)
			{
				const auto& actor2d = static_pointer_cast<Actor2D>(actor);
				if (actor2d->GetSortOrder() == inOrder)
				{
					auto idx = actor2d->Get2DIdx();
					if (worldX == idx.x && worldY == idx.y)
					{
						return actor2d.get();
					}
				}
			}
		}
	}
	return nullptr;
}
