#pragma once
#include "WCFCommon.h"
#include "Widget.h"
namespace CWPF
{
	class ICommand;

	struct ButtonWidgetInitArgs
	{
		std::wstring Text;
		Alignment Alignment;
		int width;
		int height;
		std::wstring BoundPropertyName;
	};

	class ButtonWidget : public Widget
	{
	public:
		ButtonWidget() {};
		ButtonWidget(Widget* pParent, const ButtonWidgetInitArgs& args);

		// Inherited via Widget
		virtual void Create(HWND hwnd, const Vec2& pos) override;
		virtual float GetWidth() override;
		virtual float GetHeight() override;
		virtual const wchar_t* GetName() override;
		virtual std::shared_ptr<Widget> Factory(const pugi::xml_node& node, Widget* pParent) const override;
		virtual void EnumerateBindings(std::map<std::wstring, std::vector<WidgetPropertyBinding>>& bindingsMap) const override;
		virtual void OnBoundPropertyChanged(const TaggedBindingValue& val, const std::wstring& name) override;
		virtual void OnCmdMesage(int msg) override; 
	protected:
		std::wstring m_Text;
		int m_Width;
		int m_Height;
		std::wstring m_BoundPropertyName = L"";
		ICommand* m_pCmd = nullptr;
	};

}