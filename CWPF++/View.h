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
		virtual void Create();
		inline HWND GetHWND() { return m_hWnd; }
		virtual void Show();

	protected:
		virtual const wchar_t* GetWndClassName();
		
	private:
		class RootWidget : public Widget
		{
		public:
			RootWidget(View* view);

			// Inherited via Widget
			virtual float GetWidth() override;
			virtual float GetHeight() override;
			virtual const wchar_t* GetName() override;
			virtual View* GetView() override;
			virtual void LayoutChildren() override;
			virtual std::shared_ptr<Widget> Factory(const pugi::xml_node& node, Widget* pParent) const override;
		private:
			View* m_pView = nullptr;
		};
	private:
		void CalculateActualColAndRowSizes();
		Vec2 GetWidgetPosFromColAndRow(int col, int row) const;
		void CreateFromXml();
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void AddChildren(Widget* parent, pugi::xml_node node, int recursionLevel = 0);
		void OnDataContextPropertyChanged(std::wstring name, TaggedBindingValue val);
	private:
		Event<std::wstring, TaggedBindingValue> m_eBoundControlChanged;
		std::shared_ptr<ViewModel> m_spDataContext;
		pugi::xml_document m_Doc;
		HWND m_hWnd = NULL;
		std::vector<Length> m_Columns;
		std::vector<Length> m_Rows;
		std::unique_ptr<Widget> m_RootWidget;
		IVec2 m_WindowSize;
		StartUpType m_SUT;
		EventSubscriptionToken m_hDataContextSubscription = 0;
	};

}
