#pragma once

#include "Engine/Delegate.h"

enum EEventId : unsigned char
{
	Enter,
	Exit,
	Shop,
	Transparent,
	Block
};

struct FEventData
{
public:
	FEventData(const UINT16& inX, const UINT16& inY, const EEventId& inType)
		: x(inX), y(inY), type(inType)
	{}

	UINT16 x;
	UINT16 y;
	
	EEventId type;
	/* 
		use when exists the same Layer on Level.
	*/
	std::string tag;
};

DECLARE_DELEGATE_OneParam(FOnSendEvent, const FEventData&);
