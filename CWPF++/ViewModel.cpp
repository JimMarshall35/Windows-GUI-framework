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
		assert(BindablePropertySettersByName.find(name) != BindablePropertySettersByName.end());
		BindablePropertySettersByName[name](val);
	}

	TestViewModelMain::TestViewModelMain()
	{
		REGISTER_BINDABLE_INT_SETTER(IntProperty, m_IntProperty)
		REGISTER_BINDABLE_BOOL_SETTER(BoolProperty, m_bBoolProperty)
		//REGISTER_BINDABLE_WSTRING_SETTER(StringProperty, m_StringProperty)
		REGISTER_BINDABLE_FLOAT_SETTER(FloatProperty, m_FloatProperty)

	}

	TestViewModelMain::~TestViewModelMain()
	{
		if (m_pCmd)
		{
			delete m_pCmd;
		}
	}

	void TestViewModelMain::InitializeProperties()
	{
		Set_IntProperty(2);
		Set_BoolProperty(true);
		wcsncpy_s(m_pStringBuf, L"hello world", 256);
		//Set_StringProperty(m_pStringBuf);
		Set_FloatProperty(420.0f);
		if (Get_CmdProperty())
		{
			assert(false);
		}
		Set_CmdProperty(new Command([this](void* e) -> void
		{
			printf("Command!\n");
		}));
	}

	IMPLEMENT_BINDABLE_INT_PROPERTY(TestViewModelMain, IntProperty, m_IntProperty)
	IMPLEMENT_BINDABLE_BOOL_PROPERTY(TestViewModelMain, BoolProperty, m_bBoolProperty)
	//IMPLEMENT_BINDABLE_WSTRING_PROPERTY(TestViewModelMain, StringProperty, m_StringProperty)
	IMPLEMENT_BINDABLE_FLOAT_PROPERTY(TestViewModelMain, FloatProperty, m_FloatProperty)
	IMPLEMENT_BINDABLE_COMMAND_PROPERTY(TestViewModelMain, CmdProperty, m_pCmd)

}


