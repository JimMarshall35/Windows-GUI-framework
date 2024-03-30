#include "View.h"
#include <cassert>
#include <vector>
#include <map>

namespace CWPF
{
    std::map<HWND, View*> sViewInstanceToHWND;

    
    
    View::View(const ViewInitArgs& args)
        :m_RootWidget(std::unique_ptr<Widget>(new RootWidget(this))),
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
    }

    void View::Create()
    {

    }

    Vec2 View::GetWidgetPosFromColAndRow(int col, int row) const
    {
        float x = 0;
        for (int i = 0; i < col; i++)
        {
            x += m_Columns[i].Value;
        }
        float y = 0;
        for (int i = 0; i < row; i++)
        {
            y += m_Rows[i].Value;
        }
        return Vec2{x, y};
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
                if (recursionLevel == 0)
                {
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
                    assert(row < m_Rows.size());
                    assert(column < m_Columns.size());

                    w->SetColumnAndRow(column, row);
                }
                parent->PushChild(w);
                AddChildren(w.get(), child, recursionLevel + 1);
            }
        }
    }

    static Length ParseLength(const wchar_t* val)
    {
        Length l = {};
        if (wcslen(val) == 1 && val[0] == '*')
        {
            l.Type = LengthType::Stretch;
            l.Value = 0.0;
        }
        else if (wcscmp(val, L"auto") == 0)
        {
            l.Type = LengthType::Auto;
            l.Value = 0.0;
        }
        else
        {
            l.Type = LengthType::Fixed;
            try
            {
                l.Value = std::stof(val);
            }
            catch (...)
            {
                assert(false);
                l.Type = LengthType::Invalid;
            }
        }
        return l;
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

    void View::CreateFromXml()
    {
        pugi::xml_node wnd = m_Doc.child(L"Window");

        auto PushLength = [](std::vector<Length>& outVec, const wchar_t* attributeName, pugi::xml_node& node) -> void
        {
            const wchar_t* s = node.attribute(attributeName).as_string();
            outVec.push_back(ParseLength(s));
        };


        for (pugi::xml_node col : wnd.child(L"Columns").children(L"Column"))
        {
            PushLength(m_Columns, L"width", col);
        }

        for (pugi::xml_node row : wnd.child(L"Rows").children(L"Row"))
        {
            PushLength(m_Rows, L"height", row);
        }

        AddChildren(m_RootWidget.get(), wnd);
        CalculateActualColAndRowSizes();
        m_RootWidget->LayoutChildren();
        m_RootWidget->Create(m_hWnd, { 0,0 });
    }

    void View::CalculateActualColAndRowSizes()
    {
        std::vector<std::vector<Widget*>> widgetsPerRow(m_Rows.size());
        std::vector<std::vector<Widget*>> widgetsPerCol(m_Columns.size());
        const WidgetVec& children = m_RootWidget->GetChildren();
        for (std::shared_ptr<Widget> child : children)
        {
            IVec2 cAndR = child->GetColAndRow();
            widgetsPerCol[cAndR.x].push_back(child.get());
            widgetsPerRow[cAndR.y].push_back(child.get());
        }
        RECT winRect;
        GetWindowRect(m_hWnd, &winRect);
        int winWidth = winRect.right - winRect.left;
        int winHeight = winRect.bottom - winRect.top;
        
        int total = 0;
        std::vector<int> stretchRows;
        for (int i = 0; i < m_Rows.size(); i++)
        {
            switch (m_Rows[i].Type)
            {
            case LengthType::Fixed:
                total += m_Rows[i].Value;
                break;
            case LengthType::Stretch:
                stretchRows.push_back(i);
                break;
            case LengthType::Auto:
            {
                int tallestRow = 0;
                for (Widget* w : widgetsPerRow[i])
                {
                    float height = w->GetHeight();
                    if (height > tallestRow)
                    {
                        tallestRow = height;
                    }
                }
                total += tallestRow;
                m_Rows[i].Value = tallestRow;
                break;
            }
            }
        }

        float totalStretchRowSize = winHeight - total;
        float stretchRowHeight = totalStretchRowSize / stretchRows.size();
        for (int i : stretchRows)
        {
            m_Rows[i].Value = stretchRowHeight;
        }

        total = 0;
        std::vector<int> stretchCols;
        for (int i = 0; i < m_Columns.size(); i++)
        {
            switch (m_Columns[i].Type)
            {
            case LengthType::Fixed:
                total += m_Columns[i].Value;
                break;
            case LengthType::Stretch:
                stretchCols.push_back(i);
                break;
            case LengthType::Auto:
            {
                int widestCol = 0;
                for (Widget* w : widgetsPerCol[i])
                {
                    float width = w->GetWidth();
                    if (width > widestCol)
                    {
                        widestCol = width;
                    }
                }
                total += widestCol;
                m_Columns[i].Value = widestCol;
                break;
            }
            }
        }

        float totalStretchColSize = winWidth - total;
        float stretchColWidth = totalStretchColSize / stretchCols.size();
        for (int i : stretchCols)
        {
            m_Columns[i].Value = stretchColWidth;
        }
    }

    LRESULT View::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
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
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    

    


    View* View::RootWidget::GetView()
    {
        return m_pView;
    }

    View::RootWidget::RootWidget(View* view)
        :m_pView(view)
    {
    }

    float View::RootWidget::GetWidth()
    {
        return 0.0f;
    }

    float View::RootWidget::GetHeight()
    {
        return 0.0f;
    }

    const wchar_t* View::RootWidget::GetName()
    {
        static const wchar_t* name = L"view";
        return name;
    }

    std::shared_ptr<Widget> View::RootWidget::Factory(const pugi::xml_node& node, Widget* pParent) const
    {
        return nullptr;
    }

    void View::RootWidget::LayoutChildren()
    {
        for (const std::shared_ptr<Widget> w : m_Children)
        {
            IVec2 cAndR = w->GetColAndRow();
            Vec2 pos = m_pView->GetWidgetPosFromColAndRow(cAndR.x, cAndR.y);
            w->SetPos(pos);
            w->LayoutChildren();
        }
    }
}