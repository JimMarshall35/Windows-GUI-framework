#pragma once
#include <Windows.h>
#include <list>
#include <functional>
#include <cassert>
#include <memory>

namespace CWPF
{
	class Widget;
	class ICommand;

	enum class HorizontalAlignment
	{
		Left,
		Middle,
		Right
	};

	enum class VerticalAlignment
	{
		Top,
		Middle,
		Bottom
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
		Command,
	};

	struct TaggedBindingValue
	{
		union {
			const wchar_t* s = nullptr;
			float f;
			int i;
			bool b;
			ICommand* cmd;
		}Value;
		BindingValueType VType = BindingValueType::Invalid;
	};

	struct WidgetPropertyBinding
	{
		BindingType Type;
		Widget* pWidget;
	};

	struct Colour
	{
		unsigned char r, g, b, a;
	};

	bool ParseColourAttribString(const wchar_t* str, Colour& c);

	int GetControlID();

	HorizontalAlignment ParseHAlignment(const wchar_t* t, HorizontalAlignment defaultVal = HorizontalAlignment::Left);

	VerticalAlignment ParseVAlignment(const wchar_t* t, VerticalAlignment defaultVal = VerticalAlignment::Top);
	
	Length ParseLength(const wchar_t* val);

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