#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonLinterTest)
  {
  public:
    TEST_METHOD(TestParseNull)
    {
      auto pairs = vector<tuple<wstring, bool, string>>
      {
        { L""s       , true , "Expected digit at position Line: 1 Column: 1!"s    },
        { L" "s      , true , "Expected digit at position Line: 1 Column: 2!"s    },
        { L" n"s     , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" nu"s    , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" nul"s   , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" null"s  , false, ""s                                                 },
        { L" null "s , false, ""s                                                 },
        { L" null 0"s, true , "Unexpected '0' at position Line: 1 Column: 7!"s    },
        { L" nu11 "s , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" nill "s , true , "Expected \"null\" at position Line: 1 Column: 2!"s },
        { L" mull "s , true , "Expected digit at position Line: 1 Column: 2!"s    },
      };
      for (auto& [input, expectException, exceptionMessage] : pairs)
      {
        if (!expectException)
        {
          auto tokens1 = JsonLinter::Read(              input );
          auto tokens2 = JsonLinter::Read(wstringstream(input));
          Assert::AreEqual<size_t>(1, tokens1.size());
          Assert::AreEqual<size_t>(1, tokens2.size());
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Null, tokens1[0].first);
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Null, tokens2[0].first);
          Assert::AreEqual<VALUE_TOKEN>(nullptr, tokens1[0].second);
          Assert::AreEqual<VALUE_TOKEN>(nullptr, tokens2[0].second);
        }
        else
        {
          ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
          ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
        }
      }
    }

    TEST_METHOD(TestParseString)
    {
      auto pairs = vector<pair<wstring, wstring>>
      {
        { L"\"\""s        , L""s        },  // Empty
        { L"\"a\""s       , L"a"s       },  // a
        { L"\"\\\"\""s    , L"\""s      },  // Quote
        { L"\"\\\\\""s    , L"\\"s      },  // Backslash
        { L"\"\\/\""s     , L"/"s       },  // Slash
        { L"\"\\b\""s     , L"\b"s      },  // Backspace
        { L"\"\\f\""s     , L"\f"s      },  // Form feed
        { L"\"\\n\""s     , L"\n"s      },  // New line
        { L"\"\\r\""s     , L"\r"s      },  // Carriage return
        { L"\"\\t\""s     , L"\t"s      },  // Tab
        { L"\"\\u03A9\""s , L"\u03A9"s  },  // Ω
        { L"\"ab\""s      , L"ab"s      },
        { L"\"\\\"b\""s   , L"\"b"s     },
        { L"\"\\\\b\""s   , L"\\b"s     },
        { L"\"\\/b\""s    , L"/b"s      },
        { L"\"\\bb\""s    , L"\bb"s     },
        { L"\"\\fb\""s    , L"\fb"s     },
        { L"\"\\nb\""s    , L"\nb"s     },
        { L"\"\\rb\""s    , L"\rb"s     },
        { L"\"\\tb\""s    , L"\tb"s     },
        { L"\"\\u03A9b\""s, L"\u03A9b"s },
      };

      for (auto& [input, expected] : pairs)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(1, tokens1.size());
        Assert::AreEqual<size_t>(1, tokens2.size());
        Assert::AreEqual<JsonTokenType>(JsonTokenType::String, tokens1[0].first);
        Assert::AreEqual<JsonTokenType>(JsonTokenType::String, tokens2[0].first);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens1[0].second);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens2[0].second);
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"\"asd"s      , "Expected '\"' at position Line: 1 Column: 5!"s },
        { L"\"asd\\u123"s, "Expected a hexadecimal digit at position Line: 1 Column: 10!"s },
        { L"\"asd\\a"s   , "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 7!"s },
        { L"\"asd\t"s    , "Invalid character found at position Line: 1 Column: 5!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestParseBoolean1)
    {
      auto pairs = vector<tuple<wstring, bool, string>>
      {
        { L""       , true , "Expected digit at position Line: 1 Column: 1!"s    },
        { L" "      , true , "Expected digit at position Line: 1 Column: 2!"s    },
        { L" t"     , true , "Expected \"true\" at position Line: 1 Column: 2!"s },
        { L" tr"    , true , "Expected \"true\" at position Line: 1 Column: 2!"s },
        { L" tru"   , true , "Expected \"true\" at position Line: 1 Column: 2!"s },
        { L" true"  , false, ""s                                                 },
        { L" true " , false, ""s                                                 },
        { L" true 0", true , "Unexpected '0' at position Line: 1 Column: 7!"s    },
        { L" trie " , true , "Expected \"true\" at position Line: 1 Column: 2!"s },
      };
      for (auto& [input, expectException, exceptionMessage] : pairs)
      {
        if (!expectException)
        {
          auto tokens1 = JsonLinter::Read(input);
          auto tokens2 = JsonLinter::Read(wstringstream(input));
          Assert::AreEqual<size_t>(1, tokens1.size());
          Assert::AreEqual<size_t>(1, tokens2.size());
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens1[0].first);
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens2[0].first);
          Assert::AreEqual<VALUE_TOKEN>(true, tokens1[0].second);
          Assert::AreEqual<VALUE_TOKEN>(true, tokens2[0].second);
        }
        else
        {
          ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
          ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
        }
      }
    }

    TEST_METHOD(TestParseBoolean2)
    {
      auto pairs = vector<tuple<wstring, bool, string>>
      {
        { L""        , true , "Expected digit at position Line: 1 Column: 1!"s     },
        { L" "       , true , "Expected digit at position Line: 1 Column: 2!"s     },
        { L" f"      , true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" fa"     , true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" fal"    , true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" fals"   , true , "Expected \"false\" at position Line: 1 Column: 2!"s },
        { L" false"  , false, ""s                                                  },
        { L" false " , false, ""s                                                  },
        { L" false 0", true , "Unexpected '0' at position Line: 1 Column: 8!"s     },
        { L" fakse " , true , "Expected \"false\" at position Line: 1 Column: 2!"s },
      };
      for (auto& [input, expectException, exceptionMessage] : pairs)
      {
        if (!expectException)
        {
          auto tokens1 = JsonLinter::Read(              input );
          auto tokens2 = JsonLinter::Read(wstringstream(input));
          Assert::AreEqual<size_t>(1, tokens1.size());
          Assert::AreEqual<size_t>(1, tokens2.size());
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens1[0].first);
          Assert::AreEqual<JsonTokenType>(JsonTokenType::Boolean, tokens2[0].first);
          Assert::AreEqual<VALUE_TOKEN>(false, tokens1[0].second);
          Assert::AreEqual<VALUE_TOKEN>(false, tokens2[0].second);
        }
        else
        {
          ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
          ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
        }
      }
    }

    TEST_METHOD(TestParseNumber)
    {
      auto tuples = vector<tuple<wstring, VALUE_TOKEN, JsonTokenType>>
      {
        { L"0"s          , 0i64              , JsonTokenType::Integer },
        { L"0E+1"s       , 0.0               , JsonTokenType::Real    },
        { L"0E+12"s      , 0.0               , JsonTokenType::Real    },
        { L"0E1"s        , 0.0               , JsonTokenType::Real    },
        { L"0E12"s       , 0.0               , JsonTokenType::Real    },
        { L"0E-1"s       , 0.0               , JsonTokenType::Real    },
        { L"0E-12"s      , 0.0               , JsonTokenType::Real    },
        { L"0e+1"s       , 0.0               , JsonTokenType::Real    },
        { L"0e+12"s      , 0.0               , JsonTokenType::Real    },
        { L"0e1"s        , 0.0               , JsonTokenType::Real    },
        { L"0e12"s       , 0.0               , JsonTokenType::Real    },
        { L"0e-1"s       , 0.0               , JsonTokenType::Real    },
        { L"0e-12"s      , 0.0               , JsonTokenType::Real    },
        { L"0.1"s        , 0.1               , JsonTokenType::Real    },
        { L"0.1E+1"s     , 1.0               , JsonTokenType::Real    },
        { L"0.1E+12"s    , 100000000000.0    , JsonTokenType::Real    },
        { L"0.1E1"s      , 1.0               , JsonTokenType::Real    },
        { L"0.1E12"s     , 100000000000.0    , JsonTokenType::Real    },
        { L"0.1E-1"s     , 0.01              , JsonTokenType::Real    },
        { L"0.1E-12"s    , 0.0000000000001   , JsonTokenType::Real    },
        { L"0.1e+1"s     , 1.0               , JsonTokenType::Real    },
        { L"0.1e+12"s    , 100000000000.0    , JsonTokenType::Real    },
        { L"0.1e1"s      , 1.0               , JsonTokenType::Real    },
        { L"0.1e12"s     , 100000000000.0    , JsonTokenType::Real    },
        { L"0.1e-1"s     , 0.01              , JsonTokenType::Real    },
        { L"0.1e-12"s    , 0.0000000000001   , JsonTokenType::Real    },
        { L"0.12"s       , 0.12              , JsonTokenType::Real    },
        { L"0.12E+1"s    , 1.2               , JsonTokenType::Real    },
        { L"0.12E+12"s   , 120000000000.0    , JsonTokenType::Real    },
        { L"0.12E1"s     , 1.2               , JsonTokenType::Real    },
        { L"0.12E12"s    , 120000000000.0    , JsonTokenType::Real    },
        { L"0.12E-1"s    , 0.012             , JsonTokenType::Real    },
        { L"0.12E-12"s   , 0.00000000000012  , JsonTokenType::Real    },
        { L"0.12e+1"s    , 1.2               , JsonTokenType::Real    },
        { L"0.12e+12"s   , 120000000000.0    , JsonTokenType::Real    },
        { L"0.12e1"s     , 1.2               , JsonTokenType::Real    },
        { L"0.12e12"s    , 120000000000.0    , JsonTokenType::Real    },
        { L"0.12e-1"s    , 0.012             , JsonTokenType::Real    },
        { L"0.12e-12"s   , 0.00000000000012  , JsonTokenType::Real    },
        { L"1"s          , 1i64              , JsonTokenType::Integer },
        { L"1E+1"s       , 10.0              , JsonTokenType::Real    },
        { L"1E+12"s      , 1000000000000.0   , JsonTokenType::Real    },
        { L"1E1"s        , 10.0              , JsonTokenType::Real    },
        { L"1E12"s       , 1000000000000.0   , JsonTokenType::Real    },
        { L"1E-1"s       , 0.1               , JsonTokenType::Real    },
        { L"1E-12"s      , 0.000000000001    , JsonTokenType::Real    },
        { L"1e+1"s       , 10.0              , JsonTokenType::Real    },
        { L"1e+12"s      , 1000000000000.0   , JsonTokenType::Real    },
        { L"1e1"s        , 10.0              , JsonTokenType::Real    },
        { L"1e12"s       , 1000000000000.0   , JsonTokenType::Real    },
        { L"1e-1"s       , 0.1               , JsonTokenType::Real    },
        { L"1e-12"s      , 0.000000000001    , JsonTokenType::Real    },
        { L"1.1"s        , 1.1               , JsonTokenType::Real    },
        { L"1.1E+1"s     , 11.0              , JsonTokenType::Real    },
        { L"1.1E+12"s    , 1100000000000.0   , JsonTokenType::Real    },
        { L"1.1E1"s      , 11.0              , JsonTokenType::Real    },
        { L"1.1E12"s     , 1100000000000.0   , JsonTokenType::Real    },
        { L"1.1E-1"s     , 0.11              , JsonTokenType::Real    },
        { L"1.1E-12"s    , 0.0000000000011   , JsonTokenType::Real    },
        { L"1.1e+1"s     , 11.0              , JsonTokenType::Real    },
        { L"1.1e+12"s    , 1100000000000.0   , JsonTokenType::Real    },
        { L"1.1e1"s      , 11.0              , JsonTokenType::Real    },
        { L"1.1e12"s     , 1100000000000.0   , JsonTokenType::Real    },
        { L"1.1e-1"s     , 0.11              , JsonTokenType::Real    },
        { L"1.1e-12"s    , 0.0000000000011   , JsonTokenType::Real    },
        { L"1.12"s       , 1.12              , JsonTokenType::Real    },
        { L"1.12E+1"s    , 11.2              , JsonTokenType::Real    },
        { L"1.12E+12"s   , 1120000000000.0   , JsonTokenType::Real    },
        { L"1.12E1"s     , 11.2              , JsonTokenType::Real    },
        { L"1.12E12"s    , 1120000000000.0   , JsonTokenType::Real    },
        { L"1.12E-1"s    , 0.112             , JsonTokenType::Real    },
        { L"1.12E-12"s   , 0.00000000000112  , JsonTokenType::Real    },
        { L"1.12e+1"s    , 11.2              , JsonTokenType::Real    },
        { L"1.12e+12"s   , 1120000000000.0   , JsonTokenType::Real    },
        { L"1.12e1"s     , 11.2              , JsonTokenType::Real    },
        { L"1.12e12"s    , 1120000000000.0   , JsonTokenType::Real    },
        { L"1.12e-1"s    , 0.112             , JsonTokenType::Real    },
        { L"1.12e-12"s   , 0.00000000000112  , JsonTokenType::Real    },
        { L"12"s         , 12i64             , JsonTokenType::Integer },
        { L"12E+1"s      , 120.0             , JsonTokenType::Real    },
        { L"12E+12"s     , 12000000000000.0  , JsonTokenType::Real    },
        { L"12E1"s       , 120.0             , JsonTokenType::Real    },
        { L"12E12"s      , 12000000000000.0  , JsonTokenType::Real    },
        { L"12E-1"s      , 1.2               , JsonTokenType::Real    },
        { L"12E-12"s     , 0.000000000012    , JsonTokenType::Real    },
        { L"12e+1"s      , 120.0             , JsonTokenType::Real    },
        { L"12e+12"s     , 12000000000000.0  , JsonTokenType::Real    },
        { L"12e1"s       , 120.0             , JsonTokenType::Real    },
        { L"12e12"s      , 12000000000000.0  , JsonTokenType::Real    },
        { L"12e-1"s      , 1.2               , JsonTokenType::Real    },
        { L"12e-12"s     , 0.000000000012    , JsonTokenType::Real    },
        { L"12.1"s       , 12.1              , JsonTokenType::Real    },
        { L"12.1E+1"s    , 121.0             , JsonTokenType::Real    },
        { L"12.1E+12"s   , 12100000000000.0  , JsonTokenType::Real    },
        { L"12.1E1"s     , 121.0             , JsonTokenType::Real    },
        { L"12.1E12"s    , 12100000000000.0  , JsonTokenType::Real    },
        { L"12.1E-1"s    , 1.21              , JsonTokenType::Real    },
        { L"12.1E-12"s   , 0.0000000000121   , JsonTokenType::Real    },
        { L"12.1e+1"s    , 121.0             , JsonTokenType::Real    },
        { L"12.1e+12"s   , 12100000000000.0  , JsonTokenType::Real    },
        { L"12.1e1"s     , 121.0             , JsonTokenType::Real    },
        { L"12.1e12"s    , 12100000000000.0  , JsonTokenType::Real    },
        { L"12.1e-1"s    , 1.21              , JsonTokenType::Real    },
        { L"12.1e-12"s   , 0.0000000000121   , JsonTokenType::Real    },
        { L"12.12"s      , 12.12             , JsonTokenType::Real    },
        { L"12.12E+1"s   , 121.2             , JsonTokenType::Real    },
        { L"12.12E+12"s  , 12120000000000.0  , JsonTokenType::Real    },
        { L"12.12E1"s    , 121.2             , JsonTokenType::Real    },
        { L"12.12E12"s   , 12120000000000.0  , JsonTokenType::Real    },
        { L"12.12E-1"s   , 1.212             , JsonTokenType::Real    },
        { L"12.12E-12"s  , 0.00000000001212  , JsonTokenType::Real    },
        { L"12.12e+1"s   , 121.2             , JsonTokenType::Real    },
        { L"12.12e+12"s  , 12120000000000.0  , JsonTokenType::Real    },
        { L"12.12e1"s    , 121.2             , JsonTokenType::Real    },
        { L"12.12e12"s   , 12120000000000.0  , JsonTokenType::Real    },
        { L"12.12e-1"s   , 1.212             , JsonTokenType::Real    },
        { L"12.12e-12"s  , 0.00000000001212  , JsonTokenType::Real    },
        { L"123"s        , 123i64            , JsonTokenType::Integer },
        { L"123E+1"s     , 1230.0            , JsonTokenType::Real    },
        { L"123E+12"s    , 123000000000000.0 , JsonTokenType::Real    },
        { L"123E1"s      , 1230.0            , JsonTokenType::Real    },
        { L"123E12"s     , 123000000000000.0 , JsonTokenType::Real    },
        { L"123E-1"s     , 12.3              , JsonTokenType::Real    },
        { L"123E-12"s    , 0.000000000123    , JsonTokenType::Real    },
        { L"123e+1"s     , 1230.0            , JsonTokenType::Real    },
        { L"123e+12"s    , 123000000000000.0 , JsonTokenType::Real    },
        { L"123e1"s      , 1230.0            , JsonTokenType::Real    },
        { L"123e12"s     , 123000000000000.0 , JsonTokenType::Real    },
        { L"123e-1"s     , 12.3              , JsonTokenType::Real    },
        { L"123e-12"s    , 0.000000000123    , JsonTokenType::Real    },
        { L"123.1"s      , 123.1             , JsonTokenType::Real    },
        { L"123.1E+1"s   , 1231.0            , JsonTokenType::Real    },
        { L"123.1E+12"s  , 123100000000000.0 , JsonTokenType::Real    },
        { L"123.1E1"s    , 1231.0            , JsonTokenType::Real    },
        { L"123.1E12"s   , 123100000000000.0 , JsonTokenType::Real    },
        { L"123.1E-1"s   , 12.31             , JsonTokenType::Real    },
        { L"123.1E-12"s  , 0.0000000001231   , JsonTokenType::Real    },
        { L"123.1e+1"s   , 1231.0            , JsonTokenType::Real    },
        { L"123.1e+12"s  , 123100000000000.0 , JsonTokenType::Real    },
        { L"123.1e1"s    , 1231.0            , JsonTokenType::Real    },
        { L"123.1e12"s   , 123100000000000.0 , JsonTokenType::Real    },
        { L"123.1e-1"s   , 12.31             , JsonTokenType::Real    },
        { L"123.1e-12"s  , 0.0000000001231   , JsonTokenType::Real    },
        { L"123.12"s     , 123.12            , JsonTokenType::Real    },
        { L"123.12E+1"s  , 1231.2            , JsonTokenType::Real    },
        { L"123.12E+12"s , 123120000000000.0 , JsonTokenType::Real    },
        { L"123.12E1"s   , 1231.2            , JsonTokenType::Real    },
        { L"123.12E12"s  , 123120000000000.0 , JsonTokenType::Real    },
        { L"123.12E-1"s  , 12.312            , JsonTokenType::Real    },
        { L"123.12E-12"s , 0.00000000012312  , JsonTokenType::Real    },
        { L"123.12e+1"s  , 1231.2            , JsonTokenType::Real    },
        { L"123.12e+12"s , 123120000000000.0 , JsonTokenType::Real    },
        { L"123.12e1"s   , 1231.2            , JsonTokenType::Real    },
        { L"123.12e12"s  , 123120000000000.0 , JsonTokenType::Real    },
        { L"123.12e-1"s  , 12.312            , JsonTokenType::Real    },
        { L"123.12e-12"s , 0.00000000012312  , JsonTokenType::Real    },
        { L"-0"s         , 0i64              , JsonTokenType::Integer },
        { L"-0E+1"s      , 0.0               , JsonTokenType::Real    },
        { L"-0E+12"s     , 0.0               , JsonTokenType::Real    },
        { L"-0E1"s       , 0.0               , JsonTokenType::Real    },
        { L"-0E12"s      , 0.0               , JsonTokenType::Real    },
        { L"-0E-1"s      , 0.0               , JsonTokenType::Real    },
        { L"-0E-12"s     , 0.0               , JsonTokenType::Real    },
        { L"-0e+1"s      , 0.0               , JsonTokenType::Real    },
        { L"-0e+12"s     , 0.0               , JsonTokenType::Real    },
        { L"-0e1"s       , 0.0               , JsonTokenType::Real    },
        { L"-0e12"s      , 0.0               , JsonTokenType::Real    },
        { L"-0e-1"s      , 0.0               , JsonTokenType::Real    },
        { L"-0e-12"s     , 0.0               , JsonTokenType::Real    },
        { L"-0.1"s       , -0.1              , JsonTokenType::Real    },
        { L"-0.1E+1"s    , -1.0              , JsonTokenType::Real    },
        { L"-0.1E+12"s   , -100000000000.0   , JsonTokenType::Real    },
        { L"-0.1E1"s     , -1.0              , JsonTokenType::Real    },
        { L"-0.1E12"s    , -100000000000.0   , JsonTokenType::Real    },
        { L"-0.1E-1"s    , -0.01             , JsonTokenType::Real    },
        { L"-0.1E-12"s   , -0.0000000000001  , JsonTokenType::Real    },
        { L"-0.1e+1"s    , -1.0              , JsonTokenType::Real    },
        { L"-0.1e+12"s   , -100000000000.0   , JsonTokenType::Real    },
        { L"-0.1e1"s     , -1.0              , JsonTokenType::Real    },
        { L"-0.1e12"s    , -100000000000.0   , JsonTokenType::Real    },
        { L"-0.1e-1"s    , -0.01             , JsonTokenType::Real    },
        { L"-0.1e-12"s   , -0.0000000000001  , JsonTokenType::Real    },
        { L"-0.12"s      , -0.12             , JsonTokenType::Real    },
        { L"-0.12E+1"s   , -1.2              , JsonTokenType::Real    },
        { L"-0.12E+12"s  , -120000000000.0   , JsonTokenType::Real    },
        { L"-0.12E1"s    , -1.2              , JsonTokenType::Real    },
        { L"-0.12E12"s   , -120000000000.0   , JsonTokenType::Real    },
        { L"-0.12E-1"s   , -0.012            , JsonTokenType::Real    },
        { L"-0.12E-12"s  , -0.00000000000012 , JsonTokenType::Real    },
        { L"-0.12e+1"s   , -1.2              , JsonTokenType::Real    },
        { L"-0.12e+12"s  , -120000000000.0   , JsonTokenType::Real    },
        { L"-0.12e1"s    , -1.2              , JsonTokenType::Real    },
        { L"-0.12e12"s   , -120000000000.0   , JsonTokenType::Real    },
        { L"-0.12e-1"s   , -0.012            , JsonTokenType::Real    },
        { L"-0.12e-12"s  , -0.00000000000012 , JsonTokenType::Real    },
        { L"-1"s         , -1i64             , JsonTokenType::Integer },
        { L"-1E+1"s      , -10.0             , JsonTokenType::Real    },
        { L"-1E+12"s     , -1000000000000.0  , JsonTokenType::Real    },
        { L"-1E1"s       , -10.0             , JsonTokenType::Real    },
        { L"-1E12"s      , -1000000000000.0  , JsonTokenType::Real    },
        { L"-1E-1"s      , -0.1              , JsonTokenType::Real    },
        { L"-1E-12"s     , -0.000000000001   , JsonTokenType::Real    },
        { L"-1e+1"s      , -10.0             , JsonTokenType::Real    },
        { L"-1e+12"s     , -1000000000000.0  , JsonTokenType::Real    },
        { L"-1e1"s       , -10.0             , JsonTokenType::Real    },
        { L"-1e12"s      , -1000000000000.0  , JsonTokenType::Real    },
        { L"-1e-1"s      , -0.1              , JsonTokenType::Real    },
        { L"-1e-12"s     , -0.000000000001   , JsonTokenType::Real    },
        { L"-1.1"s       , -1.1              , JsonTokenType::Real    },
        { L"-1.1E+1"s    , -11.0             , JsonTokenType::Real    },
        { L"-1.1E+12"s   , -1100000000000.0  , JsonTokenType::Real    },
        { L"-1.1E1"s     , -11.0             , JsonTokenType::Real    },
        { L"-1.1E12"s    , -1100000000000.0  , JsonTokenType::Real    },
        { L"-1.1E-1"s    , -0.11             , JsonTokenType::Real    },
        { L"-1.1E-12"s   , -0.0000000000011  , JsonTokenType::Real    },
        { L"-1.1e+1"s    , -11.0             , JsonTokenType::Real    },
        { L"-1.1e+12"s   , -1100000000000.0  , JsonTokenType::Real    },
        { L"-1.1e1"s     , -11.0             , JsonTokenType::Real    },
        { L"-1.1e12"s    , -1100000000000.0  , JsonTokenType::Real    },
        { L"-1.1e-1"s    , -0.11             , JsonTokenType::Real    },
        { L"-1.1e-12"s   , -0.0000000000011  , JsonTokenType::Real    },
        { L"-1.12"s      , -1.12             , JsonTokenType::Real    },
        { L"-1.12E+1"s   , -11.2             , JsonTokenType::Real    },
        { L"-1.12E+12"s  , -1120000000000.0  , JsonTokenType::Real    },
        { L"-1.12E1"s    , -11.2             , JsonTokenType::Real    },
        { L"-1.12E12"s   , -1120000000000.0  , JsonTokenType::Real    },
        { L"-1.12E-1"s   , -0.112            , JsonTokenType::Real    },
        { L"-1.12E-12"s  , -0.00000000000112 , JsonTokenType::Real    },
        { L"-1.12e+1"s   , -11.2             , JsonTokenType::Real    },
        { L"-1.12e+12"s  , -1120000000000.0  , JsonTokenType::Real    },
        { L"-1.12e1"s    , -11.2             , JsonTokenType::Real    },
        { L"-1.12e12"s   , -1120000000000.0  , JsonTokenType::Real    },
        { L"-1.12e-1"s   , -0.112            , JsonTokenType::Real    },
        { L"-1.12e-12"s  , -0.00000000000112 , JsonTokenType::Real    },
        { L"-12"s        , -12i64            , JsonTokenType::Integer },
        { L"-12E+1"s     , -120.0            , JsonTokenType::Real    },
        { L"-12E+12"s    , -12000000000000.0 , JsonTokenType::Real    },
        { L"-12E1"s      , -120.0            , JsonTokenType::Real    },
        { L"-12E12"s     , -12000000000000.0 , JsonTokenType::Real    },
        { L"-12E-1"s     , -1.2              , JsonTokenType::Real    },
        { L"-12E-12"s    , -0.000000000012   , JsonTokenType::Real    },
        { L"-12e+1"s     , -120.0            , JsonTokenType::Real    },
        { L"-12e+12"s    , -12000000000000.0 , JsonTokenType::Real    },
        { L"-12e1"s      , -120.0            , JsonTokenType::Real    },
        { L"-12e12"s     , -12000000000000.0 , JsonTokenType::Real    },
        { L"-12e-1"s     , -1.2              , JsonTokenType::Real    },
        { L"-12e-12"s    , -0.000000000012   , JsonTokenType::Real    },
        { L"-12.1"s      , -12.1             , JsonTokenType::Real    },
        { L"-12.1E+1"s   , -121.0            , JsonTokenType::Real    },
        { L"-12.1E+12"s  , -12100000000000.0 , JsonTokenType::Real    },
        { L"-12.1E1"s    , -121.0            , JsonTokenType::Real    },
        { L"-12.1E12"s   , -12100000000000.0 , JsonTokenType::Real    },
        { L"-12.1E-1"s   , -1.21             , JsonTokenType::Real    },
        { L"-12.1E-12"s  , -0.0000000000121  , JsonTokenType::Real    },
        { L"-12.1e+1"s   , -121.0            , JsonTokenType::Real    },
        { L"-12.1e+12"s  , -12100000000000.0 , JsonTokenType::Real    },
        { L"-12.1e1"s    , -121.0            , JsonTokenType::Real    },
        { L"-12.1e12"s   , -12100000000000.0 , JsonTokenType::Real    },
        { L"-12.1e-1"s   , -1.21             , JsonTokenType::Real    },
        { L"-12.1e-12"s  , -0.0000000000121  , JsonTokenType::Real    },
        { L"-12.12"s     , -12.12            , JsonTokenType::Real    },
        { L"-12.12E+1"s  , -121.2            , JsonTokenType::Real    },
        { L"-12.12E+12"s , -12120000000000.0 , JsonTokenType::Real    },
        { L"-12.12E1"s   , -121.2            , JsonTokenType::Real    },
        { L"-12.12E12"s  , -12120000000000.0 , JsonTokenType::Real    },
        { L"-12.12E-1"s  , -1.212            , JsonTokenType::Real    },
        { L"-12.12E-12"s , -0.00000000001212 , JsonTokenType::Real    },
        { L"-12.12e+1"s  , -121.2            , JsonTokenType::Real    },
        { L"-12.12e+12"s , -12120000000000.0 , JsonTokenType::Real    },
        { L"-12.12e1"s   , -121.2            , JsonTokenType::Real    },
        { L"-12.12e12"s  , -12120000000000.0 , JsonTokenType::Real    },
        { L"-12.12e-1"s  , -1.212            , JsonTokenType::Real    },
        { L"-12.12e-12"s , -0.00000000001212 , JsonTokenType::Real    },
        { L"-123"s       , -123i64           , JsonTokenType::Integer },
        { L"-123E+1"s    , -1230.0           , JsonTokenType::Real    },
        { L"-123E+12"s   , -123000000000000.0, JsonTokenType::Real    },
        { L"-123E1"s     , -1230.0           , JsonTokenType::Real    },
        { L"-123E12"s    , -123000000000000.0, JsonTokenType::Real    },
        { L"-123E-1"s    , -12.3             , JsonTokenType::Real    },
        { L"-123E-12"s   , -0.000000000123   , JsonTokenType::Real    },
        { L"-123e+1"s    , -1230.0           , JsonTokenType::Real    },
        { L"-123e+12"s   , -123000000000000.0, JsonTokenType::Real    },
        { L"-123e1"s     , -1230.0           , JsonTokenType::Real    },
        { L"-123e12"s    , -123000000000000.0, JsonTokenType::Real    },
        { L"-123e-1"s    , -12.3             , JsonTokenType::Real    },
        { L"-123e-12"s   , -0.000000000123   , JsonTokenType::Real    },
        { L"-123.1"s     , -123.1            , JsonTokenType::Real    },
        { L"-123.1E+1"s  , -1231.0           , JsonTokenType::Real    },
        { L"-123.1E+12"s , -123100000000000.0, JsonTokenType::Real    },
        { L"-123.1E1"s   , -1231.0           , JsonTokenType::Real    },
        { L"-123.1E12"s  , -123100000000000.0, JsonTokenType::Real    },
        { L"-123.1E-1"s  , -12.31            , JsonTokenType::Real    },
        { L"-123.1E-12"s , -0.0000000001231  , JsonTokenType::Real    },
        { L"-123.1e+1"s  , -1231.0           , JsonTokenType::Real    },
        { L"-123.1e+12"s , -123100000000000.0, JsonTokenType::Real    },
        { L"-123.1e1"s   , -1231.0           , JsonTokenType::Real    },
        { L"-123.1e12"s  , -123100000000000.0, JsonTokenType::Real    },
        { L"-123.1e-1"s  , -12.31            , JsonTokenType::Real    },
        { L"-123.1e-12"s , -0.0000000001231  , JsonTokenType::Real    },
        { L"-123.12"s    , -123.12           , JsonTokenType::Real    },
        { L"-123.12E+1"s , -1231.2           , JsonTokenType::Real    },
        { L"-123.12E+12"s, -123120000000000.0, JsonTokenType::Real    },
        { L"-123.12E1"s  , -1231.2           , JsonTokenType::Real    },
        { L"-123.12E12"s , -123120000000000.0, JsonTokenType::Real    },
        { L"-123.12E-1"s , -12.312           , JsonTokenType::Real    },
        { L"-123.12E-12"s, -0.00000000012312 , JsonTokenType::Real    },
        { L"-123.12e+1"s , -1231.2           , JsonTokenType::Real    },
        { L"-123.12e+12"s, -123120000000000.0, JsonTokenType::Real    },
        { L"-123.12e1"s  , -1231.2           , JsonTokenType::Real    },
        { L"-123.12e12"s , -123120000000000.0, JsonTokenType::Real    },
        { L"-123.12e-1"s , -12.312           , JsonTokenType::Real    },
        { L"-123.12e-12"s, -0.00000000012312 , JsonTokenType::Real    }
      };

      for (auto& [input, expected, expectedType] : tuples)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(1, tokens1.size());
        Assert::AreEqual<size_t>(1, tokens2.size());
        Assert::AreEqual<JsonTokenType>(expectedType, tokens1[0].first);
        Assert::AreEqual<JsonTokenType>(expectedType, tokens2[0].first);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens1[0].second);
        Assert::AreEqual<VALUE_TOKEN>(expected, tokens2[0].second);
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"asd"s, "Expected digit at position Line: 1 Column: 1!"s },
        { L"1."s , "Expected digit at position Line: 1 Column: 3!"s },
        { L"1e"s , "Expected digit at position Line: 1 Column: 3!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestParseObject)
    {
      auto pairs = vector<pair<wstring, vector<TOKEN>>>
      {
        // Test whitespace handling
        { L"{}"s  , { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  }"s, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":1337}"s,          { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":1337}"s,        { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :1337}"s,        { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :1337}"s,      { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":  1337}"s,        { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":  1337}"s,      { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :  1337}"s,      { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :  1337}"s,    { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":1337  }"s,        { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":1337  }"s,      { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :1337  }"s,      { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :1337  }"s,    { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\":  1337  }"s,      { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\":  1337  }"s,    { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{\"key1\"  :  1337  }"s,    { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :  1337  }"s,  { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        { L"{  \"key1\"  :  1337  } "s, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 }, { JsonTokenType::EndObject, L"}"s } } },
        // Simple object with 2 key value pair
        { L"{\"key1\":1337,\"key2\":\"value2\"}"s,
        { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::Integer, 1337i64 },
          { JsonTokenType::PropertyName, L"key2"s }, { JsonTokenType::String, L"value2"s }, { JsonTokenType::EndObject, L"}"s } } },
        // Complex object with all types of values (string, number, object, array, bool, null)
        { L"{ \"string\": \"string\", \"number\": 1337, \"object\": { \"key1\": \"value1\", \"key2\": \"value2\" }, \"array\": [ 1, 3, 3, 7 ], \"true\": true, \"false\": false, \"null\": null }"s,
        { { JsonTokenType::StartObject, L"{"s },
          { JsonTokenType::PropertyName, L"string"s }, { JsonTokenType::String, L"string"s },
          { JsonTokenType::PropertyName, L"number"s }, { JsonTokenType::Integer, 1337i64 },
          { JsonTokenType::PropertyName, L"object"s }, { JsonTokenType::StartObject, L"{"s },
          { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::String, L"value1"s },
          { JsonTokenType::PropertyName, L"key2"s }, { JsonTokenType::String, L"value2"s }, { JsonTokenType::EndObject, L"}"s },
          { JsonTokenType::PropertyName, L"array"s }, { JsonTokenType::StartArray, L"["s },
          { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 3i64 }, { JsonTokenType::Integer, 3i64 }, { JsonTokenType::Integer, 7i64 }, { JsonTokenType::EndArray, L"]"s },
          { JsonTokenType::PropertyName, L"true"s }, { JsonTokenType::Boolean, true },
          { JsonTokenType::PropertyName, L"false"s }, { JsonTokenType::Boolean, false },
          { JsonTokenType::PropertyName, L"null"s }, { JsonTokenType::Null, nullptr },
          { JsonTokenType::EndObject, L"}"s } } }
      };

      for (auto& [input, expected] : pairs)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(expected.size(), tokens1.size());
        Assert::AreEqual<size_t>(expected.size(), tokens2.size());
        for (int i = 0; i < expected.size(); ++i)
        {
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens1[i].first);
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens2[i].first);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens1[i].second);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens2[i].second);
        }
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{}}}}}}}}}}}}}}}}}}}}"s, "Depth is greater or equal to the maximum 20!"s },
        { L"{  \r\n  \"key\"  \r\n  ,}"s, "Expected ':' at position Line: 3 Column: 3!"s },
        { L"{  \r\n  \"key\"  \r\n  :  \r\n  \"value\", \"key2\" \r\n }"s, "Expected ':' at position Line: 5 Column: 2!"s },
        { L"{  \r\n  \"key\"  \r\n  :  \r\n  \"value\", \"key2\" \r\n : 1337"s, "Expected '}' at position Line: 5 Column: 8!"s },
        { L"{\"key\":1337} 0"s, "Unexpected '0' at position Line: 1 Column: 14!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestParseArray)
    {
      auto pairs = vector<pair<wstring, vector<TOKEN>>>
      {
        // Test whitespace handling
        { L"[]"s,   { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  ]"s, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,2]"s,          { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,2]"s,        { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,2]"s,        { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,  2]"s,        { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,2  ]"s,        { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,2]"s,      { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,  2]"s,      { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,2  ]"s,      { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,  2]"s,      { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,2  ]"s,      { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1,  2  ]"s,      { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,  2]"s,    { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,2  ]"s,    { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1,  2  ]"s,    { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[1  ,  2  ]"s,    { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,  2  ]"s,  { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        { L"[  1  ,  2  ] "s, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 2i64 }, { JsonTokenType::EndArray, L"]"s } } },
        // Complex array with all types of values (string, number, object, array, bool, null)
        { L"[ \"string\",1337, {   \"key1\":\"value1\" ,   \"key2\":\"value2\"  }, [ 1, 3, 3, 7 ], true, false, null    ]"s,
        { { JsonTokenType::StartArray, L"["s },
          { JsonTokenType::String, L"string"s },
          { JsonTokenType::Integer, 1337i64 },
          { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"key1"s }, { JsonTokenType::String, L"value1"s }, { JsonTokenType::PropertyName, L"key2"s }, { JsonTokenType::String, L"value2"s }, { JsonTokenType::EndObject, L"}"s },
          { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Integer, 1i64 }, { JsonTokenType::Integer, 3i64 }, { JsonTokenType::Integer, 3i64 }, { JsonTokenType::Integer, 7i64 }, { JsonTokenType::EndArray, L"]"s },
          { JsonTokenType::Boolean, true },
          { JsonTokenType::Boolean, false },
          { JsonTokenType::Null, nullptr },
          { JsonTokenType::EndArray, L"]"s } }},
      };

      for (auto& [input, expected] : pairs)
      {
        auto tokens1 = JsonLinter::Read(              input );
        auto tokens2 = JsonLinter::Read(wstringstream(input));
        Assert::AreEqual<size_t>(expected.size(), tokens1.size());
        Assert::AreEqual<size_t>(expected.size(), tokens2.size());
        for (int i = 0; i < expected.size(); ++i)
        {
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens1[i].first);
          Assert::AreEqual<JsonTokenType>(expected[i].first, tokens2[i].first);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens1[i].second);
          Assert::AreEqual<VALUE_TOKEN>(expected[i].second, tokens2[i].second);
        }
      }

      auto pairs2 = vector<pair<wstring, string>>
      {
        { L"[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]"s, "Depth is greater or equal to the maximum 20!"s },
        { L"[  \r\n  ,]"s, "Unexpected ',' at position Line: 2 Column: 3!"s },
        { L"[  \r\n  1  \r\n  "s, "Expected ']' at position Line: 3 Column: 3!"s },
        { L"[1337] 0"s, "Unexpected '0' at position Line: 1 Column: 8!"s },
      };

      for (auto& [input, exceptionMessage] : pairs2)
      {
        ExceptException<exception>([input = input]() { JsonLinter::Read(              input ); }, exceptionMessage);
        ExceptException<exception>([input = input]() { JsonLinter::Read(wstringstream(input)); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestWriteNumber)
    {
      auto pairs = vector<pair<deque<TOKEN>, wstring>>
      {
        { { { JsonTokenType::Real, 13.37f } }, L"13.37"s },
        { { { JsonTokenType::Real, 13.37 } }, L"13.37"s },
        { { { JsonTokenType::Real, 0.00000123456789 } }, L"1.23456789e-06"s },
        { { { JsonTokenType::Real, 0.00000000000001 } }, L"1e-14"s },
        { { { JsonTokenType::Real, 123456789123456.0 } }, L"123456789123456"s },
        { { { JsonTokenType::Real, 987654321987654.0 } }, L"987654321987654"s },
        { { { JsonTokenType::Real, 0.5f } }, L"0.5"s },
        { { { JsonTokenType::Real, 0.5 } }, L"0.5"s },
        { { { JsonTokenType::Real, 1337.1337 } }, L"1337.1337"s },
        { { { JsonTokenType::Real, 1.3f } }, L"1.3"s },
        { { { JsonTokenType::Real, 1.3 } }, L"1.3"s },
        { { { JsonTokenType::Real, 3.141592653589793 } }, L"3.141592653589793"s },
        { { { JsonTokenType::Real, 1.414213562373095 } }, L"1.414213562373095"s },
        { { { JsonTokenType::Real, 1.618033988749894 } }, L"1.618033988749894"s },
        { { { JsonTokenType::Real, 2.718281828459045 } }, L"2.718281828459045"s },
        { { { JsonTokenType::Real, 1.0                                                   } }, L"1"s                   },
        { { { JsonTokenType::Real, 10.0                                                  } }, L"10"s                  },
        { { { JsonTokenType::Real, 100.0                                                 } }, L"100"s                 },
        { { { JsonTokenType::Real, 1000.0                                                } }, L"1000"s                },
        { { { JsonTokenType::Real, 10000.0                                               } }, L"10000"s               },
        { { { JsonTokenType::Real, 100000.0                                              } }, L"100000"s              },
        { { { JsonTokenType::Real, 1000000.0                                             } }, L"1000000"s             },
        { { { JsonTokenType::Real, 10000000.0                                            } }, L"10000000"s            },
        { { { JsonTokenType::Real, 100000000.0                                           } }, L"100000000"s           },
        { { { JsonTokenType::Real, 1000000000.0                                          } }, L"1000000000"s          },
        { { { JsonTokenType::Real, 10000000000.0                                         } }, L"10000000000"s         },
        { { { JsonTokenType::Real, 100000000000.0                                        } }, L"100000000000"s        },
        { { { JsonTokenType::Real, 1000000000000.0                                       } }, L"1000000000000"s       },
        { { { JsonTokenType::Real, 10000000000000.0                                      } }, L"10000000000000"s      },
        { { { JsonTokenType::Real, 100000000000000.0                                     } }, L"100000000000000"s     },
        { { { JsonTokenType::Real, 1000000000000000.0                                    } }, L"1000000000000000"s    },
        { { { JsonTokenType::Real, 10000000000000000.0                                   } }, L"10000000000000000"s   },
        { { { JsonTokenType::Real, 100000000000000000.0                                  } }, L"100000000000000000"s  },
        { { { JsonTokenType::Real, 1000000000000000000.0                                 } }, L"1000000000000000000"s },
        { { { JsonTokenType::Real, 10000000000000000000.0                                } }, L"1e+19"s               },
        { { { JsonTokenType::Real, 100000000000000000000.0                               } }, L"1e+20"s               },
        { { { JsonTokenType::Real, 1000000000000000000000.0                              } }, L"1e+21"s               },
        { { { JsonTokenType::Real, 10000000000000000000000.0                             } }, L"1e+22"s               },
        { { { JsonTokenType::Real, 100000000000000000000000.0                            } }, L"1e+23"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000.0                           } }, L"1e+24"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000.0                          } }, L"1e+25"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000.0                         } }, L"1e+26"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000.0                        } }, L"1e+27"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000.0                       } }, L"1e+28"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000.0                      } }, L"1e+29"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000000.0                     } }, L"1e+30"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000000.0                    } }, L"1e+31"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000000.0                   } }, L"1e+32"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000000000.0                  } }, L"1e+33"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000000000.0                 } }, L"1e+34"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000000000.0                } }, L"1e+35"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000000000000.0               } }, L"1e+36"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000000000000.0              } }, L"1e+37"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000000000000.0             } }, L"1e+38"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000000000000000.0            } }, L"1e+39"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000000000000000.0           } }, L"1e+40"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000000000000000.0          } }, L"1e+41"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000000000000000000.0         } }, L"1e+42"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000000000000000000.0        } }, L"1e+43"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000000000000000000.0       } }, L"1e+44"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000000000000000000000.0      } }, L"1e+45"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000000000000000000000.0     } }, L"1e+46"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000000000000000000000.0    } }, L"1e+47"s               },
        { { { JsonTokenType::Real, 1000000000000000000000000000000000000000000000000.0   } }, L"1e+48"s               },
        { { { JsonTokenType::Real, 10000000000000000000000000000000000000000000000000.0  } }, L"1e+49"s               },
        { { { JsonTokenType::Real, 100000000000000000000000000000000000000000000000000.0 } }, L"1e+50"s               },
      };

      for (auto [input, expected] : pairs)
      {
        auto os = wstringstream();
        JsonLinter::Write(os, input, 0);
        Assert::AreEqual<size_t>(0, input.size());
        Assert::AreEqual(expected, os.str());
      }
    }

    TEST_METHOD(TestWriteObject)
    {
      auto pairs = vector<tuple<uint8_t, deque<TOKEN>, wstring>>
      {
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } }, L"{}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":null}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":\"\\\"💰\\\", A, Á, B, C\"}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":true}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":false}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Real, 13.37 }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":13.37}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":{}}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":[]}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":{\"Key2\":true}}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":[true]}"s },
        { 0, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\"Key1\":true,\"Key2\":false}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } }, L"{}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": null\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": \"\\\"💰\\\", A, Á, B, C\"\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": true\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": false\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Real, 13.37 }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": 13.37\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": {}\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": []\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": {\r\n  \"Key2\": true\r\n }\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": [\r\n  true\r\n ]\r\n}"s },
        { 1, { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key1"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndObject, L"}"s } }, L"{\r\n \"Key1\": true,\r\n \"Key2\": false\r\n}"s },
      };

      for (auto [indentation, input, expected] : pairs)
      {
        auto os = wstringstream();
        JsonLinter::Write(os, input, indentation);
        Assert::AreEqual<size_t>(0, input.size());
        Assert::AreEqual(expected, os.str());
      }

      auto pairs2 = vector<tuple<deque<TOKEN>, string>>
      {
        { deque<TOKEN>{ }, "Parameter 'tokens' is empty!" },
        { { { JsonTokenType::StartObject, L"{"s } }, "Expected token: EndObject!" },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::Boolean, true } }, "Expected token: PropertyName!" },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Value does not exists"s } }, "Expected one of the following tokens: Null, String, Boolean, Real, Integer, StartObject or StartArray!" },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Value does not exists"s }, { JsonTokenType::PropertyName, L"Invalid value"s } }, "Expected one of the following tokens: Null, String, Boolean, Real, Integer, StartObject or StartArray!" },
      };

      for (auto[input, exceptionMessage] : pairs2)
      {
        auto os = wstringstream();
        auto inputRef = input;
        ExceptException<exception>([&]() { JsonLinter::Write(os, inputRef, 0ui8); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestWriteArray)
    {
      auto pairs = vector<tuple<uint8_t, deque<TOKEN>, wstring>>
      {
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } }, L"[]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndArray, L"]"s } }, L"[null]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\"\\\"💰\\\", A, Á, B, C\"]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s } }, L"[true]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[false]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Real, 13.37 }, { JsonTokenType::EndArray, L"]"s } }, L"[13.37]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[{}]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[[]]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[{\"Key2\":true}]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[[true]]"s },
        { 0, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[true,false]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s } }, L"[]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Null, nullptr }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n null\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::String, L"\"💰\", A, Á, B, C"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n \"\\\"💰\\\", A, Á, B, C\"\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n true\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n false\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Real, 13.37 }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n 13.37\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n {}\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n []\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::PropertyName, L"Key2"s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndObject, L"}"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n {\r\n  \"Key2\": true\r\n }\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::EndArray, L"]"s }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n [\r\n  true\r\n ]\r\n]"s },
        { 1, { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true }, { JsonTokenType::Boolean, false }, { JsonTokenType::EndArray, L"]"s } }, L"[\r\n true,\r\n false\r\n]"s },
      };

      for (auto [indentation, input, expected] : pairs)
      {
        auto os = wstringstream();
        JsonLinter::Write(os, input, indentation);
        Assert::AreEqual<size_t>(0, input.size());
        Assert::AreEqual(expected, os.str());
      }

      auto pairs2 = vector<tuple<deque<TOKEN>, string>>
      {
        { deque<TOKEN>{ }, "Parameter 'tokens' is empty!" },
        { { { JsonTokenType::StartArray, L"["s } }, "Expected token: EndArray!" },
        { { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::Boolean, true } }, "Expected token: EndArray!" },
        { { { JsonTokenType::StartArray, L"["s }, { JsonTokenType::PropertyName, L"Invalid token"s } }, "Expected one of the following tokens: Null, String, Boolean, Real, Integer, StartObject or StartArray!" },
      };

      for (auto[input, exceptionMessage] : pairs2)
      {
        auto os = wstringstream();
        auto inputRef = input;
        ExceptException<exception>([&]() { JsonLinter::Write(os, inputRef, 0ui8); }, exceptionMessage);
      }
    }

    TEST_METHOD(TestWrite)
    {
      auto pairs = vector<pair<deque<TOKEN>, wstring>>
      {
        { { { JsonTokenType::Null, nullptr      } }, L"null"s  },
        { { { JsonTokenType::String      , L"\"💰\", A, Á, B, C"s } }, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { { { JsonTokenType::PropertyName, L"\"💰\", A, Á, B, C"s } }, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { { { JsonTokenType::Boolean, true      } }, L"true"s  },
        { { { JsonTokenType::Boolean, false     } }, L"false"s },
        { { { JsonTokenType::Real, 13.37      } }, L"13.37"s },
        { { { JsonTokenType::StartObject, L"{"s }, { JsonTokenType::EndObject, L"}"s } }, L"{}"s },
        { { { JsonTokenType::StartArray , L"["s }, { JsonTokenType::EndArray , L"]"s } }, L"[]"s },
        { { { JsonTokenType::Undefined, nullptr } }, L"null"s  },
        { { { JsonTokenType::Undefined, L"\"💰\", A, Á, B, C"s } }, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { { { JsonTokenType::Undefined, true    } }, L"true"s  },
        { { { JsonTokenType::Undefined, false   } }, L"false"s },
        { { { JsonTokenType::Undefined, 13.37   } }, L"13.37"s },
      };

      for (auto [input, expected] : pairs)
      {
        auto os = wstringstream();
        JsonLinter::Write(os, input, JsonDefault::Indentation);
        Assert::AreEqual<size_t>(0, input.size());
        Assert::AreEqual(expected, os.str());
      }
    }

    TEST_METHOD(TestDump)
    {
      auto pairs = vector<pair<VALUE_TOKEN, wstring>>
      {
        { nullptr_t() , L"null"s },
        { L"\"💰\", A, Á, B, C"s, L"\"\\\"💰\\\", A, Á, B, C\""s },
        { true, L"true"s },
        { false, L"false"s },
        { 13.37, L"13.37"s }
      };

      for (auto [input, expected] : pairs)
      {
        Assert::AreEqual(expected, JsonLinter::Dump(input));
      }
    }
  };
}