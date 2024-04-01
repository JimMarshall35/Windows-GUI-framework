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
		Colour TextColour = {0x00,0x00,0x00,0xff};
		Colour BackgroundColour = { 0xff,0xff,0xff,0xff };
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
		void SetBackgroundBrush(const HBRUSH& brush) { m_hbrBkgnd = brush; }
		HBRUSH GetBackgroundBrush() { return m_hbrBkgnd; }
		Colour GetBackgroundColour() { return m_BackgroundColour; }
		Colour GetTextColour() { return m_TextColour; }
	protected:
		std::wstring m_Text;
		int m_FontSize;
		int m_Width;
		int m_Height;
		Alignment m_Alignment;
		HFONT m_hFont;
		std::wstring m_FontName;
		Colour m_TextColour;
		Colour m_BackgroundColour;
		HBRUSH m_hbrBkgnd = NULL;
	};
}