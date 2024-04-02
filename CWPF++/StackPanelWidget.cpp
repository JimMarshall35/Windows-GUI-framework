#include "StackPanelWidget.h"
#include <cassert>

namespace CWPF {

    StackPanelWidget::StackPanelWidget(Widget* pParent, const StackPanelWidgetInitArgs& args)
        : Widget(pParent), m_Orientation(args.Orientation)
    {

    }

    std::shared_ptr<Widget> StackPanelWidget::FactoryImplementation(const pugi::xml_node& node, Widget* pParent) const
    {
        StackPanelWidgetInitArgs args;
        const wchar_t* str = node.attribute(L"orientation").as_string();
        if (str)
        {
            if (wcscmp(str, L"horizontal") == 0)
            {
                args.Orientation = StackPanelOrientation::Horizontal;
            }
            else if (wcscmp(str, L"vertical") == 0)
            {
                args.Orientation = StackPanelOrientation::Vertical;
            }
            else
            {
                assert(false);
            }
            if (args.Orientation != StackPanelOrientation::Invalid)
            {
                return std::shared_ptr<Widget>(new StackPanelWidget(pParent, args));
            }
        }
        return nullptr;
    }

    float StackPanelWidget::GetWidth()
    {
        switch (m_Orientation)
        {
        case StackPanelOrientation::Horizontal:
        {
            float total = 0;
            for (const std::shared_ptr<Widget>& w : m_Children)
            {
                total += w->GetWidth();
            }
            return total;
        }
        case StackPanelOrientation::Vertical:
            float widest = 0;
            for (const std::shared_ptr<Widget>& w : m_Children)
            {
                float width = w->GetWidth();
                if (width > widest)
                {
                    widest = width;
                }

            }
            return widest;
        }
        assert(false);
        return 0.0f;
    }

    float StackPanelWidget::GetHeight()
    {
        switch (m_Orientation)
        {
        case StackPanelOrientation::Horizontal:
        {
            float tallest = 0;
            for (const std::shared_ptr<Widget>& w : m_Children)
            {
                float height = w->GetHeight();
                if (height > tallest)
                {
                    tallest = height;
                }

            }
            return tallest;
        }
        case StackPanelOrientation::Vertical:
            float total = 0;
            for (const std::shared_ptr<Widget>& w : m_Children)
            {
                total += w->GetHeight();
            }
            return total;
        }
        assert(false);
        return 0.0f;
    }

    const wchar_t* StackPanelWidget::GetName()
    {
        static const wchar_t* name = L"StackPanel";
        return name;
    }

    void StackPanelWidget::LayoutChildren()
    {
        switch (m_Orientation)
        {
        case StackPanelOrientation::Horizontal:
        {
            float offset = 0;

            for (std::shared_ptr<Widget>& child : m_Children)
            {
                if (child)
                {
                    child->SetPos(Vec2{ m_Pos.x + offset, m_Pos.y });
                    offset += child->GetWidth();
                }
            }
            break;
        }

        case StackPanelOrientation::Vertical:
        {
            float offset = 0;

            for (std::shared_ptr<Widget>& child : m_Children)
            {
                if (child)
                {
                    child->SetPos(Vec2{ m_Pos.x , m_Pos.y + offset });
                    offset += child->GetHeight();
                }
            }
            break;
        }
        default:
            assert(false);
            break;
        }
    }

}