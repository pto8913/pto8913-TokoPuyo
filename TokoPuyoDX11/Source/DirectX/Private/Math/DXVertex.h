#pragma once

#include "DirectX/DirectXHead.h"

struct D3D_VERTEX {
	float X, Y, Z, Rhw;
	DWORD Diffuse;
};

class Rect
{
public:
	Rect(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f);

	float const& GetX() const;
	float const& GetY() const;
	float const& GetWidth() const;
	float const& GetHeight() const;
	void SetX(float newVal);
	void SetY(float newVal);
	void SetWidth(float newVal);
	void SetHeight(float newVal);
private:
	float m_x;
	float m_y;
	float m_width;
	float m_height;
};