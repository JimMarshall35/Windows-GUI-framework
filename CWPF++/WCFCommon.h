#pragma once
#include <Windows.h>

namespace CWPF
{
	enum Alignment
	{
		Top,
		TopLeft,
		TopRight,
		Left,
		Middle,
		Right,
		BottomLeft,
		Bottom,
		BottomRight
	};

	enum class LengthType
	{
		Invalid,
		Fixed,
		Stretch,
		Auto
	};

	struct Length
	{
		LengthType Type;
		float Value;
	};

	struct Rect
	{
		float t, l, b, r;
	};

	HMENU GetControlID();

	Alignment ParseAlignment(const wchar_t* t, Alignment defaultVal = Alignment::TopLeft);

}