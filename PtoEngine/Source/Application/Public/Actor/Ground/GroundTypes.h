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

enum class EGroundId
{
	None,
	Background,
};
