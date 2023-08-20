#pragma once

struct FRect
{
public:
	FRect() = default;
	FRect(const float& leftTopX, const float& leftTopY, const float& rightBottomX, const float& rightBottomY);
	FRect(const float& x);
	FRect(FRect&& other) noexcept;
	FRect(const FRect& other);

	FRect& operator=(const FRect& other);
	FRect& operator=(FRect&& other) noexcept;

	float GetWidth() const noexcept;
	float GetHeight() const noexcept;

	float left, right, top, bottom;
};