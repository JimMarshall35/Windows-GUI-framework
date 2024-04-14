#include "pch.h"
#include "BindingExpressionParserLib.h"
#include "WCFCommon.h"

template<typename T>
constexpr auto LiteralParserTestFunctionFactory(CWPF::BindingValueType expectedType)
{
	return [expectedType](std::string valueString, T value)
	{
		parser::parsed_t<CWPF::TaggedBindingValue> v = CWPF::LiteralValueParser(valueString);
		ASSERT_TRUE(v);
		EXPECT_EQ(v->second.length(), 0);
		EXPECT_EQ(v->first.VType, expectedType);
		EXPECT_EQ(std::get<T>(v->first.Value), value);
	};
}

TEST(LiteralValueParser, FloatValues) {
	auto test = LiteralParserTestFunctionFactory<float>(CWPF::BindingValueType::Flt);

	test("0.2", 0.2);
	test("42.0", 42.0);
	test("-3242.543", -3242.543);
	test("2142.2", 2142.2);
	test("42.069", 42.069);

}

TEST(LiteralValueParser, IntValues) {
	auto test = LiteralParserTestFunctionFactory<int>(CWPF::BindingValueType::Int);

	test("1", 1);
	test("42", 42);
	test("-3242", -3242);
	test("2142", 2142);
	test("0", 0);
}

TEST(LiteralValueParser, BoolValues) {
	auto test = LiteralParserTestFunctionFactory<bool>(CWPF::BindingValueType::Bool);

	test("false", false);
	test("true", true);
}

TEST(LiteralValueParser, StringValues) {
	auto test = LiteralParserTestFunctionFactory<std::wstring>(CWPF::BindingValueType::Str);

	test("hello world", L"hello world");
	test("true23", L"true23");
	test("false 23.23", L"false 23.23");

	test("04dfsd", L"04dfsd");
	test("42.69blah", L"42.69blah");
	test("-42.69blah", L"-42.69blah");
	test("2324dfsd", L"2324dfsd");
	test("dsfsdf 26", L"dsfsdf 26");
}

TEST(BindingSimpleNameParser, Test)
{
	parser::parsed_t<std::string> v = CWPF::BindingSimpleNameParser("hello");
	EXPECT_EQ(v->second.length(), 0);
	EXPECT_EQ(v->first, "hello");
	
	
	parser::parsed_t<std::string> v2 = CWPF::BindingSimpleNameParser("hello_2");
	EXPECT_EQ(v2->second.length(), 0);
	EXPECT_EQ(v2->first, "hello_2");

	parser::parsed_t<std::string> v3 = CWPF::BindingSimpleNameParser("3hello");
	EXPECT_EQ(v3, std::nullopt);

	parser::parsed_t<std::string> v4 = CWPF::BindingSimpleNameParser(" hello");
	EXPECT_EQ(v4, std::nullopt);

	parser::parsed_t<std::string> v5 = CWPF::BindingSimpleNameParser("hello world");
	EXPECT_EQ(v5->first, "hello");
	EXPECT_EQ(v5->second.length(), 6);

	parser::parsed_t<std::string> v6 = CWPF::BindingSimpleNameParser("_hello+world");
	EXPECT_EQ(v6->first, "_hello");
	EXPECT_EQ(v6->second.length(), 6);

}

TEST(StaticResourceExpressionParser, Test)
{
	auto test = [](const std::string& testString, const std::wstring& expectedName, bool expectedParseSuccess = true, int expectedRemainingStringLength = 0) 
	{
		parser::parsed_t<CWPF::StaticResourceInfo> v = CWPF::StaticResourceExpressionParser(testString);
		if (expectedParseSuccess)
		{
			EXPECT_EQ(v->first.val, expectedName);
			EXPECT_EQ(v->second.length(), expectedRemainingStringLength);

		}
		else
		{
			EXPECT_EQ(v, std::nullopt);
		}
	};
	
	test("{ StaticResource name }", L"name");
	test("{StaticResource name}", L"name");
	test("{StaticResource name2}", L"name2");
	test("{StaticResource _name2}", L"_name2");

	test("{          StaticResource           name              }", L"name");
	test("{StaticResource name", L"",false);
	test(" {StaticResource name}", L"", false);
	test("StaticResource name}", L"", false);
	test("{StaticResource 4name}", L"", false);
	test(" {StaticResource name name2}", L"", false);
	test("{staticresource name}", L"", false);

}