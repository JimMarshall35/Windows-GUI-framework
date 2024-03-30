#pragma once
#include "Widget.h"

namespace CWPF
{
	enum class StackPanelOrientation
	{
		Invalid,
		Horizontal,
		Vertical
	};

	struct StackPanelWidgetInitArgs
	{
		StackPanelOrientation Orientation = StackPanelOrientation::Invalid;
	};

	class StackPanelWidget : public Widget
	{
	public:
		StackPanelWidget() {}

		StackPanelWidget(Widget* pParent, const StackPanelWidgetInitArgs& args);
		// Inherited via Widget
		virtual float GetWidth() override;
		virtual float GetHeight() override;
		virtual const wchar_t* GetName() override;
		virtual bool IsCreated() override { return m_hWnd != NULL; }
		virtual void LayoutChildren() override;
		virtual std::shared_ptr<Widget> Factory(const pugi::xml_node& node, Widget* pParent) const override;
	private:
		StackPanelOrientation m_Orientation;
	};
}
