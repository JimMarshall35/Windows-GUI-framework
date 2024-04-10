#pragma once
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <string>
#include <Windows.h>
#include "Vec2.h"
#include "pugixml.hpp"
#include "WCFCommon.h"


namespace CWPF {
	class ViewModel;
	class View;
	class Widget;

	typedef std::function<std::shared_ptr<Widget>(const pugi::xml_node&, Widget*)> WidgetFactory;

	typedef std::vector<std::shared_ptr<Widget>> WidgetVec;

	extern std::map<std::wstring, WidgetFactory> sWidgetNameMap;

	struct CommonWidgetInitArgs
	{
		HorizontalAlignment HorizontalAlignment = HorizontalAlignment::Left;
		VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
		int Row = 0;
		int Column = 0;
	};
	
	class Widget
	{
	public:
		Widget() {}
		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;
		virtual const wchar_t* GetName() = 0;
		virtual std::shared_ptr<Widget> Factory(const pugi::xml_node& node, Widget* pParent) const;
		virtual void Create(HWND hwnd, const Vec2& pos);
		virtual void LayoutChildren();
		virtual void OnBoundPropertyChanged(const TaggedBindingValue& val, const std::wstring& name) {}
		virtual void EnumerateBindings(std::map<std::wstring, std::vector<WidgetPropertyBinding>>& bindingsMap) const {};
		virtual void OnCmdMesage(int msg) {}
		Widget(Widget* pParent);

		inline int GetID()
		{
			return m_ID;
		}

		inline void PushChild(std::shared_ptr<Widget> w)
		{
			m_Children.push_back(w);
		}

		virtual bool IsCreated() { return m_hWnd != NULL; }
		Vec2 GetPos() { return m_Pos; }
		void SetPos(const Vec2& pos) { m_Pos = pos; }

		inline IVec2 GetColAndRow()
		{
			return { m_Column, m_Row };
		}

		template<typename WidgetT>
		static void RegisterWidget()
		{
			WidgetT t;
			sWidgetNameMap[std::wstring(t.GetName())] = WidgetFactory([t](const pugi::xml_node& n, Widget* w) -> std::shared_ptr<Widget>
			{
				return t.Factory(n, w);
			});
		}
		virtual View* GetView() { return nullptr; }
		Widget* GetParent() { return m_pParent; }
		const WidgetVec& GetChildren()
		{
			return m_Children;
		}
	protected:
		virtual std::shared_ptr<Widget> FactoryImplementation(const pugi::xml_node& node, Widget* pParent) const = 0;

		static CommonWidgetInitArgs ParseCommonInitArgs(const pugi::xml_node& node);
	protected:
		Vec2 m_Pos;
		WidgetVec m_Children;
		HWND m_hWnd = NULL;
		Widget* m_pParent;
		int m_Column = 0;
		int m_Row = 0;
		int m_ID = -1;
		HorizontalAlignment m_HorizontalAlignment = HorizontalAlignment::Left;
		VerticalAlignment m_VerticalAlignment = VerticalAlignment::Top;

	};

	struct BindableProperty
	{
		TaggedBindingValue Value;
		std::wstring Name;
		BindingType Type;
		std::function<TaggedBindingValue(void)> Getter;
		std::function<void(const TaggedBindingValue&)> Setter;
	};

	enum class BindablePropertyStatus
	{
		Undefined,
		LiteralValue,
		BindingUnbound,
		BindingBound
	};

	/*class BindablePropertiesWidget : public Widget
	{
	public:
		virtual void OnBoundPropertyChanged(const TaggedBindingValue& val, const std::wstring& name);
		virtual void EnumerateBindings(std::map<std::wstring, std::vector<WidgetPropertyBinding>>& bindingsMap) const;
		virtual std::shared_ptr<Widget> Factory(const pugi::xml_node& node, Widget* pParent) const;
		BindablePropertiesWidget();
	protected:
		std::map<std::wstring, BindablePropertyStatus> m_PropertyStatuses;
		virtual std::map<std::wstring, BindableProperty>& GetBindableProperties() const = 0;
	};*/
};

