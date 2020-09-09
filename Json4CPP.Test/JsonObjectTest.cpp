#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonObjectTest)
  {
  public:
    TEST_METHOD(TestConstructorDefault)
    {
      auto object = JsonObject();
      Assert::AreEqual(0i64, object.Size());
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      JsonObject object;

      Assert::ExpectException<exception>([&]() { object = JsonBuilder(); });

      // JsonBuilder from VALUE
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(VALUE(nullptr_t())); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(VALUE(wstring())); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(VALUE(bool())); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(VALUE(double())); });
      object = JsonBuilder(VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Key2"]);
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(VALUE(JsonArray())); });

      // JsonBuilder constructors
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(nullptr_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(L""); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(wstring()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(bool()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(char()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(int8_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(uint8_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(int16_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(uint16_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(int32_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(uint32_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(int64_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(uint64_t()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(float()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(double()); });
      object = JsonBuilder(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Key2"]);
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(JsonArray()); });
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(Json()); });
      object = JsonBuilder(Json(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Key2"]);

      // JsonBuilder from initializer_list
      Assert::ExpectException<exception>([&]() { object = JsonBuilder({ L"Key1", 1 }); });
      object = JsonBuilder({ { L"Key1", 1 } });
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      object = JsonBuilder({ { L"Key1", 1 }, { L"Key2", 2 } });
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Key2"]);

      // JsonBuilder from vector
      Assert::ExpectException<exception>([&]() { object = JsonBuilder(vector<JsonBuilder>{ L"Key1", 1 }); });
      object = JsonBuilder(vector<JsonBuilder>{ { L"Key1", 1 } });
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      object = JsonBuilder(vector<JsonBuilder>{ { L"Key1", 1 }, { L"Key2", 2 } });
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Key2"]);
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      JsonObject object;

      object = { { L"Key1", 1 } };
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      object = { { L"Key1", 1 }, { L"Key2", 2 } };
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Key2"]);
      object = {
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual(6i64, object.Size());
      Assert::AreEqual(L"Null"s, object.Keys()[0]);
      Assert::AreEqual(L"String"s, object.Keys()[1]);
      Assert::AreEqual(L"Boolean"s, object.Keys()[2]);
      Assert::AreEqual(L"Number"s, object.Keys()[3]);
      Assert::AreEqual(L"Object"s, object.Keys()[4]);
      Assert::AreEqual(L"Array"s, object.Keys()[5]);
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(L"Test", object[L"String"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(1337, object[L"Number"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
      Assert::AreEqual(L"Key1"s, object[L"Object"].Keys()[0]);
      Assert::AreEqual(L"Key2"s, object[L"Object"].Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Object"][L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Object"][L"Key2"]);
      Assert::AreEqual(3i64, object[L"Array"].Size());
      Assert::AreEqual<Json>(1, object[L"Array"][0]);
      Assert::AreEqual<Json>(2, object[L"Array"][1]);
      Assert::AreEqual<Json>(3, object[L"Array"][2]);
    }

    TEST_METHOD(TestConstructorJsonObject)
    {
      JsonObject object = { { L"Key1", 1 }, { L"Key2", 2 } };

      auto object2 = object;
      Assert::AreEqual(2i64, object2.Size());
      Assert::AreEqual(L"Key1"s, object2.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object2.Keys()[1]);
      Assert::AreEqual<Json>(1, object2[L"Key1"]);
      Assert::AreEqual<Json>(2, object2[L"Key2"]);
    }

    TEST_METHOD(TestDump)
    {
      JsonObject object = {
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };

      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Number\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump());
      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Number\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump(0));
      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Number\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump(0, L' '));
      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Number\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump(0, L'\t'));

      Assert::AreEqual(
        L"{"                            "\r\n"
         " \"Null\": null,"             "\r\n"
         " \"String\": \"Test\","       "\r\n"
         " \"Boolean\": true,"          "\r\n"
         " \"Number\": 1337,"           "\r\n"
         " \"Object\": {"               "\r\n"
         "  \"Key1\": 1,"               "\r\n"
         "  \"Key2\": 2"                "\r\n"
         " },"                          "\r\n"
         " \"Array\": ["                "\r\n"
         "  1,"                         "\r\n"
         "  2,"                         "\r\n"
         "  3"                          "\r\n"
         " ]"                           "\r\n"
         "}"s, object.Dump(1));

      Assert::AreEqual(
        L"{"                            "\r\n"
         " \"Null\": null,"             "\r\n"
         " \"String\": \"Test\","       "\r\n"
         " \"Boolean\": true,"          "\r\n"
         " \"Number\": 1337,"           "\r\n"
         " \"Object\": {"               "\r\n"
         "  \"Key1\": 1,"               "\r\n"
         "  \"Key2\": 2"                "\r\n"
         " },"                          "\r\n"
         " \"Array\": ["                "\r\n"
         "  1,"                         "\r\n"
         "  2,"                         "\r\n"
         "  3"                          "\r\n"
         " ]"                           "\r\n"
         "}"s, object.Dump(1, L' '));

      Assert::AreEqual(
        L"{"                            "\r\n"
         "\t\"Null\": null,"            "\r\n"
         "\t\"String\": \"Test\","      "\r\n"
         "\t\"Boolean\": true,"         "\r\n"
         "\t\"Number\": 1337,"          "\r\n"
         "\t\"Object\": {"              "\r\n"
         "\t\t\"Key1\": 1,"             "\r\n"
         "\t\t\"Key2\": 2"              "\r\n"
         "\t},"                         "\r\n"
         "\t\"Array\": ["               "\r\n"
         "\t\t1,"                       "\r\n"
         "\t\t2,"                       "\r\n"
         "\t\t3"                        "\r\n"
         "\t]"                          "\r\n"
         "}"s, object.Dump(1, L'\t'));

      Assert::AreEqual(
        L"{"                            "\r\n"
         "  \"Null\": null,"            "\r\n"
         "  \"String\": \"Test\","      "\r\n"
         "  \"Boolean\": true,"         "\r\n"
         "  \"Number\": 1337,"          "\r\n"
         "  \"Object\": {"              "\r\n"
         "    \"Key1\": 1,"             "\r\n"
         "    \"Key2\": 2"              "\r\n"
         "  },"                         "\r\n"
         "  \"Array\": ["               "\r\n"
         "    1,"                       "\r\n"
         "    2,"                       "\r\n"
         "    3"                        "\r\n"
         "  ]"                          "\r\n"
         "}"s, object.Dump(2));

      Assert::AreEqual(
        L"{"                            "\r\n"
         "  \"Null\": null,"            "\r\n"
         "  \"String\": \"Test\","      "\r\n"
         "  \"Boolean\": true,"         "\r\n"
         "  \"Number\": 1337,"          "\r\n"
         "  \"Object\": {"              "\r\n"
         "    \"Key1\": 1,"             "\r\n"
         "    \"Key2\": 2"              "\r\n"
         "  },"                         "\r\n"
         "  \"Array\": ["               "\r\n"
         "    1,"                       "\r\n"
         "    2,"                       "\r\n"
         "    3"                        "\r\n"
         "  ]"                          "\r\n"
         "}"s, object.Dump(2, L' '));

      Assert::AreEqual(
        L"{"                            "\r\n"
         "\t\t\"Null\": null,"          "\r\n"
         "\t\t\"String\": \"Test\","    "\r\n"
         "\t\t\"Boolean\": true,"       "\r\n"
         "\t\t\"Number\": 1337,"        "\r\n"
         "\t\t\"Object\": {"            "\r\n"
         "\t\t\t\t\"Key1\": 1,"         "\r\n"
         "\t\t\t\t\"Key2\": 2"          "\r\n"
         "\t\t},"                       "\r\n"
         "\t\t\"Array\": ["             "\r\n"
         "\t\t\t\t1,"                   "\r\n"
         "\t\t\t\t2,"                   "\r\n"
         "\t\t\t\t3"                    "\r\n"
         "\t\t]"                        "\r\n"
         "}"s, object.Dump(2, L'\t'));
    }

    TEST_METHOD(TestSize)
    {
      Assert::AreEqual(0i64, JsonObject().Size());
      Assert::AreEqual(0i64, JsonObject{}.Size());
      Assert::AreEqual(1i64, JsonObject{ { L"Null", nullptr } }.Size());
      Assert::AreEqual(2i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" } }.Size());
      Assert::AreEqual(3i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true } }.Size());
      Assert::AreEqual(4i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 } }.Size());
      Assert::AreEqual(5i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 }, { L"Object", {{ L"Key1", 1 }, { L"Key2", 2 } } } }.Size());
      Assert::AreEqual(6i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 }, { L"Object", {{ L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } }.Size());
    }

    TEST_METHOD(TestClear)
    {
      auto object = JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } };
      object.Clear();
      Assert::AreEqual(0i64, object.Size());
    }

    TEST_METHOD(TestInsert)
    {
      JsonObject object;
      object.Insert({ L"Null", nullptr });
      object.Insert({ L"String", L"Test" });
      object.Insert({ L"Boolean", true });
      object.Insert({ L"Number", 1337 });
      object.Insert({ L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } });
      object.Insert({ L"Array", { 1, 2, 3 } });
      Assert::AreEqual(6i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(L"Test"s, object[L"String"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(1337, object[L"Number"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
    }

    TEST_METHOD(TestErase)
    {
      JsonObject object = {
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      object.Erase(L"String");
      object.Erase(L"Number");
      Assert::AreEqual(4i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
    }

    TEST_METHOD(TestKeys)
    {
      JsonObject object = {
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual(6i64, object.Size());
      Assert::AreEqual(6ui64, object.Keys().size());
      Assert::AreEqual(L"Null"s, object.Keys()[0]);
      Assert::AreEqual(L"String"s, object.Keys()[1]);
      Assert::AreEqual(L"Boolean"s, object.Keys()[2]);
      Assert::AreEqual(L"Number"s, object.Keys()[3]);
      Assert::AreEqual(L"Object"s, object.Keys()[4]);
      Assert::AreEqual(L"Array"s, object.Keys()[5]);
    }

    TEST_METHOD(TestOperatorIndex)
    {
      JsonObject object = {
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual(6i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(L"Test"s, object[L"String"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(1337, object[L"Number"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
      Assert::AreEqual<Json>(1, object[L"Object"][L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Object"][L"Key2"]);
      Assert::AreEqual<Json>(1, object[L"Array"][0]);
      Assert::AreEqual<Json>(2, object[L"Array"][1]);
      Assert::AreEqual<Json>(3, object[L"Array"][2]);

      for (auto key : object.Keys())
      {
        object[key] = 1337;
        Assert::AreEqual<Json>(1337, object[key]);
      }
    }

    TEST_METHOD(TestAtConst)
    {
      auto const object = JsonObject{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      auto map = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      Assert::AreEqual(6i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object.At(L"Null"));
      Assert::AreEqual<Json>(L"Test"s, object.At(L"String"));
      Assert::AreEqual<Json>(true, object.At(L"Boolean"));
      Assert::AreEqual<Json>(1337, object.At(L"Number"));
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object.At(L"Object"));
      Assert::AreEqual<Json>({ 1, 2, 3 }, object.At(L"Array"));
      Assert::AreEqual<Json>(1, object.At(L"Object")[L"Key1"]);
      Assert::AreEqual<Json>(2, object.At(L"Object")[L"Key2"]);
      Assert::AreEqual<Json>(1, object.At(L"Array")[0]);
      Assert::AreEqual<Json>(2, object.At(L"Array")[1]);
      Assert::AreEqual<Json>(3, object.At(L"Array")[2]);

      for (auto key : object.Keys())
      {
        object.At(key) = 9999;
        Assert::AreNotEqual<Json>(9999, object.At(key));
        Assert::AreEqual<Json>(map[key], object.At(key));
      }
    }

    TEST_METHOD(TestAt)
    {
      auto object = JsonObject{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual(6i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object.At(L"Null"));
      Assert::AreEqual<Json>(L"Test"s, object.At(L"String"));
      Assert::AreEqual<Json>(true, object.At(L"Boolean"));
      Assert::AreEqual<Json>(1337, object.At(L"Number"));
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object.At(L"Object"));
      Assert::AreEqual<Json>({ 1, 2, 3 }, object.At(L"Array"));
      Assert::AreEqual<Json>(1, object.At(L"Object")[L"Key1"]);
      Assert::AreEqual<Json>(2, object.At(L"Object")[L"Key2"]);
      Assert::AreEqual<Json>(1, object.At(L"Array")[0]);
      Assert::AreEqual<Json>(2, object.At(L"Array")[1]);
      Assert::AreEqual<Json>(3, object.At(L"Array")[2]);

      for (auto key : object.Keys())
      {
        object.At(key) = 9999;
        Assert::AreEqual<Json>(9999, object.At(key));
      }
    }

    TEST_METHOD(TestIterator)
    {
      auto object = JsonObject{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      auto map = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto& [key, value] : object)
      {
        value = 1337;
      }
      for (auto key : object.Keys())
      {
        Assert::AreEqual<Json>(1337, object[key]);
      }

      object = JsonObject{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      map = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto [key, value] : object)
      {
        value = 1337;
      }
      for (auto key : object.Keys())
      {
        Assert::AreEqual<Json>(map[key], object[key]);
      }
    }

    TEST_METHOD(TestIteratorConst)
    {
      auto const object = JsonObject{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      auto const map = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto [key, value] : object)
      {
        value = 1337;
      }
      for (auto key : object.Keys())
      {
        Assert::AreEqual<Json>(map.at(key), object.At(key));
      }

      auto const& object2 = JsonObject{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      auto const& map2 = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Number", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto [key, value] : object2)
      {
        value = 1337;
      }
      for (auto key : object2.Keys())
      {
        Assert::AreEqual<Json>(map2.at(key), object2.At(key));
      }
    }

    TEST_METHOD(TestOperatorInsertion)
    {
      JsonObject object = {
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      wstringstream is;
      is << object;
      Assert::AreEqual(
        L"{"                            "\r\n"
         "  \"Null\": null,"            "\r\n"
         "  \"String\": \"Test\","      "\r\n"
         "  \"Boolean\": true,"         "\r\n"
         "  \"Number\": 1337,"          "\r\n"
         "  \"Object\": {"              "\r\n"
         "    \"Key1\": 1,"             "\r\n"
         "    \"Key2\": 2"              "\r\n"
         "  },"                         "\r\n"
         "  \"Array\": ["               "\r\n"
         "    1,"                       "\r\n"
         "    2,"                       "\r\n"
         "    3"                        "\r\n"
         "  ]"                          "\r\n"
         "}"s, is.str());

      auto pairs = vector<pair<vector<variant<JsonObject, JsonIndentSize, JsonIndentChar>>, wstring>>
      {
        { { JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}"s },
        { { JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}" },
        { { JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonIndentSize(1), JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}{\r\n \"Key1\": 1,\r\n \"Key2\": 2\r\n}{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}" },
        { { JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonIndentChar(L'\t'), JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}{\r\n\t\t\"Key1\": 1,\r\n\t\t\"Key2\": 2\r\n}{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}" },
        { { JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonIndentSize(1), JsonIndentChar(L'\t'), JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}{\r\n\t\"Key1\": 1,\r\n\t\"Key2\": 2\r\n}{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}" },
        { { JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonIndentChar(L'\t'), JsonIndentSize(1), JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } }, JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}{\r\n\t\"Key1\": 1,\r\n\t\"Key2\": 2\r\n}{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}" }
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
      auto input = L"{"                            "\r\n"
                    "  \"Null\": null,"            "\r\n"
                    "  \"String\": \"Test\","      "\r\n"
                    "  \"Boolean\": true,"         "\r\n"
                    "  \"Number\": 1337,"          "\r\n"
                    "  \"Object\": {"              "\r\n"
                    "    \"Key1\": 1,"             "\r\n"
                    "    \"Key2\": 2"              "\r\n"
                    "  },"                         "\r\n"
                    "  \"Array\": ["               "\r\n"
                    "    1,"                       "\r\n"
                    "    2,"                       "\r\n"
                    "    3"                        "\r\n"
                    "  ]"                          "\r\n"
                    "}"s;
      JsonObject expected = {
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Number", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      JsonObject result;
      wstringstream ss(input);
      ss >> result;
      Assert::AreEqual(expected, result);
      for (auto key : expected.Keys())
      {
        Assert::AreEqual(expected[key], result[key]);
      }
    }

    TEST_METHOD(TestOperatorEqual)
    {
      JsonObject null = { { L"Key", nullptr } };
      JsonObject string1 = { { L"Key", L"TestString0"s } };
      JsonObject string2 = { { L"Key", L"TestString1"s } };
      JsonObject boolean1 = { { L"Key", false } };
      JsonObject boolean2 = { { L"Key", true } };
      JsonObject number1 = { { L"Key", 0.0 } };
      JsonObject number2 = { { L"Key", 1.0 } };
      JsonObject object1 = { { L"Key", JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      } } };
      JsonObject object2 = { { L"Key", JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      } } };
      JsonObject array1 = { { L"Key", JsonArray{ 1, 2, 3 } } };
      JsonObject array2 = { { L"Key", JsonArray{ 4, 5, 6 } } };

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

      Assert::IsFalse(JsonObject{ { L"Key", JsonArray{ 1, 2 } } } == JsonObject{ { L"Key", JsonArray{ 1 } } });
      Assert::IsTrue (JsonObject{ { L"Key", JsonArray{ 1, 2 } } } == JsonObject{ { L"Key", JsonArray{ 1, 2 } } });
      Assert::IsFalse(JsonObject{ { L"Key", JsonArray{ 1, 2 } } } == JsonObject{ { L"Key", JsonArray{ 1, 2, 3 } } });
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      JsonObject null = { { L"Key", nullptr } };
      JsonObject string1 = { { L"Key", L"TestString0"s } };
      JsonObject string2 = { { L"Key", L"TestString1"s } };
      JsonObject boolean1 = { { L"Key", false } };
      JsonObject boolean2 = { { L"Key", true } };
      JsonObject number1 = { { L"Key", 0.0 } };
      JsonObject number2 = { { L"Key", 1.0 } };
      JsonObject object1 = { { L"Key", JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      } } };
      JsonObject object2 = { { L"Key", JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      } } };
      JsonObject array1 = { { L"Key", JsonArray{ 1, 2, 3 } } };
      JsonObject array2 = { { L"Key", JsonArray{ 4, 5, 6 } } };

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

      Assert::IsTrue (JsonObject{ { L"Key", JsonArray{ 1, 2 } } } != JsonObject{ { L"Key", JsonArray{ 1 } } });
      Assert::IsFalse(JsonObject{ { L"Key", JsonArray{ 1, 2 } } } != JsonObject{ { L"Key", JsonArray{ 1, 2 } } });
      Assert::IsTrue (JsonObject{ { L"Key", JsonArray{ 1, 2 } } } != JsonObject{ { L"Key", JsonArray{ 1, 2, 3 } } });
    }
  };
}