#include "pch.h"
#include "BindingExpressionParserLib.h"
#include "WCFCommon.h"

template<typename T>
constexpr auto TestFunctionFactory(CWPF::BindingValueType expectedType)
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
	auto test = TestFunctionFactory<float>(CWPF::BindingValueType::Flt);

	test("0.2", 0.2);
	test("42.0", 42.0);
	test("-3242.543", -3242.543);
	test("2142.2", 2142.2);
	test("42.069", 42.069);

}

TEST(LiteralValueParser, IntValues) {
	auto test = TestFunctionFactory<int>(CWPF::BindingValueType::Int);

	test("1", 1);
	test("42", 42);
	test("-3242", -3242);
	test("2142", 2142);
	test("0", 0);
}

TEST(LiteralValueParser, BoolValues) {
	auto test = TestFunctionFactory<bool>(CWPF::BindingValueType::Bool);

	test("false", false);
	test("true", true);
}

TEST(LiteralValueParser, StringValues) {
	auto test = TestFunctionFactory<std::wstring>(CWPF::BindingValueType::Str);

	test("hello world", L"hello world");
	test("true23", L"true23");
	test("false 23.23", L"false 23.23");

	test("04dfsd", L"04dfsd");
	test("42.69blah", L"42.69blah");
	test("-42.69blah", L"-42.69blah");
	test("2324dfsd", L"2324dfsd");
	test("dsfsdf 26", L"dsfsdf 26");

}