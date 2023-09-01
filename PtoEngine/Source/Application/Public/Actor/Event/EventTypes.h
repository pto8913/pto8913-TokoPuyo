#pragma once

#include "Engine/Delegate.h"
#include "Math/Vector.h"
#include <basetsd.h>

enum EEventId : unsigned char
{
	Enter,
	Exit,
	Shop,
	Transparent,
	Block,
	EnterRoom,
	None
};

struct FEventData
{
public:
	FEventData(const FVector& pos, const EEventId& inType)
		: position(pos), type(inType)
	{}

	FVector position;
	
	EEventId type;
	/* 
		use when exists the same Layer on Level.
	*/
	std::string tag;
};

DECLARE_DELEGATE_OneParam(FOnSendEvent, const FEventData&);
