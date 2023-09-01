#pragma once

#include "Math/Box.h"

namespace Box
{
	bool IsInBox(const FBox& a, const FBox& b);
	bool IsPointInBox(const FBox& a, const FVector& b);
}