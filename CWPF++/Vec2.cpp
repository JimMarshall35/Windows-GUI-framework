#include "Vec2.h"

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2{ other.x + x, other.y + y };
}

IVec2 IVec2::operator+(const IVec2& other) const
{
    return IVec2{ other.x + x, other.y + y };
}
