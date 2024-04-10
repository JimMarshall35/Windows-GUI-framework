#pragma once
#include <string>
#include <map>
#include "WCFCommon.h"
#include "Command.h"

namespace CWPF
{
	



#define DECLARE_BINDABLE_PROPERTY_CHANGED_EVENT public:\
	Event<std::wstring, TaggedBindingValue> BindablePropertyChanged; // name, value

#define DECLARE_BINDABLE_PROPERTY_SETTER_MAP public:\
	std::map<std::wstring, std::function<void(TaggedBindingValue)>> BindablePropertySettersByName;

#define DECLARE_BINDABLE_PROPERTY(BindingName, mBindingType) \
	mBindingType Get_##BindingName();\
	void Set_##BindingName(mBindingType t);

#define IMPLEMENT_BINDABLE_COMMAND_PROPERTY(ViewmodelClassname, BindingName, BackingFieldName) \
	ICommand* ViewmodelClassname::Get_##BindingName()\
	{\
		return BackingFieldName;\
	}\
	void ViewmodelClassname::Set_##BindingName(ICommand* t) \
	{\
		BackingFieldName = t;\
		TaggedBindingValue v = TaggedBindingValue{ {nullptr}, BindingValueType::Command };\
		v.Value = t;\
		BindablePropertyChanged(L#BindingName, v);\
	}

#define IMPLEMENT_BINDABLE_WSTRING_PROPERTY(ViewmodelClassname, BindingName, BackingFieldName) \
	const wchar_t* ViewmodelClassname::Get_##BindingName()\
	{\
		return BackingFieldName;\
	}\
	void ViewmodelClassname::Set_##BindingName(const std::wstring& t) \
	{\
		BackingFieldName = t;\
		BindablePropertyChanged(L#BindingName, TaggedBindingValue{ {t}, BindingValueType::Str });\
	}

#define IMPLEMENT_BINDABLE_INT_PROPERTY(ViewmodelClassname, BindingName, BackingFieldName) \
	int ViewmodelClassname::Get_##BindingName()\
	{\
		return BackingFieldName;\
	}\
	void ViewmodelClassname::Set_##BindingName(int i) \
	{\
		BackingFieldName = i;\
		TaggedBindingValue v = TaggedBindingValue{ {nullptr}, BindingValueType::Int };\
		v.Value = i;\
		BindablePropertyChanged(L#BindingName, v);\
	}

#define IMPLEMENT_BINDABLE_BOOL_PROPERTY(ViewmodelClassname, BindingName, BackingFieldName) \
	bool ViewmodelClassname::Get_##BindingName()\
	{\
		return BackingFieldName;\
	}\
	void ViewmodelClassname::Set_##BindingName(bool b) \
	{\
		BackingFieldName = b;\
		TaggedBindingValue v = TaggedBindingValue{ {nullptr}, BindingValueType::Bool };\
		v.Value = b;\
		BindablePropertyChanged(L#BindingName, v);\
	}

#define IMPLEMENT_BINDABLE_FLOAT_PROPERTY(ViewmodelClassname, BindingName, BackingFieldName) \
	float ViewmodelClassname::Get_##BindingName()\
	{\
		return BackingFieldName;\
	}\
	void ViewmodelClassname::Set_##BindingName(float f) \
	{\
		BackingFieldName = f;\
		TaggedBindingValue v = TaggedBindingValue{ {nullptr}, BindingValueType::Flt };\
		v.Value = f;\
		BindablePropertyChanged(L#BindingName, v);\
	}

#define REGISTER_BINDABLE_INT_SETTER(BindingName, BackingFieldName)\
	BindablePropertySettersByName[std::wstring(L#BindingName)] = std::function<void(TaggedBindingValue)>([this](TaggedBindingValue v){\
		BackingFieldName = std::get<int>(v.Value);\
	});

#define REGISTER_BINDABLE_BOOL_SETTER(BindingName, BackingFieldName)\
	BindablePropertySettersByName[std::wstring(L#BindingName)] = std::function<void(TaggedBindingValue)>([this](TaggedBindingValue v){\
		BackingFieldName = std::get<bool>(v.Value);\
	});

#define REGISTER_BINDABLE_WSTRING_SETTER(BindingName, BackingFieldName)\
	BindablePropertySettersByName[std::wstring(L#BindingName)] = std::function<void(TaggedBindingValue)>([this](TaggedBindingValue v){\
		BackingFieldName = std::get<std::wstring>(v.Value);\
	});

#define REGISTER_BINDABLE_FLOAT_SETTER(BindingName, BackingFieldName)\
	BindablePropertySettersByName[std::wstring(L#BindingName)] = std::function<void(TaggedBindingValue)>([this](TaggedBindingValue v){\
		BackingFieldName = std::get<float>(v.Value);\
	});

	class ViewModel
	{
	public:
		DECLARE_BINDABLE_PROPERTY_CHANGED_EVENT
		DECLARE_BINDABLE_PROPERTY_SETTER_MAP
		ViewModel();
		void SubscribeToBoundPropertySetExternalEvent(Event<std::wstring, TaggedBindingValue>& e);
		void UnSubscribeFromBoundPropertySetExternalEvent(Event<std::wstring, TaggedBindingValue>& e);
		virtual void InitializeProperties() = 0;
	protected:
		void OnBoundPropertySetExternal(std::wstring name, TaggedBindingValue val);
		EventSubscriptionToken m_hViewSubscription = 0;
	};

	class TestViewModelMain : public ViewModel
	{
	public:
		TestViewModelMain();
		~TestViewModelMain();
		virtual void InitializeProperties() override;
		DECLARE_BINDABLE_PROPERTY(IntProperty, int)
		DECLARE_BINDABLE_PROPERTY(BoolProperty, bool)
		//DECLARE_BINDABLE_PROPERTY(StringProperty, const wchar_t*)
		DECLARE_BINDABLE_PROPERTY(FloatProperty, float)
		DECLARE_BINDABLE_PROPERTY(CmdProperty, ICommand*)

	private:
		int m_IntProperty = 0;
		bool m_bBoolProperty = false;
		const wchar_t* m_StringProperty;
		float m_FloatProperty = 0.0f;
		ICommand* m_pCmd = nullptr;
		wchar_t m_pStringBuf[256];
	};
}



