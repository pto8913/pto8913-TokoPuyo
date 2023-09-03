#pragma once

enum class EDirection
{
	Left,      /* -1,  0 */
	Right,     /*  1,  0 */
	Up,        /*  0, -1 */
	Down,      /*  0,  1 */
	LeftUp,    /* -1, -1 */
	LeftDown,  /* -1,  1 */
	RightUp,   /*  1, -1 */
	RightDown, /*  1,  1 */
	InValid
};
const EDirection ConvertToDirection(const int& x, const int& y);

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
