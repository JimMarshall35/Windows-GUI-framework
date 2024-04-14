#pragma once
#include <Windows.h>
#include <list>
#include <functional>
#include <cassert>
#include <memory>
#include <string>
#include <variant>

#ifdef CWPF_EXPORTS
#define CWPF_API __declspec(dllexport)
#else
#define CWPF_API __declspec(dllimport)
#endif

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
		Undefined,
		TwoWay,
		OneWay,
		OneWayToSource,
		OneTime,
		Default
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
		/*union {
			wchar_t* s = nullptr;
			float f;
			int i;
			bool b;
			ICommand* cmd;
		}Value;*/
		std::variant<std::wstring, float, int, bool, ICommand*> Value;
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
	class CWPF_API Event
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