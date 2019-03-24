#include "stdafx.h"
#include "CppUnitTest.h"
#include <optional>

#include "..\Json4CPP\Json.hpp"
#include "..\Foundation\Enumerable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace OpenGLUI::Foundation;

namespace Json4CPP::Test
{
  TEST_CLASS(ValueTest)
  {
  private:
    static _CrtMemState _init;
    static _CrtMemState _dest;
    static _CrtMemState _result;
  public:
    TEST_METHOD(TestGetType1)
    {
      auto input = vector<VALUE>{ nullptr_t(), wstring(), bool(), double(), JsonObject(), JsonArray() };
      auto expected = vector<JsonType>{ JsonType::Null, JsonType::String, JsonType::Boolean, JsonType::Number, JsonType::Object, JsonType::Array };
      auto output = From(input).Select<JsonType>([](VALUE const& value) { return GetType(value); });
      Assert::IsTrue(From(expected).SequenceEqual(output));
    }

    TEST_METHOD(TestGetType2)
    {
      auto input = vector<VALUE_BUILDER>
      {
        nullptr_t(), wstring(), bool(), double(), JsonObject(), JsonArray(), vector<JsonBuilder>(), vector<JsonBuilder>{ JsonBuilder(L"Key"), JsonBuilder(L"Value") },
        vector<JsonBuilder>{ vector<JsonBuilder>{ JsonBuilder(L"Key0"), JsonBuilder(L"Value0") }, vector<JsonBuilder>{ JsonBuilder(L"Key1"), JsonBuilder(L"Value1") } },
        vector<JsonBuilder>{ JsonBuilder(1), JsonBuilder(2), JsonBuilder(3) }
      };
      auto expected = vector<JsonBuilderType>
      {
        JsonBuilderType::Null, JsonBuilderType::String, JsonBuilderType::Boolean, JsonBuilderType::Number, JsonBuilderType::Object, JsonBuilderType::Array,
        JsonBuilderType::Empty, JsonBuilderType::Pair, JsonBuilderType::Object, JsonBuilderType::Array
      };
      auto output = From(input).Select<JsonBuilderType>([](VALUE_BUILDER const& value) { return GetType(value); });
      Assert::IsTrue(From(expected).SequenceEqual(output));
    }

    TEST_METHOD(TestParseNull1)
    {
      auto input = vector<wstring>{ L"", L" ", L" n", L" nu", L" nul", L" null", L" null " , L" nu11 ", L" nill ", L" mull " };
      auto expected = vector<bool>{ false, false, false, false, false, true, true, false, false, false };
      for (int i = 0; i < input.size(); ++i)
      {
        if (expected[i])
        {
          Assert::IsTrue(nullptr == ParseNull(wstringstream(input[i])));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { ParseNull(wstringstream(input[i])); });
        }
      }
    }

    TEST_METHOD(TestParseNull2)
    {
      auto input = vector<wstring>{ L"", L" ", L" n", L" nu", L" nul", L" null", L" null " , L" nu11 ", L" nill ", L" mull " };
      auto expected = vector<bool>{ false, false, false, false, false, true, true, false, false, false };
      for (int i = 0; i < input.size(); ++i)
      {
        if (expected[i])
        {
          Assert::IsTrue(nullptr == ParseNull(input[i]));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { ParseNull(input[i]); });
        }
      }
    }

    TEST_METHOD(TestParseBoolean1)
    {
      auto input = vector<wstring>{ L"", L" ", L" t", L" tr", L" tru", L" true", L" true " , L" trie ", L"", L" ", L" f", L" fa", L" fal", L" fals", L" false" , L" false " };
      auto expectException = vector<bool>{ true, true, true, true, true, false, false, true, true, true, true, true, true, true, false, false };
      auto expected = vector<bool>{ false, false, false, false, false, true, true, false, true, true, true, true, true, true, false, false };
      for (int i = 14; i < input.size(); ++i)
      {
        if (!expectException[i])
        {
          Assert::IsTrue(expected[i] == ParseBoolean(wstringstream(input[i])));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { ParseBoolean(wstringstream(input[i])); });
        }
      }
    }

    TEST_METHOD(TestParseBoolean2)
    {
      auto input = vector<wstring>{ L"", L" ", L" t", L" tr", L" tru", L" true", L" true " , L" trie ", L"", L" ", L" f", L" fa", L" fal", L" fals", L" false" , L" false " };
      auto expectException = vector<bool>{ true, true, true, true, true, false, false, true, true, true, true, true, true, true, false, false };
      auto expected = vector<bool>{ false, false, false, false, false, true, true, false, true, true, true, true, true, true, false, false };
      for (int i = 14; i < input.size(); ++i)
      {
        if (!expectException[i])
        {
          Assert::IsTrue(expected[i] == ParseBoolean(input[i]));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { ParseBoolean(input[i]); });
        }
      }
    }

    TEST_METHOD(TestValueWrite)
    {
      auto input = vector<VALUE>{ nullptr, L"Hello \"World\""s, false, true, 13.37,
        JsonObject{ { L"a", 1 }, { L"b", 2.5 } },
        JsonArray{ 1, 2, 3 }
      };
      
      auto expected = vector<wstring>
      {
        LR"(null)"s,
        LR"("Hello \"World\"")"s,
        LR"(false)"s,
        LR"(true)"s,
        LR"(13.37)"s,
        LR"({
  "a": 1,
  "b": 2.5
})"s,
        LR"([
  1,
  2,
  3
])"s,
      };

      auto output = From(input).Select<wstring>([](VALUE const& value)
      {
        auto os = wostringstream();
        ValueWrite(os, value);
        return os.str();
      });
      
      Assert::IsTrue(From(expected).SequenceEqual(output));
    }

    TEST_CLASS_INITIALIZE(ClassInitialize)
    {
      _CrtMemCheckpoint(&_init);
    }

    TEST_CLASS_CLEANUP(ClassCleanup)
    {
      _CrtMemCheckpoint(&_dest);
      if (_CrtMemDifference(&_result, &_init, &_dest))
      {
        _CrtMemDumpStatistics(&_result);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
        _CrtDumpMemoryLeaks();
        Assert::Fail();
      }
    }
  };
  _CrtMemState ValueTest::_init;
  _CrtMemState ValueTest::_dest;
  _CrtMemState ValueTest::_result;
}