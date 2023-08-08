
#include "Object/World.h"
#include "GameSettings.h"

World2D& World2D::operator=(World2D& Other)
{
	Clear();

	Tiles = std::move(Other.Tiles);
	Sprites = std::move(Other.Sprites);

	return *this;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void World2D::Init(const UINT16& x, const UINT16& y)
{
	height = y;
	width = x;

	TArray<ETileID> Rows(x, ETileID::Wall);
	Tiles.Init(y, Rows);

	TArray<std::shared_ptr<Sprite>> SRows(x, nullptr);
	Sprites.Init(y, SRows);
}
void World2D::Clear()
{
	if (Sprites.Size() > 0)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Sprites[y][x].reset();
			}
		}
	}
	Sprites.Clear();
	Tiles.Clear();
}

void World2D::DoFrame(float deltaTime)
{

}

// --------------------------
// Main : Debug
// --------------------------
void World2D::ShowTiles()
{
	OutputDebugStringA("-------- Show Tiles -------\n");
	for (int y = 0; y < height; ++y)
	{
		std::string str;
		for (int x = 0; x < width; ++x)
		{
			const auto& t = Tiles[y][x];
			switch (t)
			{
			case ETileID::Border:
				str += "B";
				break;
			case ETileID::Exit:
				str += "E";
				break;
			case ETileID::Floor:
				str += "F";
				break;
			case ETileID::Path:
				str += "P";
				break;
			case ETileID::PathCross:
				str += "C";
				break;
			case ETileID::Room:
				str += "R";
				break;
			case ETileID::Wall:
				str += " ";
				break;
			case ETileID::RoomWall:
				str += "W";
				break;
			default:
				break;
			}
		}
		OutputDebugStringA((str + "\n").c_str());
	}
	OutputDebugStringA("-------- Show Tiles -------\n");
	OutputDebugStringA("\n\n");
}

// --------------------------
// Main : Utils
// --------------------------
UINT16 World2D::GetSize() const noexcept
{
	return Tiles.Size();
}

bool World2D::IsInScreen(const UINT8& x, const UINT8& y) const noexcept
{
	return (x >= 0 && x < GameSettings::GAMESCREEN_WIDTH) && (y >= 0 && y < GameSettings::GAMESCREEN_HEIGHT);
}
bool World2D::IsInWorld(const UINT16& x, const UINT16& y) const noexcept
{
	return (x >= 0 && x < width) && (y >= 0 && y < height);
}

void World2D::SetTileID(const ETileID& NewTileID, const UINT16& x, const UINT16& y)
{
	if (IsInWorld(x, y))
	{
		Tiles[y][x] = NewTileID;
	}
}
void World2D::SetTileID(const ETileID& NewTileID, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal, INT16 inConstantXY2)
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
			Tiles[inConstantXY][i] = NewTileID;
		}
		else
		{
			Tiles[i][inConstantXY] = NewTileID;
		}
		if (inConstantXY2 > -1)
		{
			if (bConstantHorizontal)
			{
				Tiles[inConstantXY2][i] = NewTileID;
			}
			else
			{
				Tiles[i][inConstantXY2] = NewTileID;
			}
		}
	}
}
