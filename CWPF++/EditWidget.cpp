#include "EditWidget.h"
#include <cassert>

namespace CWPF {

    EditWidget::EditWidget(Widget* pParent, const EditWidgetCreateArgs& args)
        :Widget(pParent),
        m_Text(args.Text),
        m_Width(args.Width),
        m_Height(args.Height)
    {
    }

    void EditWidget::Create(HWND hwnd, const Vec2& pos)
    {
        assert(!IsCreated());
        m_ID = GetControlID();
        m_hWnd = CreateWindowEx(
            0, L"EDIT",   // predefined class 
            NULL,         // no window title 
            WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            m_Pos.x, m_Pos.y, m_Width, m_Height,
            hwnd,         // parent window 
            m_ID,   // edit control ID 
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);        // pointer not needed 

    // Add text to the window. 
        SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)m_Text.c_str());
    }

    float EditWidget::GetWidth()
    {
        return m_Width;
    }

    float EditWidget::GetHeight()
    {
        return m_Height;
    }

    const wchar_t* EditWidget::GetName()
    {
        static const wchar_t* name = L"Edit";
        return name;
    }

    std::shared_ptr<Widget> EditWidget::Factory(const pugi::xml_node& node, Widget* pParent) const
    {
        EditWidgetCreateArgs args;
        args.Height = node.attribute(L"height").as_int();
        args.Width = node.attribute(L"width").as_int();
        args.Text = node.attribute(L"text").as_string();
        return std::shared_ptr<Widget>(new EditWidget(pParent, args));
    }

}