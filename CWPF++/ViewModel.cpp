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

}


