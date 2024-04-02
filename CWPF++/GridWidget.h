#pragma once
#include "Widget.h"
#include "Vec2.h"
#include "WCFCommon.h"

namespace CWPF
{

	class GridWidget : public Widget
	{
	public:
		GridWidget() {}
		GridWidget(Widget* pParent) :Widget(pParent) {}

		// Inherited via Widget
		virtual float GetWidth() override;
		virtual float GetHeight() override;
		virtual const wchar_t* GetName() override;
		virtual void LayoutChildren();
	protected:
		virtual std::shared_ptr<Widget> FactoryImplementation(const pugi::xml_node& node, Widget* pParent) const override;
		Vec2 GetWidgetPosFromColAndRow(int col, int row) const;
		void CalculateActualColAndRowSizes();
	protected:
		std::vector<Length> m_Columns;
		std::vector<Length> m_Rows;
		Length m_Width = { LengthType::Stretch }; 
		Length m_Height = { LengthType::Stretch };

	};

}