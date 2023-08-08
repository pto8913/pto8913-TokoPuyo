#pragma once

struct FRect
{
public:
	FRect() = default;
	FRect(const int& leftTopX, const int& leftTopY, const int& rightBottomX, const int& rightBottomY);

	int GetWidth() const noexcept;
	int GetHeight() const noexcept;

	int l, r, t, b;
};