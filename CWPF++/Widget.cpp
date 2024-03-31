#include "Widget.h"
namespace CWPF {

	std::map<std::wstring, WidgetFactory> sWidgetNameMap;

    Widget::Widget(Widget* pParent)
        : m_pParent(pParent)
    {
    }

    void Widget::Create(HWND hwnd, const Vec2& pos)
    {
        for (std::shared_ptr<Widget> w : m_Children)
        {
            w->Create(hwnd, pos);
        }
    }
    void Widget::LayoutChildren()
    {
        for (std::shared_ptr<Widget> w : m_Children)
        {
            w->LayoutChildren();
        }
    }
}