#include "BindingExpressionParserLib.h"


namespace CWPF
{

	auto CWPF_API Concat(std::string_view s, std::string_view s2) -> std::string
	{
		return std::string(s) + std::string(s2);
	}

	bool CWPF_API isdigitWrapper(char c)
	{
		return isdigit(c);
	}

	float CWPF_API toFloat(std::string s)
	{
		return atof(s.c_str());
	}
	
	bool CWPF_API IsValidStringCharacter(char c)
	{
		return true;
	}

	std::wstring CWPF_API sv2ws(const std::string_view& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}
	BindingType BindingTypeFromString(const std::string& s)
	{
		if (s == "OneWay")
		{
			return BindingType::OneWay;
		}
		else if (s == "TwoWay")
		{
			return BindingType::TwoWay;
		}
		else if (s == "OneWayToSource")
		{
			return BindingType::OneWayToSource;
		}
		else if (s == "OneTime")
		{
			return BindingType::OneTime;
		}
		else if (s == "Default")
		{
			return BindingType::Default;
		}
		return BindingType::Undefined;
	}
	BindingInfo MergeBindingInfos(BindingInfo a, BindingInfo b)
	{
		return BindingInfo();
	}
}