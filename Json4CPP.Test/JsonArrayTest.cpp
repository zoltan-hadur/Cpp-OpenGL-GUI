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

    TEST_METHOD(TestConstructorJson)
    {
      JsonArray array;

      auto json = Json();
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>(nullptr, json);
      json = nullptr;
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>(nullptr, json);
      json = L"Test"s;
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>(L"Test"s, json);
      json = true;
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>(true, json);
      json = 1337;
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>(1337, json);
      json = 13.37;
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>(13.37, json);
      json = { { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } };
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>({ { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } }, json);
      json = { 1, 3, 3, 7 };
      array = json;
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, array);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);

      json = Json();
      Assert::ExpectException<exception>([&]() { array = move(json); });
      Assert::AreEqual<Json>(nullptr, json);
      json = nullptr;
      Assert::ExpectException<exception>([&]() { array = move(json); });
      Assert::AreEqual<Json>(nullptr, json);
      json = L"Test"s;
      Assert::ExpectException<exception>([&]() { array = move(json); });
      Assert::AreEqual<Json>(L"Test"s, json);
      json = true;
      Assert::ExpectException<exception>([&]() { array = move(json); });
      Assert::AreEqual<Json>(true, json);
      json = 1337;
      Assert::ExpectException<exception>([&]() { array = move(json); });
      Assert::AreEqual<Json>(1337, json);
      json = 13.37;
      Assert::ExpectException<exception>([&]() { array = move(json); });
      Assert::AreEqual<Json>(13.37, json);
      json = { { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } };
      Assert::ExpectException<exception>([&]() { array = json; });
      Assert::AreEqual<Json>({ { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } }, json);
      json = { 1, 3, 3, 7 };
      array = move(json);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, array);
      Assert::AreEqual(0i64, json.Size());
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      JsonArray array;

      auto builder = JsonBuilder();
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);

      // JsonBuilder from VALUE
      builder = JsonBuilder(VALUE(nullptr));
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(VALUE(L"Test"s));
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      builder = JsonBuilder(VALUE(true));
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder(VALUE(13.37));
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      builder = JsonBuilder(VALUE(1337i64));
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(1337, builder);
      builder = JsonBuilder(VALUE(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }));
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      builder = JsonBuilder(VALUE(JsonArray{ 1337, L"1337", true }));
      array = builder;
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1337, L"1337", true }, builder);

      // JsonBuilder constructors
      builder = JsonBuilder(nullptr);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(L"Test");
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test", builder);
      builder = JsonBuilder(L"Test"s);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      builder = JsonBuilder(true);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder('A');
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>('A', builder);
      builder = JsonBuilder(1i8);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(1i8, builder);
      builder = JsonBuilder(2ui8);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(2ui8, builder);
      builder = JsonBuilder(3i16);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(3i16, builder);
      builder = JsonBuilder(4ui16);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(4ui16, builder);
      builder = JsonBuilder(5i32);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(5i32, builder);
      builder = JsonBuilder(6ui32);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(6ui32, builder);
      builder = JsonBuilder(7i64);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(7i64, builder);
      builder = JsonBuilder(8ui64);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(8ui64, builder);
      builder = JsonBuilder(13.37f);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      builder = JsonBuilder(13.37);
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      builder = JsonBuilder(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      array = JsonBuilder(JsonArray{ 1337, L"1337", true });
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      builder = JsonBuilder(Json());
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(Json(), builder);
      builder = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      array = builder;
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>(Json(JsonArray({ 1337, L"1337", true })), builder);

      // JsonBuilder from initializer_list
      builder = JsonBuilder({ 1 });   // Deduced to JsonBuilder instead of initializer_list<JsonBuilder>
      Assert::ExpectException<exception>([&]() { array = builder; });
      Assert::AreEqual<JsonBuilder>(JsonBuilder({ 1 }), builder);
      builder = JsonBuilder({ { 1 } });
      array = builder;
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(1i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<JsonBuilder>({ { 1 } }, builder);
      builder = JsonBuilder({ 1, L"2"s });
      array = builder;
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<JsonBuilder>({ 1, L"2"s }, builder);
      builder = JsonBuilder({ { 1, L"2"s } });
      array = builder;
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(2i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<Json>(L"2"s, array[0][1]);
      Assert::AreEqual<JsonBuilder>({ { 1, L"2"s } }, builder);
      builder = JsonBuilder({ 1, L"2"s, true });
      array = builder;
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>({ 1, L"2"s, true }, builder);
      builder = JsonBuilder({ { 1, L"2"s, true } });
      array = builder;
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(3i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<Json>(L"2"s, array[0][1]);
      Assert::AreEqual<Json>(true, array[0][2]);
      Assert::AreEqual<JsonBuilder>({ { 1, L"2"s, true } }, builder);

      // JsonBuilder from vector
      builder = JsonBuilder(vector<JsonBuilder>{ 1 });
      array = builder;
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual<Json>(1i64, array[0]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1 }, builder);
      builder = JsonBuilder(vector<JsonBuilder>{ 1, L"2"s });
      array = builder;
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L"2"s }, builder);
      builder = JsonBuilder(vector<JsonBuilder>{ 1, L"2"s, true });
      array = builder;
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L"2"s, true }, builder);

      // Move
      builder = JsonBuilder();
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);

      // JsonBuilder from VALUE
      builder = JsonBuilder(VALUE(nullptr));
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(VALUE(L"Test"s));
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      builder = JsonBuilder(VALUE(true));
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder(VALUE(13.37));
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      builder = JsonBuilder(VALUE(1337i64));
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(1337, builder);
      builder = JsonBuilder(VALUE(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }));
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      builder = JsonBuilder(VALUE(JsonArray{ 1337, L"1337", true }));
      array = move(builder);
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray{ }, builder);

      // JsonBuilder constructors
      builder = JsonBuilder(nullptr);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(L"Test");
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test", builder);
      builder = JsonBuilder(L"Test"s);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      builder = JsonBuilder(true);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder('A');
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>('A', builder);
      builder = JsonBuilder(1i8);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(1i8, builder);
      builder = JsonBuilder(2ui8);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(2ui8, builder);
      builder = JsonBuilder(3i16);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(3i16, builder);
      builder = JsonBuilder(4ui16);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(4ui16, builder);
      builder = JsonBuilder(5i32);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(5i32, builder);
      builder = JsonBuilder(6ui32);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(6ui32, builder);
      builder = JsonBuilder(7i64);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(7i64, builder);
      builder = JsonBuilder(8ui64);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(8ui64, builder);
      builder = JsonBuilder(13.37f);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      builder = JsonBuilder(13.37);
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      builder = JsonBuilder(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      array = JsonBuilder(JsonArray{ 1337, L"1337", true });
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      builder = JsonBuilder(Json());
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(Json(), builder);
      builder = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      array = move(builder);
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1337, array[0]);
      Assert::AreEqual<Json>(L"1337", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>(Json(JsonArray({ })), builder);

      // JsonBuilder from initializer_list
      builder = JsonBuilder({ 1 });   // Deduced to JsonBuilder instead of initializer_list<JsonBuilder>
      Assert::ExpectException<exception>([&]() { array = move(builder); });
      Assert::AreEqual<JsonBuilder>(JsonBuilder({ 1 }), builder);
      builder = JsonBuilder({ { 1 } });
      array = move(builder);
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(1i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<JsonBuilder>({ { 1 } }, builder);
      builder = JsonBuilder({ 1, L"2"s });
      array = move(builder);
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<JsonBuilder>({ 1, L""s }, builder);
      builder = JsonBuilder({ { 1, L"2"s } });
      array = move(builder);
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(2i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<Json>(L"2"s, array[0][1]);
      Assert::AreEqual<JsonBuilder>({ { 1, L""s } }, builder);
      builder = JsonBuilder({ 1, L"2"s, true });
      array = move(builder);
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>({ 1, L""s, true }, builder);
      builder = JsonBuilder({ { 1, L"2"s, true } });
      array = move(builder);
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual(3i64, array[0].Size());
      Assert::AreEqual<Json>(1, array[0][0]);
      Assert::AreEqual<Json>(L"2"s, array[0][1]);
      Assert::AreEqual<Json>(true, array[0][2]);
      Assert::AreEqual<JsonBuilder>({ { 1, L""s, true } }, builder);

      // JsonBuilder from vector
      builder = JsonBuilder(vector<JsonBuilder>{ 1 });
      array = move(builder);
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual<Json>(1i64, array[0]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1 }, builder);
      builder = JsonBuilder(vector<JsonBuilder>{ 1, L"2"s });
      array = move(builder);
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L""s }, builder);
      builder = JsonBuilder(vector<JsonBuilder>{ 1, L"2"s, true });
      array = move(builder);
      Assert::AreEqual(3i64, array.Size());
      Assert::AreEqual<Json>(1, array[0]);
      Assert::AreEqual<Json>(L"2"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L""s, true }, builder);
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
        13.37,
        1337,
        {
          { L"Key1", 1 },
          { L"Key2", 2 }
        },
        { 1, 2, 3 }
      };
      Assert::AreEqual(7i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test", array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(13.37, array[3]);
      Assert::AreEqual<Json>(1337, array[4]);
      Assert::AreEqual<Json>({{ L"Key1", 1 }, { L"Key2", 2 }}, array[5]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[6]);
      Assert::AreEqual(2i64, array[5].Size());
      Assert::AreEqual(L"Key1"s, array[5].Keys()[0]);
      Assert::AreEqual(L"Key2"s, array[5].Keys()[1]);
      Assert::AreEqual<Json>(1, array[5][L"Key1"]);
      Assert::AreEqual<Json>(2, array[5][L"Key2"]);
      Assert::AreEqual(3i64, array[6].Size());
      Assert::AreEqual<Json>(1, array[6][0]);
      Assert::AreEqual<Json>(2, array[6][1]);
      Assert::AreEqual<Json>(3, array[6][2]);
    }

    TEST_METHOD(TestDump)
    {
      JsonArray array = { nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };

      Assert::AreEqual(L"[null,\"Test\",true,13.37,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump());
      Assert::AreEqual(L"[null,\"Test\",true,13.37,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump(0));
      Assert::AreEqual(L"[null,\"Test\",true,13.37,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump(0, L' '));
      Assert::AreEqual(L"[null,\"Test\",true,13.37,1337,{\"key1\":1,\"key2\":2},[1,2,3]]"s, array.Dump(0, L'\t'));

      Assert::AreEqual(
        L"["                      "\r\n"
         " null,"                 "\r\n"
         " \"Test\","             "\r\n"
         " true,"                 "\r\n"
         " 13.37,"                "\r\n"
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
         " 13.37,"                "\r\n"
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
         "\t13.37,"               "\r\n"
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
         "  13.37,"               "\r\n"
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
         "  13.37,"               "\r\n"
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
         "\t\t13.37,"             "\r\n"
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
      Assert::AreEqual(4i64, JsonArray{ nullptr, L"Test"s, true, 13.37 }.Size());
      Assert::AreEqual(5i64, JsonArray{ nullptr, L"Test"s, true, 13.37, 1337 }.Size());
      Assert::AreEqual(6i64, JsonArray{ nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }} }.Size());
      Assert::AreEqual(7i64, JsonArray{ nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } }.Size());
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
      array.Resize(1500, L"Test"s);
      for (int i = 0; i < 1337; ++i)
      {
        Assert::AreEqual<Json>(nullptr, array[i]);
      }
      for (int i = 1337; i < 1500; ++i)
      {
        Assert::AreEqual<Json>(L"Test"s, array[i]);
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
      Json null = nullptr;
      Json str = L"Test"s;
      Json boolean = true;
      Json real = 13.37;
      Json integer = 1337;
      Json object = { { L"key1", 1 }, { L"key2", 2 } };
      Json arr = { 1, 2, 3 };

      array.PushBack(null);
      array.PushBack(str);
      array.PushBack(boolean);
      array.PushBack(real);
      array.PushBack(integer);
      array.PushBack(object);
      array.PushBack(arr);
      Assert::AreEqual(7i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(13.37, array[3]);
      Assert::AreEqual<Json>(1337, array[4]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[5]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[6]);
      Assert::AreEqual<Json>(nullptr, null);
      Assert::AreEqual<Json>(L"Test"s, str);
      Assert::AreEqual<Json>(true, boolean);
      Assert::AreEqual<Json>(13.37, real);
      Assert::AreEqual<Json>(1337, integer);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, object);
      Assert::AreEqual<Json>({ 1, 2, 3 }, arr);

      array.Clear();
      array.PushBack(move(null));
      array.PushBack(move(str));
      array.PushBack(move(boolean));
      array.PushBack(move(real));
      array.PushBack(move(integer));
      array.PushBack(move(object));
      array.PushBack(move(arr));
      Assert::AreEqual(7i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(13.37, array[3]);
      Assert::AreEqual<Json>(1337, array[4]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[5]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[6]);
      Assert::AreEqual<Json>(nullptr, null);
      Assert::AreEqual<Json>(L""s, str);
      Assert::AreEqual<Json>(true, boolean);
      Assert::AreEqual<Json>(13.37, real);
      Assert::AreEqual<Json>(1337, integer);
      Assert::AreEqual<Json>(JsonObject{ }, object);
      Assert::AreEqual<Json>(JsonArray{ }, arr);
    }

    TEST_METHOD(TestInsert)
    {
      JsonArray array;
      Json null = nullptr;
      Json str = L"Test"s;
      Json boolean = true;
      Json real = 13.37;
      Json integer = 1337;
      Json object = { { L"key1", 1 }, { L"key2", 2 } };
      Json arr = { 1, 2, 3 };

      array.Resize(7);
      array.Insert(0, null);
      array.Insert(1, str);
      array.Insert(2, boolean);
      array.Insert(3, real);
      array.Insert(4, integer);
      array.Insert(5, object);
      array.Insert(6, arr);
      Assert::AreEqual(14i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(13.37, array[3]);
      Assert::AreEqual<Json>(1337, array[4]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[5]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[6]);
      Assert::AreEqual<Json>(nullptr, null);
      Assert::AreEqual<Json>(L"Test"s, str);
      Assert::AreEqual<Json>(true, boolean);
      Assert::AreEqual<Json>(13.37, real);
      Assert::AreEqual<Json>(1337, integer);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, object);
      Assert::AreEqual<Json>({ 1, 2, 3 }, arr);
      for (int i = 7; i < 14; ++i)
      {
        Assert::AreEqual<Json>(nullptr, array[i]);
      }

      array.Clear();
      array.Resize(7);
      array.Insert(0, move(null));
      array.Insert(1, move(str));
      array.Insert(2, move(boolean));
      array.Insert(3, move(real));
      array.Insert(4, move(integer));
      array.Insert(5, move(object));
      array.Insert(6, move(arr));
      Assert::AreEqual(14i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(13.37, array[3]);
      Assert::AreEqual<Json>(1337, array[4]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[5]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[6]);
      Assert::AreEqual<Json>(nullptr, null);
      Assert::AreEqual<Json>(L""s, str);
      Assert::AreEqual<Json>(true, boolean);
      Assert::AreEqual<Json>(13.37, real);
      Assert::AreEqual<Json>(1337, integer);
      Assert::AreEqual<Json>(JsonObject{ }, object);
      Assert::AreEqual<Json>(JsonArray{ }, arr);
      for (int i = 7; i < 14; ++i)
      {
        Assert::AreEqual<Json>(nullptr, array[i]);
      }
    }

    TEST_METHOD(TestErase)
    {
      auto array = JsonArray{ nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      array.Erase(1);
      array.Erase(3);
      Assert::AreEqual(5i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(true, array[1]);
      Assert::AreEqual<Json>(13.37, array[2]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[3]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[4]);
    }

    TEST_METHOD(TestOperatorSubscript)
    {
      auto array = JsonArray{ nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(13.37, array[3]);
      Assert::AreEqual<Json>(1337, array[4]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[5]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[6]);

      for (int i = 0; i < array.Size(); ++i)
      {
        array[i] = 1337;
        Assert::AreEqual<Json>(1337, array[i]);
      }
    }

    TEST_METHOD(TestAtConst)
    {
      auto const array = JsonArray{ nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      Assert::AreEqual<Json>(nullptr, array.At(0));
      Assert::AreEqual<Json>(L"Test"s, array.At(1));
      Assert::AreEqual<Json>(true, array.At(2));
      Assert::AreEqual<Json>(13.37, array.At(3));
      Assert::AreEqual<Json>(1337, array.At(4));
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array.At(5));
      Assert::AreEqual<Json>({ 1, 2, 3 }, array.At(6));

      static_assert(is_const<remove_reference<decltype(array.At(0))>::type>::value, "JsonArray::At(index) return type must be Json const&");
    }

    TEST_METHOD(TestAt)
    {
      auto array = JsonArray{ nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
      Assert::AreEqual<Json>(true, array[2]);
      Assert::AreEqual<Json>(13.37, array[3]);
      Assert::AreEqual<Json>(1337, array[4]);
      Assert::AreEqual<Json>({ { L"key1", 1 }, { L"key2", 2 } }, array[5]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, array[6]);

      for (int i = 0; i < array.Size(); ++i)
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
      JsonArray array = { nullptr, L"Test"s, true, 13.37, 1337, { { L"key1", 1 }, { L"key2", 2 } }, { 1, 2, 3 } };
      wstringstream is;
      is << array;
      Assert::AreEqual(
        L"["                      "\r\n"
         "  null,"                "\r\n"
         "  \"Test\","            "\r\n"
         "  true,"                "\r\n"
         "  13.37,"               "\r\n"
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
                    "  13.37,"               "\r\n"
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
      JsonArray expected = { nullptr, L"Test"s, true, 13.37, 1337, {{ L"key1", 1 }, { L"key2", 2 }}, { 1, 2, 3 } };
      JsonArray result;
      wstringstream ss(input);
      ss >> result;
      Assert::AreEqual(expected, result);
      for (int i = 0; i < expected.Size(); ++i)
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
      JsonArray numberi1 = { 0 };
      JsonArray numberi2 = { 1 };
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
      Assert::IsFalse(array1   == numberi1);
      Assert::IsFalse(array1   == numberi2);
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
      Assert::IsFalse(array2   == numberi1);
      Assert::IsFalse(array2   == numberi2);
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
      Assert::IsTrue (boolean1 == numberi1);
      Assert::IsFalse(boolean1 == numberi2);
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
      Assert::IsFalse(boolean2 == numberi1);
      Assert::IsTrue (boolean2 == numberi2);
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
      Assert::IsFalse(null     == numberi1);
      Assert::IsFalse(null     == numberi2);
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
      Assert::IsTrue (number1  == numberi1);
      Assert::IsFalse(number1  == numberi2);
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
      Assert::IsFalse(number2  == numberi1);
      Assert::IsTrue (number2  == numberi2);
      Assert::IsFalse(number2  == object1 );
      Assert::IsFalse(number2  == object2 );
      Assert::IsFalse(number2  == string1 );
      Assert::IsFalse(number2  == string2 );
      Assert::IsFalse(numberi1 == array1  );
      Assert::IsFalse(numberi1 == array2  );
      Assert::IsTrue (numberi1 == boolean1);
      Assert::IsFalse(numberi1 == boolean2);
      Assert::IsFalse(numberi1 == null    );
      Assert::IsTrue (numberi1 == number1 );
      Assert::IsFalse(numberi1 == number2 );
      Assert::IsTrue (numberi1 == numberi1);
      Assert::IsFalse(numberi1 == numberi2);
      Assert::IsFalse(numberi1 == object1 );
      Assert::IsFalse(numberi1 == object2 );
      Assert::IsFalse(numberi1 == string1 );
      Assert::IsFalse(numberi1 == string2 );
      Assert::IsFalse(numberi2 == array1  );
      Assert::IsFalse(numberi2 == array2  );
      Assert::IsFalse(numberi2 == boolean1);
      Assert::IsTrue (numberi2 == boolean2);
      Assert::IsFalse(numberi2 == null    );
      Assert::IsFalse(numberi2 == number1 );
      Assert::IsTrue (numberi2 == number2 );
      Assert::IsFalse(numberi2 == numberi1);
      Assert::IsTrue (numberi2 == numberi2);
      Assert::IsFalse(numberi2 == object1 );
      Assert::IsFalse(numberi2 == object2 );
      Assert::IsFalse(numberi2 == string1 );
      Assert::IsFalse(numberi2 == string2 );
      Assert::IsFalse(object1  == array1  );
      Assert::IsFalse(object1  == array2  );
      Assert::IsFalse(object1  == boolean1);
      Assert::IsFalse(object1  == boolean2);
      Assert::IsFalse(object1  == null    );
      Assert::IsFalse(object1  == number1 );
      Assert::IsFalse(object1  == number2 );
      Assert::IsFalse(object1  == numberi1);
      Assert::IsFalse(object1  == numberi2);
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
      Assert::IsFalse(object2  == numberi1);
      Assert::IsFalse(object2  == numberi2);
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
      Assert::IsFalse(string1  == numberi1);
      Assert::IsFalse(string1  == numberi2);
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
      Assert::IsFalse(string2  == numberi1);
      Assert::IsFalse(string2  == numberi2);
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
      JsonArray numberi1 = { 0 };
      JsonArray numberi2 = { 1 };
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
      Assert::IsTrue (array1   != numberi1);
      Assert::IsTrue (array1   != numberi2);
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
      Assert::IsTrue (array2   != numberi1);
      Assert::IsTrue (array2   != numberi2);
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
      Assert::IsFalse(boolean1 != numberi1);
      Assert::IsTrue (boolean1 != numberi2);
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
      Assert::IsTrue (boolean2 != numberi1);
      Assert::IsFalse(boolean2 != numberi2);
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
      Assert::IsTrue (null     != numberi1);
      Assert::IsTrue (null     != numberi2);
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
      Assert::IsFalse(number1  != numberi1);
      Assert::IsTrue (number1  != numberi2);
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
      Assert::IsTrue (number2  != numberi1);
      Assert::IsFalse(number2  != numberi2);
      Assert::IsTrue (number2  != object1 );
      Assert::IsTrue (number2  != object2 );
      Assert::IsTrue (number2  != string1 );
      Assert::IsTrue (number2  != string2 );
      Assert::IsTrue (numberi1 != array1  );
      Assert::IsTrue (numberi1 != array2  );
      Assert::IsFalse(numberi1 != boolean1);
      Assert::IsTrue (numberi1 != boolean2);
      Assert::IsTrue (numberi1 != null    );
      Assert::IsFalse(numberi1 != number1 );
      Assert::IsTrue (numberi1 != number2 );
      Assert::IsFalse(numberi1 != numberi1);
      Assert::IsTrue (numberi1 != numberi2);
      Assert::IsTrue (numberi1 != object1 );
      Assert::IsTrue (numberi1 != object2 );
      Assert::IsTrue (numberi1 != string1 );
      Assert::IsTrue (numberi1 != string2 );
      Assert::IsTrue (numberi2 != array1  );
      Assert::IsTrue (numberi2 != array2  );
      Assert::IsTrue (numberi2 != boolean1);
      Assert::IsFalse(numberi2 != boolean2);
      Assert::IsTrue (numberi2 != null    );
      Assert::IsTrue (numberi2 != number1 );
      Assert::IsFalse(numberi2 != number2 );
      Assert::IsTrue (numberi2 != numberi1);
      Assert::IsFalse(numberi2 != numberi2);
      Assert::IsTrue (numberi2 != object1 );
      Assert::IsTrue (numberi2 != object2 );
      Assert::IsTrue (numberi2 != string1 );
      Assert::IsTrue (numberi2 != string2 );
      Assert::IsTrue (object1  != array1  );
      Assert::IsTrue (object1  != array2  );
      Assert::IsTrue (object1  != boolean1);
      Assert::IsTrue (object1  != boolean2);
      Assert::IsTrue (object1  != null    );
      Assert::IsTrue (object1  != number1 );
      Assert::IsTrue (object1  != number2 );
      Assert::IsTrue (object1  != numberi1);
      Assert::IsTrue (object1  != numberi2);
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
      Assert::IsTrue (object2  != numberi1);
      Assert::IsTrue (object2  != numberi2);
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
      Assert::IsTrue (string1  != numberi1);
      Assert::IsTrue (string1  != numberi2);
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
      Assert::IsTrue (string2  != numberi1);
      Assert::IsTrue (string2  != numberi2);
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