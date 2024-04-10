#include "Widget.h"
#include "BindingExpressionParserLib.h"
#include <string.h>

namespace CWPF {

	std::map<std::wstring, WidgetFactory> sWidgetNameMap;

    Widget::Widget(Widget* pParent)
        : m_pParent(pParent)
    {
    }

    CommonWidgetInitArgs Widget::ParseCommonInitArgs(const pugi::xml_node& node)
    {
        CommonWidgetInitArgs a;
        a.HorizontalAlignment = HorizontalAlignment::Left;
        a.VerticalAlignment = VerticalAlignment::Top;
        if (pugi::xml_attribute attr = node.attribute(L"halign"))
        {
            a.HorizontalAlignment = ParseHAlignment(attr.as_string());
        }
        if (pugi::xml_attribute attr = node.attribute(L"valign"))
        {
            a.VerticalAlignment = ParseVAlignment(attr.as_string());
        }
        if (pugi::xml_attribute attr = node.attribute(L"column"))
        {
            a.Column = attr.as_int();
        }
        if (pugi::xml_attribute attr = node.attribute(L"row"))
        {
            a.Row = attr.as_int();
        }
        return a; 
    }

    std::shared_ptr<Widget> Widget::Factory(const pugi::xml_node& node, Widget* pParent) const
    {
        // rely on subclass for actual implementation
        std::shared_ptr<Widget> w = FactoryImplementation(node, pParent);
        // but decorate the result with extra common properties
        CommonWidgetInitArgs wArgs = ParseCommonInitArgs(node);
        w->m_VerticalAlignment = wArgs.VerticalAlignment;
        w->m_HorizontalAlignment = wArgs.HorizontalAlignment;
        w->m_Row = wArgs.Row;
        w->m_Column = wArgs.Column;
        return w;
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
    //void BindablePropertiesWidget::OnBoundPropertyChanged(const TaggedBindingValue& val, const std::wstring& name)
    //{
    //    std::map<std::wstring, BindableProperty>& v = GetBindableProperties();
    //    if (v.find(name) != v.end())
    //    {
    //        v[name].Setter(val);
    //    }
    //}
    //void BindablePropertiesWidget::EnumerateBindings(std::map<std::wstring, std::vector<WidgetPropertyBinding>>& bindingsMap) const
    //{
    //    std::map<std::wstring, BindableProperty>& v = GetBindableProperties();
    //    for (const std::pair< std::wstring, BindableProperty>& bp : v)
    //    {
    //        bindingsMap[bp.first].push_back({ bp.second.Type, (Widget*)this });
    //    }
    //}
    //std::shared_ptr<Widget> BindablePropertiesWidget::Factory(const pugi::xml_node& node, Widget* pParent) const
    //{
    //    std::shared_ptr<Widget> instance = Widget::Factory(node, pParent);
    //    BindablePropertiesWidget* cast = (BindablePropertiesWidget*)instance.get();
    //    std::map<std::wstring, BindableProperty>& v = cast->GetBindableProperties();
    //    for (const std::pair<std::wstring, BindableProperty>& kvp : v)
    //    {
    //        if (pugi::xml_attribute atrr = node.attribute(kvp.first.c_str()))
    //        {
    //            const wchar_t* attrString = atrr.as_string();
    //            if (BindingExpressionParser::IsLiteralValue(attrString))
    //            {
    //                TaggedBindingValue val;
    //                BindingValueType t = kvp.second.Value.VType;
    //                val.VType = t;
    //                switch (t) {
    //                case BindingValueType::Bool:
    //                    val.Value.b = atrr.as_bool();
    //                    break;
    //                /*case BindingValueType::Command:
    //                    break;*/
    //                case BindingValueType::Flt:
    //                    val.Value.f = atrr.as_float();
    //                    break;
    //                case BindingValueType::Int:
    //                    val.Value.i = atrr.as_int();
    //                    break;
    //                case BindingValueType::Str:
    //                    val.Value.s = new wchar_t[wcslen(attrString) + 1];
    //                    //wcscpy(val.Value.s, attrString);
    //                    break;
    //                }
    //                kvp.second.Setter(val);
    //                if (t == BindingValueType::Str)
    //                {
    //                    delete[] val.Value.s;
    //                }
    //            }
    //            else
    //            {

    //            }
    //        }
    //    }
    //    return instance;
    //}
    //BindablePropertiesWidget::BindablePropertiesWidget()
    //{
    //    std::map<std::wstring, BindableProperty>& v = GetBindableProperties();
    //    for (const std::pair< std::wstring, BindableProperty>& bp : v)
    //    {
    //        m_PropertyStatuses[bp.first] = BindablePropertyStatus::Undefined;
    //    }
    //}
}