#pragma once

#include <type_traits>

static constexpr const int sc_redShift = 16;
static constexpr const int sc_greenShift = 8;
static constexpr const int sc_blueShift = 0;

static constexpr const int sc_redMask = 0xff << sc_redShift;
static constexpr const int sc_greenMask = 0xff << sc_greenShift;
static constexpr const int sc_blueMask = 0xff << sc_blueShift;

struct FColor
{
public:
    FColor() : r(0.f), g(0.f), b(0.f), a(1.f)
    {
    }
	FColor(const float& R, const float& G, const float& B, const float& A)
		: r(R), g(G), b(B), a(A)
	{
	}
    FColor(const float& R, const float& G, const float& B)
        : r(R), g(G), b(B), a(1.f)
    {
    }
    FColor(const float& RGB, const float& A)
        : r(RGB), g(RGB), b(RGB), a(A)
    {
    }
    FColor(int rgb, float alpha)
    {
        r = static_cast<float>((rgb & sc_redMask) >> sc_redShift) / 255.f;
        g = static_cast<float>((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
        b = static_cast<float>((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
        a = alpha;
    }
    FColor(const FColor& other)
    {
        *this = other;
    }
    FColor(FColor&& other)
    {
        *this = std::move(other);
    }

    FColor& operator=(const FColor& other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    }

    FColor& operator=(FColor&& other)
    {
        r = std::move(other.r);
        g = std::move(other.g);
        b = std::move(other.b);
        a = std::move(other.a);
        return *this;
    }

    FColor operator*(const float& other) const
    {
        FColor out(*this);
        return out *= other;
    }
    FColor& operator*=(const float& other)
    {
        r *= other;
        g *= other;
        b *= other;
        a *= other;
        return *this;
    }

    FColor operator+(const FColor& other) const
    {
        FColor out(*this);
        return out += other;
    }
    FColor& operator+=(const FColor& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }

    bool operator==(const FColor& other) const noexcept
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    float r, g, b, a;
};