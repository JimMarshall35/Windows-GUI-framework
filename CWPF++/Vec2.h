#pragma once
struct Vec2
{
	float x, y;
	Vec2 operator+(const Vec2& other) const;
};

struct IVec2
{
	int x, y;
	IVec2 operator+(const IVec2& other) const;
};

