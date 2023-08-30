
#include "Engine/Color.h"

#include "Algorithm/Math2.h"

#pragma once

#include <format>

static constexpr const int sc_redShift = 16;
static constexpr const int sc_greenShift = 8;
static constexpr const int sc_blueShift = 0;

static constexpr const int sc_redMask = 0xff << sc_redShift;
static constexpr const int sc_greenMask = 0xff << sc_greenShift;
static constexpr const int sc_blueMask = 0xff << sc_blueShift;

FColor::FColor() : r(0.f), g(0.f), b(0.f), a(1.f)
{
}
FColor::FColor(const float& R, const float& G, const float& B, const float& A)
    : r(R), g(G), b(B), a(A)
{
}
FColor::FColor(const float& R, const float& G, const float& B)
    : r(R), g(G), b(B), a(1.f)
{
}
FColor::FColor(const float& RGB, const float& A)
    : r(RGB), g(RGB), b(RGB), a(A)
{
}
FColor::FColor(int rgb, float alpha)
{
    r = static_cast<float>((rgb & sc_redMask) >> sc_redShift) / 255.f;
    g = static_cast<float>((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
    b = static_cast<float>((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
    a = alpha;
}
FColor::FColor(const FColor& other)
{
    *this = other;
}
FColor::FColor(FColor&& other)
{
    *this = std::move(other);
}

FColor& FColor::operator=(const FColor& other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
    return *this;
}

FColor& FColor::operator=(FColor&& other) noexcept
{
    r = std::move(other.r);
    g = std::move(other.g);
    b = std::move(other.b);
    a = std::move(other.a);
    return *this;
}

FColor FColor::operator-(const float& other) const
{
    FColor out(*this);
    return out -= other;
}
FColor& FColor::operator-=(const float& other)
{
    r -= other;
    g -= other;
    b -= other;
    a -= other;
    *this = Round(3);
    return *this;
}
FColor FColor::operator*(const float& other) const
{
    FColor out(*this);
    return out *= other;
}
FColor& FColor::operator*=(const float& other)
{
    r *= other;
    g *= other;
    b *= other;
    a *= other;
    *this = Round(3);
    return *this;
}

FColor FColor::operator+(const FColor& other) const
{
    FColor out(*this);
    return out += other;
}
FColor& FColor::operator+=(const FColor& other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    a += other.a;
    *this = Round(3);
    return *this;
}
FColor FColor::operator-(const FColor& other) const
{
    FColor out(*this);
    return out -= other;
}
FColor& FColor::operator-=(const FColor& other)
{
    r -= other.r;
    g -= other.g;
    b -= other.b;
    a -= other.a;
    *this = Round(3);
    return *this;
}

bool FColor::operator==(const FColor& other) const noexcept
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}
bool FColor::operator<(const float& other) const noexcept
{
    return r < other && g < other && b < other && a < other;
}

std::string FColor::ToString() const
{
    FColor rounded = Round(3);
    return std::format(
        "(r, g, b, a) = ({}, {}, {}, {})\n",
        rounded.r,
        rounded.g,
        rounded.b,
        rounded.a
    );
}

FColor FColor::Round(const int& decimal) const
{
    FColor out(*this);
    const int place = pow(10, decimal);
    out.r = ceilf(out.r * place) / place;
    out.g = ceilf(out.g * place) / place;
    out.b = ceilf(out.b * place) / place;
    out.a = ceilf(out.a * place) / place;
    return out;
}

bool FColor::IsNearlyEqual(const FColor& other, const float& epsilon)
{
    return Algo::IsNearlyEqual(*this, other, epsilon);
}

FColor FColor::Abs()
{
    FColor out(*this);
    out.r = fabs(out.r);
    out.g = fabs(out.g);
    out.b = fabs(out.b);
    out.a = fabs(out.a);
    return out;
}