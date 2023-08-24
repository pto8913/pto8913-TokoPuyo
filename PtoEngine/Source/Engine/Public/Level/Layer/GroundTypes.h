#pragma once

enum class EGroundType
{
	Plain,
	Forest,
	Cave,
	Water
};

enum class EGroundTile
{
	Room,
	Wall,
	Path,
	None,
};

enum class EGroundId
{
	Room_Plain,
	Room_Forest,
	Room_Cave,
	Room_Water,

	Wall_Plain,
	Wall_Forest,
	Wall_Cave,
	Wall_Water,

	Path_Plain,
	Path_Forest,
	Path_Cave,
	Path_Water,

	None,
};

const EGroundTile ConvertToGroundTile(const EGroundId& groundLayer);
const EGroundId ConvertToGround(const EGroundTile& tileType, const EGroundType& groundType);
