#pragma once
#include "Widget.h"
#include "WCFCommon.h"

namespace CWPF {
	struct LabelWidgetInitArgs
	{
		std::wstring Text;
		int FontSize;
		Alignment Alignment;
		std::wstring Font;
	};

	class LabelWidget : public Widget
	{
	public:
		~LabelWidget();

		LabelWidget() {};
		LabelWidget(Widget* pParent, const LabelWidgetInitArgs& args);

		// Inherited via Widget
		virtual void Create(HWND hwnd, const Vec2& pos) override;
		virtual float GetWidth() override;
		virtual float GetHeight() override;
		virtual const wchar_t* GetName() override;
		virtual std::shared_ptr<Widget> Factory(const pugi::xml_node& node, Widget* pParent) const override;
	protected:
		std::wstring m_Text;
		int m_FontSize;
		int m_Width;
		int m_Height;
		Alignment m_Alignment;
		HMENU m_ID;
		HFONT m_hFont;
		std::wstring m_FontName;
	};
}