#pragma once
#include <Windows.h>
#include <memory>	
#include <vector>
#include "pugixml.hpp"
#include <cassert>
#include "Vec2.h"
#include <string>
#include <map>
#include <functional>
#include "Widget.h"
#include "WCFCommon.h"

namespace CWPF {

	enum class StartUpType
	{
		Maximized,
		Minimized,
		Normal
	};

	struct ViewInitArgs
	{
		const wchar_t* xmlPath;
		std::shared_ptr<ViewModel> dataContext;
		std::wstring windowText;
		IVec2 size = { 500,500 };
		StartUpType SUT = StartUpType::Normal;
	};

	class View
	{
	public:
		View(const ViewInitArgs& args);
		~View();

		void SetDataContext(std::shared_ptr<ViewModel> dataContext);
		inline HWND GetHWND() { return m_hWnd; }
		virtual void Show();
		void SignalBoundPropertyChanged(const std::wstring& name, const TaggedBindingValue& val);
	protected:
		virtual const wchar_t* GetWndClassName();
		
	private:
		class RootWidget : public Widget
		{
		public:
			RootWidget(View* view, int w, int h);

			// Inherited via Widget
			virtual float GetWidth() override;
			virtual float GetHeight() override;
			virtual const wchar_t* GetName() override;
			virtual View* GetView() override;
		protected:
			virtual std::shared_ptr<Widget> FactoryImplementation(const pugi::xml_node& node, Widget* pParent) const override;
		private:
			View* m_pView = nullptr;
			float m_Width;
			float m_Height;
		};
	private:
		void CreateFromXml();
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void AddChildren(Widget* parent, pugi::xml_node node, int recursionLevel = 0);
		void OnDataContextPropertyChanged(std::wstring name, TaggedBindingValue val);
	private:
		Event<std::wstring, TaggedBindingValue> m_eBoundControlChanged;
		std::shared_ptr<ViewModel> m_spDataContext;
		pugi::xml_document m_Doc;
		HWND m_hWnd = NULL;
		std::unique_ptr<Widget> m_RootWidget;
		IVec2 m_WindowSize;
		StartUpType m_SUT;
		EventSubscriptionToken m_hDataContextSubscription = 0;
		std::map<std::wstring, std::vector<WidgetPropertyBinding>> m_Bindings;
		static std::map<HMENU, std::shared_ptr<Widget>> s_WidgetIDMap;
	};

}
