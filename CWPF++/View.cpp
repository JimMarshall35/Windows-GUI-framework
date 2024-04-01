#include "View.h"
#include <cassert>
#include <vector>
#include <map>
#include <iostream>
#include "ViewModel.h"
#include "LabelWidget.h"

namespace CWPF
{
    std::map<HWND, View*> sViewInstanceToHWND;

    std::map<HMENU, std::shared_ptr<Widget>> View::s_WidgetIDMap;

    View::View(const ViewInitArgs& args)
        :m_RootWidget(std::unique_ptr<Widget>(new RootWidget(this, args.size.x, args.size.y))),
        m_WindowSize(args.size),
        m_SUT(args.SUT)
    {
        m_spDataContext = args.dataContext;

        pugi::xml_parse_result result = m_Doc.load_file(args.xmlPath);

        // Register the window class.
        const wchar_t CLASS_NAME[] = L"Sample Window Class";

        WNDCLASS wc = { };
        HINSTANCE hInstance = GetModuleHandle(NULL);
        wc.lpfnWndProc = View::WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);

        // Create the window.

        HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            CLASS_NAME,                     // Window class
            args.windowText.c_str(),    // Window text
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style
            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, m_WindowSize.x, m_WindowSize.y,
            NULL,       // Parent window    
            NULL,       // Menu
            hInstance,  // Instance handle
            this        // Additional application data
        );
        m_hWnd = hwnd;

        assert(m_hWnd);
        sViewInstanceToHWND[m_hWnd] = this;
    }

    View::~View()
    {
        assert(sViewInstanceToHWND.find(m_hWnd) != sViewInstanceToHWND.end());
        sViewInstanceToHWND.erase(m_hWnd);
        if (m_spDataContext)
        {
            if (m_hDataContextSubscription)
            {
                m_spDataContext->BindablePropertyChanged -= m_hDataContextSubscription;
                m_spDataContext->UnSubscribeFromBoundPropertySetExternalEvent(m_eBoundControlChanged);
            }
        }
    }

    void View::SetDataContext(std::shared_ptr<ViewModel> dataContext)
    {
        if (m_hDataContextSubscription != 0)
        {
            m_spDataContext->BindablePropertyChanged -= m_hDataContextSubscription;
            m_spDataContext->UnSubscribeFromBoundPropertySetExternalEvent(m_eBoundControlChanged);
        }
        m_spDataContext = dataContext;
        m_hDataContextSubscription =  dataContext->BindablePropertyChanged += 
            std::function<void(std::wstring, TaggedBindingValue)>([this](std::wstring s, TaggedBindingValue v) { OnDataContextPropertyChanged(s, v); });

        m_spDataContext->SubscribeToBoundPropertySetExternalEvent(m_eBoundControlChanged);
        m_spDataContext->InitializeProperties();
    }


    const wchar_t* View::GetWndClassName()
    {
        static const wchar_t* name = L"View";
        return name;
    }

    void View::AddChildren(Widget* parent, pugi::xml_node node, int recursionLevel/*=0*/)
    {
        for (pugi::xml_node child : node.children())
        {
            if (sWidgetNameMap.find(child.name()) != sWidgetNameMap.end())
            {
                std::shared_ptr<Widget> w = sWidgetNameMap[child.name()](child, parent);
                int id = (int)w->GetID();
                if (id > 0)
                {
                    s_WidgetIDMap[(HMENU)id] = w;
                }
                w->EnumerateBindings(m_Bindings);

                int column = 0;
                int row = 0;
                if (pugi::xml_attribute attr = child.attribute(L"column"))
                {
                    column = attr.as_int();
                }
                if (pugi::xml_attribute attr = child.attribute(L"row"))
                {
                    row = attr.as_int();
                }

                w->SetColumnAndRow(column, row);
                parent->PushChild(w);
                AddChildren(w.get(), child, recursionLevel + 1);
            }
        }
    }

    void View::OnDataContextPropertyChanged(std::wstring name, TaggedBindingValue val)
    {
        if (m_Bindings.find(name) != m_Bindings.end())
        {
            for (const WidgetPropertyBinding& b : m_Bindings[name])
            {
                b.pWidget->OnBoundPropertyChanged(val, name);
            }
        }
    }


    void View::Show()
    {
        int MyEnumToWin32[3]
        {
            SW_SHOWMAXIMIZED,
            SW_SHOWMINIMIZED,
            SW_SHOWNORMAL,
        };

        ShowWindow(m_hWnd, SW_SHOWDEFAULT);// MyEnumToWin32[(int)m_SUT]);
    }

    void View::SignalBoundPropertyChanged(const std::wstring& name, const TaggedBindingValue& val)
    {
        m_eBoundControlChanged(name, val);
    }

    void View::CreateFromXml()
    {
        pugi::xml_node wnd = m_Doc.child(L"Window");

        auto PushLength = [](std::vector<Length>& outVec, const wchar_t* attributeName, pugi::xml_node& node) -> void
        {
            const wchar_t* s = node.attribute(attributeName).as_string();
            outVec.push_back(ParseLength(s));
        };

        m_Bindings.clear();
        AddChildren(m_RootWidget.get(), wnd);
        m_RootWidget->LayoutChildren();
        m_RootWidget->Create(m_hWnd, { 0,0 });
    }


    LRESULT View::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CTLCOLORSTATIC:
        {
            int id = GetDlgCtrlID((HWND)lParam);
            if (s_WidgetIDMap.find((HMENU)id) != s_WidgetIDMap.end())
            {
                std::shared_ptr<Widget> w = s_WidgetIDMap[(HMENU)id];
                LabelWidget* label = (LabelWidget*)w.get();

                HDC hdcStatic = (HDC)wParam;
                Colour bg = label->GetBackgroundColour();
                Colour txt = label->GetTextColour();
                SetTextColor(hdcStatic, RGB(txt.r, txt.g, txt.b));
                SetBkColor(hdcStatic, RGB(bg.r, bg.g, bg.b));

                if (label->GetBackgroundBrush() == NULL)
                {
                    label->SetBackgroundBrush(CreateSolidBrush(RGB(bg.r, bg.g, bg.b)));
                }
                return (INT_PTR)label->GetBackgroundBrush();
            }
            return 0;

        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);
        }
        return 0;
        case WM_CREATE:
        {
            HFONT hfDefault;
            HWND hEdit;
            CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
            View* pView = (View*)pCreateStruct->lpCreateParams;
            pView->m_hWnd = hwnd;
            pView->CreateFromXml();
        }
        return 0;
        case WM_COMMAND:
        {
            WORD hiWord = HIWORD(wParam);
            HMENU controlID = (HMENU)LOWORD(wParam);
            switch (hiWord)
            {
            case BN_CLICKED:
                if(s_WidgetIDMap.find(controlID) != s_WidgetIDMap.end())
                {
                    s_WidgetIDMap[controlID]->OnCmdMesage(hiWord);
                }
                break;
            }
        }
        return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    

    


    View* View::RootWidget::GetView()
    {
        return m_pView;
    }

    View::RootWidget::RootWidget(View* view, int w, int h)
        :m_pView(view),
        m_Width(w),
        m_Height(h)
    {
    }

    float View::RootWidget::GetWidth()
    {
        return m_Width;
    }

    float View::RootWidget::GetHeight()
    {
        return m_Height;
    }

    const wchar_t* View::RootWidget::GetName()
    {
        static const wchar_t* name = L"view";
        return name;
    }

    std::shared_ptr<Widget> View::RootWidget::Factory(const pugi::xml_node& node, Widget* pParent) const
    {
        assert(false);
        return nullptr;
    }
}