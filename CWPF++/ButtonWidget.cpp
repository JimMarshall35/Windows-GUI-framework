#include "ButtonWidget.h"

namespace CWPF {

    ButtonWidget::ButtonWidget(Widget* pParent, const ButtonWidgetInitArgs& args)
        :Widget(pParent),
        m_Height(args.height),
        m_Width(args.width),
        m_Text(args.Text)
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
        m_ID = GetControlID();
        m_hWnd = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            m_Text.c_str(),      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            m_Pos.x,         // x position 
            m_Pos.y,         // y position 
            m_Width,        // Button width
            m_Height,        // Button height
            parent,     // Parent window
            m_ID,
            (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), // instance handle
            NULL);      // Pointer not needed.
    }

    std::shared_ptr<Widget> ButtonWidget::Factory(const pugi::xml_node& node, Widget* pParent) const
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
        return std::shared_ptr<Widget>(new ButtonWidget(pParent, args));
    }
}