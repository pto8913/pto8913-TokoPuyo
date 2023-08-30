#pragma once

#include <string>

struct FColor
{
public:
    FColor();
    FColor(const float& R, const float& G, const float& B, const float& A);
    FColor(const float& R, const float& G, const float& B);
    FColor(const float& RGB, const float& A);
    FColor(int rgb, float alpha);
    FColor(const FColor& other);
    FColor(FColor&& other);
    FColor& operator=(const FColor& other);
    FColor& operator=(FColor&& other) noexcept;
    FColor operator-(const float& other) const;
    FColor& operator-=(const float& other);
    FColor operator*(const float& other) const;
    FColor& operator*=(const float& other);

    FColor operator+(const FColor& other) const;
    FColor& operator+=(const FColor& other);
    FColor operator-(const FColor& other) const;
    FColor& operator-=(const FColor& other);

    bool operator==(const FColor& other) const noexcept;
    bool operator<(const float& other) const noexcept;

    std::string ToString() const;
    FColor Round(const int& decimal = 2) const;
    bool IsNearlyEqual(const FColor& other, const float& epsilon);
    FColor Abs();

    float r, g, b, a;
};