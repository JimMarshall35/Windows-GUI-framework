#include "CommonWidgets.h"
#include "EditWidget.h"
#include "LabelWidget.h"
#include "StackPanelWidget.h"
#include "ButtonWidget.h"
#include "GridWidget.h"

void CWPF::RegisterCommonWidgets()
{
    CWPF::Widget::RegisterWidget<CWPF::ButtonWidget>();
    CWPF::Widget::RegisterWidget<CWPF::StackPanelWidget>();
    CWPF::Widget::RegisterWidget<CWPF::EditWidget>();
    CWPF::Widget::RegisterWidget<CWPF::LabelWidget>();
    CWPF::Widget::RegisterWidget<CWPF::GridWidget>();
}
