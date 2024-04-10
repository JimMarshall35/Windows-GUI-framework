#include "ButtonWidget.h"
#include "Command.h"
#include <variant>

namespace CWPF {

    ButtonWidget::ButtonWidget(Widget* pParent, const ButtonWidgetInitArgs& args)
        :Widget(pParent),
        m_Height(args.height),
        m_Width(args.width),
        m_Text(args.Text),
        m_BoundPropertyName(args.BoundPropertyName)
    {

    }

    float ButtonWidget::GetWidth()
    {
        return m_Width;
    }

    float ButtonWidget::GetHeight()
    {
        return m_Height;
    }

    const wchar_t* ButtonWidget::GetName()
    {
        static const wchar_t* buttonWidget = L"Button";
        return buttonWidget;
    }

    void ButtonWidget::Create(HWND parent, const Vec2& pos)
    {
        m_hWnd = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            m_Text.c_str(),      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            m_Pos.x,         // x position 
            m_Pos.y,         // y position 
            m_Width,        // Button width
            m_Height,        // Button height
            parent,     // Parent window
            (HMENU)m_ID,
            (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), // instance handle
            NULL);      // Pointer not needed.
    }

    std::shared_ptr<Widget> ButtonWidget::FactoryImplementation(const pugi::xml_node& node, Widget* pParent) const
    {
        ButtonWidgetInitArgs args;
        if (pugi::xml_attribute n = node.attribute(L"height"))
        {
            args.height = n.as_int();
        }
        else
        {
            printf("noheight");
        }
        args.width = node.attribute(L"width").as_int();
        args.Text = node.attribute(L"text").as_string();
        if (pugi::xml_attribute n = node.attribute(L"command"))
        {
            args.BoundPropertyName = n.as_string();
        }
        auto p = new ButtonWidget(pParent, args);
        auto sp =  std::shared_ptr<Widget>(p);
        p->m_ID = GetControlID();
        return sp;
    }
    void ButtonWidget::EnumerateBindings(std::map<std::wstring, std::vector<WidgetPropertyBinding>>& bindingsMap) const
    {
        if (m_BoundPropertyName.length() > 0)
        {
            bindingsMap[m_BoundPropertyName].push_back(WidgetPropertyBinding{ BindingType::OneWay, (Widget*)this });
        }
    }
    void ButtonWidget::OnBoundPropertyChanged(const TaggedBindingValue& val, const std::wstring& name)
    {
        assert(val.VType == BindingValueType::Command);
        m_pCmd = std::get<ICommand*>(val.Value);
    }
    void ButtonWidget::OnCmdMesage(int msg)
    {
        switch (msg)
        {
        case BN_CLICKED:
        {
            if (m_pCmd)
            {
                m_pCmd->Execute(nullptr);
            }
            break;
        }
            
        }
    }
}