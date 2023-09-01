
#include "Math/Box.h"

// ------------------------------------------------------
// FBox
// ------------------------------------------------------
FBox::FBox(const FVector& left, const FVector& right)
	: leftTop(left), rightBottom(right)
{
}
FBox::FBox(FVector&& left, FVector&& right)
	: leftTop(std::move(left)), rightBottom(std::move(right))
{
}

float FBox::GetWidth() const
{
	return abs(leftTop.x - rightBottom.x);
}
float FBox::GetHeight() const
{
	return abs(leftTop.z - rightBottom.z);
}
