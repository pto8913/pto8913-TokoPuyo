
#include "Actor/Ground/GroundTypes.h"

const EDirection ConvertToDirection(const int& x, const int& y)
{
	if(x == -1 && y ==  0)
	{
		return EDirection::Left;
	}
	if(x ==  1 && y ==  0)
	{
		return EDirection::Right;
	}
	if(x ==  0 && y == -1)
	{
		return EDirection::Up;
	}
	if(x ==  0 && y ==  1)
	{
		return EDirection::Down;
	}
	if(x == -1 && y == -1)
	{
		return EDirection::LeftUp;
	}
	if(x == -1 && y ==  1)
	{
		return EDirection::LeftDown;
	}
	if(x ==  1 && y == -1)
	{
		return EDirection::RightUp;
	}
	if(x ==  1 && y ==  1)
	{
		return EDirection::RightDown;
	}
	return EDirection::InValid;
}