#pragma once

#include <variant>
#include <string>

#include "WCFCommon.h"
#include "parser/parser.hpp"

#include <string.h>
#include <functional>
#include <vector>
#include <codecvt>
#include <locale>
/*
	*
	*   BINDING LANGUAGE GRAMMAR
	***********************************
	s
	EXAMPLE 'SENTENCE'

	 {Binding      path=       BindingPath         ,   converter=     {StaticResource    StaticResourceName     }   }
	   |            |              |               |        |                 |                |                |   |
	  '{Binding'  'path='    <bindingSimpleName>  ','  'convertor='  '{StaticResource'   <bindingSimpleName>   '}' '}'
		   \           \     /                      \              \              \            |              /   /
			\        <pathPair>                      \              \               <StaticResourceExpression>  /
			 \           |                            \              \             /                          /
			  \ <bindingExpressionPair>                \             <converterPair>                        /
			   \         \                              \                   |                             /
				\         \                              \         <bindingExpressionPair>              /
				 \         \                              \       /                                   /
				  \         \					 ( ',' <bindingExpressionPair>)+                    /
				   \	     \				            /                                         /
					\ <bindingExpressionPair>( ',' <bindingExpressionPair>)+                    /
					  \                                  |                                    /
						\		                         |                                  /
						  \                              |                                /
							\                            |                              /
							  \                          |                            /
								\                        |                          /
								  \                      |                        /
									\                    |                      /
									  \                  |                    /
										\                |                  /
										  \              |                /
											\            |              /
											  \          |            /
												\        |          /
												  \      |        /
													\    |      /
												<bindingExpression>
														 |
													 <attribute>



	<attribute> = <bindingExpression> | <StaticResourceExpression> | <literalValue>


	<bindingExpression> = '{Binding' (<bindingSimpleName> | <bindingExpressionPair> | <bindingExpressionPair> ( ',' <bindingExpressionPair>)+ ) '}'
	<bindingExpressionPair> = <pathPair> | <modePair> | <converterPair> | <sourcePair>
	<pathPair> = 'path=' <bindingSimpleName>
	<modePair> = 'mode=' ('OneWay' | 'TwoWay' | 'OneWayToSource' | 'OneTime' | 'Default)
	<converterPair> = 'convertor='<StaticResource>
	<sourcePair> = 'source='<StaticResource>

	<bindingExpressionKey> = 'path' | 'mode' | 'source' | 'convertor'

	<StaticResourceExpression> = '{StaticResource' <bindingSimpleName> '}'

	<bindingSimpleName> = <alphabetic> (<alphanumeric>)*


	<literalValue> = <sint> | <float> | <stringLit> | <boolLit>
	<float> = <sint> '.' <numeric>+
	<sint> = ['-'] <uint>
	<uint> = '0' | (<nonzero> (<numeric>)*)
	<boolLit> = 'true' | 'false'
	<stringLit> = <emptyString> | <alphanumeric><stringLit>

	<nonzero> = '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
	<numeric> = <nonzero> | '0'
	<alphabetic> = 'a' ! 'b' | 'c |                               ... ect, including caps
	<whilespace> =
	<specialChars> =
	<emptyString> =
*/
namespace CWPF
{
	struct BindingInfo
	{
		BindingType type;
		std::wstring path;
		std::wstring converter;
		std::wstring src;
	};

	struct StaticResourceInfo
	{
		std::wstring val;
	};

	enum class AttributeType
	{
		LiteralValue,
		Binding,
		StaticResource
	};

	struct Attribute
	{
		AttributeType Type;
		std::variant<TaggedBindingValue, BindingInfo, StaticResourceInfo> v;
	};


	/* parser combinators */
	
	/*
	* parse only succeeds if it manages to parse the entire string
	*/
	template<parser::Parser P> constexpr auto MustConsumeAll(P&& p) {
		return[p = std::forward<P>(p)](std::string_view str)-> parser::parser_result_t<P> {
			auto res = std::invoke(p, str);
			if (!res) return {};
			if (std::empty(res->second)) 
			{
				return std::make_pair(res->first, res->second);
			}

			return std::nullopt;
		};
	}
	constexpr auto must_consume_all =
		parser::piped([]<typename P>(P && p) { return MustConsumeAll(p); });




	/////////////////////////////////////////////////////////////////////////////////////// bool


	constexpr auto BoolStringParser = parser::str("true") | parser::or_with(parser::str("false"));
	constexpr auto BoolParser = BoolStringParser | parser::transform([](const std::string_view s) {return s[0] == 't' ? true : false; });
	constexpr auto BoolUnionParser = BoolParser |
		parser::transform([](bool b) { return TaggedBindingValue{ {b}, BindingValueType::Bool }; }) |
		must_consume_all();




	/////////////////////////////////////////////////////////////////////////////////////// int

	constexpr auto CharToInt(char c) { return c - '0'; }

	constexpr auto AppendDigits(int a, char b) { return a * 10 + CharToInt(b); }

	constexpr auto AppendDigitsNegating(int a, char b) { int val = (a * 10 + CharToInt(b));  return val > 0 ? -val : val; }

	constexpr auto AppendDigitsToString(std::string s, char c)
	{
		s += c;
		return s;
	}


	constexpr auto DigitParser = parser::one_of("0123456789");

	constexpr auto PositiveIntParser =
		parser::one_of("123456789") |
		parser::then([](char c) {
		return parser::many(DigitParser, CharToInt(c), AppendDigits);
	});

	constexpr auto NegativeIntParser = parser::symbol('-') |
		parser::ignore_previous(parser::one_of("123456789") |
			parser::then([](char c) {
		return parser::many(DigitParser, CharToInt(c), AppendDigitsNegating);
	}));

	constexpr auto SignedIntParser = PositiveIntParser | parser::or_with(NegativeIntParser);
	constexpr auto IntUnionParser = SignedIntParser |
		parser::transform([](int i) { return TaggedBindingValue{ {i}, BindingValueType::Int }; }) |
		must_consume_all();;


	/////////////////////////////////////////////////////////////////////////////////////// float


	constexpr auto PositiveIntToStringParser =
		parser::one_of("123456789")//
		| parser::then([](char c) {
		return parser::many(DigitParser, std::string("") + c, AppendDigitsToString);
	});

	constexpr auto NegativeIntToStringParser = parser::symbol('-') | parser::ignore_previous(parser::one_of("123456789")//
		| parser::then([](char c) {
		return parser::many(DigitParser, std::string("-") + c, AppendDigitsToString);
	}));

	constexpr auto SignedIntToStringParser = PositiveIntToStringParser | parser::or_with(NegativeIntToStringParser);


	auto Concat(std::string_view s, std::string_view s2)->std::string;
	bool isdigitWrapper(char c);
	float toFloat(std::string s);
	bool IsValidStringCharacter(char c);
	std::wstring sv2ws(const std::string_view& str);

	constexpr auto MantissaParser = parser::many1_if(isdigitWrapper);

	constexpr auto FloatParser = SignedIntToStringParser |
		parser::combine_with(parser::str("."), Concat) |
		parser::combine_with(MantissaParser, Concat) |
		parser::transform(toFloat);

	constexpr auto FloatUnionParser = FloatParser |
		parser::transform([](float f) { return TaggedBindingValue{ {f}, BindingValueType::Flt }; }) |
		must_consume_all();;

	/////////////////////////////////////////////////////////////////////////////////////// string parser

	constexpr auto StringParser = parser::many1_if(IsValidStringCharacter);
	constexpr auto StringUnionParser = StringParser |
		parser::transform([](std::string_view s) { return TaggedBindingValue{/* TODO: get rid of this by changing parsing lib to use wstring_view */{sv2ws(s)}, BindingValueType::Str}; }) |
		must_consume_all();

	/////////////////////////////////////////////////////////////////////////////////////// literal value

	constexpr auto LiteralValueParser = BoolUnionParser |
		parser::or_with(FloatUnionParser) |
		parser::or_with(IntUnionParser) |
		parser::or_with(StringUnionParser);

	/////////////////////////////////////////////////////////////////////////////////////// binding simple name

	//constexpr auto BindingSimpleNameParser = parser::one_of(
	//	"abcdefghijklmnopqrstuvwxyz"
	//	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	//	"_"
	//) |
	//	parser::combine_with();
}