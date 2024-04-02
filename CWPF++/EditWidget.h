#pragma once
#include "Widget.h"

namespace CWPF
{
	struct EditWidgetCreateArgs
	{
		int Width;
		int Height;
		std::wstring Text;
	};

	class EditWidget : public Widget
	{
	public:
		EditWidget() {}
		EditWidget(Widget* pParent, const EditWidgetCreateArgs& args);
		// Inherited via Widget
		virtual void Create(HWND hwnd, const Vec2& pos) override;
		virtual float GetWidth() override;
		virtual float GetHeight() override;
		virtual const wchar_t* GetName() override;
	protected:
		virtual std::shared_ptr<Widget> FactoryImplementation(const pugi::xml_node& node, Widget* pParent) const override;
	protected:
		int m_Width;
		int m_Height;
		std::wstring m_Text;
	};
}