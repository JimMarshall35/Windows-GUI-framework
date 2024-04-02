#pragma once
#include "Widget.h"
#include "WCFCommon.h"

namespace CWPF {
	struct LabelWidgetInitArgs
	{
		std::wstring Text;
		int FontSize;
		std::wstring Font;
		CommonWidgetInitArgs common;
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
		HBRUSH GetBackgroundBrush();
		Colour GetBackgroundColour() { return m_BackgroundColour; }
		Colour GetTextColour() { return m_TextColour; }
	protected:
		virtual std::shared_ptr<Widget> FactoryImplementation(const pugi::xml_node& node, Widget* pParent) const override;
	protected:
		std::wstring m_Text;
		int m_FontSize;
		int m_Width;
		int m_Height;
		HFONT m_hFont;
		std::wstring m_FontName;
		Colour m_TextColour;
		Colour m_BackgroundColour;
		HBRUSH m_hbrBkgnd = NULL;
	};
}