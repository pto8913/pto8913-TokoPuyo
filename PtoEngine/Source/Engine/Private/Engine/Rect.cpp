
#include "Engine/Rect.h"

#include <xutility>

FRect::FRect(const float& leftTopX, const float& leftTopY, const float& rightBottomX, const float& rightBottomY)
	: left(leftTopX), right(rightBottomX), top(leftTopY), bottom(rightBottomY)
{
}
FRect::FRect(float&& leftTopX, float&& leftTopY, float&& rightBottomX, float&& rightBottomY)
	: left(std::move(leftTopX)), right(std::move(rightBottomX)), top(std::move(leftTopY)), bottom(std::move(rightBottomY))
{
}
FRect::FRect(const float& x)
	: left(x), right(x), top(x), bottom(x)
{
}
FRect::FRect(float&& x)
	: left(x), right(x), top(x), bottom(std::move(x))
{
}
FRect::FRect(const FRect& other)
{
	*this = other;
}
FRect::FRect(FRect&& other) noexcept
{
	*this = std::move(other);
}
FRect& FRect::operator=(const FRect& other)
{
	left = other.left;
	top = other.top;
	right = other.right;
	bottom= other.bottom;
	return *this;
}
FRect& FRect::operator=(FRect&& other) noexcept
{
	left = std::move(other.left);
	top = std::move(other.top);
	right = std::move(other.right);
	bottom = std::move(other.bottom);
	return *this;
}

float FRect::GetWidth() const noexcept
{
	return right - left;
}
float FRect::GetHeight() const noexcept
{
	return bottom - top;
}