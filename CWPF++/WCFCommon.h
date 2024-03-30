#pragma once
#include <Windows.h>
#include <list>
#include <functional>
#include <cassert>

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

	enum class BindingType
	{
		TwoWay,
		OneWay,
		OneWayToSource
	};

	enum class BindingValueType
	{
		Invalid = 0,
		Str,
		Flt,
		Int,
		Bool,

	};

	struct TaggedBindingValue
	{
		union {
			const wchar_t* s = nullptr;
			float f;
			int i;
			bool b;
		}Value;
		BindingValueType VType = BindingValueType::Invalid;
	};

	HMENU GetControlID();

	Alignment ParseAlignment(const wchar_t* t, Alignment defaultVal = Alignment::TopLeft);

	typedef int EventSubscriptionToken;
	template<typename... ArgTs>
	class Event
	{
	public:
		EventSubscriptionToken operator +=(std::function<void(ArgTs...)> fn)
		{
			static EventSubscriptionToken t = 1;
			EventSubscriptionToken thisT = t++;
			m_Listeners.push_back({ fn, thisT });
			return thisT;
		}
		void operator -=(EventSubscriptionToken h)
		{
			m_Listeners.remove_if([h](const Listener& l) { return l.handle == h; });
		}
		void operator()(ArgTs...args)
		{
			for (Listener l : m_Listeners)
			{
				l.fn(args...);
			}
		}
	private:
		struct Listener
		{
			std::function<void(ArgTs...)> fn;
			EventSubscriptionToken handle;
		};
		std::list<Listener> m_Listeners;
	};
}