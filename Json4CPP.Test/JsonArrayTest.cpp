#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonArrayTest)
  {
  public:
    TEST_METHOD(TestConstructorDefault)
    {
      auto array = JsonArray();
      Assert::AreEqual(0i64, array.Size());
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      JsonArray array;

      Assert::ExpectException<exception>([&]() { array = JsonBuilder(); });

      // JsonBuilder from VALUE
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(VALUE(nullptr_t())); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(VALUE(wstring())); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(VALUE(bool())); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(VALUE(double())); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(VALUE(JsonObject())); });
      array = JsonBuilder(VALUE(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);

      // JsonBuilder constructors
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(nullptr_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(L""); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(wstring()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(bool()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(char()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(int8_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(uint8_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(int16_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(uint16_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(int32_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(uint32_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(int64_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(uint64_t()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(float()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(double()); });
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(JsonObject()); });
      array = JsonBuilder(JsonArray({ 1337, L"1337", true }));
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::ExpectException<exception>([&]() { array = JsonBuilder(Json()); });
      array = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);

      // JsonBuilder from initializer_list
      Assert::ExpectException<exception>([&]() { array = JsonBuilder({ 1 }); });  // Deduced to JsonBuilder instead of initializer_list<JsonBuilder>
      array = JsonBuilder({ { 1 } });
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(1i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      array = JsonBuilder({ 1, 2 });
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(2, array[1]);
      array = JsonBuilder({ { 1, 2 } });
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(2i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<Json>(2, array[0][1]);
      array = JsonBuilder({ 1, 2, 3 });
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(2, array[1]);
      Assert::AreEqual<Json>(3, array[2]);
      array = JsonBuilder({ { 1, 2, 3 } });
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(3i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<Json>(2, array[0][1]);
      Assert::AreEqual<Json>(3, array[0][2]);

      // JsonBuilder from vector
      array = JsonBuilder(vector<JsonBuilder>{ 1 });
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual<Json>(1i64, array[0]);
      array = JsonBuilder(vector<JsonBuilder>{ 1, 2 });
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(2, array[1]);
      array = JsonBuilder(vector<JsonBuilder>{ 1, 2, 3 });
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(2, array[1]);
      Assert::AreEqual<Json>(3, array[2]);
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      JsonArray array;

      array = { 1 };
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual<Json>(1i64, array[0]);
      array = { 1, 2 };
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(2, array[1]);
      array = { 1, 2, 3 };
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(2, array[1]);
      Assert::AreEqual<Json>(3, array[2]);
      array = {
        nullptr,
        L"Test",
        true,
        1337,
        {
          { L"Key1", 1 },
          { L"Key2", 2 }
        },
        { 1, 2, 3 }
      };
      Assert::AreEqual(6i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(1337, array[3]);
      Assert::AreEqual<Json>({{ L"Key1", 1 }, { L"Key2", 2 }}, array[4]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[5]);
      Assert::AreEqual(2i64, array[4].Size());
      Assert::AreEqual(L"Key1"s, array[4].Keys()[0]);
      Assert::AreEqual(L"Key2"s, array[4].Keys()[1]);
      Assert::AreEqual<Json>(1, array[4][L"Key1"]);
      Assert::AreEqual<Json>(2, array[4][L"Key2"]);
      Assert::AreEqual(3i64, array[5].Size());
      Assert::AreEqual<Json>(1, array[5][0]);
      Assert::AreEqual<Json>(2, array[5][1]);
      Assert::AreEqual<Json>(3, array[5][2]);
    }

    TEST_METHOD(TestConstructorJsonArray)
    {
      JsonArray array = { 1, 2, 3 };

      auto array2 = array;
      Assert::AreEqual(3i64, array2.Size());
      Assert::AreEqual<Json>(1, array2[0]);
      Assert::AreEqual<Json>(2, array2[1]);
      Assert::AreEqual<Json>(3, array2[2]);
    }

    TEST_METHOD(TestDump)
    {
      JsonArray array = { nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };

      Assert::AreEqual(L"[null,\"Test\",true,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump());
      Assert::AreEqual(L"[null,\"Test\",true,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump(0));
      Assert::AreEqual(L"[null,\"Test\",true,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump(0, L' '));
      Assert::AreEqual(L"[null,\"Test\",true,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump(0, L'\t'));

      Assert::AreEqual(
        L"["                      "\r\n"
         " null,"                 "\r\n"
         " \"Test\","             "\r\n"
         " true,"                 "\r\n"
         " 1337,"                 "\r\n"
         " {"                     "\r\n"
         "  \"key1\": 1,"         "\r\n"
         "  \"key2\": 2"          "\r\n"
         " },"                    "\r\n"
         " ["                     "\r\n"
         "  1,"                   "\r\n"
         "  2,"                   "\r\n"
         "  3"                    "\r\n"
         " ]"                     "\r\n"
         "]"s, array.Dump(1));

      Assert::AreEqual(
        L"["                      "\r\n"
         " null,"                 "\r\n"
         " \"Test\","             "\r\n"
         " true,"                 "\r\n"
         " 1337,"                 "\r\n"
         " {"                     "\r\n"
         "  \"key1\": 1,"         "\r\n"
         "  \"key2\": 2"          "\r\n"
         " },"                    "\r\n"
         " ["                     "\r\n"
         "  1,"                   "\r\n"
         "  2,"                   "\r\n"
         "  3"                    "\r\n"
         " ]"                     "\r\n"
         "]"s, array.Dump(1, L' '));

      Assert::AreEqual(
        L"["                      "\r\n"
         "\tnull,"                "\r\n"
         "\t\"Test\","            "\r\n"
         "\ttrue,"                "\r\n"
         "\t1337,"                "\r\n"
         "\t{"                    "\r\n"
         "\t\t\"key1\": 1,"       "\r\n"
         "\t\t\"key2\": 2"        "\r\n"
         "\t},"                   "\r\n"
         "\t["                    "\r\n"
         "\t\t1,"                 "\r\n"
         "\t\t2,"                 "\r\n"
         "\t\t3"                  "\r\n"
         "\t]"                    "\r\n"
         "]"s, array.Dump(1, L'\t'));

      Assert::AreEqual(
        L"["                      "\r\n"
         "  null,"                "\r\n"
         "  \"Test\","            "\r\n"
         "  true,"                "\r\n"
         "  1337,"                "\r\n"
         "  {"                    "\r\n"
         "    \"key1\": 1,"       "\r\n"
         "    \"key2\": 2"        "\r\n"
         "  },"                   "\r\n"
         "  ["                    "\r\n"
         "    1,"                 "\r\n"
         "    2,"                 "\r\n"
         "    3"                  "\r\n"
         "  ]"                    "\r\n"
         "]"s, array.Dump(2));

      Assert::AreEqual(
        L"["                      "\r\n"
         "  null,"                "\r\n"
         "  \"Test\","            "\r\n"
         "  true,"                "\r\n"
         "  1337,"                "\r\n"
         "  {"                    "\r\n"
         "    \"key1\": 1,"       "\r\n"
         "    \"key2\": 2"        "\r\n"
         "  },"                   "\r\n"
         "  ["                    "\r\n"
         "    1,"                 "\r\n"
         "    2,"                 "\r\n"
         "    3"                  "\r\n"
         "  ]"                    "\r\n"
         "]"s, array.Dump(2, L' '));

      Assert::AreEqual(
        L"["                      "\r\n"
         "\t\tnull,"              "\r\n"
         "\t\t\"Test\","          "\r\n"
         "\t\ttrue,"              "\r\n"
         "\t\t1337,"              "\r\n"
         "\t\t{"                  "\r\n"
         "\t\t\t\t\"key1\": 1,"   "\r\n"
         "\t\t\t\t\"key2\": 2"    "\r\n"
         "\t\t},"                 "\r\n"
         "\t\t["                  "\r\n"
         "\t\t\t\t1,"             "\r\n"
         "\t\t\t\t2,"             "\r\n"
         "\t\t\t\t3"              "\r\n"
         "\t\t]"                  "\r\n"
         "]"s, array.Dump(2, L'\t'));
    }

    TEST_METHOD(TestSize)
    {
      Assert::AreEqual(0i64, JsonArray().Size());
      Assert::AreEqual(0i64, JsonArray{}.Size());
      Assert::AreEqual(1i64, JsonArray{ nullptr }.Size());
      Assert::AreEqual(2i64, JsonArray{ nullptr, L"Test"s }.Size());
      Assert::AreEqual(3i64, JsonArray{ nullptr, L"Test"s, true }.Size());
      Assert::AreEqual(4i64, JsonArray{ nullptr, L"Test"s, true, 1337 }.Size());
      Assert::AreEqual(5i64, JsonArray{ nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }} }.Size());
      Assert::AreEqual(6i64, JsonArray{ nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } }.Size());
    }

    TEST_METHOD(TestResize)
    {
      auto array = JsonArray();
      Assert::AreEqual(0i64, array.Size());
      array.Resize(1337);
      Assert::AreEqual(1337i64, array.Size());
      for (int i = 0; i < 1337; ++i)
      {
        Assert::AreEqual<Json>(nullptr, array[i]);
      }
    }

    TEST_METHOD(TestClear)
    {
      auto array = JsonArray{ 1, 2, 3 };
      array.Clear();
      Assert::AreEqual(0i64, array.Size());
    }

    TEST_METHOD(TestPushBack)
    {
      JsonArray array;
      array.PushBack(nullptr);
      array.PushBack(L"Test"s);
      array.PushBack(true);
      array.PushBack(1337);
      array.PushBack({ { L"key1", 1 }, { L"key2", 2 } });
      array.PushBack({ 1, 2, 3 });
      Assert::AreEqual(6i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(1337, array[3]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[4]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[5]);
    }

    TEST_METHOD(TestInsert)
    {
      JsonArray array;
      array.Resize(6);
      array.Insert(0, nullptr);
      array.Insert(1, L"Test"s);
      array.Insert(2, true);
      array.Insert(3, 1337);
      array.Insert(4, { { L"key1", 1 }, { L"key2", 2 } });
      array.Insert(5, { 1, 2, 3 });
      Assert::AreEqual(12i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(1337, array[3]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[4]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[5]);
      for (int i = 6; i < 12; ++i)
      {
        Assert::AreEqual<Json>(nullptr, array[i]);
      }
    }

    TEST_METHOD(TestErase)
    {
      auto array = JsonArray{ nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      array.Erase(1);
      array.Erase(3);
      Assert::AreEqual(4i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(true, array[1]);
      Assert::AreEqual<Json>(1337, array[2]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[3]);
    }

    TEST_METHOD(TestOperatorIndex)
    {
      auto array = JsonArray{ nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(1337, array[3]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[4]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[5]);

      for (int i = 0; i < 6; ++i)
      {
        array[i] = 1337;
        Assert::AreEqual<Json>(1337, array[i]);
      }
    }

    TEST_METHOD(TestAtConst)
    {
      auto const array = JsonArray{ nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      Assert::AreEqual<Json>(nullptr, array.At(0));
      Assert::AreEqual<Json>(L"Test"s, array.At(1));
      Assert::AreEqual<Json>(true, array.At(2));
      Assert::AreEqual<Json>(1337, array.At(3));
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array.At(4));
      Assert::AreEqual<Json>({ 1, 2, 3 }, array.At(5));

      static_assert(is_const<remove_reference<decltype(array.At(0))>::type>::value, "JsonArray::At(index) return type must be Json const&");
    }

    TEST_METHOD(TestAt)
    {
      auto array = JsonArray{ nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(1337, array[3]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[4]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[5]);

      for (int i = 0; i < 6; ++i)
      {
        array.At(i) = 9999;
        Assert::AreEqual<Json>(9999, array.At(i));
      }
    }

    TEST_METHOD(TestIterator)
    {
      auto array = JsonArray{ 1, 2, 3, 4, 5 };
      for (auto& value : array)
      {
        value = 1337;
      }
      for (int i = 0; i < array.Size(); ++i)
      {
        Assert::AreEqual<Json>(1337, array[i]);
      }

      array = JsonArray{ 1, 2, 3, 4, 5 };
      for (auto value : array)
      {
        value = 1337;
      }
      for (int i = 0; i < array.Size(); ++i)
      {
        Assert::AreEqual<Json>(i + 1, array[i]);
      }
    }

    TEST_METHOD(TestIteratorConst)
    {
      auto const array = JsonArray{ 1, 2, 3, 4, 5 };
      for (auto value : array)
      {
        value = 1337;
      }
      for (int i = 0; i < array.Size(); ++i)
      {
        Assert::AreEqual<Json>(i + 1, array.At(i));
      }

      auto const& array2 = JsonArray{ 1, 2, 3, 4, 5 };
      for (auto value : array2)
      {
        value = 1337;
      }
      for (int i = 0; i < array2.Size(); ++i)
      {
        Assert::AreEqual<Json>(i + 1, array2.At(i));
      }
    }

    TEST_METHOD(TestOperatorInsertion)
    {
      JsonArray array = { nullptr, L"Test"s, true, 1337, { { L"key1", 1 }, { L"key2", 2 } }, { 1, 2, 3 } };
      wstringstream is;
      is << array;
      Assert::AreEqual(
        L"["                      "\r\n"
         "  null,"                "\r\n"
         "  \"Test\","            "\r\n"
         "  true,"                "\r\n"
         "  1337,"                "\r\n"
         "  {"                    "\r\n"
         "    \"key1\": 1,"       "\r\n"
         "    \"key2\": 2"        "\r\n"
         "  },"                   "\r\n"
         "  ["                    "\r\n"
         "    1,"                 "\r\n"
         "    2,"                 "\r\n"
         "    3"                  "\r\n"
         "  ]"                    "\r\n"
         "]"s, is.str());

      auto pairs = vector<pair<vector<variant<JsonArray, JsonIndentSize, JsonIndentChar>>, wstring>>
      {
        { { JsonArray{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]"s },
        { { JsonArray{ 1, 3, 3, 7 }, JsonArray{ 1, 3, 3, 7 }, JsonArray{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { JsonArray{ 1, 3, 3, 7 }, JsonIndentSize(1), JsonArray{ 1, 3, 3, 7 }, JsonArray{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n 1,\r\n 3,\r\n 3,\r\n 7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { JsonArray{ 1, 3, 3, 7 }, JsonIndentChar(L'\t'), JsonArray{ 1, 3, 3, 7 }, JsonArray{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t\t1,\r\n\t\t3,\r\n\t\t3,\r\n\t\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { JsonArray{ 1, 3, 3, 7 }, JsonIndentSize(1), JsonIndentChar(L'\t'), JsonArray{ 1, 3, 3, 7 }, JsonArray{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t1,\r\n\t3,\r\n\t3,\r\n\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { JsonArray{ 1, 3, 3, 7 }, JsonIndentChar(L'\t'), JsonIndentSize(1), JsonArray{ 1, 3, 3, 7 }, JsonArray{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t1,\r\n\t3,\r\n\t3,\r\n\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" }
      };
      for (auto& [inputs, expected] : pairs)
      {
        wstringstream is;
        for (auto& input : inputs)
        {
          visit(Overload{
            [&](auto const& v) { is << v; }
          }, input);
        }
        Assert::AreEqual(expected, is.str());
      }
    }

    TEST_METHOD(TestOperatorExtraction)
    {
      auto input = L"["                      "\r\n"
                    "  null,"                "\r\n"
                    "  \"Test\","            "\r\n"
                    "  true,"                "\r\n"
                    "  1337,"                "\r\n"
                    "  {"                    "\r\n"
                    "    \"key1\": 1,"       "\r\n"
                    "    \"key2\": 2"        "\r\n"
                    "  },"                   "\r\n"
                    "  ["                    "\r\n"
                    "    1,"                 "\r\n"
                    "    2,"                 "\r\n"
                    "    3"                  "\r\n"
                    "  ]"                    "\r\n"
                    "]"s;
      JsonArray expected = { nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      JsonArray result;
      wstringstream ss(input);
      ss >> result;
      Assert::AreEqual(expected, result);
      for (int i = 0; i < 6; ++i)
      {
        Assert::AreEqual(expected[i], result[i]);
      }
    }

    TEST_METHOD(TestOperatorEqual)
    {
      JsonArray null = { nullptr };
      JsonArray string1 = { L"TestString0"s };
      JsonArray string2 = { L"TestString1"s };
      JsonArray boolean1 = { false };
      JsonArray boolean2 = { true };
      JsonArray number1 = { 0.0 };
      JsonArray number2 = { 1.0 };
      JsonArray object1 = { JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      } };
      JsonArray object2 = { JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      } };
      JsonArray array1 = { JsonArray{ 1, 2, 3 } };
      JsonArray array2 = { JsonArray{ 4, 5, 6 } };

      Assert::IsTrue (array1   == array1  );
      Assert::IsFalse(array1   == array2  );
      Assert::IsFalse(array1   == boolean1);
      Assert::IsFalse(array1   == boolean2);
      Assert::IsFalse(array1   == null    );
      Assert::IsFalse(array1   == number1 );
      Assert::IsFalse(array1   == number2 );
      Assert::IsFalse(array1   == object1 );
      Assert::IsFalse(array1   == object2 );
      Assert::IsFalse(array1   == string1 );
      Assert::IsFalse(array1   == string2 );
      Assert::IsFalse(array2   == array1  );
      Assert::IsTrue (array2   == array2  );
      Assert::IsFalse(array2   == boolean1);
      Assert::IsFalse(array2   == boolean2);
      Assert::IsFalse(array2   == null    );
      Assert::IsFalse(array2   == number1 );
      Assert::IsFalse(array2   == number2 );
      Assert::IsFalse(array2   == object1 );
      Assert::IsFalse(array2   == object2 );
      Assert::IsFalse(array2   == string1 );
      Assert::IsFalse(array2   == string2 );
      Assert::IsFalse(boolean1 == array1  );
      Assert::IsFalse(boolean1 == array2  );
      Assert::IsTrue (boolean1 == boolean1);
      Assert::IsFalse(boolean1 == boolean2);
      Assert::IsFalse(boolean1 == null    );
      Assert::IsTrue (boolean1 == number1 );
      Assert::IsFalse(boolean1 == number2 );
      Assert::IsFalse(boolean1 == object1 );
      Assert::IsFalse(boolean1 == object2 );
      Assert::IsFalse(boolean1 == string1 );
      Assert::IsFalse(boolean1 == string2 );
      Assert::IsFalse(boolean2 == array1  );
      Assert::IsFalse(boolean2 == array2  );
      Assert::IsFalse(boolean2 == boolean1);
      Assert::IsTrue (boolean2 == boolean2);
      Assert::IsFalse(boolean2 == null    );
      Assert::IsFalse(boolean2 == number1 );
      Assert::IsTrue (boolean2 == number2 );
      Assert::IsFalse(boolean2 == object1 );
      Assert::IsFalse(boolean2 == object2 );
      Assert::IsFalse(boolean2 == string1 );
      Assert::IsFalse(boolean2 == string2 );
      Assert::IsFalse(null     == array1  );
      Assert::IsFalse(null     == array2  );
      Assert::IsFalse(null     == boolean1);
      Assert::IsFalse(null     == boolean2);
      Assert::IsTrue (null     == null    );
      Assert::IsFalse(null     == number1 );
      Assert::IsFalse(null     == number2 );
      Assert::IsFalse(null     == object1 );
      Assert::IsFalse(null     == object2 );
      Assert::IsFalse(null     == string1 );
      Assert::IsFalse(null     == string2 );
      Assert::IsFalse(number1  == array1  );
      Assert::IsFalse(number1  == array2  );
      Assert::IsTrue (number1  == boolean1);
      Assert::IsFalse(number1  == boolean2);
      Assert::IsFalse(number1  == null    );
      Assert::IsTrue (number1  == number1 );
      Assert::IsFalse(number1  == number2 );
      Assert::IsFalse(number1  == object1 );
      Assert::IsFalse(number1  == object2 );
      Assert::IsFalse(number1  == string1 );
      Assert::IsFalse(number1  == string2 );
      Assert::IsFalse(number2  == array1  );
      Assert::IsFalse(number2  == array2  );
      Assert::IsFalse(number2  == boolean1);
      Assert::IsTrue (number2  == boolean2);
      Assert::IsFalse(number2  == null    );
      Assert::IsFalse(number2  == number1 );
      Assert::IsTrue (number2  == number2 );
      Assert::IsFalse(number2  == object1 );
      Assert::IsFalse(number2  == object2 );
      Assert::IsFalse(number2  == string1 );
      Assert::IsFalse(number2  == string2 );
      Assert::IsFalse(object1  == array1  );
      Assert::IsFalse(object1  == array2  );
      Assert::IsFalse(object1  == boolean1);
      Assert::IsFalse(object1  == boolean2);
      Assert::IsFalse(object1  == null    );
      Assert::IsFalse(object1  == number1 );
      Assert::IsFalse(object1  == number2 );
      Assert::IsTrue (object1  == object1 );
      Assert::IsFalse(object1  == object2 );
      Assert::IsFalse(object1  == string1 );
      Assert::IsFalse(object1  == string2 );
      Assert::IsFalse(object2  == array1  );
      Assert::IsFalse(object2  == array2  );
      Assert::IsFalse(object2  == boolean1);
      Assert::IsFalse(object2  == boolean2);
      Assert::IsFalse(object2  == null    );
      Assert::IsFalse(object2  == number1 );
      Assert::IsFalse(object2  == number2 );
      Assert::IsFalse(object2  == object1 );
      Assert::IsTrue (object2  == object2 );
      Assert::IsFalse(object2  == string1 );
      Assert::IsFalse(object2  == string2 );
      Assert::IsFalse(string1  == array1  );
      Assert::IsFalse(string1  == array2  );
      Assert::IsFalse(string1  == boolean1);
      Assert::IsFalse(string1  == boolean2);
      Assert::IsFalse(string1  == null    );
      Assert::IsFalse(string1  == number1 );
      Assert::IsFalse(string1  == number2 );
      Assert::IsFalse(string1  == object1 );
      Assert::IsFalse(string1  == object2 );
      Assert::IsTrue (string1  == string1 );
      Assert::IsFalse(string1  == string2 );
      Assert::IsFalse(string2  == array1  );
      Assert::IsFalse(string2  == array2  );
      Assert::IsFalse(string2  == boolean1);
      Assert::IsFalse(string2  == boolean2);
      Assert::IsFalse(string2  == null    );
      Assert::IsFalse(string2  == number1 );
      Assert::IsFalse(string2  == number2 );
      Assert::IsFalse(string2  == object1 );
      Assert::IsFalse(string2  == object2 );
      Assert::IsFalse(string2  == string1 );
      Assert::IsTrue (string2  == string2 );

      Assert::IsFalse(JsonArray{ 1, 2 } == JsonArray{ 1 });
      Assert::IsTrue (JsonArray{ 1, 2 } == JsonArray{ 1, 2 });
      Assert::IsFalse(JsonArray{ 1, 2 } == JsonArray{ 1, 2, 3 });
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      JsonArray null = { nullptr };
      JsonArray string1 = { L"TestString0"s };
      JsonArray string2 = { L"TestString1"s };
      JsonArray boolean1 = { false };
      JsonArray boolean2 = { true };
      JsonArray number1 = { 0.0 };
      JsonArray number2 = { 1.0 };
      JsonArray object1 = { JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      } };
      JsonArray object2 = { JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      } };
      JsonArray array1 = { JsonArray{ 1, 2, 3 } };
      JsonArray array2 = { JsonArray{ 4, 5, 6 } };

      Assert::IsFalse(array1   != array1  );
      Assert::IsTrue (array1   != array2  );
      Assert::IsTrue (array1   != boolean1);
      Assert::IsTrue (array1   != boolean2);
      Assert::IsTrue (array1   != null    );
      Assert::IsTrue (array1   != number1 );
      Assert::IsTrue (array1   != number2 );
      Assert::IsTrue (array1   != object1 );
      Assert::IsTrue (array1   != object2 );
      Assert::IsTrue (array1   != string1 );
      Assert::IsTrue (array1   != string2 );
      Assert::IsTrue (array2   != array1  );
      Assert::IsFalse(array2   != array2  );
      Assert::IsTrue (array2   != boolean1);
      Assert::IsTrue (array2   != boolean2);
      Assert::IsTrue (array2   != null    );
      Assert::IsTrue (array2   != number1 );
      Assert::IsTrue (array2   != number2 );
      Assert::IsTrue (array2   != object1 );
      Assert::IsTrue (array2   != object2 );
      Assert::IsTrue (array2   != string1 );
      Assert::IsTrue (array2   != string2 );
      Assert::IsTrue (boolean1 != array1  );
      Assert::IsTrue (boolean1 != array2  );
      Assert::IsFalse(boolean1 != boolean1);
      Assert::IsTrue (boolean1 != boolean2);
      Assert::IsTrue (boolean1 != null    );
      Assert::IsFalse(boolean1 != number1 );
      Assert::IsTrue (boolean1 != number2 );
      Assert::IsTrue (boolean1 != object1 );
      Assert::IsTrue (boolean1 != object2 );
      Assert::IsTrue (boolean1 != string1 );
      Assert::IsTrue (boolean1 != string2 );
      Assert::IsTrue (boolean2 != array1  );
      Assert::IsTrue (boolean2 != array2  );
      Assert::IsTrue (boolean2 != boolean1);
      Assert::IsFalse(boolean2 != boolean2);
      Assert::IsTrue (boolean2 != null    );
      Assert::IsTrue (boolean2 != number1 );
      Assert::IsFalse(boolean2 != number2 );
      Assert::IsTrue (boolean2 != object1 );
      Assert::IsTrue (boolean2 != object2 );
      Assert::IsTrue (boolean2 != string1 );
      Assert::IsTrue (boolean2 != string2 );
      Assert::IsTrue (null     != array1  );
      Assert::IsTrue (null     != array2  );
      Assert::IsTrue (null     != boolean1);
      Assert::IsTrue (null     != boolean2);
      Assert::IsFalse(null     != null    );
      Assert::IsTrue (null     != number1 );
      Assert::IsTrue (null     != number2 );
      Assert::IsTrue (null     != object1 );
      Assert::IsTrue (null     != object2 );
      Assert::IsTrue (null     != string1 );
      Assert::IsTrue (null     != string2 );
      Assert::IsTrue (number1  != array1  );
      Assert::IsTrue (number1  != array2  );
      Assert::IsFalse(number1  != boolean1);
      Assert::IsTrue (number1  != boolean2);
      Assert::IsTrue (number1  != null    );
      Assert::IsFalse(number1  != number1 );
      Assert::IsTrue (number1  != number2 );
      Assert::IsTrue (number1  != object1 );
      Assert::IsTrue (number1  != object2 );
      Assert::IsTrue (number1  != string1 );
      Assert::IsTrue (number1  != string2 );
      Assert::IsTrue (number2  != array1  );
      Assert::IsTrue (number2  != array2  );
      Assert::IsTrue (number2  != boolean1);
      Assert::IsFalse(number2  != boolean2);
      Assert::IsTrue (number2  != null    );
      Assert::IsTrue (number2  != number1 );
      Assert::IsFalse(number2  != number2 );
      Assert::IsTrue (number2  != object1 );
      Assert::IsTrue (number2  != object2 );
      Assert::IsTrue (number2  != string1 );
      Assert::IsTrue (number2  != string2 );
      Assert::IsTrue (object1  != array1  );
      Assert::IsTrue (object1  != array2  );
      Assert::IsTrue (object1  != boolean1);
      Assert::IsTrue (object1  != boolean2);
      Assert::IsTrue (object1  != null    );
      Assert::IsTrue (object1  != number1 );
      Assert::IsTrue (object1  != number2 );
      Assert::IsFalse(object1  != object1 );
      Assert::IsTrue (object1  != object2 );
      Assert::IsTrue (object1  != string1 );
      Assert::IsTrue (object1  != string2 );
      Assert::IsTrue (object2  != array1  );
      Assert::IsTrue (object2  != array2  );
      Assert::IsTrue (object2  != boolean1);
      Assert::IsTrue (object2  != boolean2);
      Assert::IsTrue (object2  != null    );
      Assert::IsTrue (object2  != number1 );
      Assert::IsTrue (object2  != number2 );
      Assert::IsTrue (object2  != object1 );
      Assert::IsFalse(object2  != object2 );
      Assert::IsTrue (object2  != string1 );
      Assert::IsTrue (object2  != string2 );
      Assert::IsTrue (string1  != array1  );
      Assert::IsTrue (string1  != array2  );
      Assert::IsTrue (string1  != boolean1);
      Assert::IsTrue (string1  != boolean2);
      Assert::IsTrue (string1  != null    );
      Assert::IsTrue (string1  != number1 );
      Assert::IsTrue (string1  != number2 );
      Assert::IsTrue (string1  != object1 );
      Assert::IsTrue (string1  != object2 );
      Assert::IsFalse(string1  != string1 );
      Assert::IsTrue (string1  != string2 );
      Assert::IsTrue (string2  != array1  );
      Assert::IsTrue (string2  != array2  );
      Assert::IsTrue (string2  != boolean1);
      Assert::IsTrue (string2  != boolean2);
      Assert::IsTrue (string2  != null    );
      Assert::IsTrue (string2  != number1 );
      Assert::IsTrue (string2  != number2 );
      Assert::IsTrue (string2  != object1 );
      Assert::IsTrue (string2  != object2 );
      Assert::IsTrue (string2  != string1 );
      Assert::IsFalse(string2  != string2 );

      Assert::IsTrue (JsonArray{ 1, 2 } != JsonArray{ 1 });
      Assert::IsFalse(JsonArray{ 1, 2 } != JsonArray{ 1, 2 });
      Assert::IsTrue (JsonArray{ 1, 2 } != JsonArray{ 1, 2, 3 });
    }
  };
}