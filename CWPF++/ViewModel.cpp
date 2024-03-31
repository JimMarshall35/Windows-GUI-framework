#include "ViewModel.h"

namespace CWPF
{
	ViewModel::ViewModel()
	{
	}

	void ViewModel::SubscribeToBoundPropertySetExternalEvent(Event<std::wstring, TaggedBindingValue>& e)
	{
		m_hViewSubscription = e += std::function<void(std::wstring, TaggedBindingValue)>([this](std::wstring s, TaggedBindingValue v)
		{
			OnBoundPropertySetExternal(s, v);
		});
	}

	void ViewModel::UnSubscribeFromBoundPropertySetExternalEvent(Event<std::wstring, TaggedBindingValue>& e)
	{
		if (m_hViewSubscription)
		{
			e -= m_hViewSubscription;
			m_hViewSubscription = 0;
		}
	}

	void ViewModel::OnBoundPropertySetExternal(std::wstring name, TaggedBindingValue val)
	{
	}

	TestViewModelMain::TestViewModelMain()
	{
	}

	IMPLEMENT_BINDABLE_INT_PROPERTY(TestViewModelMain, IntProperty, m_IntProperty)
	IMPLEMENT_BINDABLE_BOOL_PROPERTY(TestViewModelMain, BoolProperty, m_bBoolProperty)
	IMPLEMENT_BINDABLE_WSTRING_PROPERTY(TestViewModelMain, StringProperty, m_StringProperty)
	IMPLEMENT_BINDABLE_FLOAT_PROPERTY(TestViewModelMain, FloatProperty, m_FloatProperty)


}


