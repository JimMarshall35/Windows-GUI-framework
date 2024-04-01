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
    /*void BindablePropertiesWidget::OnBoundPropertyChanged(const TaggedBindingValue& val, const std::wstring& name)
    {
        std::map<std::wstring, BindableProperty>& v = GetBindableProperties();
        if (v.find(name) != v.end())
        {
            v[name].Setter(val);
        }
    }
    void BindablePropertiesWidget::EnumerateBindings(std::map<std::wstring, std::vector<WidgetPropertyBinding>>& bindingsMap) const
    {
        std::map<std::wstring, BindableProperty>& v = GetBindableProperties();
        for (const std::pair< std::wstring, BindableProperty>& bp : v)
        {
            bindingsMap[bp.first].emplace_back(bp.second.Type, (Widget*)this);
        }
    }
    BindablePropertiesWidget::BindablePropertiesWidget()
    {
    }*/
}