#include "LabelWidget.h"
#include "View.h"
#include "WCFCommon.h"
namespace CWPF {

    LabelWidget::~LabelWidget()
    {
        DeleteObject(m_hFont);
    }

    LabelWidget::LabelWidget(Widget* pParent, const LabelWidgetInitArgs& args)
        :Widget(pParent),
        m_Text(args.Text),
        m_FontSize(args.FontSize),
        m_Alignment(args.Alignment),
        m_FontName(args.Font),
        m_TextColour(args.TextColour),
        m_BackgroundColour(args.BackgroundColour)
    {
        Widget* parent = pParent;
        while (!parent->GetView())
        {
            parent = parent->GetParent();
        }
        View* view = parent->GetView();
        HWND hwnd = view->GetHWND();
        HDC hdc = GetDC(hwnd);
        int oldMode = SetMapMode(hdc, MM_TEXT);

        int nHeight = -MulDiv(m_FontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        m_hFont = CreateFont(
            nHeight, 0, 0, 0,
            FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
            m_FontName.c_str());
        HGDIOBJ oldFont = SelectObject(hdc, m_hFont);
        SIZE textSize;
        int result = GetTextExtentPoint32(hdc, m_Text.c_str(), m_Text.size(), &textSize);
        m_Width = textSize.cx;
        m_Height = textSize.cy;
        SelectObject(hdc, oldFont);

        SetMapMode(hdc, oldMode);
        ReleaseDC(hwnd, hdc);

    }
    void LabelWidget::Create(HWND hwnd, const Vec2& pos)
    {
        m_hWnd = CreateWindowEx(0, TEXT("STATIC"), m_Text.c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT,
            m_Pos.x, m_Pos.y, m_Width, m_Height, hwnd, HMENU(m_ID), GetModuleHandle(NULL), NULL);

        SendMessage(m_hWnd, WM_SETFONT, WPARAM(m_hFont), TRUE);
    }

    float LabelWidget::GetWidth()
    {
        return m_Width;
    }
    float LabelWidget::GetHeight()
    {
        return m_Height;
    }
    const wchar_t* LabelWidget::GetName()
    {
        static const wchar_t* s = L"Label";
        return s;
    }

    std::shared_ptr<Widget> LabelWidget::Factory(const pugi::xml_node& node, Widget* pParent) const
    {
        const wchar_t* text = node.attribute(L"text").as_string();
        const wchar_t* align = node.attribute(L"alignment").as_string();
        LabelWidgetInitArgs args;

        if (pugi::xml_attribute n = node.attribute(L"font"))
        {
            args.Font = n.as_string();
        }
        else
        {
            args.Font = L"Arial";
        }
        args.Text = text;
        args.Alignment = ParseAlignment(align);
        if (pugi::xml_attribute n = node.attribute(L"fontSize"))
        {
            args.FontSize = n.as_int();

        }
        else
        {
            args.FontSize = 11;
        }
        if (pugi::xml_attribute n = node.attribute(L"textColour"))
        {
            bool b = ParseColourAttribString(n.as_string(), args.TextColour);
            assert(b);
        }
        if (pugi::xml_attribute n = node.attribute(L"backgroundColour"))
        {
            bool b = ParseColourAttribString(n.as_string(), args.BackgroundColour);
            assert(b);
        }
        auto p = new LabelWidget(pParent, args);
        p->m_ID = GetControlID();
        return std::shared_ptr<Widget>(p);
    }
    HBRUSH LabelWidget::GetBackgroundBrush()
    {
        if (m_hbrBkgnd == NULL)
        {
            Colour bg = GetBackgroundColour();
            m_hbrBkgnd = CreateSolidBrush(RGB(bg.r, bg.g, bg.b));
        }
        return m_hbrBkgnd;
    }
}