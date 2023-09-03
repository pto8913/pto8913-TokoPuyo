
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

const EGroundTile ConvertToGroundTile(const EGroundId& groundLayer)
{
	switch (groundLayer)
	{
	case EGroundId::Room_Plain:
	case EGroundId::Room_Forest:
	case EGroundId::Room_Cave:
	case EGroundId::Room_Water:
		return EGroundTile::Room;
	case EGroundId::Wall_Plain:
	case EGroundId::Wall_Forest:
	case EGroundId::Wall_Cave:
	case EGroundId::Wall_Water:
		return EGroundTile::Wall;
	case EGroundId::Path_Plain:
	case EGroundId::Path_Forest:
	case EGroundId::Path_Cave:
	case EGroundId::Path_Water:
		return EGroundTile::Path;
	default:
		break;
	}
	return EGroundTile::None;
}

const EGroundId ConvertToGround(const EGroundTile& tileType, const EGroundType& groundType)
{
	switch (tileType)
	{
	case EGroundTile::Room:
		switch (groundType)
		{
		case EGroundType::Plain:
			return EGroundId::Room_Plain;
		case EGroundType::Forest:
			return EGroundId::Room_Forest;
		case EGroundType::Cave:
			return EGroundId::Room_Cave;
		case EGroundType::Water:
			return EGroundId::Room_Water;
		default:
			break;
		}
		break;
	case EGroundTile::Wall:
		switch (groundType)
		{
		case EGroundType::Plain:
			return EGroundId::Wall_Plain;
		case EGroundType::Forest:
			return EGroundId::Wall_Forest;
		case EGroundType::Cave:
			return EGroundId::Wall_Cave;
		case EGroundType::Water:
			return EGroundId::Wall_Water;
		default:
			break;
		}
		break;
	case EGroundTile::Path:
		switch (groundType)
		{
		case EGroundType::Plain:
			return EGroundId::Path_Plain;
		case EGroundType::Forest:
			return EGroundId::Path_Forest;
		case EGroundType::Cave:
			return EGroundId::Path_Cave;
		case EGroundType::Water:
			return EGroundId::Path_Water;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return EGroundId::None;
}