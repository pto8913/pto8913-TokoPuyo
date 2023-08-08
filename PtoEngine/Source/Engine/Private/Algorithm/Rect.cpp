
#include "Algorithm/Rect.h"

FRect::FRect(const int& leftTopX, const int& leftTopY, const int& rightBottomX, const int& rightBottomY)
	: l(leftTopX), r(rightBottomX), t(leftTopY), b(rightBottomY)
{

}

int FRect::GetWidth() const noexcept
{
	return r - l;
}
int FRect::GetHeight() const noexcept
{
	return b - t;
}