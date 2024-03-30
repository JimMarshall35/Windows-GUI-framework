#pragma once
#include "WCFCommon.h"
#include "Widget.h"
namespace CWPF
{

	struct ButtonWidgetInitArgs
	{
		std::wstring Text;
		Alignment Alignment;
		int width;
		int height;
	};

	class ButtonWidget : public Widget
	{
	public:
		ButtonWidget() {};
		ButtonWidget(Widget* pParent, const ButtonWidgetInitArgs& args);

		// Inherited via Widget
		virtual void Create(HWND hwnd, const Vec2& pos) override;
		virtual float GetWidth() override;
		virtual float GetHeight() override;
		virtual const wchar_t* GetName() override;
		virtual std::shared_ptr<Widget> Factory(const pugi::xml_node& node, Widget* pParent) const override;
	protected:
		std::wstring m_Text;
		int m_Width;
		int m_Height;
		HMENU m_ID;
	};

}