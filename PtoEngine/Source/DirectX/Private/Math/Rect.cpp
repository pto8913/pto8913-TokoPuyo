
#include "Math/Rect.h"

Rect::Rect(float x, float y, float width, float height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}

float const& Rect::GetX() const
{
	return m_x;
}
float const& Rect::GetY() const
{
	return m_y;
}
float const& Rect::GetWidth() const
{
	return m_width;
}
float const& Rect::GetHeight() const
{
	return m_height;
}
void Rect::SetX(float newVal)
{
	m_x = newVal;
}
void Rect::SetY(float newVal)
{
	m_y = newVal;
}
void Rect::SetWidth(float newVal)
{
	m_width = newVal;
}
void Rect::SetHeight(float newVal)
{
	m_height = newVal;
}