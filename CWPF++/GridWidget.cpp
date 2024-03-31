#include "GridWidget.h"
#include <cassert>

namespace CWPF
{
    float GridWidget::GetWidth()
    {
        switch (m_Width.Type)
        {
        case LengthType::Stretch:
            return m_pParent->GetWidth();
        /*case LengthType::Auto: // NOT SUPPORTED*/
        case LengthType::Fixed:
            return m_Width.Value;        
        }
        assert(false);
        return 0.0;
    }

    float GridWidget::GetHeight()
    {
        switch (m_Height.Type)
        {
        case LengthType::Stretch:
            return m_pParent->GetHeight();
            /*case LengthType::Auto: // NOT SUPPORTED*/
        case LengthType::Fixed:
            return m_Height.Value;
        }
        assert(false);
        return 0.0;
    }

    const wchar_t* GridWidget::GetName()
    {
        static const wchar_t* name = L"Grid";
        return name;
    }

    std::shared_ptr<Widget> GridWidget::Factory(const pugi::xml_node& wnd, Widget* pParent) const
    {
        std::shared_ptr<Widget> g = std::make_shared<GridWidget>(pParent);
        GridWidget* pGrid = static_cast<GridWidget*>(g.get());
        for (pugi::xml_node col : wnd.child(L"Columns").children(L"Column"))
        {
            const wchar_t* s = col.attribute(L"width").as_string();
            pGrid->m_Columns.push_back(ParseLength(s));
        }

        for (pugi::xml_node row : wnd.child(L"Rows").children(L"Row"))
        {
            const wchar_t* s = row.attribute(L"height").as_string();
            pGrid->m_Rows.push_back(ParseLength(s));
        }

        if (pugi::xml_attribute att = wnd.attribute(L"width"))
        {            
            pGrid->m_Width = ParseLength(att.as_string());
        }

        if (pugi::xml_attribute att = wnd.attribute(L"height"))
        {
            pGrid->m_Height = ParseLength(att.as_string());
        }

        return g;
    }

    void GridWidget::LayoutChildren()
    {
        CalculateActualColAndRowSizes();
        for (const std::shared_ptr<Widget> w : m_Children)
        {
            IVec2 cAndR = w->GetColAndRow();
            Vec2 pos = GetWidgetPosFromColAndRow(cAndR.x, cAndR.y);
            w->SetPos(pos);
            w->LayoutChildren();
        }
    }

    Vec2 GridWidget::GetWidgetPosFromColAndRow(int col, int row) const
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
        return Vec2{ x, y };
    }

    void CWPF::GridWidget::CalculateActualColAndRowSizes()
    {
        std::vector<std::vector<Widget*>> widgetsPerRow(m_Rows.size());
        std::vector<std::vector<Widget*>> widgetsPerCol(m_Columns.size());
        for (std::shared_ptr<Widget> child : m_Children)
        {
            IVec2 cAndR = child->GetColAndRow();
            widgetsPerCol[cAndR.x].push_back(child.get());
            widgetsPerRow[cAndR.y].push_back(child.get());
        }
        int winWidth = GetWidth();
        int winHeight = GetHeight();

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

}