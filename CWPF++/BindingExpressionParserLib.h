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
		BindingType type = BindingType::Undefined;
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

	constexpr auto AppendDigitsNegating(int a, char b) {
		int val = (a * 10 + CharToInt(b)); return val; }

	constexpr auto AppendDigitsToString(std::string s, char c)
	{
		s += c;
		return s;
	}


	constexpr auto DigitParser = parser::one_of("0123456789");

	constexpr auto ZeroIntParser = 
		parser::one_of("0") |
		parser::then([](char c) {
			return parser::many(DigitParser, CharToInt(c), AppendDigits);
		});

	constexpr auto PositiveIntParser =
		parser::one_of("123456789") |
		parser::then([](char c) {
			return parser::many(DigitParser, CharToInt(c), AppendDigits);
		});

	constexpr auto NegativeIntParser = parser::symbol('-') |
		parser::ignore_previous(parser::one_of("123456789")) |
		parser::then([](char c) {
			return parser::many(DigitParser, CharToInt(c), AppendDigitsNegating);
		}) | 
		parser::transform([](int v) { return -v; });

	constexpr auto SignedIntParser = ZeroIntParser | 
		parser::or_with(PositiveIntParser) | 
		parser::or_with(NegativeIntParser);

	constexpr auto IntUnionParser = SignedIntParser |
		parser::transform([](int i) { return TaggedBindingValue{ {i}, BindingValueType::Int }; }) |
		must_consume_all();;


	/////////////////////////////////////////////////////////////////////////////////////// float
	
	constexpr auto ZeroParser = parser::one_of("0") |
	parser::then([](char c) {
		return parser::many(DigitParser, std::string("") + c, AppendDigitsToString);
	});

	constexpr auto PositiveIntToStringParser =
		parser::one_of("123456789") |
		parser::then([](char c) {
			return parser::many(DigitParser, std::string("") + c, AppendDigitsToString);
		});

	constexpr auto NegativeIntToStringParser = parser::symbol('-') | 
		parser::ignore_previous(parser::one_of("123456789") |
		parser::then([](char c) {
			return parser::many(DigitParser, std::string("-") + c, AppendDigitsToString);
		}));

	constexpr auto SignedIntToStringParser = ZeroParser |
		parser::or_with(PositiveIntToStringParser) |
		parser::or_with(NegativeIntToStringParser);


	auto CWPF_API Concat(std::string_view s, std::string_view s2)->std::string;
	bool CWPF_API isdigitWrapper(char c);
	float CWPF_API toFloat(std::string s);
	bool CWPF_API IsValidStringCharacter(char c);
	std::wstring CWPF_API sv2ws(const std::string_view& str);

	constexpr auto MantissaParser = parser::many1_if(isdigitWrapper);

	constexpr auto FloatParser = SignedIntToStringParser |
		parser::combine_with(parser::str("."), Concat) |
		parser::combine_with(MantissaParser, Concat) |
		parser::transform(toFloat);

	constexpr auto FloatUnionParser = FloatParser |
		parser::transform([](float f) { return TaggedBindingValue{ {f}, BindingValueType::Flt }; }) |
		must_consume_all();;

	/////////////////////////////////////////////////////////////////////////////////////// string parser

	constexpr auto StringParser = parser::many_if(IsValidStringCharacter);
	constexpr auto StringUnionParser = StringParser |
		parser::transform([](std::string_view s) { return TaggedBindingValue{/* TODO: get rid of this by changing parsing lib to use wstring_view */{sv2ws(s)}, BindingValueType::Str}; }) |
		must_consume_all();

	/////////////////////////////////////////////////////////////////////////////////////// literal value

	constexpr auto LiteralValueParser = BoolUnionParser |
		parser::or_with(FloatUnionParser) |
		parser::or_with(IntUnionParser) |
		parser::or_with(StringUnionParser);

	/////////////////////////////////////////////////////////////////////////////////////// binding simple name

	constexpr auto BindingSimpleNameCharacters = parser::one_of(
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"_"
		"0123456789"
	);
	constexpr auto BindingSimpleNameCharacters_FirstChar = parser::one_of(
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"_"
	);

	auto BindingSimpleNameParser = BindingSimpleNameCharacters_FirstChar |
		parser::combine_with(
			parser::many(BindingSimpleNameCharacters, std::string(""), AppendDigitsToString), 
			[](char c, std::string s) {return std::string("") + c + s; });

	/////////////////////////////////////////////////////////////////////////////////////// Static resource Expression

	auto OneOrMoreSpacesParser = parser::one_of(" ") |
		parser::then([](char c) {
			return parser::many(parser::one_of(" "), std::string("") + c, AppendDigitsToString);
		});

	auto OptionalSpace = parser::many(parser::one_of(" "), std::string(""), AppendDigitsToString);


	auto ExpressionEndParser = OptionalSpace |
		parser::then([](std::string s) {
			return parser::one_of("}");
		});

	auto StaticResourceExpressionParser = parser::one_of("{") |
		parser::ignore_previous(OptionalSpace) |
		parser::ignore_previous(parser::str("StaticResource")) |
		parser::ignore_previous(OneOrMoreSpacesParser) |
		parser::ignore_previous(BindingSimpleNameParser) |
		parser::transform([](std::string s) { return StaticResourceInfo{ sv2ws(s) }; }) |
		parser::ignore(ExpressionEndParser);


	/////////////////////////////////////////////////////////////////////////////////////// Binding Expression


	/*
		<bindingExpression> = '{Binding' (<bindingSimpleName> | <bindingExpressionPair> | <bindingExpressionPair> ( ',' <bindingExpressionPair>)+ ) '}'
		<bindingExpressionPair> = <pathPair> | <modePair> | <converterPair> | <sourcePair>
		<pathPair> = 'path=' <bindingSimpleName>
		<modePair> = 'mode=' ('OneWay' | 'TwoWay' | 'OneWayToSource' | 'OneTime' | 'Default)
		<converterPair> = 'convertor='<StaticResource>
		<sourcePair> = 'source='<StaticResource>

	*/
	
	auto ModeStringParser = parser::str("OneWay") |
		parser::or_with(parser::str("TwoWay")) |
		parser::or_with(parser::str("OneWayToSource")) |
		parser::or_with(parser::str("OneTime")) |
		parser::or_with(parser::str("Default"));

	BindingType BindingTypeFromString(const std::string& s);

	auto PathPairParser = parser::str("path=") |
		parser::ignore_previous(OptionalSpace) |
		parser::ignore_previous(BindingSimpleNameParser);

	auto ModePairParser = parser::str("mode=") |
		parser::ignore_previous(OptionalSpace) |
		parser::ignore_previous(ModeStringParser);

	auto ConvertorPairParser = parser::str("converter=") |
		parser::ignore_previous(OptionalSpace) |
		parser::ignore_previous(BindingSimpleNameParser);

	auto PathPairToBindingInfoParser = PathPairParser |
		parser::transform([](std::string s) { 
			BindingInfo b;
			b.path = sv2ws(s);
			return b;
		});

	auto ConvertorPairToBindingInfoParser = ConvertorPairParser |
		parser::transform([](std::string s) {
			BindingInfo b;
			b.converter = sv2ws(s);
			return b;
		});

	auto ModePairToBindingInfoParser = ConvertorPairParser |
		parser::transform([](std::string s) {
			BindingInfo b;
			b.type = BindingTypeFromString(s);
			return b;
		});

	auto BindingExpressionPairParser = ModePairToBindingInfoParser |
		parser::or_with(ConvertorPairToBindingInfoParser) |
		parser::or_with(PathPairToBindingInfoParser);

	auto BindingExpressionPairListParser = parser::one_of(",") |
		parser::ignore_previous(OptionalSpace) |
		parser::ignore_previous(BindingExpressionPairParser);

	BindingInfo MergeBindingInfos(BindingInfo a, BindingInfo b);

	/*auto BindingBodyParser = 
		parser::combine_with(
			BindingExpressionPairParser,
			parser::many(BindingExpressionPairListParser, BindingInfo{}, MergeBindingInfos ),
			MergeBindingInfos
		);*/
	
	//auto SourcePair = parser::str("source=") |
	//	parser::ignore_previous(OptionalSpace) |
	//	parser::ignore_previous(StaticResourceExpressionParser);

}

