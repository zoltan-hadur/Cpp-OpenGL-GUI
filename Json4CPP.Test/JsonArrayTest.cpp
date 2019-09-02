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
         "   null,"               "\r\n"
         "   \"Test\","           "\r\n"
         "   true,"               "\r\n"
         "   1337,"               "\r\n"
         "   {"                   "\r\n"
         "      \"key1\": 1,"     "\r\n"
         "      \"key2\": 2"      "\r\n"
         "   },"                  "\r\n"
         "   ["                   "\r\n"
         "      1,"               "\r\n"
         "      2,"               "\r\n"
         "      3"                "\r\n"
         "   ]"                   "\r\n"
         "]"s, array.Dump(3));

      Assert::AreEqual(
        L"["                      "\r\n"
         "    null,"              "\r\n"
         "    \"Test\","          "\r\n"
         "    true,"              "\r\n"
         "    1337,"              "\r\n"
         "    {"                  "\r\n"
         "        \"key1\": 1,"   "\r\n"
         "        \"key2\": 2"    "\r\n"
         "    },"                 "\r\n"
         "    ["                  "\r\n"
         "        1,"             "\r\n"
         "        2,"             "\r\n"
         "        3"              "\r\n"
         "    ]"                  "\r\n"
         "]"s, array.Dump(4));
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
      array.Insert(nullptr, 0);
      array.Insert(L"Test"s, 1);
      array.Insert(true, 2);
      array.Insert(1337, 3);
      array.Insert({ { L"key1", 1 }, { L"key2", 2 } }, 4);
      array.Insert({ 1, 2, 3 }, 5);
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
      auto vec = vector<Json>{ nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      Assert::AreEqual<Json>(nullptr, array.At(0));
      Assert::AreEqual<Json>(L"Test"s, array.At(1));
      Assert::AreEqual<Json>(true, array.At(2));
      Assert::AreEqual<Json>(1337, array.At(3));
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array.At(4));
      Assert::AreEqual<Json>({ 1, 2, 3 }, array.At(5));

      for (int i = 0; i < 6; ++i)
      {
        array.At(i) = 9999;
        Assert::AreNotEqual<Json>(9999, array.At(i));
        Assert::AreEqual<Json>(vec[i], array.At(i));
      }
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
      JsonDefault::Indentation = 2;
      JsonArray array = { nullptr, L"Test"s, true, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
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
    }
  };
}