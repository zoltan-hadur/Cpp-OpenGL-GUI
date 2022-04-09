#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace std::filesystem;
using namespace Json4CPP;
using namespace Json4CPP::Detail;
using namespace Json4CPP::Helper;
using namespace OpenGLUI::Foundation;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonTest)
  {
  public:
    TEST_METHOD(TestLiteral)
    {
      Assert::AreEqual<Json>({ nullptr }, L"[null]"_Json);
      Assert::AreEqual<Json>({ L"TestString" }, L"[\"TestString\"]"_Json);
      Assert::AreEqual<Json>({ true }, L"[true]"_Json);
      Assert::AreEqual<Json>({ 13.37 }, L"[13.37]"_Json);
      Assert::AreEqual<Json>({ 1337 }, L"[1337]"_Json);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, L"{ \"Key1\": 1, \"Key2\": 2 }"_Json);
      Assert::AreEqual<Json>({ 1, 2, 3 }, L"[ 1, 2, 3 ]"_Json);
    }

    TEST_METHOD(TestConstructorDefault)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::AreEqual<Json>(nullptr, json);
    }

    TEST_METHOD(TestConstructorJson)
    {
      auto json = Json(nullptr);
      Assert::AreEqual<Json>(nullptr, Json(json));
      Assert::AreEqual<Json>(nullptr, json);
      json = Json(L"Test");
      Assert::AreEqual<Json>(L"Test", Json(json));
      Assert::AreEqual<Json>(L"Test"s, json);
      auto str = L"Test"s;
      json = Json(str);
      Assert::AreEqual<Json>(L"Test"s, Json(json));
      Assert::AreEqual<Json>(L"Test"s, json);
      json = Json(L"Test"s);
      Assert::AreEqual<Json>(L"Test"s, Json(json));
      Assert::AreEqual<Json>(L"Test"s, json);
      json = Json(false);
      Assert::AreEqual<Json>(false, Json(json));
      Assert::AreEqual<Json>(false, json);
      json = Json(true);
      Assert::AreEqual<Json>(true, Json(json));
      Assert::AreEqual<Json>(true, json);
      json = Json((char)1);
      Assert::AreEqual<Json>((char)1, Json(json));
      Assert::AreEqual<Json>((char)1, json);
      json = Json(2i8);
      Assert::AreEqual<Json>(2i8, Json(json));
      Assert::AreEqual<Json>(2i8, json);
      json = Json(3ui8);
      Assert::AreEqual<Json>(3ui8, Json(json));
      Assert::AreEqual<Json>(3ui8, json);
      json = Json(4i16);
      Assert::AreEqual<Json>(4i16, Json(json));
      Assert::AreEqual<Json>(4i16, json);
      json = Json(5ui16);
      Assert::AreEqual<Json>(5ui16, Json(json));
      Assert::AreEqual<Json>(5ui16, json);
      json = Json(6i32);
      Assert::AreEqual<Json>(6i32, Json(json));
      Assert::AreEqual<Json>(6i32, json);
      json = Json(7ui32);
      Assert::AreEqual<Json>(7ui32, Json(json));
      Assert::AreEqual<Json>(7ui32, json);
      json = Json(8i64);
      Assert::AreEqual<Json>(8i64, Json(json));
      Assert::AreEqual<Json>(8i64, json);
      json = Json(9ui64);
      Assert::AreEqual<Json>(9ui64, Json(json));
      Assert::AreEqual<Json>(9ui64, json);
      json = Json(13.37f);
      Assert::AreEqual<Json>(13.37f, Json(json));
      Assert::AreEqual<Json>(13.37f, json);
      json = Json(13.37);
      Assert::AreEqual<Json>(13.37, Json(json));
      Assert::AreEqual<Json>(13.37, json);
      json = Json({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } });
      Assert::AreEqual<Json>({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } }, Json(json));
      Assert::AreEqual<Json>({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } }, json);
      json = Json({ 1, 3, 3, 7 });
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, Json(json));
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);

      json = Json(nullptr);
      Assert::AreEqual<Json>(nullptr, Json(move(json)));
      Assert::AreEqual<Json>(nullptr, json);
      json = Json(L"Test");
      Assert::AreEqual<Json>(L"Test", Json(move(json)));
      Assert::AreEqual<Json>(L""s, json);
      str = L"Test"s;
      json = Json(str);
      Assert::AreEqual<Json>(L"Test"s, Json(move(json)));
      Assert::AreEqual<Json>(L""s, json);
      json = Json(L"Test"s);
      Assert::AreEqual<Json>(L"Test"s, Json(move(json)));
      Assert::AreEqual<Json>(L""s, json);
      json = Json(false);
      Assert::AreEqual<Json>(false, Json(move(json)));
      Assert::AreEqual<Json>(false, json);
      json = Json(true);
      Assert::AreEqual<Json>(true, Json(move(json)));
      Assert::AreEqual<Json>(true, json);
      json = Json((char)1);
      Assert::AreEqual<Json>((char)1, Json(move(json)));
      Assert::AreEqual<Json>((char)1, json);
      json = Json(2i8);
      Assert::AreEqual<Json>(2i8, Json(move(json)));
      Assert::AreEqual<Json>(2i8, json);
      json = Json(3ui8);
      Assert::AreEqual<Json>(3ui8, Json(move(json)));
      Assert::AreEqual<Json>(3ui8, json);
      json = Json(4i16);
      Assert::AreEqual<Json>(4i16, Json(move(json)));
      Assert::AreEqual<Json>(4i16, json);
      json = Json(5ui16);
      Assert::AreEqual<Json>(5ui16, Json(move(json)));
      Assert::AreEqual<Json>(5ui16, json);
      json = Json(6i32);
      Assert::AreEqual<Json>(6i32, Json(move(json)));
      Assert::AreEqual<Json>(6i32, json);
      json = Json(7ui32);
      Assert::AreEqual<Json>(7ui32, Json(move(json)));
      Assert::AreEqual<Json>(7ui32, json);
      json = Json(8i64);
      Assert::AreEqual<Json>(8i64, Json(move(json)));
      Assert::AreEqual<Json>(8i64, json);
      json = Json(9ui64);
      Assert::AreEqual<Json>(9ui64, Json(move(json)));
      Assert::AreEqual<Json>(9ui64, json);
      json = Json(13.37f);
      Assert::AreEqual<Json>(13.37f, Json(move(json)));
      Assert::AreEqual<Json>(13.37f, json);
      json = Json(13.37);
      Assert::AreEqual<Json>(13.37, Json(move(json)));
      Assert::AreEqual<Json>(13.37, json);
      json = Json({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } });
      Assert::AreEqual<Json>({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } }, Json(move(json)));
      Assert::AreEqual<Json>(JsonObject(), json);
      json = Json({ 1, 3, 3, 7 });
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, Json(move(json)));
      Assert::AreEqual<Json>(JsonArray(), json);
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      Json json;

      auto builder = JsonBuilder();
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);

      // JsonBuilder from VALUE const&
      auto value = VALUE(nullptr);
      builder = JsonBuilder(value);
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"TestString"s);
      builder = JsonBuilder(value);
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<VALUE>(L"TestString"s, value);
      value = VALUE(true);
      builder = JsonBuilder(value);
      json = builder;
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(value);
      json = builder;
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(value);
      json = builder;
      Assert::AreEqual<Json>(1337i64, json);
      Assert::AreEqual<JsonBuilder>(1337i64, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      builder = JsonBuilder(value);
      json = builder;
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), value);
      value = VALUE(JsonArray({ 1337, L"1337", true }));
      builder = JsonBuilder(value);
      json = builder;
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), value);

      // JsonBuilder from VALUE &&
      value = VALUE(nullptr);
      builder = JsonBuilder(move(value));
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"TestString"s);
      builder = JsonBuilder(move(value));
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<VALUE>(L""s, value);
      value = VALUE(true);
      builder = JsonBuilder(move(value));
      json = builder;
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(move(value));
      json = builder;
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(move(value));
      json = builder;
      Assert::AreEqual<Json>(1337i64, json);
      Assert::AreEqual<JsonBuilder>(1337i64, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      builder = JsonBuilder(move(value));
      json = builder;
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual<VALUE>(JsonObject(), value);
      value = VALUE(JsonArray({ 1337, L"1337", true }));
      builder = JsonBuilder(move(value));
      json = builder;
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual<VALUE>(JsonArray(), value);

      // JsonBuilder constructors
      builder = JsonBuilder(nullptr);
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(L"TestString");
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      auto str = L"TestString"s;
      builder = JsonBuilder(str);
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual(L"TestString"s, str);
      str = L"TestString"s;
      builder = JsonBuilder(move(str));
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual(L""s, str);
      builder = JsonBuilder(true);
      json = builder;
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder((char)1);
      json = builder;
      Assert::AreEqual<Json>(1i64, json);
      Assert::AreEqual<JsonBuilder>(1i64, builder);
      builder = JsonBuilder((int8_t)2);
      json = builder;
      Assert::AreEqual<Json>(2i64, json);
      Assert::AreEqual<JsonBuilder>(2i64, builder);
      builder = JsonBuilder((uint8_t)3);
      json = builder;
      Assert::AreEqual<Json>(3i64, json);
      Assert::AreEqual<JsonBuilder>(3i64, builder);
      builder = JsonBuilder((int16_t)4);
      json = builder;
      Assert::AreEqual<Json>(4i64, json);
      Assert::AreEqual<JsonBuilder>(4i64, builder);
      builder = JsonBuilder((uint16_t)5);
      json = builder;
      Assert::AreEqual<Json>(5i64, json);
      Assert::AreEqual<JsonBuilder>(5i64, builder);
      builder = JsonBuilder((int32_t)6);
      json = builder;
      Assert::AreEqual<Json>(6i64, json);
      Assert::AreEqual<JsonBuilder>(6i64, builder);
      builder = JsonBuilder((uint32_t)7);
      json = builder;
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual<JsonBuilder>(7i64, builder);
      builder = JsonBuilder((int64_t)8);
      json = builder;
      Assert::AreEqual<Json>(8i64, json);
      Assert::AreEqual<JsonBuilder>(8i64, builder);
      builder = JsonBuilder((uint64_t)9);
      json = builder;
      Assert::AreEqual<Json>(9i64, json);
      Assert::AreEqual<JsonBuilder>(9i64, builder);
      builder = JsonBuilder(13.37f);
      json = builder;
      Assert::AreEqual<Json>((double)13.37f, json);
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      builder = JsonBuilder(13.37);
      json = builder;
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      auto obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      builder = JsonBuilder(obj);
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      builder = JsonBuilder(move(obj));
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual(JsonObject(), obj);
      auto arr = JsonArray({ 1337, L"1337", true });
      builder = JsonBuilder(arr);
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual(JsonArray({ 1337, L"1337", true }), arr);
      arr = JsonArray({ 1337, L"1337", true });
      builder = JsonBuilder(move(arr));
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual(JsonArray(), arr);

      // JsonBuilder from Json
      auto json2 = Json();
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json();
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json(nullptr);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json(nullptr);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json(L"TestString");
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<Json>(L"TestString"s, json2);
      json2 = Json(L"TestString");
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<Json>(L""s, json2);
      str = L"TestString"s;
      json2 = Json(str);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<Json>(L"TestString"s, json2);
      Assert::AreEqual(L"TestString"s, str);
      str = L"TestString"s;
      json2 = Json(str);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<Json>(L""s, json2);
      Assert::AreEqual(L"TestString"s, str);
      str = L"TestString"s;
      json2 = Json(move(str));
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<Json>(L"TestString"s, json2);
      Assert::AreEqual(L""s, str);
      str = L"TestString"s;
      json2 = Json(move(str));
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L"TestString"s, builder);
      Assert::AreEqual<Json>(L""s, json2);
      Assert::AreEqual(L""s, str);
      json2 = Json(true);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<Json>(true, json2);
      json2 = Json(true);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<Json>(true, json2);
      json2 = Json((char)1);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(1i64, json);
      Assert::AreEqual<JsonBuilder>(1, builder);
      Assert::AreEqual<Json>(1, json2);
      json2 = Json((char)1);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(1i64, json);
      Assert::AreEqual<JsonBuilder>(1, builder);
      Assert::AreEqual<Json>(1, json2);
      json2 = Json((int8_t)2);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(2i64, json);
      Assert::AreEqual<JsonBuilder>(2, builder);
      Assert::AreEqual<Json>(2, json2);
      json2 = Json((int8_t)2);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(2i64, json);
      Assert::AreEqual<JsonBuilder>(2, builder);
      Assert::AreEqual<Json>(2, json2);
      json2 = Json((uint8_t)3);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(3i64, json);
      Assert::AreEqual<JsonBuilder>(3, builder);
      Assert::AreEqual<Json>(3, json2);
      json2 = Json((uint8_t)3);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(3i64, json);
      Assert::AreEqual<JsonBuilder>(3, builder);
      Assert::AreEqual<Json>(3, json2);
      json2 = Json((int16_t)4);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(4i64, json);
      Assert::AreEqual<JsonBuilder>(4, builder);
      Assert::AreEqual<Json>(4, json2);
      json2 = Json((int16_t)4);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(4i64, json);
      Assert::AreEqual<JsonBuilder>(4, builder);
      Assert::AreEqual<Json>(4, json2);
      json2 = Json((uint16_t)5);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(5i64, json);
      Assert::AreEqual<JsonBuilder>(5, builder);
      Assert::AreEqual<Json>(5, json2);
      json2 = Json((uint16_t)5);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(5i64, json);
      Assert::AreEqual<JsonBuilder>(5, builder);
      Assert::AreEqual<Json>(5, json2);
      json2 = Json((int32_t)6);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(6i64, json);
      Assert::AreEqual<JsonBuilder>(6, builder);
      Assert::AreEqual<Json>(6, json2);
      json2 = Json((int32_t)6);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(6i64, json);
      Assert::AreEqual<JsonBuilder>(6, builder);
      Assert::AreEqual<Json>(6, json2);
      json2 = Json((uint32_t)7);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual<JsonBuilder>(7, builder);
      Assert::AreEqual<Json>(7, json2);
      json2 = Json((uint32_t)7);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual<JsonBuilder>(7, builder);
      Assert::AreEqual<Json>(7, json2);
      json2 = Json((int64_t)8);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(8i64, json);
      Assert::AreEqual<JsonBuilder>(8, builder);
      Assert::AreEqual<Json>(8, json2);
      json2 = Json((int64_t)8);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(8i64, json);
      Assert::AreEqual<JsonBuilder>(8, builder);
      Assert::AreEqual<Json>(8, json2);
      json2 = Json((uint64_t)9);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(9i64, json);
      Assert::AreEqual<JsonBuilder>(9, builder);
      Assert::AreEqual<Json>(9, json2);
      json2 = Json((uint64_t)9);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(9i64, json);
      Assert::AreEqual<JsonBuilder>(9, builder);
      Assert::AreEqual<Json>(9, json2);
      json2 = Json(13.37f);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(13.37f, json);
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      Assert::AreEqual<Json>(13.37f, json2);
      json2 = Json(13.37f);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(13.37f, json);
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      Assert::AreEqual<Json>(13.37f, json2);
      json2 = Json(13.37);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<Json>(13.37, json2);
      json2 = Json(13.37);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<Json>(13.37, json2);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(obj);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json2);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(move(obj));
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json2);
      Assert::AreEqual<Json>(JsonObject(), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(obj);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual<Json>(JsonObject(), json2);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(move(obj));
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), builder);
      Assert::AreEqual<Json>(JsonObject(), json2);
      Assert::AreEqual<Json>(JsonObject(), obj);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(arr);
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json2);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), arr);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(move(arr));
      builder = JsonBuilder(json2);
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json2);
      Assert::AreEqual<Json>(JsonArray(), arr);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(arr);
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual<Json>(JsonArray(), json2);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), arr);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(move(arr));
      builder = JsonBuilder(move(json2));
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray({ 1337, L"1337", true }), builder);
      Assert::AreEqual<Json>(JsonArray(), json2);
      Assert::AreEqual<Json>(JsonArray(), arr);

      // JsonBuilder from initializer_list
      builder = JsonBuilder({ L"Key1", L"Value1" });
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1", json[0]);
      Assert::AreEqual<Json>(L"Value1", json[1]);
      Assert::AreEqual<JsonBuilder>({ L"Key1", L"Value1" }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" } });
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1", L"Value2" } });
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Value2", json[0][2]);
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1", L"Value2" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } });
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual(3i64, json[1].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Key2", json[1][0]);
      Assert::AreEqual<Json>(L"Value2", json[1][1]);
      Assert::AreEqual<Json>(L"Value3", json[1][2]);
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } }, builder);
      builder = JsonBuilder({ 1 });   // Deduced to JsonBuilder instead of initializer_list<JsonBuilder>
      json = builder;
      Assert::AreEqual<Json>(1, json);
      Assert::AreEqual<JsonBuilder>(1, builder);
      builder = JsonBuilder({ { 1 } });
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(1i64, json[0].Size());
      Assert::AreEqual<Json>(1, json[0][0]);
      Assert::AreEqual<JsonBuilder>({ { 1 } }, builder);
      builder = JsonBuilder({ 1, L"2"s });
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<JsonBuilder>({ 1, L"2"s }, builder);
      builder = JsonBuilder({ { 1, L"2"s } });
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual<Json>(1, json[0][0]);
      Assert::AreEqual<Json>(L"2"s, json[0][1]);
      Assert::AreEqual<JsonBuilder>({ { 1, L"2"s } }, builder);
      builder = JsonBuilder({ 1, L"2"s, true });
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>({ 1, L"2"s, true }, builder);
      builder = JsonBuilder({ { 1, L"2"s, true } });
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(1, json[0][0]);
      Assert::AreEqual<Json>(L"2"s, json[0][1]);
      Assert::AreEqual<Json>(true, json[0][2]);
      Assert::AreEqual<JsonBuilder>({ { 1, L"2"s, true } }, builder);

      // JsonBuilder from vector
      auto vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1", json[0]);
      Assert::AreEqual<Json>(L"Value1", json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"Key1", L"Value1" }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" } }, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value2" } };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Value2", json[0][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value2" }}, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual(3i64, json[1].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Key2", json[1][0]);
      Assert::AreEqual<Json>(L"Value2", json[1][1]);
      Assert::AreEqual<Json>(L"Value3", json[1][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1", json[0]);
      Assert::AreEqual<Json>(L"Value1", json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"Key1", L"Value1" }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value2" } };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Value2", json[0][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value2" }}, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual(3i64, json[1].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Key2", json[1][0]);
      Assert::AreEqual<Json>(L"Value2", json[1][1]);
      Assert::AreEqual<Json>(L"Value3", json[1][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ 1 };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1 }, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L"2"s }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s, true };
      builder = JsonBuilder(vec);
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L"2"s, true }, builder);
      Assert::AreEqual(3ui64, vec.size());
      vec = vector<JsonBuilder>{ 1 };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1 }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L"2"s }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s, true };
      builder = JsonBuilder(move(vec));
      json = builder;
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L"2"s, true }, builder);
      Assert::AreEqual(0ui64, vec.size());

      // Move
      builder = JsonBuilder();
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);

      // JsonBuilder from VALUE const&
      value = VALUE(nullptr);
      builder = JsonBuilder(value);
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"TestString"s);
      builder = JsonBuilder(value);
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<VALUE>(L"TestString"s, value);
      value = VALUE(true);
      builder = JsonBuilder(value);
      json = move(builder);
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(value);
      json = move(builder);
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(value);
      json = move(builder);
      Assert::AreEqual<Json>(1337i64, json);
      Assert::AreEqual<JsonBuilder>(1337i64, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      builder = JsonBuilder(value);
      json = move(builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), value);
      value = VALUE(JsonArray({ 1337, L"1337", true }));
      builder = JsonBuilder(value);
      json = move(builder);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), value);

      // JsonBuilder from VALUE &&
      value = VALUE(nullptr);
      builder = JsonBuilder(move(value));
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"TestString"s);
      builder = JsonBuilder(move(value));
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<VALUE>(L""s, value);
      value = VALUE(true);
      builder = JsonBuilder(move(value));
      json = move(builder);
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(move(value));
      json = move(builder);
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(move(value));
      json = move(builder);
      Assert::AreEqual<Json>(1337i64, json);
      Assert::AreEqual<JsonBuilder>(1337i64, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      builder = JsonBuilder(move(value));
      json = move(builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<VALUE>(JsonObject(), value);
      value = VALUE(JsonArray({ 1337, L"1337", true }));
      builder = JsonBuilder(move(value));
      json = move(builder);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual<VALUE>(JsonArray(), value);

      // JsonBuilder constructors
      builder = JsonBuilder(nullptr);
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(L"TestString");
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      str = L"TestString"s;
      builder = JsonBuilder(str);
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual(L"TestString"s, str);
      str = L"TestString"s;
      builder = JsonBuilder(move(str));
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual(L""s, str);
      builder = JsonBuilder(true);
      json = move(builder);
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder((char)1);
      json = move(builder);
      Assert::AreEqual<Json>(1i64, json);
      Assert::AreEqual<JsonBuilder>(1i64, builder);
      builder = JsonBuilder((int8_t)2);
      json = move(builder);
      Assert::AreEqual<Json>(2i64, json);
      Assert::AreEqual<JsonBuilder>(2i64, builder);
      builder = JsonBuilder((uint8_t)3);
      json = move(builder);
      Assert::AreEqual<Json>(3i64, json);
      Assert::AreEqual<JsonBuilder>(3i64, builder);
      builder = JsonBuilder((int16_t)4);
      json = move(builder);
      Assert::AreEqual<Json>(4i64, json);
      Assert::AreEqual<JsonBuilder>(4i64, builder);
      builder = JsonBuilder((uint16_t)5);
      json = move(builder);
      Assert::AreEqual<Json>(5i64, json);
      Assert::AreEqual<JsonBuilder>(5i64, builder);
      builder = JsonBuilder((int32_t)6);
      json = move(builder);
      Assert::AreEqual<Json>(6i64, json);
      Assert::AreEqual<JsonBuilder>(6i64, builder);
      builder = JsonBuilder((uint32_t)7);
      json = move(builder);
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual<JsonBuilder>(7i64, builder);
      builder = JsonBuilder((int64_t)8);
      json = move(builder);
      Assert::AreEqual<Json>(8i64, json);
      Assert::AreEqual<JsonBuilder>(8i64, builder);
      builder = JsonBuilder((uint64_t)9);
      json = move(builder);
      Assert::AreEqual<Json>(9i64, json);
      Assert::AreEqual<JsonBuilder>(9i64, builder);
      builder = JsonBuilder(13.37f);
      json = move(builder);
      Assert::AreEqual<Json>((double)13.37f, json);
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      builder = JsonBuilder(13.37);
      json = move(builder);
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      builder = JsonBuilder(obj);
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      builder = JsonBuilder(move(obj));
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual(JsonObject(), obj);
      arr = JsonArray({ 1337, L"1337", true });
      builder = JsonBuilder(arr);
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual(JsonArray({ 1337, L"1337", true }), arr);
      arr = JsonArray({ 1337, L"1337", true });
      builder = JsonBuilder(move(arr));
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual(JsonArray(), arr);

      // JsonBuilder from Json
      json2 = Json();
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json();
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json(nullptr);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json(nullptr);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<Json>(nullptr, json2);
      json2 = Json(L"TestString");
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<Json>(L"TestString"s, json2);
      json2 = Json(L"TestString");
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<Json>(L""s, json2);
      str = L"TestString"s;
      json2 = Json(str);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<Json>(L"TestString"s, json2);
      Assert::AreEqual(L"TestString"s, str);
      str = L"TestString"s;
      json2 = Json(str);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<Json>(L""s, json2);
      Assert::AreEqual(L"TestString"s, str);
      str = L"TestString"s;
      json2 = Json(move(str));
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<Json>(L"TestString"s, json2);
      Assert::AreEqual(L""s, str);
      str = L"TestString"s;
      json2 = Json(move(str));
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual<JsonBuilder>(L""s, builder);
      Assert::AreEqual<Json>(L""s, json2);
      Assert::AreEqual(L""s, str);
      json2 = Json(true);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<Json>(true, json2);
      json2 = Json(true);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<Json>(true, json2);
      json2 = Json((char)1);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(1i64, json);
      Assert::AreEqual<JsonBuilder>(1, builder);
      Assert::AreEqual<Json>(1, json2);
      json2 = Json((char)1);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(1i64, json);
      Assert::AreEqual<JsonBuilder>(1, builder);
      Assert::AreEqual<Json>(1, json2);
      json2 = Json((int8_t)2);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(2i64, json);
      Assert::AreEqual<JsonBuilder>(2, builder);
      Assert::AreEqual<Json>(2, json2);
      json2 = Json((int8_t)2);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(2i64, json);
      Assert::AreEqual<JsonBuilder>(2, builder);
      Assert::AreEqual<Json>(2, json2);
      json2 = Json((uint8_t)3);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(3i64, json);
      Assert::AreEqual<JsonBuilder>(3, builder);
      Assert::AreEqual<Json>(3, json2);
      json2 = Json((uint8_t)3);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(3i64, json);
      Assert::AreEqual<JsonBuilder>(3, builder);
      Assert::AreEqual<Json>(3, json2);
      json2 = Json((int16_t)4);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(4i64, json);
      Assert::AreEqual<JsonBuilder>(4, builder);
      Assert::AreEqual<Json>(4, json2);
      json2 = Json((int16_t)4);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(4i64, json);
      Assert::AreEqual<JsonBuilder>(4, builder);
      Assert::AreEqual<Json>(4, json2);
      json2 = Json((uint16_t)5);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(5i64, json);
      Assert::AreEqual<JsonBuilder>(5, builder);
      Assert::AreEqual<Json>(5, json2);
      json2 = Json((uint16_t)5);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(5i64, json);
      Assert::AreEqual<JsonBuilder>(5, builder);
      Assert::AreEqual<Json>(5, json2);
      json2 = Json((int32_t)6);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(6i64, json);
      Assert::AreEqual<JsonBuilder>(6, builder);
      Assert::AreEqual<Json>(6, json2);
      json2 = Json((int32_t)6);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(6i64, json);
      Assert::AreEqual<JsonBuilder>(6, builder);
      Assert::AreEqual<Json>(6, json2);
      json2 = Json((uint32_t)7);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual<JsonBuilder>(7, builder);
      Assert::AreEqual<Json>(7, json2);
      json2 = Json((uint32_t)7);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual<JsonBuilder>(7, builder);
      Assert::AreEqual<Json>(7, json2);
      json2 = Json((int64_t)8);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(8i64, json);
      Assert::AreEqual<JsonBuilder>(8, builder);
      Assert::AreEqual<Json>(8, json2);
      json2 = Json((int64_t)8);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(8i64, json);
      Assert::AreEqual<JsonBuilder>(8, builder);
      Assert::AreEqual<Json>(8, json2);
      json2 = Json((uint64_t)9);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(9i64, json);
      Assert::AreEqual<JsonBuilder>(9, builder);
      Assert::AreEqual<Json>(9, json2);
      json2 = Json((uint64_t)9);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(9i64, json);
      Assert::AreEqual<JsonBuilder>(9, builder);
      Assert::AreEqual<Json>(9, json2);
      json2 = Json(13.37f);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(13.37f, json);
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      Assert::AreEqual<Json>(13.37f, json2);
      json2 = Json(13.37f);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(13.37f, json);
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      Assert::AreEqual<Json>(13.37f, json2);
      json2 = Json(13.37);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<Json>(13.37, json2);
      json2 = Json(13.37);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<Json>(13.37, json2);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(obj);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json2);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(move(obj));
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json2);
      Assert::AreEqual<Json>(JsonObject(), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(obj);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<Json>(JsonObject(), json2);
      Assert::AreEqual<Json>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), obj);
      obj = JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } });
      json2 = Json(move(obj));
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<Json>(JsonObject(), json2);
      Assert::AreEqual<Json>(JsonObject(), obj);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(arr);
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json2);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), arr);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(move(arr));
      builder = JsonBuilder(json2);
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), json2);
      Assert::AreEqual<Json>(JsonArray(), arr);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(arr);
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual<Json>(JsonArray(), json2);
      Assert::AreEqual<Json>(JsonArray({ 1337, L"1337", true }), arr);
      arr = JsonArray({ 1337, L"1337", true });
      json2 = Json(move(arr));
      builder = JsonBuilder(move(json2));
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(JsonArray(), builder);
      Assert::AreEqual<Json>(JsonArray(), json2);
      Assert::AreEqual<Json>(JsonArray(), arr);

      // JsonBuilder from initializer_list
      builder = JsonBuilder({ L"Key1", L"Value1" });
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1", json[0]);
      Assert::AreEqual<Json>(L"Value1", json[1]);
      Assert::AreEqual<JsonBuilder>({ L"", L"" }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" } });
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<JsonBuilder>({ { L"", L"" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1", L"Value2" } });
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Value2", json[0][2]);
      Assert::AreEqual<JsonBuilder>({ { L"", L"", L"" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>({ { L"", L"" }, { L"", L"" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } });
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual(3i64, json[1].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Key2", json[1][0]);
      Assert::AreEqual<Json>(L"Value2", json[1][1]);
      Assert::AreEqual<Json>(L"Value3", json[1][2]);
      Assert::AreEqual<JsonBuilder>({ { L"", L"" }, { L"", L"", L"" } }, builder);
      builder = JsonBuilder({ 1 });   // Deduced to JsonBuilder instead of initializer_list<JsonBuilder>
      json = move(builder);
      Assert::AreEqual<Json>(1, json);
      Assert::AreEqual<JsonBuilder>(1, builder);
      builder = JsonBuilder({ { 1 } });
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(1i64, json[0].Size());
      Assert::AreEqual<Json>(1, json[0][0]);
      Assert::AreEqual<JsonBuilder>({ { 1 } }, builder);
      builder = JsonBuilder({ 1, L"2"s });
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<JsonBuilder>({ 1, L""s }, builder);
      builder = JsonBuilder({ { 1, L"2"s } });
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual<Json>(1, json[0][0]);
      Assert::AreEqual<Json>(L"2"s, json[0][1]);
      Assert::AreEqual<JsonBuilder>({ { 1, L""s } }, builder);
      builder = JsonBuilder({ 1, L"2"s, true });
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>({ 1, L""s, true }, builder);
      builder = JsonBuilder({ { 1, L"2"s, true } });
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(1, json[0][0]);
      Assert::AreEqual<Json>(L"2"s, json[0][1]);
      Assert::AreEqual<Json>(true, json[0][2]);
      Assert::AreEqual<JsonBuilder>({ { 1, L""s, true } }, builder);

      // JsonBuilder from vector
      vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1", json[0]);
      Assert::AreEqual<Json>(L"Value1", json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"", L"" }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" } }, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value2" } };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Value2", json[0][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"", L"" }}, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, { L"", L"" } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual(3i64, json[1].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Key2", json[1][0]);
      Assert::AreEqual<Json>(L"Value2", json[1][1]);
      Assert::AreEqual<Json>(L"Value3", json[1][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, { L"", L"", L"" } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1", json[0]);
      Assert::AreEqual<Json>(L"Value1", json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"", L"" }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value2" } };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual(3i64, json[0].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Value2", json[0][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"", L"" }}, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual<Json>(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", json[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", json[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, { L"", L"" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(2i64, json[0].Size());
      Assert::AreEqual(3i64, json[1].Size());
      Assert::AreEqual<Json>(L"Key1", json[0][0]);
      Assert::AreEqual<Json>(L"Value1", json[0][1]);
      Assert::AreEqual<Json>(L"Key2", json[1][0]);
      Assert::AreEqual<Json>(L"Value2", json[1][1]);
      Assert::AreEqual<Json>(L"Value3", json[1][2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, { L"", L"", L"" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ 1 };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1 }, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L""s }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s, true };
      builder = JsonBuilder(vec);
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L""s, true }, builder);
      Assert::AreEqual(3ui64, vec.size());
      vec = vector<JsonBuilder>{ 1 };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1 }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L""s }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ 1, L"2"s, true };
      builder = JsonBuilder(move(vec));
      json = move(builder);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(L"2"s, json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ 1, L""s, true }, builder);
      Assert::AreEqual(0ui64, vec.size());
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      Json json = {
        nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
        { { L"Key1", nullptr }, { L"Key2", L"Test1" }, { L"Key3", L"Test2"s }, { L"Key4", false }, { L"Key5", true }, { L"Key6", (char)1 }, { L"Key7", 2i8 }, { L"Key8", 3ui8 },
          { L"Key9", 4i16 }, { L"Key10", 5ui16 }, { L"Key11", 6i32 }, { L"Key12", 7ui32 }, { L"Key13", 8i64 }, { L"Key14", 9ui64 }, { L"Key15", 13.37f }, { L"Key16", 313.37 },
          { L"Key17", { { L"InnerKey1", L"Value1" }, { L"InnerKey2", L"Value2" } } },
          { L"Key18", { 1, 3, 3, 7 } } },
        { nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
          { { L"InnerKey1", L"Value1" }, { L"InnerKey2", L"Value2" } },
          { 1, 3, 3, 7 } }
      };

      Assert::AreEqual<Json>(18, json.Size());
      Assert::AreEqual<Json>(18, json[16].Size());
      Assert::AreEqual<Json>(18, json[17].Size());

      auto values = vector<Json>{ nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37 };
      for (int i = 0; i < values.size(); ++i)
      {
        Assert::AreEqual<Json>(values[i], json[i]);
        Assert::AreEqual<Json>(values[i], json[16][L"Key"s + to_wstring(i + 1)]);
        Assert::AreEqual<Json>(values[i], json[17][i]);
      }

      Assert::AreEqual<Json>(2, json[16][L"Key17"].Size());
      Assert::AreEqual<Json>(L"Value1"s, json[16][L"Key17"][L"InnerKey1"]);
      Assert::AreEqual<Json>(L"Value2"s, json[16][L"Key17"][L"InnerKey2"]);
      Assert::AreEqual<Json>(4, json[16][L"Key18"].Size());
      Assert::AreEqual<Json>(1, json[16][L"Key18"][0]);
      Assert::AreEqual<Json>(3, json[16][L"Key18"][1]);
      Assert::AreEqual<Json>(3, json[16][L"Key18"][2]);
      Assert::AreEqual<Json>(7, json[16][L"Key18"][3]);

      Assert::AreEqual<Json>(2, json[17][16].Size());
      Assert::AreEqual<Json>(L"Value1"s, json[17][16][L"InnerKey1"]);
      Assert::AreEqual<Json>(L"Value2"s, json[17][16][L"InnerKey2"]);
      Assert::AreEqual<Json>(4, json[17][17].Size());
      Assert::AreEqual<Json>(1, json[17][17][0]);
      Assert::AreEqual<Json>(3, json[17][17][1]);
      Assert::AreEqual<Json>(3, json[17][17][2]);
      Assert::AreEqual<Json>(7, json[17][17][3]);
    }

    TEST_METHOD(TestConstructorNullptr)
    {
      auto value = nullptr;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::IsTrue(json.Type() == JsonType::Null);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual(nullptr, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::IsTrue(json.Type() == JsonType::Null);
      Assert::AreEqual<Json>(nullptr, json);
      Assert::AreEqual(nullptr, value);
    }

    TEST_METHOD(TestConstructorConstWcharPointer)
    {
      auto value = L"TestString";
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::IsTrue(json.Type() == JsonType::String);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual(L"TestString", value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::IsTrue(json.Type() == JsonType::String);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual(L"TestString", value);
    }

    TEST_METHOD(TestConstructorWstring)
    {
      auto value = L"TestString"s;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::IsTrue(json.Type() == JsonType::String);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual(L"TestString"s, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::IsTrue(json.Type() == JsonType::String);
      Assert::AreEqual<Json>(L"TestString"s, json);
      Assert::AreEqual(L""s, value);
    }

    TEST_METHOD(TestConstructorBool)
    {
      auto value = true;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Boolean));
      Assert::IsTrue(json.Type() == JsonType::Boolean);
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual(true, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Boolean));
      Assert::IsTrue(json.Type() == JsonType::Boolean);
      Assert::AreEqual<Json>(true, json);
      Assert::AreEqual(true, value);
    }

    TEST_METHOD(TestConstructorChar)
    {
      auto value = L'C';
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(L'C', json);
      Assert::AreEqual(L'C', value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(L'C', json);
      Assert::AreEqual(L'C', value);
    }

    TEST_METHOD(TestConstructorInt8_t)
    {
      auto value = 1i8;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(1i8, json);
      Assert::AreEqual(1i8, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(1i8, json);
      Assert::AreEqual(1i8, value);
    }

    TEST_METHOD(TestConstructorUInt8_t)
    {
      auto value = 2ui8;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(2ui8, json);
      Assert::AreEqual(2ui8, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(2ui8, json);
      Assert::AreEqual(2ui8, value);
    }

    TEST_METHOD(TestConstructorInt16_t)
    {
      auto value = 3i16;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(3i16, json);
      Assert::AreEqual(3i16, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(3i16, json);
      Assert::AreEqual(3i16, value);
    }

    TEST_METHOD(TestConstructorUInt16_t)
    {
      auto value = 4ui16;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(4ui16, json);
      Assert::AreEqual(4ui16, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(4ui16, json);
      Assert::AreEqual(4ui16, value);
    }

    TEST_METHOD(TestConstructorInt32_t)
    {
      auto value = 5i32;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(5i32, json);
      Assert::AreEqual(5i32, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(5i32, json);
      Assert::AreEqual(5i32, value);
    }

    TEST_METHOD(TestConstructorUInt32_t)
    {
      auto value = 6ui32;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(6ui32, json);
      Assert::AreEqual(6ui32, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(6ui32, json);
      Assert::AreEqual(6ui32, value);
    }

    TEST_METHOD(TestConstructorInt64_t)
    {
      auto value = 7i64;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual(7i64, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(7i64, json);
      Assert::AreEqual(7i64, value);
    }

    TEST_METHOD(TestConstructorUInt64_t)
    {
      auto value = 8ui64;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(8ui64, json);
      Assert::AreEqual(8ui64, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::IsTrue(json.Type() == JsonType::Integer);
      Assert::AreEqual<Json>(8ui64, json);
      Assert::AreEqual(8ui64, value);
    }

    TEST_METHOD(TestConstructorFloat)
    {
      auto value = 13.37f;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::IsTrue(json.Type() == JsonType::Real);
      Assert::AreEqual<Json>(13.37f, json);
      Assert::AreEqual(13.37f, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::IsTrue(json.Type() == JsonType::Real);
      Assert::AreEqual<Json>(13.37f, json);
      Assert::AreEqual(13.37f, value);
    }

    TEST_METHOD(TestConstructorDouble)
    {
      auto value = 13.37;
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::IsTrue(json.Type() == JsonType::Real);
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual(13.37, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::IsTrue(json.Type() == JsonType::Real);
      Assert::AreEqual<Json>(13.37, json);
      Assert::AreEqual(13.37, value);
    }

    TEST_METHOD(TestConstructorJsonObject)
    {
      auto value = JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::IsTrue(json.Type() == JsonType::Object);
      Assert::AreEqual<Json>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, json);
      Assert::AreEqual(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::IsTrue(json.Type() == JsonType::Object);
      Assert::AreEqual<Json>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, json);
      Assert::AreEqual(JsonObject(), value);
    }

    TEST_METHOD(TestConstructorJsonArray)
    {
      auto value = JsonArray{ 1, 3, 3, 7 };
      auto json = Json(value);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::IsTrue(json.Type() == JsonType::Array);
      Assert::AreEqual<Json>(JsonArray{ 1, 3, 3, 7 }, json);
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, value);
      json = Json(move(value));
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::IsTrue(json.Type() == JsonType::Array);
      Assert::AreEqual<Json>(JsonArray{ 1, 3, 3, 7 }, json);
      Assert::AreEqual(JsonArray(), value);
    }

    TEST_METHOD(TestType)
    {
      auto tuples = vector<tuple<Json, JsonType, JsonType>>
      {
        { nullptr_t (), JsonType::Null   , JsonType::Simple  },
        { wstring   (), JsonType::String , JsonType::Simple  },
        { bool      (), JsonType::Boolean, JsonType::Simple  },
        { double    (), JsonType::Real   , JsonType::Number  },
        { int64_t   (), JsonType::Integer, JsonType::Number  },
        { double    (), JsonType::Real   , JsonType::Simple  },
        { int64_t   (), JsonType::Integer, JsonType::Simple  },
        { JsonObject(), JsonType::Object , JsonType::Complex },
        { JsonArray (), JsonType::Array  , JsonType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::AreEqual(expected1, input.Type());
        Assert::AreEqual(expected2, input.Type());
      }
    }

    TEST_METHOD(TestIs)
    {
      auto tuples = vector<tuple<Json, JsonType, JsonType>>
      {
        { nullptr_t() , JsonType::Null   , JsonType::Simple  },
        { wstring()   , JsonType::String , JsonType::Simple  },
        { bool()      , JsonType::Boolean, JsonType::Simple  },
        { double    (), JsonType::Real   , JsonType::Number  },
        { int64_t   (), JsonType::Integer, JsonType::Number  },
        { double    (), JsonType::Real   , JsonType::Simple  },
        { int64_t   (), JsonType::Integer, JsonType::Simple  },
        { JsonObject(), JsonType::Object , JsonType::Complex },
        { JsonArray() , JsonType::Array  , JsonType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::IsTrue(input.Is(expected1));
        Assert::IsTrue(input.Is(expected2));
      }
    }

    TEST_METHOD(TestDump)
    {
      auto pairs = vector<pair<Json, wstring>>
      {
        { nullptr  , L"null"s      },
        { L"Test1" , L"\"Test1\""s },
        { L"Test2"s, L"\"Test2\""s },
        { false    , L"false"s     },
        { true     , L"true"s      },
        { (char)1  , L"1"s         },
        { 2i8      , L"2"s         },
        { 3ui8     , L"3"s         },
        { 4i16     , L"4"s         },
        { 5ui16    , L"5"s         },
        { 6i32     , L"6"s         },
        { 7ui32    , L"7"s         },
        { 8i64     , L"8"s         },
        { 9ui64    , L"9"s         },
        { 13.37f   , L"13.37"s     },
        { 313.37   , L"313.37"s    },
        { 337184288, L"337184288"s },
        { 1.3f     , L"1.3"        },
        { 1.3      , L"1.3"        }
      };
      for (auto& [input, expected] : pairs)
      {
        for (int i = 0; i < 4; ++i)
        {
          for (auto c : { L' ', L'\t' })
          {
            Assert::AreEqual(expected, input.Dump(i, c));
          }
        }
      }

      Json object = {
        { L"Key1", nullptr },
        { L"Key2", L"Test1" },
        { L"Key3", L"Test2"s },
        { L"Key4", false },
        { L"Key5", true },
        { L"Key6", (char)1 },
        { L"Key7", 2i8 },
        { L"Key8", 3ui8 },
        { L"Key9", 4i16 },
        { L"Key10", 5ui16 },
        { L"Key11", 6i32 },
        { L"Key12", 7ui32 },
        { L"Key13", 8i64 },
        { L"Key14", 9ui64 },
        { L"Key15", 13.37f },
        { L"Key16", 313.37 },
        { L"Key17", {
          { L"InnerKey1", L"Value1" },
          { L"InnerKey2", L"Value2" } }
        },
        { L"Key18", { 1, 3, 3, 7 } }
      };

      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump());
      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump(0));
      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump(0, L' '));
      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump(0, L'\t'));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         " \"Key1\": null,"                   "\r\n"
         " \"Key2\": \"Test1\","              "\r\n"
         " \"Key3\": \"Test2\","              "\r\n"
         " \"Key4\": false,"                  "\r\n"
         " \"Key5\": true,"                   "\r\n"
         " \"Key6\": 1,"                      "\r\n"
         " \"Key7\": 2,"                      "\r\n"
         " \"Key8\": 3,"                      "\r\n"
         " \"Key9\": 4,"                      "\r\n"
         " \"Key10\": 5,"                     "\r\n"
         " \"Key11\": 6,"                     "\r\n"
         " \"Key12\": 7,"                     "\r\n"
         " \"Key13\": 8,"                     "\r\n"
         " \"Key14\": 9,"                     "\r\n"
         " \"Key15\": 13.37,"                 "\r\n"
         " \"Key16\": 313.37,"                "\r\n"
         " \"Key17\": {"                      "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " \"Key18\": ["                      "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "}"s, object.Dump(1));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         " \"Key1\": null,"                   "\r\n"
         " \"Key2\": \"Test1\","              "\r\n"
         " \"Key3\": \"Test2\","              "\r\n"
         " \"Key4\": false,"                  "\r\n"
         " \"Key5\": true,"                   "\r\n"
         " \"Key6\": 1,"                      "\r\n"
         " \"Key7\": 2,"                      "\r\n"
         " \"Key8\": 3,"                      "\r\n"
         " \"Key9\": 4,"                      "\r\n"
         " \"Key10\": 5,"                     "\r\n"
         " \"Key11\": 6,"                     "\r\n"
         " \"Key12\": 7,"                     "\r\n"
         " \"Key13\": 8,"                     "\r\n"
         " \"Key14\": 9,"                     "\r\n"
         " \"Key15\": 13.37,"                 "\r\n"
         " \"Key16\": 313.37,"                "\r\n"
         " \"Key17\": {"                      "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " \"Key18\": ["                      "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "}"s, object.Dump(1, L' '));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "\t\"Key1\": null,"                  "\r\n"
         "\t\"Key2\": \"Test1\","             "\r\n"
         "\t\"Key3\": \"Test2\","             "\r\n"
         "\t\"Key4\": false,"                 "\r\n"
         "\t\"Key5\": true,"                  "\r\n"
         "\t\"Key6\": 1,"                     "\r\n"
         "\t\"Key7\": 2,"                     "\r\n"
         "\t\"Key8\": 3,"                     "\r\n"
         "\t\"Key9\": 4,"                     "\r\n"
         "\t\"Key10\": 5,"                    "\r\n"
         "\t\"Key11\": 6,"                    "\r\n"
         "\t\"Key12\": 7,"                    "\r\n"
         "\t\"Key13\": 8,"                    "\r\n"
         "\t\"Key14\": 9,"                    "\r\n"
         "\t\"Key15\": 13.37,"                "\r\n"
         "\t\"Key16\": 313.37,"               "\r\n"
         "\t\"Key17\": {"                     "\r\n"
         "\t\t\"InnerKey1\": \"Value1\","     "\r\n"
         "\t\t\"InnerKey2\": \"Value2\""      "\r\n"
         "\t},"                               "\r\n"
         "\t\"Key18\": ["                     "\r\n"
         "\t\t1,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t7"                              "\r\n"
         "\t]"                                "\r\n"
         "}"s, object.Dump(1, L'\t'));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "  \"Key1\": null,"                  "\r\n"
         "  \"Key2\": \"Test1\","             "\r\n"
         "  \"Key3\": \"Test2\","             "\r\n"
         "  \"Key4\": false,"                 "\r\n"
         "  \"Key5\": true,"                  "\r\n"
         "  \"Key6\": 1,"                     "\r\n"
         "  \"Key7\": 2,"                     "\r\n"
         "  \"Key8\": 3,"                     "\r\n"
         "  \"Key9\": 4,"                     "\r\n"
         "  \"Key10\": 5,"                    "\r\n"
         "  \"Key11\": 6,"                    "\r\n"
         "  \"Key12\": 7,"                    "\r\n"
         "  \"Key13\": 8,"                    "\r\n"
         "  \"Key14\": 9,"                    "\r\n"
         "  \"Key15\": 13.37,"                "\r\n"
         "  \"Key16\": 313.37,"               "\r\n"
         "  \"Key17\": {"                     "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  \"Key18\": ["                     "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "}"s, object.Dump(2));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "  \"Key1\": null,"                  "\r\n"
         "  \"Key2\": \"Test1\","             "\r\n"
         "  \"Key3\": \"Test2\","             "\r\n"
         "  \"Key4\": false,"                 "\r\n"
         "  \"Key5\": true,"                  "\r\n"
         "  \"Key6\": 1,"                     "\r\n"
         "  \"Key7\": 2,"                     "\r\n"
         "  \"Key8\": 3,"                     "\r\n"
         "  \"Key9\": 4,"                     "\r\n"
         "  \"Key10\": 5,"                    "\r\n"
         "  \"Key11\": 6,"                    "\r\n"
         "  \"Key12\": 7,"                    "\r\n"
         "  \"Key13\": 8,"                    "\r\n"
         "  \"Key14\": 9,"                    "\r\n"
         "  \"Key15\": 13.37,"                "\r\n"
         "  \"Key16\": 313.37,"               "\r\n"
         "  \"Key17\": {"                     "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  \"Key18\": ["                     "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "}"s, object.Dump(2, L' '));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "\t\t\"Key1\": null,"                "\r\n"
         "\t\t\"Key2\": \"Test1\","           "\r\n"
         "\t\t\"Key3\": \"Test2\","           "\r\n"
         "\t\t\"Key4\": false,"               "\r\n"
         "\t\t\"Key5\": true,"                "\r\n"
         "\t\t\"Key6\": 1,"                   "\r\n"
         "\t\t\"Key7\": 2,"                   "\r\n"
         "\t\t\"Key8\": 3,"                   "\r\n"
         "\t\t\"Key9\": 4,"                   "\r\n"
         "\t\t\"Key10\": 5,"                  "\r\n"
         "\t\t\"Key11\": 6,"                  "\r\n"
         "\t\t\"Key12\": 7,"                  "\r\n"
         "\t\t\"Key13\": 8,"                  "\r\n"
         "\t\t\"Key14\": 9,"                  "\r\n"
         "\t\t\"Key15\": 13.37,"              "\r\n"
         "\t\t\"Key16\": 313.37,"             "\r\n"
         "\t\t\"Key17\": {"                   "\r\n"
         "\t\t\t\t\"InnerKey1\": \"Value1\"," "\r\n"
         "\t\t\t\t\"InnerKey2\": \"Value2\""  "\r\n"
         "\t\t},"                             "\r\n"
         "\t\t\"Key18\": ["                   "\r\n"
         "\t\t\t\t1,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t7"                          "\r\n"
         "\t\t]"                              "\r\n"
         "}"s, object.Dump(2, L'\t'));

      Json array = {
        nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
        {
          { L"InnerKey1", L"Value1" },
          { L"InnerKey2", L"Value2" }
        },
        { 1, 3, 3, 7 }
      };

      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump());
      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump(0));
      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump(0, L' '));
      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump(0, L'\t'));

      Assert::AreEqual(
        L"["                                  "\r\n"
         " null,"                             "\r\n"
         " \"Test1\","                        "\r\n"
         " \"Test2\","                        "\r\n"
         " false,"                            "\r\n"
         " true,"                             "\r\n"
         " 1,"                                "\r\n"
         " 2,"                                "\r\n"
         " 3,"                                "\r\n"
         " 4,"                                "\r\n"
         " 5,"                                "\r\n"
         " 6,"                                "\r\n"
         " 7,"                                "\r\n"
         " 8,"                                "\r\n"
         " 9,"                                "\r\n"
         " 13.37,"                            "\r\n"
         " 313.37,"                           "\r\n"
         " {"                                 "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " ["                                 "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "]"s, array.Dump(1));

      Assert::AreEqual(
        L"["                                  "\r\n"
         " null,"                             "\r\n"
         " \"Test1\","                        "\r\n"
         " \"Test2\","                        "\r\n"
         " false,"                            "\r\n"
         " true,"                             "\r\n"
         " 1,"                                "\r\n"
         " 2,"                                "\r\n"
         " 3,"                                "\r\n"
         " 4,"                                "\r\n"
         " 5,"                                "\r\n"
         " 6,"                                "\r\n"
         " 7,"                                "\r\n"
         " 8,"                                "\r\n"
         " 9,"                                "\r\n"
         " 13.37,"                            "\r\n"
         " 313.37,"                           "\r\n"
         " {"                                 "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " ["                                 "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "]"s, array.Dump(1, L' '));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "\tnull,"                            "\r\n"
         "\t\"Test1\","                       "\r\n"
         "\t\"Test2\","                       "\r\n"
         "\tfalse,"                           "\r\n"
         "\ttrue,"                            "\r\n"
         "\t1,"                               "\r\n"
         "\t2,"                               "\r\n"
         "\t3,"                               "\r\n"
         "\t4,"                               "\r\n"
         "\t5,"                               "\r\n"
         "\t6,"                               "\r\n"
         "\t7,"                               "\r\n"
         "\t8,"                               "\r\n"
         "\t9,"                               "\r\n"
         "\t13.37,"                           "\r\n"
         "\t313.37,"                          "\r\n"
         "\t{"                                "\r\n"
         "\t\t\"InnerKey1\": \"Value1\","     "\r\n"
         "\t\t\"InnerKey2\": \"Value2\""      "\r\n"
         "\t},"                               "\r\n"
         "\t["                                "\r\n"
         "\t\t1,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t7"                              "\r\n"
         "\t]"                                "\r\n"
         "]"s, array.Dump(1, L'\t'));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "  null,"                            "\r\n"
         "  \"Test1\","                       "\r\n"
         "  \"Test2\","                       "\r\n"
         "  false,"                           "\r\n"
         "  true,"                            "\r\n"
         "  1,"                               "\r\n"
         "  2,"                               "\r\n"
         "  3,"                               "\r\n"
         "  4,"                               "\r\n"
         "  5,"                               "\r\n"
         "  6,"                               "\r\n"
         "  7,"                               "\r\n"
         "  8,"                               "\r\n"
         "  9,"                               "\r\n"
         "  13.37,"                           "\r\n"
         "  313.37,"                          "\r\n"
         "  {"                                "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  ["                                "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "]"s, array.Dump(2));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "  null,"                            "\r\n"
         "  \"Test1\","                       "\r\n"
         "  \"Test2\","                       "\r\n"
         "  false,"                           "\r\n"
         "  true,"                            "\r\n"
         "  1,"                               "\r\n"
         "  2,"                               "\r\n"
         "  3,"                               "\r\n"
         "  4,"                               "\r\n"
         "  5,"                               "\r\n"
         "  6,"                               "\r\n"
         "  7,"                               "\r\n"
         "  8,"                               "\r\n"
         "  9,"                               "\r\n"
         "  13.37,"                           "\r\n"
         "  313.37,"                          "\r\n"
         "  {"                                "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  ["                                "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "]"s, array.Dump(2, L' '));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "\t\tnull,"                          "\r\n"
         "\t\t\"Test1\","                     "\r\n"
         "\t\t\"Test2\","                     "\r\n"
         "\t\tfalse,"                         "\r\n"
         "\t\ttrue,"                          "\r\n"
         "\t\t1,"                             "\r\n"
         "\t\t2,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t4,"                             "\r\n"
         "\t\t5,"                             "\r\n"
         "\t\t6,"                             "\r\n"
         "\t\t7,"                             "\r\n"
         "\t\t8,"                             "\r\n"
         "\t\t9,"                             "\r\n"
         "\t\t13.37,"                         "\r\n"
         "\t\t313.37,"                        "\r\n"
         "\t\t{"                              "\r\n"
         "\t\t\t\t\"InnerKey1\": \"Value1\"," "\r\n"
         "\t\t\t\t\"InnerKey2\": \"Value2\""  "\r\n"
         "\t\t},"                             "\r\n"
         "\t\t["                              "\r\n"
         "\t\t\t\t1,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t7"                          "\r\n"
         "\t\t]"                              "\r\n"
         "]"s, array.Dump(2, L'\t'));
    }

    TEST_METHOD(TestParse)
    {
      auto str = u8"𤭢"s;
      auto wstr = L"𤭢"s;
      auto ustr = U"𤭢"s;
      auto expected = JsonArray{ L"𤭢"s };

      auto i8list = initializer_list<int8_t>{ (int8_t)'[', (int8_t)'"', (int8_t)str[0], (int8_t)str[1], (int8_t)str[2], (int8_t)str[3], (int8_t)'"', (int8_t)']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<int8_t>{ (int8_t)'[', (int8_t)'"', (int8_t)str[0], (int8_t)str[1], (int8_t)str[2], (int8_t)str[3], (int8_t)'"', (int8_t)']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(i8list));
      Assert::AreEqual<Json>(expected, Json::Parse(i8list.begin(), i8list.end()));
      auto i8enumerable = From(i8list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(i8list)));
      Assert::AreEqual<Json>(expected, Json::Parse(i8enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(i8enumerable.begin(), i8enumerable.end()));

      auto ui8list = initializer_list<uint8_t>{ (uint8_t)'[', (uint8_t)'"', (uint8_t)str[0], (uint8_t)str[1], (uint8_t)str[2], (uint8_t)str[3], (uint8_t)'"', (uint8_t)']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<uint8_t>{ (uint8_t)'[', (uint8_t)'"', (uint8_t)str[0], (uint8_t)str[1], (uint8_t)str[2], (uint8_t)str[3], (uint8_t)'"', (uint8_t)']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8list));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8list.begin(), ui8list.end()));
      auto ui8enumerable = From(ui8list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(ui8list)));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8enumerable.begin(), ui8enumerable.end()));

      auto i16list = initializer_list<int16_t>{ (int16_t)L'[', (int16_t)L'"', (int16_t)wstr[0], (int16_t)wstr[1], (int16_t)L'"', (int16_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<int16_t>{ (int16_t)L'[', (int16_t)L'"', (int16_t)wstr[0], (int16_t)wstr[1], (int16_t)L'"', (int16_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(i16list));
      Assert::AreEqual<Json>(expected, Json::Parse(i16list.begin(), i16list.end()));
      auto i16enumerable = From(i16list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(i16list)));
      Assert::AreEqual<Json>(expected, Json::Parse(i16enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(i16enumerable.begin(), i16enumerable.end()));

      auto ui16list = initializer_list<uint16_t>{ (uint16_t)L'[', (uint16_t)L'"', (uint16_t)wstr[0], (uint16_t)wstr[1], (uint16_t)L'"', (uint16_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<uint16_t>{ (uint16_t)L'[', (uint16_t)L'"', (uint16_t)wstr[0], (uint16_t)wstr[1], (uint16_t)L'"', (uint16_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16list));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16list.begin(), ui16list.end()));
      auto ui16enumerable = From(ui16list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(ui16list)));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16enumerable.begin(), ui16enumerable.end()));

      auto i32list = initializer_list<int32_t>{ (int32_t)L'[', (int32_t)L'"', (int32_t)ustr[0], (int32_t)L'"', (int32_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<int32_t>{ (int32_t)L'[', (int32_t)L'"', (int32_t)ustr[0], (int32_t)L'"', (int32_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(i32list));
      Assert::AreEqual<Json>(expected, Json::Parse(i32list.begin(), i32list.end()));
      auto i32enumerable = From(i32list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(i32list)));
      Assert::AreEqual<Json>(expected, Json::Parse(i32enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(i32enumerable.begin(), i32enumerable.end()));

      auto ui32list = initializer_list<uint32_t>{ (uint32_t)L'[', (uint32_t)L'"', (uint32_t)ustr[0], (uint32_t)L'"', (uint32_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<uint32_t>{ (uint32_t)L'[', (uint32_t)L'"', (uint32_t)ustr[0], (uint32_t)L'"', (uint32_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32list));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32list.begin(), ui32list.end()));
      auto ui32enumerable = From(ui32list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(ui32list)));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32enumerable.begin(), ui32enumerable.end()));
    }

    TEST_METHOD(TestRead)
    {
      auto input = Json::Read(L"pass01.json");
      
      Assert::IsTrue(input.Is(JsonType::Array));
      Assert::AreEqual(20i64, input.Size());

      Assert::AreEqual<Json>(L"JSON Test Pattern pass1"s, input[0]);

      Assert::IsTrue(input[1].Is(JsonType::Object));
      Assert::AreEqual(1i64, input[1].Size());
      Assert::IsTrue(input[1][L"object with 1 member"].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[1][L"object with 1 member"].Size());
      Assert::AreEqual<Json>(L"array with 1 element", input[1][L"object with 1 member"][0]);

      Assert::IsTrue(input[2].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[2].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[2]);

      Assert::IsTrue(input[3].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[3].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[3]);

      Assert::AreEqual<Json>(-42, input[4]);

      Assert::AreEqual<Json>(true, input[5]);

      Assert::AreEqual<Json>(false, input[6]);

      Assert::AreEqual<Json>(nullptr, input[7]);

      Assert::IsTrue(input[8].Is(JsonType::Object));
      Assert::AreEqual(32i64, input[8].Size());
      Assert::AreEqual<Json>(1234567890, input[8][L"integer"]);
      Assert::AreEqual<Json>(-9876.543210, input[8][L"real"]);
      Assert::AreEqual<Json>(0.123456789e-12, input[8][L"e"]);
      Assert::AreEqual<Json>(1.234567890E+34, input[8][L"E"]);
      Assert::AreEqual<Json>(23456789012E66, input[8][L""]);
      Assert::AreEqual<Json>(0, input[8][L"zero"]);
      Assert::AreEqual<Json>(1, input[8][L"one"]);
      Assert::AreEqual<Json>(L" "s, input[8][L"space"]);
      Assert::AreEqual<Json>(L"\""s, input[8][L"quote"]);
      Assert::AreEqual<Json>(L"\\"s, input[8][L"backslash"]);
      Assert::AreEqual<Json>(L"\b\f\n\r\t"s, input[8][L"controls"]);
      Assert::AreEqual<Json>(L"/ & /"s, input[8][L"slash"]);
      Assert::AreEqual<Json>(L"abcdefghijklmnopqrstuvwyz"s, input[8][L"alpha"]);
      Assert::AreEqual<Json>(L"ABCDEFGHIJKLMNOPQRSTUVWYZ"s, input[8][L"ALPHA"]);
      Assert::AreEqual<Json>(L"0123456789"s, input[8][L"digit"]);
      Assert::AreEqual<Json>(L"digit"s, input[8][L"0123456789"]);
      Assert::AreEqual<Json>(L"`1~!@#$%^&*()_+-={':[,]}|;.</>?"s, input[8][L"special"]);
      Assert::AreEqual<Json>(L"\u0123\u4567\u89AB\uCDEF\uabcd\uef4A"s, input[8][L"hex"]);
      Assert::AreEqual<Json>(true, input[8][L"true"]);
      Assert::AreEqual<Json>(false, input[8][L"false"]);
      Assert::AreEqual<Json>(nullptr, input[8][L"null"]);
      Assert::IsTrue(input[8][L"array"].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[8][L"array"].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[8][L"array"]);
      Assert::IsTrue(input[8][L"object"].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[8][L"object"].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[8][L"object"]);
      Assert::AreEqual<Json>(L"50 St. James Street", input[8][L"address"]);
      Assert::AreEqual<Json>(L"http://www.JSON.org/", input[8][L"url"]);
      Assert::AreEqual<Json>(L"// /* <!-- --", input[8][L"comment"]);
      Assert::AreEqual<Json>(L" ", input[8][L"# -- --> */"]);
      Assert::IsTrue(input[8][L" s p a c e d "].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L" s p a c e d "].Size());
      Assert::AreEqual<Json>(1, input[8][L" s p a c e d "][0]);
      Assert::AreEqual<Json>(2, input[8][L" s p a c e d "][1]);
      Assert::AreEqual<Json>(3, input[8][L" s p a c e d "][2]);
      Assert::AreEqual<Json>(4, input[8][L" s p a c e d "][3]);
      Assert::AreEqual<Json>(5, input[8][L" s p a c e d "][4]);
      Assert::AreEqual<Json>(6, input[8][L" s p a c e d "][5]);
      Assert::AreEqual<Json>(7, input[8][L" s p a c e d "][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L" s p a c e d "]);
      Assert::IsTrue(input[8][L"compact"].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L"compact"].Size());
      Assert::AreEqual<Json>(1, input[8][L"compact"][0]);
      Assert::AreEqual<Json>(2, input[8][L"compact"][1]);
      Assert::AreEqual<Json>(3, input[8][L"compact"][2]);
      Assert::AreEqual<Json>(4, input[8][L"compact"][3]);
      Assert::AreEqual<Json>(5, input[8][L"compact"][4]);
      Assert::AreEqual<Json>(6, input[8][L"compact"][5]);
      Assert::AreEqual<Json>(7, input[8][L"compact"][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L"compact"]);
      Assert::AreEqual<Json>(L"{\"object with 1 member\":[\"array with 1 element\"]}", input[8][L"jsontext"]);
      Assert::AreEqual<Json>(L"&#34; \u0022 %22 0x22 034 &#x22;", input[8][L"quotes"]);
      Assert::AreEqual<Json>(L"A key can be any string", input[8][L"/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"]);

      Assert::AreEqual<Json>(0.5, input[9]);

      Assert::AreEqual<Json>(98.6, input[10]);

      Assert::AreEqual<Json>(99.44, input[11]);

      Assert::AreEqual<Json>(1066, input[12]);

      Assert::AreEqual<Json>(1e1, input[13]);

      Assert::AreEqual<Json>(0.1e1, input[14]);

      Assert::AreEqual<Json>(1e-1, input[15]);

      Assert::AreEqual<Json>(1e00, input[16]);

      Assert::AreEqual<Json>(2e+00, input[17]);

      Assert::AreEqual<Json>(2e-00, input[18]);

      Assert::AreEqual<Json>(L"rosebud", input[19]);
    }

    TEST_METHOD(TestWrite)
    {
      Json::Read(L"pass01.json").Write(L"pass01_copy.json");
      auto input = Json::Read(L"pass01_copy.json");

      Assert::AreEqual<Json>(Json::Read(L"pass01.json"), input);
      Assert::AreEqual<Json>(
        {
          L"JSON Test Pattern pass1"s,
          { { L"object with 1 member"s, { L"array with 1 element"s } } },
          JsonObject{},
          JsonArray{},
          -42,
          true,
          false,
          nullptr,
          {
            { L"integer"s, 1234567890 },
            { L"real"s, -9876.543210 },
            { L"e"s, 0.123456789e-12 },
            { L"E"s, 1.234567890E+34 },
            { L""s, 23456789012E66 },
            { L"zero"s, 0 },
            { L"one"s, 1 },
            { L"space"s, L" "s },
            { L"quote"s, L"\""s },
            { L"backslash"s, L"\\"s },
            { L"controls"s, L"\b\f\n\r\t"s },
            { L"slash"s, L"/ & /"s },
            { L"alpha"s, L"abcdefghijklmnopqrstuvwyz"s },
            { L"ALPHA"s, L"ABCDEFGHIJKLMNOPQRSTUVWYZ"s },
            { L"digit"s, L"0123456789"s },
            { L"0123456789"s, L"digit"s },
            { L"special"s, L"`1~!@#$%^&*()_+-={':[,]}|;.</>?"s },
            { L"hex"s, L"\u0123\u4567\u89AB\uCDEF\uabcd\uef4A"s },
            { L"true"s, true },
            { L"false"s, false },
            { L"null"s, nullptr },
            { L"array"s, JsonArray{} },
            { L"object"s, JsonObject{} },
            { L"address"s, L"50 St. James Street"s },
            { L"url"s, L"http://www.JSON.org/"s },
            { L"comment"s, L"// /* <!-- --"s },
            { L"# -- --> */"s, L" "s },
            { L" s p a c e d "s, { 1, 2, 3, 4, 5, 6, 7 } },
            { L"compact"s, { 1, 2, 3, 4, 5, 6, 7 } },
            { L"jsontext"s, L"{\"object with 1 member\":[\"array with 1 element\"]}"s },
            { L"quotes"s, L"&#34; \u0022 %22 0x22 034 &#x22;"s },
            { L"/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"s, L"A key can be any string"s }
          },
          0.5,
          98.6,
          99.44,
          1066,
          1e1,
          0.1e1,
          1e-1,
          1e00,
          2e+00,
          2e-00,
          L"rosebud"s
        }
        , input);

      auto json = Json{ 1, 3, 3, 7 };
      json.Write(L"test_write_00.json");
      json.Write(L"test_write_01.json", L'\t');
      json.Write(L"test_write_10.json", 1ui8);
      json.Write(L"test_write_11.json", 1ui8, L'\t');

      Assert::AreEqual(ReadAllText(L"test_write_00.json"), L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]"s);
      Assert::AreEqual(ReadAllText(L"test_write_01.json"), L"[\r\n\t\t1,\r\n\t\t3,\r\n\t\t3,\r\n\t\t7\r\n]"s);
      Assert::AreEqual(ReadAllText(L"test_write_10.json"), L"[\r\n 1,\r\n 3,\r\n 3,\r\n 7\r\n]"s);
      Assert::AreEqual(ReadAllText(L"test_write_11.json"), L"[\r\n\t1,\r\n\t3,\r\n\t3,\r\n\t7\r\n]"s);
    }

    TEST_METHOD(TestGet)
    {
      auto null = Json(nullptr);
      Assert::ExpectException<exception>([&]() { null.Get<wstring   >(); });
      Assert::ExpectException<exception>([&]() { null.Get<bool      >(); });
      Assert::ExpectException<exception>([&]() { null.Get<double    >(); });
      Assert::ExpectException<exception>([&]() { null.Get<int64_t   >(); });
      Assert::ExpectException<exception>([&]() { null.Get<JsonObject>(); });
      Assert::ExpectException<exception>([&]() { null.Get<JsonArray >(); });
      Assert::AreEqual(nullptr, null.Get<nullptr_t>());

      auto wstr = Json(L"asd"s);
      Assert::ExpectException<exception>([&]() { wstr.Get<nullptr_t >(); });
      Assert::ExpectException<exception>([&]() { wstr.Get<bool      >(); });
      Assert::ExpectException<exception>([&]() { wstr.Get<double    >(); });
      Assert::ExpectException<exception>([&]() { wstr.Get<int64_t   >(); });
      Assert::ExpectException<exception>([&]() { wstr.Get<JsonObject>(); });
      Assert::ExpectException<exception>([&]() { wstr.Get<JsonArray >(); });
      Assert::AreEqual(L"asd"s, wstr.Get<wstring>());

      auto boolean = Json(true);
      Assert::ExpectException<exception>([&]() { boolean.Get<nullptr_t >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<wstring   >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<double    >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<int64_t   >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<JsonObject>(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<JsonArray >(); });
      Assert::AreEqual(true, boolean.Get<bool>());

      boolean = Json(false);
      Assert::ExpectException<exception>([&]() { boolean.Get<nullptr_t >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<wstring   >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<double    >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<int64_t   >(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<JsonObject>(); });
      Assert::ExpectException<exception>([&]() { boolean.Get<JsonArray >(); });
      Assert::AreEqual(false, boolean.Get<bool>());

      auto real = Json(13.37);
      Assert::ExpectException<exception>([&]() { real.Get<nullptr_t >(); });
      Assert::ExpectException<exception>([&]() { real.Get<wstring   >(); });
      Assert::ExpectException<exception>([&]() { real.Get<bool      >(); });
      Assert::ExpectException<exception>([&]() { real.Get<int64_t   >(); });
      Assert::ExpectException<exception>([&]() { real.Get<JsonObject>(); });
      Assert::ExpectException<exception>([&]() { real.Get<JsonArray >(); });
      Assert::AreEqual(13.37, real.Get<double>());

      auto integer = Json(1337);
      Assert::ExpectException<exception>([&]() { integer.Get<nullptr_t >(); });
      Assert::ExpectException<exception>([&]() { integer.Get<wstring   >(); });
      Assert::ExpectException<exception>([&]() { integer.Get<bool      >(); });
      Assert::ExpectException<exception>([&]() { integer.Get<double    >(); });
      Assert::ExpectException<exception>([&]() { integer.Get<JsonObject>(); });
      Assert::ExpectException<exception>([&]() { integer.Get<JsonArray >(); });
      Assert::AreEqual(1337i64, integer.Get<int64_t>());

      auto object = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::ExpectException<exception>([&]() { object.Get<nullptr_t >(); });
      Assert::ExpectException<exception>([&]() { object.Get<wstring   >(); });
      Assert::ExpectException<exception>([&]() { object.Get<bool      >(); });
      Assert::ExpectException<exception>([&]() { object.Get<double    >(); });
      Assert::ExpectException<exception>([&]() { object.Get<int64_t   >(); });
      Assert::ExpectException<exception>([&]() { object.Get<JsonArray >(); });
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, object.Get<JsonObject>());

      auto array = Json(JsonArray{ 1, 2, 3 });
      Assert::ExpectException<exception>([&]() { array.Get<nullptr_t >(); });
      Assert::ExpectException<exception>([&]() { array.Get<wstring   >(); });
      Assert::ExpectException<exception>([&]() { array.Get<bool      >(); });
      Assert::ExpectException<exception>([&]() { array.Get<double    >(); });
      Assert::ExpectException<exception>([&]() { array.Get<int64_t   >(); });
      Assert::ExpectException<exception>([&]() { array.Get<JsonObject>(); });
      Assert::AreEqual(JsonArray{ 1, 2, 3 }, array.Get<JsonArray>());
    }

    TEST_METHOD(TestGetIf)
    {
      auto null = Json(nullptr);
      Assert::IsTrue(null.GetIf<wstring   >() == nullptr);
      Assert::IsTrue(null.GetIf<bool      >() == nullptr);
      Assert::IsTrue(null.GetIf<double    >() == nullptr);
      Assert::IsTrue(null.GetIf<int64_t   >() == nullptr);
      Assert::IsTrue(null.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(null.GetIf<JsonArray >() == nullptr);
      auto ptrnull = null.GetIf<nullptr_t>();
      Assert::IsTrue(*ptrnull == nullptr);

      auto wstr = Json(L"asd"s);
      Assert::IsTrue(wstr.GetIf<nullptr_t >() == nullptr);
      Assert::IsTrue(wstr.GetIf<bool      >() == nullptr);
      Assert::IsTrue(wstr.GetIf<double    >() == nullptr);
      Assert::IsTrue(wstr.GetIf<int64_t   >() == nullptr);
      Assert::IsTrue(wstr.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(wstr.GetIf<JsonArray >() == nullptr);
      auto ptrwstr = wstr.GetIf<wstring>();
      Assert::AreEqual<wstring>(L"asd"s, *ptrwstr);
      *ptrwstr = L"test";
      Assert::AreEqual<wstring>(L"test"s, *ptrwstr);
      Assert::AreEqual<Json>(L"test"s, wstr);

      auto boolean = Json(true);
      Assert::IsTrue(boolean.GetIf<nullptr_t >() == nullptr);
      Assert::IsTrue(boolean.GetIf<wstring   >() == nullptr);
      Assert::IsTrue(boolean.GetIf<double    >() == nullptr);
      Assert::IsTrue(boolean.GetIf<int64_t   >() == nullptr);
      Assert::IsTrue(boolean.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(boolean.GetIf<JsonArray >() == nullptr);
      auto ptrboolean = boolean.GetIf<bool>();
      Assert::AreEqual<bool>(true, *ptrboolean);
      *ptrboolean = false;
      Assert::AreEqual<bool>(false, *ptrboolean);
      Assert::AreEqual<Json>(false, boolean);

      auto real = Json(13.37);
      Assert::IsTrue(real.GetIf<nullptr_t >() == nullptr);
      Assert::IsTrue(real.GetIf<wstring   >() == nullptr);
      Assert::IsTrue(real.GetIf<bool      >() == nullptr);
      Assert::IsTrue(real.GetIf<int64_t   >() == nullptr);
      Assert::IsTrue(real.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(real.GetIf<JsonArray >() == nullptr);
      auto ptrreal = real.GetIf<double>();
      Assert::AreEqual<double>(13.37, *ptrreal);
      *ptrreal = 313.37;
      Assert::AreEqual<double>(313.37, *ptrreal);
      Assert::AreEqual<Json>(313.37, real);

      auto integer = Json(1337);
      Assert::IsTrue(integer.GetIf<nullptr_t >() == nullptr);
      Assert::IsTrue(integer.GetIf<wstring   >() == nullptr);
      Assert::IsTrue(integer.GetIf<bool      >() == nullptr);
      Assert::IsTrue(integer.GetIf<double    >() == nullptr);
      Assert::IsTrue(integer.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(integer.GetIf<JsonArray >() == nullptr);
      auto ptrinteger = integer.GetIf<int64_t>();
      Assert::AreEqual<int64_t>(1337, *ptrinteger);
      *ptrinteger = 31337;
      Assert::AreEqual<int64_t>(31337, *ptrinteger);
      Assert::AreEqual<Json>(31337, integer);

      auto object = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(object.GetIf<nullptr_t>() == nullptr);
      Assert::IsTrue(object.GetIf<wstring  >() == nullptr);
      Assert::IsTrue(object.GetIf<bool     >() == nullptr);
      Assert::IsTrue(object.GetIf<double   >() == nullptr);
      Assert::IsTrue(object.GetIf<int64_t  >() == nullptr);
      Assert::IsTrue(object.GetIf<JsonArray>() == nullptr);
      auto ptrobject = object.GetIf<JsonObject>();
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, *ptrobject);
      *ptrobject = JsonObject{ { L"Key3"s, 3 }, { L"Key4"s, 4 } };
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key3"s, 3 }, { L"Key4"s, 4 } }, *ptrobject);
      Assert::AreEqual<Json>(JsonObject{ { L"Key3"s, 3 }, { L"Key4"s, 4 } }, object);

      auto array = Json(JsonArray{ 1, 2, 3 });
      Assert::IsTrue(array.GetIf<nullptr_t >() == nullptr);
      Assert::IsTrue(array.GetIf<wstring   >() == nullptr);
      Assert::IsTrue(array.GetIf<bool      >() == nullptr);
      Assert::IsTrue(array.GetIf<double    >() == nullptr);
      Assert::IsTrue(array.GetIf<int64_t   >() == nullptr);
      Assert::IsTrue(array.GetIf<JsonObject>() == nullptr);
      auto ptrarray = array.GetIf<JsonArray>();
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 2, 3 }, *ptrarray);
      *ptrarray = JsonArray{ 1, 2, 3, 4 };
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 2, 3, 4 }, *ptrarray);
      Assert::AreEqual<Json>(JsonArray{ 1, 2, 3, 4 }, array);
    }

    TEST_METHOD(TestStringify)
    {
      auto json = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(json));
      Assert::AreEqual<Json>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json);
      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(move(json)));
      Assert::AreEqual<Json>(JsonObject(), json);

      auto builder = JsonBuilder{ { L"Key1"s, 1 }, { L"Key2"s, 2 } };
      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(builder));
      Assert::AreEqual<JsonBuilder>(JsonBuilder{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, builder);
      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(move(builder)));
      Assert::AreEqual<JsonBuilder>(JsonBuilder{ { L""s, 1 }, { L""s, 2 } }, builder);

      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(initializer_list<JsonBuilder>{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }));

      Assert::AreEqual(L"null"s, Json::Stringify(nullptr));

      Assert::AreEqual(L"\"asd\""s, Json::Stringify(L"asd"));

      auto wstr = L"asd"s;
      Assert::AreEqual(L"\"asd\""s, Json::Stringify(wstr));
      Assert::AreEqual(L"asd"s, wstr);
      Assert::AreEqual(L"\"asd\""s, Json::Stringify(move(wstr)));
      Assert::AreEqual(L""s, wstr);

      Assert::AreEqual(L"true"s, Json::Stringify(true));
      Assert::AreEqual(L"false"s, Json::Stringify(false));

      Assert::AreEqual(L"65"s, Json::Stringify('A'));
      Assert::AreEqual(L"1"s, Json::Stringify(1i8));
      Assert::AreEqual(L"2"s, Json::Stringify(2ui8));
      Assert::AreEqual(L"3"s, Json::Stringify(3i16));
      Assert::AreEqual(L"4"s, Json::Stringify(4ui16));
      Assert::AreEqual(L"5"s, Json::Stringify(5i32));
      Assert::AreEqual(L"6"s, Json::Stringify(6ui32));
      Assert::AreEqual(L"7"s, Json::Stringify(7i64));
      Assert::AreEqual(L"8"s, Json::Stringify(8ui64));
      Assert::AreEqual(L"13.37"s, Json::Stringify(13.37));
      Assert::AreEqual(L"313.37"s, Json::Stringify(313.37));
      Assert::AreEqual(L"1337"s, Json::Stringify(1337));
      Assert::AreEqual(L"1e+50"s, Json::Stringify(1e50));

      auto object = JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } };
      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(object));
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, object);
      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(move(object)));
      Assert::AreEqual(JsonObject(), object);

      auto array = JsonArray{ 1, 2, 3 };
      Assert::AreEqual(L"[1,2,3]"s, Json::Stringify(array));
      Assert::AreEqual(JsonArray{ 1, 2, 3 }, array);
      Assert::AreEqual(L"[1,2,3]"s, Json::Stringify(move(array)));
      Assert::AreEqual(JsonArray(), array);

      Assert::AreEqual(L"Complex"s, Json::Stringify(JsonType::Complex));
      Assert::AreEqual(L"PropertyName"s, Json::Stringify(JsonTokenType::PropertyName));
    }

    TEST_METHOD(TestSize)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(wstring  ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(bool     ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(double   ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(int64_t  ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");

      Assert::AreEqual(0i64, Json(JsonObject()).Size());
      Assert::AreEqual(0i64, Json(JsonArray ()).Size());

      Assert::AreEqual(2i64, Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }).Size());
      Assert::AreEqual(4i64, Json(JsonArray{ 1, 3, 3, 7 }).Size());
    }

    TEST_METHOD(TestCount)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).Count(wstring()); }, "Count(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(wstring  ()).Count(wstring()); }, "Count(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(bool     ()).Count(wstring()); }, "Count(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(double   ()).Count(wstring()); }, "Count(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(int64_t  ()).Count(wstring()); }, "Count(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(JsonArray()).Count(wstring()); }, "Count(wstring const& key) is only defined for JsonObject!");

      auto json = Json{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };

      Assert::AreEqual(0i64, json.Count(L"asd"));
      Assert::AreEqual(0i64, json.Count(L"asd"s));
      for (auto& key : json.Keys())
      {
        Assert::AreEqual(1i64, json.Count(key));
      }
    }

    TEST_METHOD(TestContains)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).Contains(wstring()); }, "Contains(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(wstring  ()).Contains(wstring()); }, "Contains(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(bool     ()).Contains(wstring()); }, "Contains(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(double   ()).Contains(wstring()); }, "Contains(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(int64_t  ()).Contains(wstring()); }, "Contains(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(JsonArray()).Contains(wstring()); }, "Contains(wstring const& key) is only defined for JsonObject!");

      auto json = Json{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };

      Assert::IsFalse(json.Contains(L"asd"));
      Assert::IsFalse(json.Contains(L"asd"s));
      for (auto& key : json.Keys())
      {
        Assert::IsTrue(json.Contains(key));
      }
    }

    TEST_METHOD(TestResize)
    {
      ExpectException<exception>([]() { Json(nullptr_t ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(wstring   ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(bool      ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(double    ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(int64_t   ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(JsonObject()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");

      ExpectException<exception>([]() { Json(nullptr_t ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(wstring   ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(bool      ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(double    ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(int64_t   ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(JsonObject()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");

      auto array = Json(JsonArray());
      Assert::AreEqual(0i64, array.Size());
      array.Resize(1);
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      array.Resize(2, L"Test"s);
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
    }

    TEST_METHOD(TestClear)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(wstring  ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(bool     ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(double   ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { Json(int64_t  ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");

      auto object = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::AreEqual(2i64, object.Size());
      object.Clear();
      Assert::AreEqual(0i64, object.Size());

      auto array = Json(JsonArray{ 1, 3, 3, 7 });
      Assert::AreEqual(4i64, array.Size());
      array.Clear();
      Assert::AreEqual(0i64, array.Size());
    }

    TEST_METHOD(TestPushBack)
    {
      auto defaultJson = Json();
      ExpectException<exception>([&]() { Json(wstring   ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(bool      ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(double    ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(int64_t   ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(JsonObject()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(wstring   ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(bool      ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(double    ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(int64_t   ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(JsonObject()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json.PushBack(1337);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Json integer = 31337;
      json.PushBack(integer);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(31337, json[1]);
      Json str = L"test"s;
      json.PushBack(move(str));
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(L"test"s, json[2]);
      Assert::AreEqual<Json>(L""s, str);
      Json obj = { { L"Key"s, 1 } };
      json.PushBack(move(obj));
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(4i64, json.Size());
      Assert::AreEqual<Json>({ { L"Key"s, 1 } }, json[3]);
      Assert::AreEqual<Json>(JsonObject(), obj);
    }

    TEST_METHOD(TestInsert1)
    {
      auto defaultPair = pair<wstring, Json>{ wstring(), Json() };
      ExpectException<exception>([&]() { Json(wstring  ()).Insert(defaultPair); }, "Insert(pair<wstring, Json> const& pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(bool     ()).Insert(defaultPair); }, "Insert(pair<wstring, Json> const& pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(double   ()).Insert(defaultPair); }, "Insert(pair<wstring, Json> const& pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(int64_t  ()).Insert(defaultPair); }, "Insert(pair<wstring, Json> const& pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(JsonArray()).Insert(defaultPair); }, "Insert(pair<wstring, Json> const& pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(wstring  ()).Insert({ wstring(), Json() }); }, "Insert(pair<wstring, Json> && pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(bool     ()).Insert({ wstring(), Json() }); }, "Insert(pair<wstring, Json> && pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(double   ()).Insert({ wstring(), Json() }); }, "Insert(pair<wstring, Json> && pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(int64_t  ()).Insert({ wstring(), Json() }); }, "Insert(pair<wstring, Json> && pair) is only defined for JsonObject!");
      ExpectException<exception>([&]() { Json(JsonArray()).Insert({ wstring(), Json() }); }, "Insert(pair<wstring, Json> && pair) is only defined for JsonObject!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json.Insert({ L"Key1"s, 1 });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[L"Key1"s]);
      json.Insert({ L"Key2"s, 2 });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
      auto key3 = pair<wstring, Json>{ L"Key3"s, 3 };
      json.Insert(key3);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(3, json[L"Key3"s]);
      auto key4 = pair<wstring, Json>{ L"Key4"s, 4 };
      json.Insert(key4);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(4i64, json.Size());
      Assert::AreEqual<Json>(4, json[L"Key4"s]);
      auto key5 = pair<wstring, Json>{ L"Key5"s, 5 };
      json.Insert(move(key5));
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(5i64, json.Size());
      Assert::AreEqual<Json>(5, json[L"Key5"s]);
      Assert::AreEqual(L""s, key5.first);
      Assert::AreEqual<Json>(5, key5.second);
      auto key6 = pair<wstring, Json>{ L"Key6"s, { { L"InnerKey1"s, 1 } } };
      json.Insert(move(key6));
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(6i64, json.Size());
      Assert::AreEqual<Json>({ { L"InnerKey1"s, 1 } }, json[L"Key6"s]);
      Assert::AreEqual(L""s, key6.first);
      Assert::AreEqual<Json>(JsonObject(), key6.second);
    }

    TEST_METHOD(TestInsert2)
    {
      auto defaultValue = Json();
      ExpectException<exception>([&]() { Json(wstring   ()).Insert(int64_t(), defaultValue); }, "Insert(int64_t index, Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(bool      ()).Insert(int64_t(), defaultValue); }, "Insert(int64_t index, Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(double    ()).Insert(int64_t(), defaultValue); }, "Insert(int64_t index, Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(int64_t   ()).Insert(int64_t(), defaultValue); }, "Insert(int64_t index, Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(JsonObject()).Insert(int64_t(), defaultValue); }, "Insert(int64_t index, Json const& value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(wstring   ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(bool      ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(double    ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(int64_t   ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExpectException<exception>([&]() { Json(JsonObject()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json.Insert(0, 1337);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      json.Insert(0, 31337);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(31337, json[0]);
      Assert::AreEqual<Json>(1337, json[1]);
      auto value = Json{ { L"Key1"s, 1 } };
      json.Insert(0, move(value));
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>({ { L"Key1"s, 1 } }, json[0]);
      Assert::AreEqual<Json>(31337, json[1]);
      Assert::AreEqual<Json>(1337, json[2]);
      Assert::AreEqual<Json>(JsonObject(), value);
    }

    TEST_METHOD(TestErase1)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).Erase(wstring()); }, "Erase(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(wstring  ()).Erase(wstring()); }, "Erase(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(bool     ()).Erase(wstring()); }, "Erase(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(double   ()).Erase(wstring()); }, "Erase(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(int64_t  ()).Erase(wstring()); }, "Erase(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(JsonArray()).Erase(wstring()); }, "Erase(wstring const& key) is only defined for JsonObject!");

      auto json = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      json.Erase(L"Key1"s);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
    }

    TEST_METHOD(TestErase2)
    {
      ExpectException<exception>([]() { Json(nullptr_t ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(wstring   ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(bool      ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(double    ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(int64_t   ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExpectException<exception>([]() { Json(JsonObject()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");

      auto json = Json(JsonArray{ 1, 3, 3, 7 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(4i64, json.Size());
      json.Erase(0);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(3, json[0]);
      Assert::AreEqual<Json>(3, json[1]);
      Assert::AreEqual<Json>(7, json[2]);
    }

    TEST_METHOD(TestKeys)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(wstring  ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(bool     ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(double   ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(int64_t  ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(JsonArray()).Keys(); }, "Keys() is only defined for JsonObject!");

      auto json = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      auto keys = json.Keys();
      Assert::AreEqual<size_t>(2, keys.size());
      Assert::AreEqual(L"Key1"s, keys[0]);
      Assert::AreEqual(L"Key2"s, keys[1]);
    }

    TEST_METHOD(TestKeysView)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).KeysView(); }, "KeysView() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(wstring  ()).KeysView(); }, "KeysView() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(bool     ()).KeysView(); }, "KeysView() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(double   ()).KeysView(); }, "KeysView() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(int64_t  ()).KeysView(); }, "KeysView() is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(JsonArray()).KeysView(); }, "KeysView() is only defined for JsonObject!");

      auto json = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      auto keys = json.KeysView();
      Assert::AreEqual<size_t>(2, keys.size());
      Assert::AreEqual(L"Key1"s, keys[0].get());
      Assert::AreEqual(L"Key2"s, keys[1].get());
    }

    TEST_METHOD(TestOperatorSubscriptJsonPointer)
    {
      ExpectException<exception>([]() { auto temp = Json(nullptr_t())[JsonPointer()]; }, "Operator[](JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(wstring  ())[JsonPointer()]; }, "Operator[](JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(bool     ())[JsonPointer()]; }, "Operator[](JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(double   ())[JsonPointer()]; }, "Operator[](JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(int64_t  ())[JsonPointer()]; }, "Operator[](JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");

      auto json0 = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(json0, json0[L""_JsonPointer]);
      Assert::AreEqual<Json>(json0[0], json0[L"/0"_JsonPointer]);
      Assert::AreEqual<Json>(json0[1], json0[L"/1"_JsonPointer]);
      Assert::AreEqual<Json>(json0[2], json0[L"/2"_JsonPointer]);
      Assert::AreEqual<Json>(json0[3], json0[L"/3"_JsonPointer]);
      json0[L"/2"_JsonPointer] = 1337;
      Assert::AreEqual<Json>(1337, json0[L"/2"_JsonPointer]);
      auto json1 = Json{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual<Json>(json1                    , json1[L""_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Null"]           , json1[L"/Null"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"String"]         , json1[L"/String"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Boolean"]        , json1[L"/Boolean"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Real"]           , json1[L"/Real"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Integer"]        , json1[L"/Integer"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Object"]         , json1[L"/Object"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Object"][L"Key1"], json1[L"/Object/Key1"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Object"][L"Key2"], json1[L"/Object/Key2"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Array"]          , json1[L"/Array"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Array"][0]       , json1[L"/Array/0"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Array"][1]       , json1[L"/Array/1"_JsonPointer]);
      Assert::AreEqual<Json>(json1[L"Array"][2]       , json1[L"/Array/2"_JsonPointer]);
      auto json2 = Json{
        { L"foo"s   , { L"bar"s, L"baz"s } } ,
        { L""s      , 0 },
        { L"a/b"s   , 1 },
        { L"c%d"s   , 2 },
        { L"e^f"s   , 3 },
        { L"g|h"s   , 4 },
        { L"i\\j"s  , 5},
        { L"k\"l"s  , 6},
        { L" "s     , 7 },
        { L"m~n"s   , 8 }
      };
      Assert::AreEqual<Json>(json2            , json2[L""_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"foo"s]   , json2[L"/foo"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"foo"s][0], json2[L"/foo/0"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L""s]      , json2[L"/"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"a/b"s]   , json2[L"/a~1b"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"c%d"s]   , json2[L"/c%d"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"e^f"s]   , json2[L"/e^f"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"g|h"s]   , json2[L"/g|h"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"i\\j"s]  , json2[L"/i\\j"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"k\"l"s]  , json2[L"/k\"l"_JsonPointer]);
      Assert::AreEqual<Json>(json2[L" "s]     , json2[L"/ "_JsonPointer]);
      Assert::AreEqual<Json>(json2[L"m~n"s]   , json2[L"/m~0n"_JsonPointer]);

      ExpectException<exception>([&]() { auto temp = json2[L"/Non/Existent"_JsonPointer]; }      , "Reference token \"Non\" at path \"/Non\" not found!");
      ExpectException<exception>([&]() { auto temp = json1[L"/Object/NonExistent"_JsonPointer]; }, "Reference token \"NonExistent\" at path \"/Object/NonExistent\" not found!");
      ExpectException<exception>([&]() { auto temp = json2[L"/foo/-"_JsonPointer]; }             , "Reference token \"-\" at path \"/foo/-\" points to the member after the last array element which does not exist!");
      ExpectException<exception>([&]() { auto temp = json2[L"/foo/00"_JsonPointer]; }            , "Reference token \"00\" at path \"/foo/00\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2[L"/foo/01"_JsonPointer]; }            , "Reference token \"01\" at path \"/foo/01\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2[L"/foo/-1"_JsonPointer]; }            , "Reference token \"-1\" at path \"/foo/-1\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2[L"/foo/1a"_JsonPointer]; }            , "Reference token \"1a\" at path \"/foo/1a\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2[L"/foo/3"_JsonPointer]; }             , "Reference token \"3\" at path \"/foo/3\" is out of range!");
    }

    TEST_METHOD(TestOperatorSubscriptWString)
    {
      ExpectException<exception>([]() { auto temp = Json(wstring  ())[L""      ]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(bool     ())[L""      ]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(double   ())[L""      ]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(int64_t  ())[L""      ]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(JsonArray())[L""      ]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(wstring  ())[wstring()]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(bool     ())[wstring()]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(double   ())[wstring()]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(int64_t  ())[wstring()]; }, "Operator[](wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto temp = Json(JsonArray())[wstring()]; }, "Operator[](wstring const& key) is only defined for JsonObject!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json[L"Key1"s] = 1;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[L"Key1"s]);
      json[L"Key2"s] = 2;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
      json[L"Key2"s] = 1337;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1337, json[L"Key2"s]);
      json[L"Key2"] = 2;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(2, json[L"Key2"]);
    }

    TEST_METHOD(TestOperatorSubscriptIndex)
    {
      auto jsons = vector<Json>{ Json(nullptr_t()), Json(wstring()), Json(bool()), Json(double()), Json(int64_t()), Json(JsonObject()) };
      for (auto& json : jsons)
      {
        ExpectException<exception>([&]() { json[int8_t  ()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json[uint8_t ()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json[int16_t ()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json[uint16_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json[int32_t ()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json[uint32_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json[int64_t ()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json[uint64_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
      }

      auto json = Json(JsonArray());
      json.Resize(4);
      json[0] = 1;
      json[1] = 3;
      json[2] = 3;
      json[3] = 7;
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(3, json[1]);
      Assert::AreEqual<Json>(3, json[2]);
      Assert::AreEqual<Json>(7, json[3]);
      Assert::AreEqual<Json>(7, json[3i8]);
      Assert::AreEqual<Json>(7, json[3ui8]);
      Assert::AreEqual<Json>(7, json[3i16]);
      Assert::AreEqual<Json>(7, json[3ui16]);
      Assert::AreEqual<Json>(7, json[3i32]);
      Assert::AreEqual<Json>(7, json[3ui32]);
      Assert::AreEqual<Json>(7, json[3i64]);
      Assert::AreEqual<Json>(7, json[3ui64]);
    }

    TEST_METHOD(TestAtJsonPointer)
    {
      ExpectException<exception>([]() { auto temp = Json(nullptr_t()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(wstring  ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(bool     ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(double   ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto temp = Json(int64_t  ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");

      auto json0 = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(json0, json0.At(L""_JsonPointer));
      Assert::AreEqual<Json>(json0.At(0), json0.At(L"/0"_JsonPointer));
      Assert::AreEqual<Json>(json0.At(1), json0.At(L"/1"_JsonPointer));
      Assert::AreEqual<Json>(json0.At(2), json0.At(L"/2"_JsonPointer));
      Assert::AreEqual<Json>(json0.At(3), json0.At(L"/3"_JsonPointer));
      json0.At(L"/2"_JsonPointer) = 1337;
      Assert::AreEqual<Json>(1337, json0.At(L"/2"_JsonPointer));

      auto json1 = Json{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual<Json>(json1                          , json1.At(L""_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Null")              , json1.At(L"/Null"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"String")            , json1.At(L"/String"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Boolean")           , json1.At(L"/Boolean"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Real")              , json1.At(L"/Real"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Integer")           , json1.At(L"/Integer"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Object")            , json1.At(L"/Object"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key1"), json1.At(L"/Object/Key1"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key2"), json1.At(L"/Object/Key2"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array")             , json1.At(L"/Array"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array").At(0)       , json1.At(L"/Array/0"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array").At(1)       , json1.At(L"/Array/1"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array").At(2)       , json1.At(L"/Array/2"_JsonPointer));

      auto json2 = Json{
        { L"foo"s   , { L"bar"s, L"baz"s } } ,
        { L""s      , 0 },
        { L"a/b"s   , 1 },
        { L"c%d"s   , 2 },
        { L"e^f"s   , 3 },
        { L"g|h"s   , 4 },
        { L"i\\j"s  , 5},
        { L"k\"l"s  , 6},
        { L" "s     , 7 },
        { L"m~n"s   , 8 }
      };
      Assert::AreEqual<Json>(json2                  , json2.At(L""_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"foo"s)      , json2.At(L"/foo"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"foo"s).At(0), json2.At(L"/foo/0"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L""s)         , json2.At(L"/"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"a/b"s)      , json2.At(L"/a~1b"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"c%d"s)      , json2.At(L"/c%d"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"e^f"s)      , json2.At(L"/e^f"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"g|h"s)      , json2.At(L"/g|h"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"i\\j"s)     , json2.At(L"/i\\j"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"k\"l"s)     , json2.At(L"/k\"l"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L" "s)        , json2.At(L"/ "_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"m~n"s)      , json2.At(L"/m~0n"_JsonPointer));

      ExpectException<exception>([&]() { auto temp = json2.At(L"/Non/Existent"_JsonPointer); }      , "Reference token \"Non\" at path \"/Non\" not found!");
      ExpectException<exception>([&]() { auto temp = json1.At(L"/Object/NonExistent"_JsonPointer); }, "Reference token \"NonExistent\" at path \"/Object/NonExistent\" not found!");
      ExpectException<exception>([&]() { auto temp = json2.At(L"/foo/-"_JsonPointer); }             , "Reference token \"-\" at path \"/foo/-\" points to the member after the last array element which does not exist!");
      ExpectException<exception>([&]() { auto temp = json2.At(L"/foo/00"_JsonPointer); }            , "Reference token \"00\" at path \"/foo/00\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2.At(L"/foo/01"_JsonPointer); }            , "Reference token \"01\" at path \"/foo/01\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2.At(L"/foo/-1"_JsonPointer); }            , "Reference token \"-1\" at path \"/foo/-1\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2.At(L"/foo/1a"_JsonPointer); }            , "Reference token \"1a\" at path \"/foo/1a\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = json2.At(L"/foo/3"_JsonPointer); }             , "Reference token \"3\" at path \"/foo/3\" is out of range!");
    }

    TEST_METHOD(TestAtJsonPointerConst)
    {
      ExpectException<exception>([]() { auto const temp = Json(nullptr_t()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto const temp = Json(wstring  ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto const temp = Json(bool     ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto const temp = Json(double   ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { auto const temp = Json(int64_t  ()).At(JsonPointer()); }, "At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");

      auto const json0 = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(json0, json0.At(L""_JsonPointer));
      Assert::AreEqual<Json>(json0.At(0), json0.At(L"/0"_JsonPointer));
      Assert::AreEqual<Json>(json0.At(1), json0.At(L"/1"_JsonPointer));
      Assert::AreEqual<Json>(json0.At(2), json0.At(L"/2"_JsonPointer));
      Assert::AreEqual<Json>(json0.At(3), json0.At(L"/3"_JsonPointer));

      auto const json1 = Json{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual<Json>(json1                          , json1.At(L""_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Null")              , json1.At(L"/Null"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"String")            , json1.At(L"/String"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Boolean")           , json1.At(L"/Boolean"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Real")              , json1.At(L"/Real"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Integer")           , json1.At(L"/Integer"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Object")            , json1.At(L"/Object"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key1"), json1.At(L"/Object/Key1"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key2"), json1.At(L"/Object/Key2"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array")             , json1.At(L"/Array"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array").At(0)       , json1.At(L"/Array/0"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array").At(1)       , json1.At(L"/Array/1"_JsonPointer));
      Assert::AreEqual<Json>(json1.At(L"Array").At(2)       , json1.At(L"/Array/2"_JsonPointer));

      auto const json2 = Json{
        { L"foo"s   , { L"bar"s, L"baz"s } } ,
        { L""s      , 0 },
        { L"a/b"s   , 1 },
        { L"c%d"s   , 2 },
        { L"e^f"s   , 3 },
        { L"g|h"s   , 4 },
        { L"i\\j"s  , 5},
        { L"k\"l"s  , 6},
        { L" "s     , 7 },
        { L"m~n"s   , 8 }
      };
      Assert::AreEqual<Json>(json2                  , json2.At(L""_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"foo"s)      , json2.At(L"/foo"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"foo"s).At(0), json2.At(L"/foo/0"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L""s)         , json2.At(L"/"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"a/b"s)      , json2.At(L"/a~1b"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"c%d"s)      , json2.At(L"/c%d"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"e^f"s)      , json2.At(L"/e^f"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"g|h"s)      , json2.At(L"/g|h"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"i\\j"s)     , json2.At(L"/i\\j"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"k\"l"s)     , json2.At(L"/k\"l"_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L" "s)        , json2.At(L"/ "_JsonPointer));
      Assert::AreEqual<Json>(json2.At(L"m~n"s)      , json2.At(L"/m~0n"_JsonPointer));

      ExpectException<exception>([&]() { auto const temp = json2.At(L"/Non/Existent"_JsonPointer); }      , "Reference token \"Non\" at path \"/Non\" not found!");
      ExpectException<exception>([&]() { auto const temp = json1.At(L"/Object/NonExistent"_JsonPointer); }, "Reference token \"NonExistent\" at path \"/Object/NonExistent\" not found!");
      ExpectException<exception>([&]() { auto const temp = json2.At(L"/foo/-"_JsonPointer); }             , "Reference token \"-\" at path \"/foo/-\" points to the member after the last array element which does not exist!");
      ExpectException<exception>([&]() { auto const temp = json2.At(L"/foo/00"_JsonPointer); }            , "Reference token \"00\" at path \"/foo/00\" is not an array index!");
      ExpectException<exception>([&]() { auto const temp = json2.At(L"/foo/01"_JsonPointer); }            , "Reference token \"01\" at path \"/foo/01\" is not an array index!");
      ExpectException<exception>([&]() { auto const temp = json2.At(L"/foo/-1"_JsonPointer); }            , "Reference token \"-1\" at path \"/foo/-1\" is not an array index!");
      ExpectException<exception>([&]() { auto const temp = json2.At(L"/foo/1a"_JsonPointer); }            , "Reference token \"1a\" at path \"/foo/1a\" is not an array index!");
      ExpectException<exception>([&]() { auto const temp = json2.At(L"/foo/3"_JsonPointer); }             , "Reference token \"3\" at path \"/foo/3\" is out of range!");

      static_assert(is_const<remove_reference<decltype(json0.At(L"/0"_JsonPointer))>::type>::value, "JsonObject::At(JsonPointer) return type must be Json const&");
    }

    TEST_METHOD(TestAtWstring)
    {
      ExpectException<exception>([]() { Json(nullptr_t()).At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(wstring  ()).At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(bool     ()).At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(double   ()).At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(int64_t  ()).At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(JsonArray()).At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(nullptr_t()).At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(wstring  ()).At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(bool     ()).At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(double   ()).At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(int64_t  ()).At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { Json(JsonArray()).At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");

      auto json = Json{
        { L"Key1"s, 1 },
        { L"Key2"s, 2 }
      };
      Assert::AreEqual<Json>(1, json.At(L"Key1"s));
      Assert::AreEqual<Json>(2, json.At(L"Key2"s));
      json.At(L"Key1"s) = 1337;
      json.At(L"Key2"s) = 31337;
      Assert::AreEqual<Json>(1337, json.At(L"Key1"s));
      Assert::AreEqual<Json>(31337, json.At(L"Key2"s));
      Assert::AreEqual<Json>(1337, json.At(L"Key1"));
      Assert::AreEqual<Json>(31337, json.At(L"Key2"));
      json.At(L"Key1") = 1;
      json.At(L"Key2") = 2;
      Assert::AreEqual<Json>(1, json.At(L"Key1"));
      Assert::AreEqual<Json>(2, json.At(L"Key2"));
    }

    TEST_METHOD(TestAtWstringConst)
    {
      ExpectException<exception>([]() { auto const temp = Json(nullptr_t()); auto tmp = temp.At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(wstring  ()); auto tmp = temp.At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(bool     ()); auto tmp = temp.At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(double   ()); auto tmp = temp.At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(int64_t  ()); auto tmp = temp.At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(JsonArray()); auto tmp = temp.At(L""      ); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(nullptr_t()); auto tmp = temp.At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(wstring  ()); auto tmp = temp.At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(bool     ()); auto tmp = temp.At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(double   ()); auto tmp = temp.At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(int64_t  ()); auto tmp = temp.At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");
      ExpectException<exception>([]() { auto const temp = Json(JsonArray()); auto tmp = temp.At(wstring()); }, "At(wstring const& key) is only defined for JsonObject!");

      auto const json = Json{
        { L"Key1"s, 1 },
        { L"Key2"s, 2 }
      };
      Assert::AreEqual<Json>(1, json.At(L"Key1"s));
      Assert::AreEqual<Json>(2, json.At(L"Key2"s));
      Assert::AreEqual<Json>(1, json.At(L"Key1"));
      Assert::AreEqual<Json>(2, json.At(L"Key2"));
      static_assert(is_const<remove_reference<decltype(json.At(L"Key1"s))>::type>::value, "JsonObject::At(wstring) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(L"Key1" ))>::type>::value, "JsonObject::At(wstring) return type must be Json const&");
    }

    TEST_METHOD(TestAtInteger)
    {
      auto jsons = vector<Json>{ Json(nullptr_t()), Json(wstring()), Json(bool()), Json(double()), Json(int64_t()), Json(JsonObject()) };
      for (auto& json : jsons)
      {
        ExpectException<exception>([&]() { json.At(int8_t  ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint8_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(int16_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint16_t()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(int32_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint32_t()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(int64_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      }

      auto json = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(1, json.At(0));
      Assert::AreEqual<Json>(3, json.At(1));
      Assert::AreEqual<Json>(3, json.At(2));
      Assert::AreEqual<Json>(7, json.At(3));
      json.At(0) = 1337;
      json.At(1) = 1338;
      json.At(2) = 1339;
      json.At(3) = 1340;
      Assert::AreEqual<Json>(1337, json.At(0));
      Assert::AreEqual<Json>(1338, json.At(1));
      Assert::AreEqual<Json>(1339, json.At(2));
      Assert::AreEqual<Json>(1340, json.At(3));
      Assert::AreEqual<Json>(1340, json.At(3i8));
      Assert::AreEqual<Json>(1340, json.At(3ui8));
      Assert::AreEqual<Json>(1340, json.At(3i16));
      Assert::AreEqual<Json>(1340, json.At(3ui16));
      Assert::AreEqual<Json>(1340, json.At(3i32));
      Assert::AreEqual<Json>(1340, json.At(3ui32));
      Assert::AreEqual<Json>(1340, json.At(3i64));
      Assert::AreEqual<Json>(1340, json.At(3ui64));

      Assert::ExpectException<out_of_range>([&]() { auto result = json.At(-1); });
      Assert::ExpectException<out_of_range>([&]() { auto result = json.At( 4); });
    }

    TEST_METHOD(TestAtIntegerConst)
    {
      auto const jsons = vector<Json>{ Json(nullptr_t()), Json(wstring()), Json(bool()), Json(double()), Json(int64_t()), Json(JsonObject()) };
      for (auto const& json : jsons)
      {
        ExpectException<exception>([&]() { json.At(int8_t  ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint8_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(int16_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint16_t()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(int32_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint32_t()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(int64_t ()); }, "At(int64_t index) is only defined for JsonArray!");
        ExpectException<exception>([&]() { json.At(uint64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      }

      auto const json = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(1, json.At(0));
      Assert::AreEqual<Json>(3, json.At(1));
      Assert::AreEqual<Json>(3, json.At(2));
      Assert::AreEqual<Json>(7, json.At(3));
      Assert::AreEqual<Json>(7, json.At(3i8));
      Assert::AreEqual<Json>(7, json.At(3ui8));
      Assert::AreEqual<Json>(7, json.At(3i16));
      Assert::AreEqual<Json>(7, json.At(3ui16));
      Assert::AreEqual<Json>(7, json.At(3i32));
      Assert::AreEqual<Json>(7, json.At(3ui32));
      Assert::AreEqual<Json>(7, json.At(3i64));
      Assert::AreEqual<Json>(7, json.At(3ui64));
      static_assert(is_const<remove_reference<decltype(json.At(0    ))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0i8  ))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0ui8 ))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0i16 ))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0ui16))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0i32 ))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0ui32))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0i64 ))>::type>::value, "JsonArray::At(index) return type must be Json const&");
      static_assert(is_const<remove_reference<decltype(json.At(0ui64))>::type>::value, "JsonArray::At(index) return type must be Json const&");

      Assert::ExpectException<out_of_range>([&]() { auto result = json.At(-1); });
      Assert::ExpectException<out_of_range>([&]() { auto result = json.At( 4); });
    }

    TEST_METHOD(TestOperatorConversionNullptr)
    {
      ExpectException<exception>([]() { auto temp = (nullptr_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Null'!");
      ExpectException<exception>([]() { auto temp = (nullptr_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Null'!");
      ExpectException<exception>([]() { auto temp = (nullptr_t)Json(double    ()); }, "Invalid conversion: Cannot convert type 'Real' to 'Null'!");
      ExpectException<exception>([]() { auto temp = (nullptr_t)Json(int64_t   ()); }, "Invalid conversion: Cannot convert type 'Integer' to 'Null'!");
      ExpectException<exception>([]() { auto temp = (nullptr_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Null'!");
      ExpectException<exception>([]() { auto temp = (nullptr_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Null'!");
      Assert::AreEqual(nullptr, (nullptr_t)Json(nullptr_t()));
    }

    TEST_METHOD(TestOperatorConversionWstring)
    {
      ExpectException<exception>([]() { auto temp = (wstring)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'String'!");
      ExpectException<exception>([]() { auto temp = (wstring)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'String'!");
      ExpectException<exception>([]() { auto temp = (wstring)Json(double    ()); }, "Invalid conversion: Cannot convert type 'Real' to 'String'!");
      ExpectException<exception>([]() { auto temp = (wstring)Json(int64_t   ()); }, "Invalid conversion: Cannot convert type 'Integer' to 'String'!");
      ExpectException<exception>([]() { auto temp = (wstring)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'String'!");
      ExpectException<exception>([]() { auto temp = (wstring)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'String'!");
      Assert::AreEqual<wstring>(L"Test"s, (wstring)Json(L"Test"s));

      auto json = Json(L"Test"s);
      auto value = (wstring)json;
      Assert::AreEqual<Json>(L"Test"s, json);
      Assert::AreEqual<wstring>(L"Test"s, value);
      value = L"asd"s;
      Assert::AreEqual<Json>(L"Test"s, json);
      Assert::AreEqual<wstring>(L"asd"s, value);
      auto value2 = move((wstring&&)json);
      Assert::AreEqual<Json>(L""s, json);
      Assert::AreEqual<wstring>(L"Test"s, value2);
      value2 = L"asd"s;
      Assert::AreEqual<Json>(L""s, json);
      Assert::AreEqual<wstring>(L"asd"s, value2);
    }

    TEST_METHOD(TestOperatorConversionBool)
    {
      Assert::IsFalse((bool)Json(nullptr_t ()));
      Assert::IsTrue ((bool)Json(L"Test"s    ));
      Assert::IsTrue ((bool)Json(true        ));
      Assert::IsFalse((bool)Json(false       ));
      Assert::IsTrue ((bool)Json(1.0         ));
      Assert::IsFalse((bool)Json(0.0         ));
      Assert::IsTrue ((bool)Json(1i64        ));
      Assert::IsFalse((bool)Json(0i64        ));
      Assert::IsTrue ((bool)Json(JsonObject()));
      Assert::IsTrue ((bool)Json(JsonArray ()));
    }

    TEST_METHOD(TestOperatorConversionChar)
    {
      ExpectException<exception>([]() { auto temp = (char)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (char)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (char)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (char)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (char)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<char>('A', (char)Json(65.0));
      Assert::AreEqual<char>('A', (char)Json('A'));
    }

    TEST_METHOD(TestOperatorConversionInt8)
    {
      ExpectException<exception>([]() { auto temp = (int8_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int8_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int8_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int8_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int8_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<int8_t>(-100, (int8_t)Json(-100.0));
      Assert::AreEqual<int8_t>(-100, (int8_t)Json(-100));
    }

    TEST_METHOD(TestOperatorConversionUInt8)
    {
      ExpectException<exception>([]() { auto temp = (uint8_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint8_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint8_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint8_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint8_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<uint8_t>(200, (uint8_t)Json(200.0));
      Assert::AreEqual<uint8_t>(200, (uint8_t)Json(200));
    }

    TEST_METHOD(TestOperatorConversionInt16)
    {
      ExpectException<exception>([]() { auto temp = (int16_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int16_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int16_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int16_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int16_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<int16_t>(-30'000, (int16_t)Json(-30'000.0));
      Assert::AreEqual<int16_t>(-30'000, (int16_t)Json(-30'000));
    }

    TEST_METHOD(TestOperatorConversionUInt16)
    {
      ExpectException<exception>([]() { auto temp = (uint16_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint16_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint16_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint16_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint16_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<uint16_t>(60'000, (uint16_t)Json(60'000.0));
      Assert::AreEqual<uint16_t>(60'000, (uint16_t)Json(60'000));
    }

    TEST_METHOD(TestOperatorConversionInt32)
    {
      ExpectException<exception>([]() { auto temp = (int32_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int32_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int32_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int32_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int32_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<int32_t>(-2'000'000'000, (int32_t)Json(-2'000'000'000.0));
      Assert::AreEqual<int32_t>(-2'000'000'000, (int32_t)Json(-2'000'000'000));
    }

    TEST_METHOD(TestOperatorConversionUInt32)
    {
      ExpectException<exception>([]() { auto temp = (uint32_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint32_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint32_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint32_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint32_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<uint32_t>(4'000'000'000, (uint32_t)Json(4'000'000'000.0));
      Assert::AreEqual<uint32_t>(4'000'000'000, (uint32_t)Json(4'000'000'000ui32));
    }

    TEST_METHOD(TestOperatorConversionInt64)
    {
      ExpectException<exception>([]() { auto temp = (int64_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int64_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int64_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int64_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (int64_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<int64_t>(-16'000'000'000, (int64_t)Json(-16'000'000'000.0));
      Assert::AreEqual<int64_t>(-16'000'000'000, (int64_t)Json(-16'000'000'000i64));
    }

    TEST_METHOD(TestOperatorConversionUInt64)
    {
      ExpectException<exception>([]() { auto temp = (uint64_t)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint64_t)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint64_t)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint64_t)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Integer'!");
      ExpectException<exception>([]() { auto temp = (uint64_t)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Integer'!");
      Assert::AreEqual<uint64_t>(32'000'000'000, (uint64_t)Json(32'000'000'000.0));
      Assert::AreEqual<uint64_t>(32'000'000'000, (uint64_t)Json(32'000'000'000ui64));
    }

    TEST_METHOD(TestOperatorConversionFloat)
    {
      ExpectException<exception>([]() { auto temp = (float)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (float)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (float)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (float)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (float)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Real'!");
      Assert::AreEqual<float>(13.37f, (float)Json(13.37));
      Assert::AreEqual<float>(13.37f, (float)Json(13.37f));
      Assert::AreEqual<float>(1337.0, (float)Json(1337));
    }

    TEST_METHOD(TestOperatorConversionDouble)
    {
      ExpectException<exception>([]() { auto temp = (double)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (double)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (double)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (double)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Real'!");
      ExpectException<exception>([]() { auto temp = (double)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Real'!");
      Assert::AreEqual<double>(13.37, (double)Json(13.37));
      Assert::AreEqual<double>(13.37f, (double)Json(13.37f));
      Assert::AreEqual<double>(1337.0, (double)Json(1337));
    }

    TEST_METHOD(TestOperatorConversionJsonObject)
    {
      ExpectException<exception>([]() { auto temp = (JsonObject)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Object'!");
      ExpectException<exception>([]() { auto temp = (JsonObject)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Object'!");
      ExpectException<exception>([]() { auto temp = (JsonObject)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Object'!");
      ExpectException<exception>([]() { auto temp = (JsonObject)Json(double    ()); }, "Invalid conversion: Cannot convert type 'Real' to 'Object'!");
      ExpectException<exception>([]() { auto temp = (JsonObject)Json(int64_t   ()); }, "Invalid conversion: Cannot convert type 'Integer' to 'Object'!");
      ExpectException<exception>([]() { auto temp = (JsonObject)Json(JsonArray ()); }, "Invalid conversion: Cannot convert type 'Array' to 'Object'!");
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, (JsonObject)Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }));

      auto json = Json{ { L"Key1"s, 1 }, { L"Key2"s, 2 } };
      Assert::AreEqual<Json>({ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json);
      auto value = move((JsonObject&&)json);
      Assert::AreEqual<Json>(JsonObject(), json);
      Assert::AreEqual<JsonObject>({ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, value);
    }

    TEST_METHOD(TestOperatorConversionJsonArray)
    {
      ExpectException<exception>([]() { auto temp = (JsonArray)Json(nullptr_t ()); }, "Invalid conversion: Cannot convert type 'Null' to 'Array'!");
      ExpectException<exception>([]() { auto temp = (JsonArray)Json(wstring   ()); }, "Invalid conversion: Cannot convert type 'String' to 'Array'!");
      ExpectException<exception>([]() { auto temp = (JsonArray)Json(bool      ()); }, "Invalid conversion: Cannot convert type 'Boolean' to 'Array'!");
      ExpectException<exception>([]() { auto temp = (JsonArray)Json(double    ()); }, "Invalid conversion: Cannot convert type 'Real' to 'Array'!");
      ExpectException<exception>([]() { auto temp = (JsonArray)Json(int64_t   ()); }, "Invalid conversion: Cannot convert type 'Integer' to 'Array'!");
      ExpectException<exception>([]() { auto temp = (JsonArray)Json(JsonObject()); }, "Invalid conversion: Cannot convert type 'Object' to 'Array'!");
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 3, 3, 7 }, (JsonArray)Json(JsonArray{ 1, 3, 3, 7 }));

      auto json = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
      auto value = move((JsonArray&&)json);
      Assert::AreEqual<Json>(JsonArray(), json);
      Assert::AreEqual<JsonArray>({ 1, 3, 3, 7 }, value);
    }

    TEST_METHOD(TestOperatorAssignmentNullptr)
    {
      auto json = Json(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      json = nullptr;
      Assert::AreEqual(nullptr, json.Get<nullptr_t>());
    }

    TEST_METHOD(TestOperatorAssignmentConstWcharPointer)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = L"Test";
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
    }

    TEST_METHOD(TestOperatorAssignmentWstring)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = L"Test"s;
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
      auto wstr = L"asd"s;
      json = move(wstr);
      Assert::AreEqual(L"asd"s, json.Get<wstring>());
      Assert::AreEqual(L""s, wstr);
    }

    TEST_METHOD(TestOperatorAssignmentBool)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = true;
      Assert::AreEqual(true, json.Get<bool>());
    }

    TEST_METHOD(TestOperatorAssignmentChar)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = 'A';
      Assert::AreEqual(65i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt8)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int8_t)-100;
      Assert::AreEqual(-100i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt8)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint8_t)200;
      Assert::AreEqual(200i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt16)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int16_t)-30'000;
      Assert::AreEqual(-30'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt16)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint16_t)60'000;
      Assert::AreEqual(60'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt32)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int32_t)-2'000'000'000;
      Assert::AreEqual(-2'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt32)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint32_t)4'000'000'000;
      Assert::AreEqual(4'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt64)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int64_t)-16'000'000'000;
      Assert::AreEqual(-16'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt64)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint64_t)32'000'000'000;
      Assert::AreEqual(32'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentFloat)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = 13.37f;
      Assert::AreEqual<double>(13.37f, json.Get<double>());
    }

    TEST_METHOD(TestOperatorAssignmentDouble)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = 13.37;
      Assert::AreEqual(13.37, json.Get<double>());
    }

    TEST_METHOD(TestOperatorAssignmentJson)
    {
      auto json = Json(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      auto value = Json(nullptr);
      json = value;
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::AreEqual(nullptr, json.Get<nullptr_t>());
      Assert::AreEqual<Json>(nullptr, value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::AreEqual(nullptr, json.Get<nullptr_t>());
      Assert::AreEqual<Json>(nullptr, value);

      value = Json(L"Test"s);
      json = value;
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
      Assert::AreEqual<Json>(L"Test"s, value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
      Assert::AreEqual<Json>(L""s, value);

      value = Json(true);
      json = value;
      Assert::IsTrue(json.Is(JsonType::Boolean));
      Assert::AreEqual(true, json.Get<bool>());
      Assert::AreEqual<Json>(true, value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Boolean));
      Assert::AreEqual(true, json.Get<bool>());
      Assert::AreEqual<Json>(true, value);

      value = Json(13.37);
      json = value;
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::AreEqual(13.37, json.Get<double>());
      Assert::AreEqual<Json>(13.37, value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::AreEqual(13.37, json.Get<double>());
      Assert::AreEqual<Json>(13.37, value);

      value = Json(1337);
      json = value;
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::AreEqual(1337i64, json.Get<int64_t>());
      Assert::AreEqual<Json>(1337i64, value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::AreEqual(1337i64, json.Get<int64_t>());
      Assert::AreEqual<Json>(1337i64, value);

      value = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      json = value;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());
      Assert::AreEqual<Json>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());
      Assert::AreEqual<Json>(JsonObject(), value);

      value = Json(JsonArray{ 1, 3, 3, 7 });
      json = value;
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
      Assert::AreEqual<Json>(JsonArray{ 1, 3, 3, 7 }, value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
      Assert::AreEqual<Json>(JsonArray(), value);
    }

    TEST_METHOD(TestOperatorAssignmentJsonObject)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      auto value = JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } };
      json = value;
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());
      Assert::AreEqual<Json>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, value);
      json = move(value);
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());
      Assert::AreEqual<Json>(JsonObject(), value);
    }

    TEST_METHOD(TestOperatorAssignmentJsonArray)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      auto value = JsonArray{ 1, 3, 3, 7 };
      json = value;
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
      Assert::AreEqual<Json>(JsonArray{ 1, 3, 3, 7 }, value);
      json = move(value);
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
      Assert::AreEqual<Json>(JsonArray(), value);
    }

    TEST_METHOD(TestOperatorAssignmentJsonBuilder)
    {
      auto json = Json(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      json = JsonBuilder(nullptr);
      Assert::AreEqual(nullptr, json.Get<nullptr_t>());

      auto value = JsonBuilder(L"Test"s);
      json = value;
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
      Assert::AreEqual<JsonBuilder>(JsonBuilder(L"Test"s), value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
      Assert::AreEqual<JsonBuilder>(JsonBuilder(L""s), value);

      json = JsonBuilder(true);
      Assert::IsTrue(json.Is(JsonType::Boolean));
      Assert::AreEqual(true, json.Get<bool>());

      json = JsonBuilder(13.37);
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::AreEqual(13.37, json.Get<double>());

      json = JsonBuilder(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::AreEqual(1337i64, json.Get<int64_t>());

      value = JsonBuilder(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      json = value;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());
      Assert::AreEqual<JsonBuilder>(JsonBuilder(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }), value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());
      Assert::AreEqual<JsonBuilder>(JsonBuilder(JsonObject()), value);

      value = JsonBuilder(JsonArray{ 1, 3, 3, 7 });
      json = value;
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
      Assert::AreEqual<JsonBuilder>(JsonBuilder(JsonArray{ 1, 3, 3, 7 }), value);
      json = move(value);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
      Assert::AreEqual<JsonBuilder>(JsonBuilder(JsonArray()), value);
    }

    TEST_METHOD(TestOperatorAssignmentInitializerList)
    {
      auto json = Json(initializer_list<JsonBuilder>());
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(0i64, json.Size());

      json = Json(initializer_list<JsonBuilder>{ nullptr });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(nullptr, json[0]);

      json = Json(initializer_list<JsonBuilder>{ L"Test"s });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(L"Test"s, json[0]);

      json = Json(initializer_list<JsonBuilder>{ true });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(true, json[0]);

      json = Json(initializer_list<JsonBuilder>{ 13.37 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(13.37, json[0]);

      json = Json(initializer_list<JsonBuilder>{ 1337 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);

      json = Json(initializer_list<JsonBuilder>{ JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } } });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json[0]);

      json = Json(initializer_list<JsonBuilder>{ JsonArray{ 1, 3, 3, 7 } });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(JsonArray{ 1, 3, 3, 7 }, json[0]);

      json = Json(initializer_list<JsonBuilder>{ 1, 2 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(2, json[1]);

      json = Json(initializer_list<JsonBuilder>{ L"Key1"s, 1 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json[0]);
      Assert::AreEqual<Json>(1, json[1]);

      json = Json(initializer_list<JsonBuilder>{ L"Key1"s, 1, L"Key2"s });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json[0]);
      Assert::AreEqual<Json>(1, json[1]);
      Assert::AreEqual<Json>(L"Key2"s, json[2]);

      json = Json(initializer_list<JsonBuilder>{ L"Key1"s, 1, L"Key2"s, 2 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(4i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json[0]);
      Assert::AreEqual<Json>(1, json[1]);
      Assert::AreEqual<Json>(L"Key2"s, json[2]);
      Assert::AreEqual<Json>(2, json[3]);

      json = Json(initializer_list<JsonBuilder>{ initializer_list<JsonBuilder>{ L"Key1"s, 1 }, initializer_list<JsonBuilder>{ L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[L"Key1"s]);
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
    }

    TEST_METHOD(TestOperatorInsertion)
    {
      auto pairs = vector<pair<vector<variant<Json, JsonIndentSize, JsonIndentChar>>, wstring>>
      {
        { { Json(nullptr ) }, L"null"s     },
        { { Json(L"Test"s) }, L"\"Test\""s },
        { { Json(true    ) }, L"true"s     },
        { { Json(false   ) }, L"false"s    },
        { { Json(13.37   ) }, L"13.37"s    },
        { { Json(1337    ) }, L"1337"s     },
        { { Json{ { L"Key1"s, 1 }, { L"Key2"s, 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}"s },
        { { Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]"s },
        { { Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentSize(1), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n 1,\r\n 3,\r\n 3,\r\n 7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentChar(L'\t'), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t\t1,\r\n\t\t3,\r\n\t\t3,\r\n\t\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentSize(1), JsonIndentChar(L'\t'), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t1,\r\n\t3,\r\n\t3,\r\n\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentChar(L'\t'), JsonIndentSize(1), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
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
      auto pairs = vector<pair<wstring, Json>>
      {
        { L"[null]"s    , Json{nullptr } },
        { L"[\"Test\"]"s, Json{L"Test"s} },
        { L"[true]"s    , Json{true    } },
        { L"[false]"s   , Json{false   } },
        { L"[13.37]"s   , Json{13.37   } },
        { L"[1337]"s    , Json{1337    } },
        { L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}"s, Json{ { L"Key1"s, 1 }, { L"Key2"s, 2 } } },
        { L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]"s, Json{ 1, 3, 3, 7 } }
      };
      for (auto& [input, expected] : pairs)
      {
        wstringstream is;
        is.str(input);
        Json output;
        is >> output;
        Assert::AreEqual(expected, output);
      }
    }

    //http://json.org/JSON_checker/
    TEST_METHOD(TestFail)
    {
      auto pairs = vector<pair<string, string>>
      {
        { "fail01.json"s, "Invalid token: String, with invalid data: \"A JSON payload should be an object or array, not a string.\"!"s },
        { "fail02.json"s, "Expected ',' or ']' at position Line: 1 Column: 18!"s },
        { "fail03.json"s, "Expected '\"' at position Line: 1 Column: 2!"s },
        { "fail04.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 16!"s },
        { "fail05.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 23!"s },
        { "fail06.json"s, "Unexpected ',' at position Line: 1 Column: 5!"s },
        { "fail07.json"s, "Unexpected ',' at position Line: 1 Column: 26!"s },
        { "fail08.json"s, "Unexpected ']' at position Line: 1 Column: 16!"s },
        { "fail09.json"s, "Expected '\"' at position Line: 1 Column: 22!"s },
        { "fail10.json"s, "Unexpected '\"' at position Line: 1 Column: 35!"s },
        { "fail11.json"s, "Expected ',' or '}' at position Line: 1 Column: 25!"s },
        { "fail12.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 24!"s },
        { "fail13.json"s, "Unexpected '0' at position Line: 1 Column: 40!"s },
        { "fail14.json"s, "Unexpected '0' at position Line: 1 Column: 27!"s },
        { "fail15.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 30!"s },
        { "fail16.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 2!"s },
        { "fail17.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 30!"s },
        { "fail18.json"s, "Depth is greater or equal to the maximum 20!"s },
        { "fail19.json"s, "Expected ':' at position Line: 1 Column: 18!"s },
        { "fail20.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 17!"s },
        { "fail21.json"s, "Expected ':' at position Line: 1 Column: 26!"s },
        { "fail22.json"s, "Expected ',' or ']' at position Line: 1 Column: 26!"s },
        { "fail23.json"s, "Expected 'true' at position Line: 1 Column: 15!"s },
        { "fail24.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 2!"s },
        { "fail25.json"s, "Invalid character found at position Line: 1 Column: 3!"s },
        { "fail26.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 7!"s },
        { "fail27.json"s, "Invalid character found at position Line: 1 Column: 7!"s },
        { "fail28.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 8!"s },
        { "fail29.json"s, "Expected digit at position Line: 1 Column: 4!"s },
        { "fail30.json"s, "Expected digit at position Line: 1 Column: 5!"s },
        { "fail31.json"s, "Expected digit at position Line: 1 Column: 5!"s },
        { "fail32.json"s, "Expected '\"' at position Line: 1 Column: 41!"s },
        { "fail33.json"s, "Expected ',' or ']' at position Line: 1 Column: 12!"s }
      };
      for (auto& [input, expected] : pairs)
      {
        ExpectException<exception>([&, input = input]() { Json::Read(input); }, expected);
      }
    }

    //http://json.org/JSON_checker/
    TEST_METHOD(TestPass)
    {
      auto input = Json::Read(L"pass01.json");
      
      Assert::IsTrue(input.Is(JsonType::Array));
      Assert::AreEqual(20i64, input.Size());

      Assert::AreEqual<Json>(L"JSON Test Pattern pass1"s, input[0]);

      Assert::IsTrue(input[1].Is(JsonType::Object));
      Assert::AreEqual(1i64, input[1].Size());
      Assert::IsTrue(input[1][L"object with 1 member"].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[1][L"object with 1 member"].Size());
      Assert::AreEqual<Json>(L"array with 1 element", input[1][L"object with 1 member"][0]);

      Assert::IsTrue(input[2].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[2].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[2]);

      Assert::IsTrue(input[3].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[3].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[3]);

      Assert::AreEqual<Json>(-42, input[4]);

      Assert::AreEqual<Json>(true, input[5]);

      Assert::AreEqual<Json>(false, input[6]);

      Assert::AreEqual<Json>(nullptr, input[7]);

      Assert::IsTrue(input[8].Is(JsonType::Object));
      Assert::AreEqual(32i64, input[8].Size());
      Assert::AreEqual<Json>(1234567890, input[8][L"integer"]);
      Assert::AreEqual<Json>(-9876.543210, input[8][L"real"]);
      Assert::AreEqual<Json>(0.123456789e-12, input[8][L"e"]);
      Assert::AreEqual<Json>(1.234567890E+34, input[8][L"E"]);
      Assert::AreEqual<Json>(23456789012E66, input[8][L""]);
      Assert::AreEqual<Json>(0, input[8][L"zero"]);
      Assert::AreEqual<Json>(1, input[8][L"one"]);
      Assert::AreEqual<Json>(L" "s, input[8][L"space"]);
      Assert::AreEqual<Json>(L"\""s, input[8][L"quote"]);
      Assert::AreEqual<Json>(L"\\"s, input[8][L"backslash"]);
      Assert::AreEqual<Json>(L"\b\f\n\r\t"s, input[8][L"controls"]);
      Assert::AreEqual<Json>(L"/ & /"s, input[8][L"slash"]);
      Assert::AreEqual<Json>(L"abcdefghijklmnopqrstuvwyz"s, input[8][L"alpha"]);
      Assert::AreEqual<Json>(L"ABCDEFGHIJKLMNOPQRSTUVWYZ"s, input[8][L"ALPHA"]);
      Assert::AreEqual<Json>(L"0123456789"s, input[8][L"digit"]);
      Assert::AreEqual<Json>(L"digit"s, input[8][L"0123456789"]);
      Assert::AreEqual<Json>(L"`1~!@#$%^&*()_+-={':[,]}|;.</>?"s, input[8][L"special"]);
      Assert::AreEqual<Json>(L"\u0123\u4567\u89AB\uCDEF\uabcd\uef4A"s, input[8][L"hex"]);
      Assert::AreEqual<Json>(true, input[8][L"true"]);
      Assert::AreEqual<Json>(false, input[8][L"false"]);
      Assert::AreEqual<Json>(nullptr, input[8][L"null"]);
      Assert::IsTrue(input[8][L"array"].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[8][L"array"].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[8][L"array"]);
      Assert::IsTrue(input[8][L"object"].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[8][L"object"].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[8][L"object"]);
      Assert::AreEqual<Json>(L"50 St. James Street", input[8][L"address"]);
      Assert::AreEqual<Json>(L"http://www.JSON.org/", input[8][L"url"]);
      Assert::AreEqual<Json>(L"// /* <!-- --", input[8][L"comment"]);
      Assert::AreEqual<Json>(L" ", input[8][L"# -- --> */"]);
      Assert::IsTrue(input[8][L" s p a c e d "].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L" s p a c e d "].Size());
      Assert::AreEqual<Json>(1, input[8][L" s p a c e d "][0]);
      Assert::AreEqual<Json>(2, input[8][L" s p a c e d "][1]);
      Assert::AreEqual<Json>(3, input[8][L" s p a c e d "][2]);
      Assert::AreEqual<Json>(4, input[8][L" s p a c e d "][3]);
      Assert::AreEqual<Json>(5, input[8][L" s p a c e d "][4]);
      Assert::AreEqual<Json>(6, input[8][L" s p a c e d "][5]);
      Assert::AreEqual<Json>(7, input[8][L" s p a c e d "][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L" s p a c e d "]);
      Assert::IsTrue(input[8][L"compact"].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L"compact"].Size());
      Assert::AreEqual<Json>(1, input[8][L"compact"][0]);
      Assert::AreEqual<Json>(2, input[8][L"compact"][1]);
      Assert::AreEqual<Json>(3, input[8][L"compact"][2]);
      Assert::AreEqual<Json>(4, input[8][L"compact"][3]);
      Assert::AreEqual<Json>(5, input[8][L"compact"][4]);
      Assert::AreEqual<Json>(6, input[8][L"compact"][5]);
      Assert::AreEqual<Json>(7, input[8][L"compact"][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L"compact"]);
      Assert::AreEqual<Json>(L"{\"object with 1 member\":[\"array with 1 element\"]}", input[8][L"jsontext"]);
      Assert::AreEqual<Json>(L"&#34; \u0022 %22 0x22 034 &#x22;", input[8][L"quotes"]);
      Assert::AreEqual<Json>(L"A key can be any string", input[8][L"/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"]);

      Assert::AreEqual<Json>(0.5, input[9]);

      Assert::AreEqual<Json>(98.6, input[10]);

      Assert::AreEqual<Json>(99.44, input[11]);

      Assert::AreEqual<Json>(1066, input[12]);

      Assert::AreEqual<Json>(1e1, input[13]);

      Assert::AreEqual<Json>(0.1e1, input[14]);

      Assert::AreEqual<Json>(1e-1, input[15]);

      Assert::AreEqual<Json>(1e00, input[16]);

      Assert::AreEqual<Json>(2e+00, input[17]);

      Assert::AreEqual<Json>(2e-00, input[18]);

      Assert::AreEqual<Json>(L"rosebud", input[19]);


      input = Json::Read(L"pass02.json");
      Assert::IsTrue(        input.Is(JsonType::Array));
      Assert::AreEqual(1i64, input.Size());
      Assert::IsTrue(        input[0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0].Size());
      Assert::IsTrue(        input[0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0].Size());
      Assert::IsTrue(        input[0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(                          input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::String));
      Assert::AreEqual<Json>(L"Not too deep"s, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]);

      input = Json::Read(L"pass03.json");
      Assert::IsTrue(input.Is(JsonType::Object));
      Assert::AreEqual(1i64, input.Size());
      Assert::AreEqual<size_t>(1i64, input.Keys().size());
      Assert::AreEqual(L"JSON Test Pattern pass3"s, input.Keys()[0]);
      Assert::IsTrue(input[L"JSON Test Pattern pass3"s].Is(JsonType::Object));
      Assert::AreEqual(2i64, input[L"JSON Test Pattern pass3"s].Size());
      Assert::AreEqual<size_t>(2i64, input[L"JSON Test Pattern pass3"s].Keys().size());
      Assert::AreEqual(L"The outermost value"s, input[L"JSON Test Pattern pass3"s].Keys()[0]);
      Assert::AreEqual(L"In this test"s, input[L"JSON Test Pattern pass3"s].Keys()[1]);
      Assert::AreEqual<Json>(L"must be an object or array."s, input[L"JSON Test Pattern pass3"s][L"The outermost value"s]);
      Assert::AreEqual<Json>(L"It is an object."s, input[L"JSON Test Pattern pass3"s][L"In this test"s]);
    }

    //http://json.org/JSON_checker/
    TEST_METHOD(TestRoundtrip)
    {
      for (int i = 1; i <= 27; ++i)
      {
        wstringstream ss;
        ss << setw(2) << setfill(L'0') << i;
        auto const index = ss.str();
        auto origPath = L"roundtrip"s + index + L".json"s;
        auto newPath = L"roundtrip"s + index + L"_new.json"s;

        Json::Read(origPath).Write(newPath);
        Assert::AreEqual(Json::Read(origPath), Json::Read(newPath));
      }
    }

    TEST_METHOD(TestOperatorEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

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
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

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
    }

    TEST_METHOD(TestOperatorLessThan)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   < array1  ; });
      Assert::ExpectException<exception>([&] { array1   < array2  ; });
      Assert::ExpectException<exception>([&] { array1   < boolean1; });
      Assert::ExpectException<exception>([&] { array1   < boolean2; });
      Assert::IsTrue (array1   < null    );
      Assert::ExpectException<exception>([&] { array1   < number1 ; });
      Assert::ExpectException<exception>([&] { array1   < number2 ; });
      Assert::ExpectException<exception>([&] { array1   < numberi1; });
      Assert::ExpectException<exception>([&] { array1   < numberi2; });
      Assert::ExpectException<exception>([&] { array1   < object1 ; });
      Assert::ExpectException<exception>([&] { array1   < object2 ; });
      Assert::ExpectException<exception>([&] { array1   < string1 ; });
      Assert::ExpectException<exception>([&] { array1   < string2 ; });
      Assert::ExpectException<exception>([&] { array2   < array1  ; });
      Assert::ExpectException<exception>([&] { array2   < array2  ; });
      Assert::ExpectException<exception>([&] { array2   < boolean1; });
      Assert::ExpectException<exception>([&] { array2   < boolean2; });
      Assert::IsTrue (array2   < null    );
      Assert::ExpectException<exception>([&] { array2   < number1 ; });
      Assert::ExpectException<exception>([&] { array2   < number2 ; });
      Assert::ExpectException<exception>([&] { array2   < numberi1; });
      Assert::ExpectException<exception>([&] { array2   < numberi2; });
      Assert::ExpectException<exception>([&] { array2   < object1 ; });
      Assert::ExpectException<exception>([&] { array2   < object2 ; });
      Assert::ExpectException<exception>([&] { array2   < string1 ; });
      Assert::ExpectException<exception>([&] { array2   < string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 < array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 < array2  ; });
      Assert::IsFalse(boolean1 < boolean1);
      Assert::IsTrue (boolean1 < boolean2);
      Assert::IsTrue (boolean1 < null    );
      Assert::IsFalse(boolean1 < number1 );
      Assert::IsTrue (boolean1 < number2 );
      Assert::IsFalse(boolean1 < numberi1);
      Assert::IsTrue (boolean1 < numberi2);
      Assert::ExpectException<exception>([&] { boolean1 < object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 < object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 < string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 < string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 < array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 < array2  ; });
      Assert::IsFalse(boolean2 < boolean1);
      Assert::IsFalse(boolean2 < boolean2);
      Assert::IsTrue (boolean2 < null    );
      Assert::IsFalse(boolean2 < number1 );
      Assert::IsFalse(boolean2 < number2 );
      Assert::IsFalse(boolean2 < numberi1);
      Assert::IsFalse(boolean2 < numberi2);
      Assert::ExpectException<exception>([&] { boolean2 < object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 < object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 < string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 < string2 ; });
      Assert::IsFalse(null     < array1  );
      Assert::IsFalse(null     < array2  );
      Assert::IsFalse(null     < boolean1);
      Assert::IsFalse(null     < boolean2);
      Assert::IsFalse(null     < null    );
      Assert::IsFalse(null     < number1 );
      Assert::IsFalse(null     < number2 );
      Assert::IsFalse(null     < numberi1);
      Assert::IsFalse(null     < numberi2);
      Assert::IsFalse(null     < object1 );
      Assert::IsFalse(null     < object2 );
      Assert::IsFalse(null     < string1 );
      Assert::IsFalse(null     < string2 );
      Assert::ExpectException<exception>([&] { number1  < array1  ; });
      Assert::ExpectException<exception>([&] { number1  < array2  ; });
      Assert::IsFalse(number1  < boolean1);
      Assert::IsTrue (number1  < boolean2);
      Assert::IsTrue (number1  < null    );
      Assert::IsFalse(number1  < number1 );
      Assert::IsTrue (number1  < number2 );
      Assert::IsFalse(number1  < numberi1);
      Assert::IsTrue (number1  < numberi2);
      Assert::ExpectException<exception>([&] { number1  < object1 ; });
      Assert::ExpectException<exception>([&] { number1  < object2 ; });
      Assert::ExpectException<exception>([&] { number1  < string1 ; });
      Assert::ExpectException<exception>([&] { number1  < string2 ; });
      Assert::ExpectException<exception>([&] { number2  < array1  ; });
      Assert::ExpectException<exception>([&] { number2  < array2  ; });
      Assert::IsFalse(number2  < boolean1);
      Assert::IsFalse(number2  < boolean2);
      Assert::IsTrue (number2  < null    );
      Assert::IsFalse(number2  < number1 );
      Assert::IsFalse(number2  < number2 );
      Assert::IsFalse(number2  < numberi1);
      Assert::IsFalse(number2  < numberi2);
      Assert::ExpectException<exception>([&] { number2  < object1 ; });
      Assert::ExpectException<exception>([&] { number2  < object2 ; });
      Assert::ExpectException<exception>([&] { number2  < string1 ; });
      Assert::ExpectException<exception>([&] { number2  < string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 < array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 < array2  ; });
      Assert::IsFalse(numberi1 < boolean1);
      Assert::IsTrue (numberi1 < boolean2);
      Assert::IsTrue (numberi1 < null    );
      Assert::IsFalse(numberi1 < number1 );
      Assert::IsTrue (numberi1 < number2 );
      Assert::IsFalse(numberi1 < numberi1);
      Assert::IsTrue (numberi1 < numberi2);
      Assert::ExpectException<exception>([&] { numberi1 < object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 < object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 < string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 < string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 < array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 < array2  ; });
      Assert::IsFalse(numberi2 < boolean1);
      Assert::IsFalse(numberi2 < boolean2);
      Assert::IsTrue (numberi2 < null    );
      Assert::IsFalse(numberi2 < number1 );
      Assert::IsFalse(numberi2 < number2 );
      Assert::IsFalse(numberi2 < numberi1);
      Assert::IsFalse(numberi2 < numberi2);
      Assert::ExpectException<exception>([&] { numberi2 < object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 < object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 < string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 < string2 ; });
      Assert::ExpectException<exception>([&] { object1  < array1  ; });
      Assert::ExpectException<exception>([&] { object1  < array2  ; });
      Assert::ExpectException<exception>([&] { object1  < boolean1; });
      Assert::ExpectException<exception>([&] { object1  < boolean2; });
      Assert::IsTrue (object1  < null    );
      Assert::ExpectException<exception>([&] { object1  < number1 ; });
      Assert::ExpectException<exception>([&] { object1  < number2 ; });
      Assert::ExpectException<exception>([&] { object1  < numberi1; });
      Assert::ExpectException<exception>([&] { object1  < numberi2; });
      Assert::ExpectException<exception>([&] { object1  < object1 ; });
      Assert::ExpectException<exception>([&] { object1  < object2 ; });
      Assert::ExpectException<exception>([&] { object1  < string1 ; });
      Assert::ExpectException<exception>([&] { object1  < string2 ; });
      Assert::ExpectException<exception>([&] { object2  < array1  ; });
      Assert::ExpectException<exception>([&] { object2  < array2  ; });
      Assert::ExpectException<exception>([&] { object2  < boolean1; });
      Assert::ExpectException<exception>([&] { object2  < boolean2; });
      Assert::IsTrue (object2  < null    );
      Assert::ExpectException<exception>([&] { object2  < number1 ; });
      Assert::ExpectException<exception>([&] { object2  < number2 ; });
      Assert::ExpectException<exception>([&] { object2  < numberi1; });
      Assert::ExpectException<exception>([&] { object2  < numberi2; });
      Assert::ExpectException<exception>([&] { object2  < object1 ; });
      Assert::ExpectException<exception>([&] { object2  < object2 ; });
      Assert::ExpectException<exception>([&] { object2  < string1 ; });
      Assert::ExpectException<exception>([&] { object2  < string2 ; });
      Assert::ExpectException<exception>([&] { string1  < array1  ; });
      Assert::ExpectException<exception>([&] { string1  < array2  ; });
      Assert::ExpectException<exception>([&] { string1  < boolean1; });
      Assert::ExpectException<exception>([&] { string1  < boolean2; });
      Assert::IsTrue (string1  < null    );
      Assert::ExpectException<exception>([&] { string1  < number1 ; });
      Assert::ExpectException<exception>([&] { string1  < number2 ; });
      Assert::ExpectException<exception>([&] { string1  < numberi1; });
      Assert::ExpectException<exception>([&] { string1  < numberi2; });
      Assert::ExpectException<exception>([&] { string1  < object1 ; });
      Assert::ExpectException<exception>([&] { string1  < object2 ; });
      Assert::IsFalse(string1  < string1 );
      Assert::IsTrue (string1  < string2 );
      Assert::ExpectException<exception>([&] { string2  < array1  ; });
      Assert::ExpectException<exception>([&] { string2  < array2  ; });
      Assert::ExpectException<exception>([&] { string2  < boolean1; });
      Assert::ExpectException<exception>([&] { string2  < boolean2; });
      Assert::IsTrue (string2  < null    );
      Assert::ExpectException<exception>([&] { string2  < number1 ; });
      Assert::ExpectException<exception>([&] { string2  < number2 ; });
      Assert::ExpectException<exception>([&] { string2  < numberi1; });
      Assert::ExpectException<exception>([&] { string2  < numberi2; });
      Assert::ExpectException<exception>([&] { string2  < object1 ; });
      Assert::ExpectException<exception>([&] { string2  < object2 ; });
      Assert::IsFalse(string2  < string1 );
      Assert::IsFalse(string2  < string2 );
    }

    TEST_METHOD(TestOperatorLessThanOrEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   <= array1  ; });
      Assert::ExpectException<exception>([&] { array1   <= array2  ; });
      Assert::ExpectException<exception>([&] { array1   <= boolean1; });
      Assert::ExpectException<exception>([&] { array1   <= boolean2; });
      Assert::IsTrue (array1   <= null    );
      Assert::ExpectException<exception>([&] { array1   <= number1 ; });
      Assert::ExpectException<exception>([&] { array1   <= number2 ; });
      Assert::ExpectException<exception>([&] { array1   <= numberi1; });
      Assert::ExpectException<exception>([&] { array1   <= numberi2; });
      Assert::ExpectException<exception>([&] { array1   <= object1 ; });
      Assert::ExpectException<exception>([&] { array1   <= object2 ; });
      Assert::ExpectException<exception>([&] { array1   <= string1 ; });
      Assert::ExpectException<exception>([&] { array1   <= string2 ; });
      Assert::ExpectException<exception>([&] { array2   <= array1  ; });
      Assert::ExpectException<exception>([&] { array2   <= array2  ; });
      Assert::ExpectException<exception>([&] { array2   <= boolean1; });
      Assert::ExpectException<exception>([&] { array2   <= boolean2; });
      Assert::IsTrue (array2   <= null    );
      Assert::ExpectException<exception>([&] { array2   <= number1 ; });
      Assert::ExpectException<exception>([&] { array2   <= number2 ; });
      Assert::ExpectException<exception>([&] { array2   <= numberi1; });
      Assert::ExpectException<exception>([&] { array2   <= numberi2; });
      Assert::ExpectException<exception>([&] { array2   <= object1 ; });
      Assert::ExpectException<exception>([&] { array2   <= object2 ; });
      Assert::ExpectException<exception>([&] { array2   <= string1 ; });
      Assert::ExpectException<exception>([&] { array2   <= string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 <= array2  ; });
      Assert::IsTrue (boolean1 <= boolean1);
      Assert::IsTrue (boolean1 <= boolean2);
      Assert::IsTrue (boolean1 <= null    );
      Assert::IsTrue (boolean1 <= number1 );
      Assert::IsTrue (boolean1 <= number2 );
      Assert::IsTrue (boolean1 <= numberi1);
      Assert::IsTrue (boolean1 <= numberi2);
      Assert::ExpectException<exception>([&] { boolean1 <= object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 <= array2  ; });
      Assert::IsFalse(boolean2 <= boolean1);
      Assert::IsTrue (boolean2 <= boolean2);
      Assert::IsTrue (boolean2 <= null    );
      Assert::IsFalse(boolean2 <= number1 );
      Assert::IsTrue (boolean2 <= number2 );
      Assert::IsFalse(boolean2 <= numberi1);
      Assert::IsTrue (boolean2 <= numberi2);
      Assert::ExpectException<exception>([&] { boolean2 <= object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= string2 ; });
      Assert::IsFalse(null     <= array1  );
      Assert::IsFalse(null     <= array2  );
      Assert::IsFalse(null     <= boolean1);
      Assert::IsFalse(null     <= boolean2);
      Assert::IsTrue (null     <= null    );
      Assert::IsFalse(null     <= number1 );
      Assert::IsFalse(null     <= number2 );
      Assert::IsFalse(null     <= numberi1);
      Assert::IsFalse(null     <= numberi2);
      Assert::IsFalse(null     <= object1 );
      Assert::IsFalse(null     <= object2 );
      Assert::IsFalse(null     <= string1 );
      Assert::IsFalse(null     <= string2 );
      Assert::ExpectException<exception>([&] { number1  <= array1  ; });
      Assert::ExpectException<exception>([&] { number1  <= array2  ; });
      Assert::IsTrue (number1  <= boolean1);
      Assert::IsTrue (number1  <= boolean2);
      Assert::IsTrue (number1  <= null    );
      Assert::IsTrue (number1  <= number1 );
      Assert::IsTrue (number1  <= number2 );
      Assert::IsTrue (number1  <= numberi1);
      Assert::IsTrue (number1  <= numberi2);
      Assert::ExpectException<exception>([&] { number1  <= object1 ; });
      Assert::ExpectException<exception>([&] { number1  <= object2 ; });
      Assert::ExpectException<exception>([&] { number1  <= string1 ; });
      Assert::ExpectException<exception>([&] { number1  <= string2 ; });
      Assert::ExpectException<exception>([&] { number2  <= array1  ; });
      Assert::ExpectException<exception>([&] { number2  <= array2  ; });
      Assert::IsFalse(number2  <= boolean1);
      Assert::IsTrue (number2  <= boolean2);
      Assert::IsTrue (number2  <= null    );
      Assert::IsFalse(number2  <= number1 );
      Assert::IsTrue (number2  <= number2 );
      Assert::IsFalse(number2  <= numberi1);
      Assert::IsTrue (number2  <= numberi2);
      Assert::ExpectException<exception>([&] { number2  <= object1 ; });
      Assert::ExpectException<exception>([&] { number2  <= object2 ; });
      Assert::ExpectException<exception>([&] { number2  <= string1 ; });
      Assert::ExpectException<exception>([&] { number2  <= string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 <= array2  ; });
      Assert::IsTrue (numberi1 <= boolean1);
      Assert::IsTrue (numberi1 <= boolean2);
      Assert::IsTrue (numberi1 <= null    );
      Assert::IsTrue (numberi1 <= number1 );
      Assert::IsTrue (numberi1 <= number2 );
      Assert::IsTrue (numberi1 <= numberi1);
      Assert::IsTrue (numberi1 <= numberi2);
      Assert::ExpectException<exception>([&] { numberi1 <= object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 <= array2  ; });
      Assert::IsFalse(numberi2 <= boolean1);
      Assert::IsTrue (numberi2 <= boolean2);
      Assert::IsTrue (numberi2 <= null    );
      Assert::IsFalse(numberi2 <= number1 );
      Assert::IsTrue (numberi2 <= number2 );
      Assert::IsFalse(numberi2 <= numberi1);
      Assert::IsTrue (numberi2 <= numberi2);
      Assert::ExpectException<exception>([&] { numberi2 <= object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= string2 ; });
      Assert::ExpectException<exception>([&] { object1  <= array1  ; });
      Assert::ExpectException<exception>([&] { object1  <= array2  ; });
      Assert::ExpectException<exception>([&] { object1  <= boolean1; });
      Assert::ExpectException<exception>([&] { object1  <= boolean2; });
      Assert::IsTrue (object1  <= null    );
      Assert::ExpectException<exception>([&] { object1  <= number1 ; });
      Assert::ExpectException<exception>([&] { object1  <= number2 ; });
      Assert::ExpectException<exception>([&] { object1  <= numberi1; });
      Assert::ExpectException<exception>([&] { object1  <= numberi2; });
      Assert::ExpectException<exception>([&] { object1  <= object1 ; });
      Assert::ExpectException<exception>([&] { object1  <= object2 ; });
      Assert::ExpectException<exception>([&] { object1  <= string1 ; });
      Assert::ExpectException<exception>([&] { object1  <= string2 ; });
      Assert::ExpectException<exception>([&] { object2  <= array1  ; });
      Assert::ExpectException<exception>([&] { object2  <= array2  ; });
      Assert::ExpectException<exception>([&] { object2  <= boolean1; });
      Assert::ExpectException<exception>([&] { object2  <= boolean2; });
      Assert::IsTrue (object2  <= null    );
      Assert::ExpectException<exception>([&] { object2  <= number1 ; });
      Assert::ExpectException<exception>([&] { object2  <= number2 ; });
      Assert::ExpectException<exception>([&] { object2  <= numberi1; });
      Assert::ExpectException<exception>([&] { object2  <= numberi2; });
      Assert::ExpectException<exception>([&] { object2  <= object1 ; });
      Assert::ExpectException<exception>([&] { object2  <= object2 ; });
      Assert::ExpectException<exception>([&] { object2  <= string1 ; });
      Assert::ExpectException<exception>([&] { object2  <= string2 ; });
      Assert::ExpectException<exception>([&] { string1  <= array1  ; });
      Assert::ExpectException<exception>([&] { string1  <= array2  ; });
      Assert::ExpectException<exception>([&] { string1  <= boolean1; });
      Assert::ExpectException<exception>([&] { string1  <= boolean2; });
      Assert::IsTrue (string1  <= null    );
      Assert::ExpectException<exception>([&] { string1  <= number1 ; });
      Assert::ExpectException<exception>([&] { string1  <= number2 ; });
      Assert::ExpectException<exception>([&] { string1  <= numberi1; });
      Assert::ExpectException<exception>([&] { string1  <= numberi2; });
      Assert::ExpectException<exception>([&] { string1  <= object1 ; });
      Assert::ExpectException<exception>([&] { string1  <= object2 ; });
      Assert::IsTrue (string1  <= string1 );
      Assert::IsTrue (string1  <= string2 );
      Assert::ExpectException<exception>([&] { string2  <= array1  ; });
      Assert::ExpectException<exception>([&] { string2  <= array2  ; });
      Assert::ExpectException<exception>([&] { string2  <= boolean1; });
      Assert::ExpectException<exception>([&] { string2  <= boolean2; });
      Assert::IsTrue (string2  <= null    );
      Assert::ExpectException<exception>([&] { string2  <= number1 ; });
      Assert::ExpectException<exception>([&] { string2  <= number2 ; });
      Assert::ExpectException<exception>([&] { string2  <= numberi1; });
      Assert::ExpectException<exception>([&] { string2  <= numberi2; });
      Assert::ExpectException<exception>([&] { string2  <= object1 ; });
      Assert::ExpectException<exception>([&] { string2  <= object2 ; });
      Assert::IsFalse(string2  <= string1 );
      Assert::IsTrue (string2  <= string2 );
    }

    TEST_METHOD(TestOperatorGreaterThan)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   > array1  ; });
      Assert::ExpectException<exception>([&] { array1   > array2  ; });
      Assert::ExpectException<exception>([&] { array1   > boolean1; });
      Assert::ExpectException<exception>([&] { array1   > boolean2; });
      Assert::IsFalse(array1   > null    );
      Assert::ExpectException<exception>([&] { array1   > number1 ; });
      Assert::ExpectException<exception>([&] { array1   > number2 ; });
      Assert::ExpectException<exception>([&] { array1   > numberi1; });
      Assert::ExpectException<exception>([&] { array1   > numberi2; });
      Assert::ExpectException<exception>([&] { array1   > object1 ; });
      Assert::ExpectException<exception>([&] { array1   > object2 ; });
      Assert::ExpectException<exception>([&] { array1   > string1 ; });
      Assert::ExpectException<exception>([&] { array1   > string2 ; });
      Assert::ExpectException<exception>([&] { array2   > array1  ; });
      Assert::ExpectException<exception>([&] { array2   > array2  ; });
      Assert::ExpectException<exception>([&] { array2   > boolean1; });
      Assert::ExpectException<exception>([&] { array2   > boolean2; });
      Assert::IsFalse(array2   > null    );
      Assert::ExpectException<exception>([&] { array2   > number1 ; });
      Assert::ExpectException<exception>([&] { array2   > number2 ; });
      Assert::ExpectException<exception>([&] { array2   > numberi1; });
      Assert::ExpectException<exception>([&] { array2   > numberi2; });
      Assert::ExpectException<exception>([&] { array2   > object1 ; });
      Assert::ExpectException<exception>([&] { array2   > object2 ; });
      Assert::ExpectException<exception>([&] { array2   > string1 ; });
      Assert::ExpectException<exception>([&] { array2   > string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 > array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 > array2  ; });
      Assert::IsFalse(boolean1 > boolean1);
      Assert::IsFalse(boolean1 > boolean2);
      Assert::IsFalse(boolean1 > null    );
      Assert::IsFalse(boolean1 > number1 );
      Assert::IsFalse(boolean1 > number2 );
      Assert::IsFalse(boolean1 > numberi1);
      Assert::IsFalse(boolean1 > numberi2);
      Assert::ExpectException<exception>([&] { boolean1 > object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 > object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 > string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 > string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 > array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 > array2  ; });
      Assert::IsTrue (boolean2 > boolean1);
      Assert::IsFalse(boolean2 > boolean2);
      Assert::IsFalse(boolean2 > null    );
      Assert::IsTrue (boolean2 > number1 );
      Assert::IsFalse(boolean2 > number2 );
      Assert::IsTrue (boolean2 > numberi1);
      Assert::IsFalse(boolean2 > numberi2);
      Assert::ExpectException<exception>([&] { boolean2 > object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 > object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 > string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 > string2 ; });
      Assert::IsTrue (null     > array1  );
      Assert::IsTrue (null     > array2  );
      Assert::IsTrue (null     > boolean1);
      Assert::IsTrue (null     > boolean2);
      Assert::IsFalse(null     > null    );
      Assert::IsTrue (null     > number1 );
      Assert::IsTrue (null     > number2 );
      Assert::IsTrue (null     > numberi1);
      Assert::IsTrue (null     > numberi2);
      Assert::IsTrue (null     > object1 );
      Assert::IsTrue (null     > object2 );
      Assert::IsTrue (null     > string1 );
      Assert::IsTrue (null     > string2 );
      Assert::ExpectException<exception>([&] { number1  > array1  ; });
      Assert::ExpectException<exception>([&] { number1  > array2  ; });
      Assert::IsFalse(number1  > boolean1);
      Assert::IsFalse(number1  > boolean2);
      Assert::IsFalse(number1  > null    );
      Assert::IsFalse(number1  > number1 );
      Assert::IsFalse(number1  > number2 );
      Assert::IsFalse(number1  > numberi1);
      Assert::IsFalse(number1  > numberi2);
      Assert::ExpectException<exception>([&] { number1  > object1 ; });
      Assert::ExpectException<exception>([&] { number1  > object2 ; });
      Assert::ExpectException<exception>([&] { number1  > string1 ; });
      Assert::ExpectException<exception>([&] { number1  > string2 ; });
      Assert::ExpectException<exception>([&] { number2  > array1  ; });
      Assert::ExpectException<exception>([&] { number2  > array2  ; });
      Assert::IsTrue (number2  > boolean1);
      Assert::IsFalse(number2  > boolean2);
      Assert::IsFalse(number2  > null    );
      Assert::IsTrue (number2  > number1 );
      Assert::IsFalse(number2  > number2 );
      Assert::IsTrue (number2  > numberi1);
      Assert::IsFalse(number2  > numberi2);
      Assert::ExpectException<exception>([&] { number2  > object1 ; });
      Assert::ExpectException<exception>([&] { number2  > object2 ; });
      Assert::ExpectException<exception>([&] { number2  > string1 ; });
      Assert::ExpectException<exception>([&] { number2  > string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 > array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 > array2  ; });
      Assert::IsFalse(numberi1 > boolean1);
      Assert::IsFalse(numberi1 > boolean2);
      Assert::IsFalse(numberi1 > null    );
      Assert::IsFalse(numberi1 > number1 );
      Assert::IsFalse(numberi1 > number2 );
      Assert::IsFalse(numberi1 > numberi1);
      Assert::IsFalse(numberi1 > numberi2);
      Assert::ExpectException<exception>([&] { numberi1 > object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 > object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 > string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 > string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 > array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 > array2  ; });
      Assert::IsTrue (numberi2 > boolean1);
      Assert::IsFalse(numberi2 > boolean2);
      Assert::IsFalse(numberi2 > null    );
      Assert::IsTrue (numberi2 > number1 );
      Assert::IsFalse(numberi2 > number2 );
      Assert::IsTrue (numberi2 > numberi1);
      Assert::IsFalse(numberi2 > numberi2);
      Assert::ExpectException<exception>([&] { numberi2 > object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 > object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 > string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 > string2 ; });
      Assert::ExpectException<exception>([&] { object1  > array1  ; });
      Assert::ExpectException<exception>([&] { object1  > array2  ; });
      Assert::ExpectException<exception>([&] { object1  > boolean1; });
      Assert::ExpectException<exception>([&] { object1  > boolean2; });
      Assert::IsFalse(object1  > null    );
      Assert::ExpectException<exception>([&] { object1  > number1 ; });
      Assert::ExpectException<exception>([&] { object1  > number2 ; });
      Assert::ExpectException<exception>([&] { object1  > numberi1; });
      Assert::ExpectException<exception>([&] { object1  > numberi2; });
      Assert::ExpectException<exception>([&] { object1  > object1 ; });
      Assert::ExpectException<exception>([&] { object1  > object2 ; });
      Assert::ExpectException<exception>([&] { object1  > string1 ; });
      Assert::ExpectException<exception>([&] { object1  > string2 ; });
      Assert::ExpectException<exception>([&] { object2  > array1  ; });
      Assert::ExpectException<exception>([&] { object2  > array2  ; });
      Assert::ExpectException<exception>([&] { object2  > boolean1; });
      Assert::ExpectException<exception>([&] { object2  > boolean2; });
      Assert::IsFalse(object2  > null    );
      Assert::ExpectException<exception>([&] { object2  > number1 ; });
      Assert::ExpectException<exception>([&] { object2  > number2 ; });
      Assert::ExpectException<exception>([&] { object2  > numberi1; });
      Assert::ExpectException<exception>([&] { object2  > numberi2; });
      Assert::ExpectException<exception>([&] { object2  > object1 ; });
      Assert::ExpectException<exception>([&] { object2  > object2 ; });
      Assert::ExpectException<exception>([&] { object2  > string1 ; });
      Assert::ExpectException<exception>([&] { object2  > string2 ; });
      Assert::ExpectException<exception>([&] { string1  > array1  ; });
      Assert::ExpectException<exception>([&] { string1  > array2  ; });
      Assert::ExpectException<exception>([&] { string1  > boolean1; });
      Assert::ExpectException<exception>([&] { string1  > boolean2; });
      Assert::IsFalse(string1  > null    );
      Assert::ExpectException<exception>([&] { string1  > number1 ; });
      Assert::ExpectException<exception>([&] { string1  > number2 ; });
      Assert::ExpectException<exception>([&] { string1  > numberi1; });
      Assert::ExpectException<exception>([&] { string1  > numberi2; });
      Assert::ExpectException<exception>([&] { string1  > object1 ; });
      Assert::ExpectException<exception>([&] { string1  > object2 ; });
      Assert::IsFalse(string1  > string1 );
      Assert::IsFalse(string1  > string2 );
      Assert::ExpectException<exception>([&] { string2  > array1  ; });
      Assert::ExpectException<exception>([&] { string2  > array2  ; });
      Assert::ExpectException<exception>([&] { string2  > boolean1; });
      Assert::ExpectException<exception>([&] { string2  > boolean2; });
      Assert::IsFalse(string2  > null    );
      Assert::ExpectException<exception>([&] { string2  > number1 ; });
      Assert::ExpectException<exception>([&] { string2  > number2 ; });
      Assert::ExpectException<exception>([&] { string2  > numberi1; });
      Assert::ExpectException<exception>([&] { string2  > numberi2; });
      Assert::ExpectException<exception>([&] { string2  > object1 ; });
      Assert::ExpectException<exception>([&] { string2  > object2 ; });
      Assert::IsTrue (string2  > string1 );
      Assert::IsFalse(string2  > string2 );
    }

    TEST_METHOD(TestOperatorGreaterThanOrEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   >= array1  ; });
      Assert::ExpectException<exception>([&] { array1   >= array2  ; });
      Assert::ExpectException<exception>([&] { array1   >= boolean1; });
      Assert::ExpectException<exception>([&] { array1   >= boolean2; });
      Assert::IsFalse(array1   >= null    );
      Assert::ExpectException<exception>([&] { array1   >= number1 ; });
      Assert::ExpectException<exception>([&] { array1   >= number2 ; });
      Assert::ExpectException<exception>([&] { array1   >= numberi1; });
      Assert::ExpectException<exception>([&] { array1   >= numberi2; });
      Assert::ExpectException<exception>([&] { array1   >= object1 ; });
      Assert::ExpectException<exception>([&] { array1   >= object2 ; });
      Assert::ExpectException<exception>([&] { array1   >= string1 ; });
      Assert::ExpectException<exception>([&] { array1   >= string2 ; });
      Assert::ExpectException<exception>([&] { array2   >= array1  ; });
      Assert::ExpectException<exception>([&] { array2   >= array2  ; });
      Assert::ExpectException<exception>([&] { array2   >= boolean1; });
      Assert::ExpectException<exception>([&] { array2   >= boolean2; });
      Assert::IsFalse(array2   >= null    );
      Assert::ExpectException<exception>([&] { array2   >= number1 ; });
      Assert::ExpectException<exception>([&] { array2   >= number2 ; });
      Assert::ExpectException<exception>([&] { array2   >= numberi1; });
      Assert::ExpectException<exception>([&] { array2   >= numberi2; });
      Assert::ExpectException<exception>([&] { array2   >= object1 ; });
      Assert::ExpectException<exception>([&] { array2   >= object2 ; });
      Assert::ExpectException<exception>([&] { array2   >= string1 ; });
      Assert::ExpectException<exception>([&] { array2   >= string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 >= array2  ; });
      Assert::IsTrue (boolean1 >= boolean1);
      Assert::IsFalse(boolean1 >= boolean2);
      Assert::IsFalse(boolean1 >= null    );
      Assert::IsTrue (boolean1 >= number1 );
      Assert::IsFalse(boolean1 >= number2 );
      Assert::IsTrue (boolean1 >= numberi1);
      Assert::IsFalse(boolean1 >= numberi2);
      Assert::ExpectException<exception>([&] { boolean1 >= object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 >= array2  ; });
      Assert::IsTrue (boolean2 >= boolean1);
      Assert::IsTrue (boolean2 >= boolean2);
      Assert::IsFalse(boolean2 >= null    );
      Assert::IsTrue (boolean2 >= number1 );
      Assert::IsTrue(boolean2 >= number2 );
      Assert::IsTrue (boolean2 >= numberi1 );
      Assert::IsTrue(boolean2 >= numberi2);
      Assert::ExpectException<exception>([&] { boolean2 >= object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= string2 ; });
      Assert::IsTrue (null     >= array1  );
      Assert::IsTrue (null     >= array2  );
      Assert::IsTrue (null     >= boolean1);
      Assert::IsTrue (null     >= boolean2);
      Assert::IsTrue (null     >= null    );
      Assert::IsTrue (null     >= number1 );
      Assert::IsTrue (null     >= number2 );
      Assert::IsTrue (null     >= numberi1);
      Assert::IsTrue (null     >= numberi2);
      Assert::IsTrue (null     >= object1 );
      Assert::IsTrue (null     >= object2 );
      Assert::IsTrue (null     >= string1 );
      Assert::IsTrue (null     >= string2 );
      Assert::ExpectException<exception>([&] { number1  >= array1  ; });
      Assert::ExpectException<exception>([&] { number1  >= array2  ; });
      Assert::IsTrue (number1  >= boolean1);
      Assert::IsFalse(number1  >= boolean2);
      Assert::IsFalse(number1  >= null    );
      Assert::IsTrue (number1  >= number1 );
      Assert::IsFalse(number1  >= number2 );
      Assert::IsTrue (number1  >= numberi1);
      Assert::IsFalse(number1  >= numberi2);
      Assert::ExpectException<exception>([&] { number1  >= object1 ; });
      Assert::ExpectException<exception>([&] { number1  >= object2 ; });
      Assert::ExpectException<exception>([&] { number1  >= string1 ; });
      Assert::ExpectException<exception>([&] { number1  >= string2 ; });
      Assert::ExpectException<exception>([&] { number2  >= array1  ; });
      Assert::ExpectException<exception>([&] { number2  >= array2  ; });
      Assert::IsTrue (number2  >= boolean1);
      Assert::IsTrue (number2  >= boolean2);
      Assert::IsFalse(number2  >= null    );
      Assert::IsTrue (number2  >= number1 );
      Assert::IsTrue (number2  >= number2 );
      Assert::IsTrue (number2  >= numberi1);
      Assert::IsTrue (number2  >= numberi2);
      Assert::ExpectException<exception>([&] { number2  >= object1 ; });
      Assert::ExpectException<exception>([&] { number2  >= object2 ; });
      Assert::ExpectException<exception>([&] { number2  >= string1 ; });
      Assert::ExpectException<exception>([&] { number2  >= string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 >= array2  ; });
      Assert::IsTrue (numberi1 >= boolean1);
      Assert::IsFalse(numberi1 >= boolean2);
      Assert::IsFalse(numberi1 >= null    );
      Assert::IsTrue (numberi1 >= number1 );
      Assert::IsFalse(numberi1 >= number2 );
      Assert::IsTrue (numberi1 >= numberi1);
      Assert::IsFalse(numberi1 >= numberi2);
      Assert::ExpectException<exception>([&] { numberi1 >= object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 >= array2  ; });
      Assert::IsTrue (numberi2 >= boolean1);
      Assert::IsTrue (numberi2 >= boolean2);
      Assert::IsFalse(numberi2 >= null    );
      Assert::IsTrue (numberi2 >= number1 );
      Assert::IsTrue (numberi2 >= number2 );
      Assert::IsTrue (numberi2 >= numberi1);
      Assert::IsTrue (numberi2 >= numberi2);
      Assert::ExpectException<exception>([&] { numberi2 >= object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= string2 ; });
      Assert::ExpectException<exception>([&] { object1  >= array1  ; });
      Assert::ExpectException<exception>([&] { object1  >= array2  ; });
      Assert::ExpectException<exception>([&] { object1  >= boolean1; });
      Assert::ExpectException<exception>([&] { object1  >= boolean2; });
      Assert::IsFalse(object1  >= null    );
      Assert::ExpectException<exception>([&] { object1  >= number1 ; });
      Assert::ExpectException<exception>([&] { object1  >= number2 ; });
      Assert::ExpectException<exception>([&] { object1  >= numberi1; });
      Assert::ExpectException<exception>([&] { object1  >= numberi2; });
      Assert::ExpectException<exception>([&] { object1  >= object1 ; });
      Assert::ExpectException<exception>([&] { object1  >= object2 ; });
      Assert::ExpectException<exception>([&] { object1  >= string1 ; });
      Assert::ExpectException<exception>([&] { object1  >= string2 ; });
      Assert::ExpectException<exception>([&] { object2  >= array1  ; });
      Assert::ExpectException<exception>([&] { object2  >= array2  ; });
      Assert::ExpectException<exception>([&] { object2  >= boolean1; });
      Assert::ExpectException<exception>([&] { object2  >= boolean2; });
      Assert::IsFalse(object2  >= null    );
      Assert::ExpectException<exception>([&] { object2  >= number1 ; });
      Assert::ExpectException<exception>([&] { object2  >= number2 ; });
      Assert::ExpectException<exception>([&] { object2  >= numberi1; });
      Assert::ExpectException<exception>([&] { object2  >= numberi2; });
      Assert::ExpectException<exception>([&] { object2  >= object1 ; });
      Assert::ExpectException<exception>([&] { object2  >= object2 ; });
      Assert::ExpectException<exception>([&] { object2  >= string1 ; });
      Assert::ExpectException<exception>([&] { object2  >= string2 ; });
      Assert::ExpectException<exception>([&] { string1  >= array1  ; });
      Assert::ExpectException<exception>([&] { string1  >= array2  ; });
      Assert::ExpectException<exception>([&] { string1  >= boolean1; });
      Assert::ExpectException<exception>([&] { string1  >= boolean2; });
      Assert::IsFalse(string1  >= null    );
      Assert::ExpectException<exception>([&] { string1  >= number1 ; });
      Assert::ExpectException<exception>([&] { string1  >= number2 ; });
      Assert::ExpectException<exception>([&] { string1  >= numberi1; });
      Assert::ExpectException<exception>([&] { string1  >= numberi2; });
      Assert::ExpectException<exception>([&] { string1  >= object1 ; });
      Assert::ExpectException<exception>([&] { string1  >= object2 ; });
      Assert::IsTrue (string1  >= string1 );
      Assert::IsFalse(string1  >= string2 );
      Assert::ExpectException<exception>([&] { string2  >= array1  ; });
      Assert::ExpectException<exception>([&] { string2  >= array2  ; });
      Assert::ExpectException<exception>([&] { string2  >= boolean1; });
      Assert::ExpectException<exception>([&] { string2  >= boolean2; });
      Assert::IsFalse(string2  >= null    );
      Assert::ExpectException<exception>([&] { string2  >= number1 ; });
      Assert::ExpectException<exception>([&] { string2  >= number2 ; });
      Assert::ExpectException<exception>([&] { string2  >= numberi1; });
      Assert::ExpectException<exception>([&] { string2  >= numberi2; });
      Assert::ExpectException<exception>([&] { string2  >= object1 ; });
      Assert::ExpectException<exception>([&] { string2  >= object2 ; });
      Assert::IsTrue (string2  >= string1 );
      Assert::IsTrue (string2  >= string2 );
    }

    TEST_METHOD(TestOperatorAdd)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   + array1  ; });
      Assert::ExpectException<exception>([&] { array1   + array2  ; });
      Assert::ExpectException<exception>([&] { array1   + boolean1; });
      Assert::ExpectException<exception>([&] { array1   + boolean2; });
      Assert::AreEqual<Json>(null    , array1   + null    );
      Assert::ExpectException<exception>([&] { array1   + number1 ; });
      Assert::ExpectException<exception>([&] { array1   + number2 ; });
      Assert::ExpectException<exception>([&] { array1   + numberi1; });
      Assert::ExpectException<exception>([&] { array1   + numberi2; });
      Assert::ExpectException<exception>([&] { array1   + object1 ; });
      Assert::ExpectException<exception>([&] { array1   + object2 ; });
      Assert::ExpectException<exception>([&] { array1   + string1 ; });
      Assert::ExpectException<exception>([&] { array1   + string2 ; });
      Assert::ExpectException<exception>([&] { array2   + array1  ; });
      Assert::ExpectException<exception>([&] { array2   + array2  ; });
      Assert::ExpectException<exception>([&] { array2   + boolean1; });
      Assert::ExpectException<exception>([&] { array2   + boolean2; });
      Assert::AreEqual<Json>(null    , array2   + null    );
      Assert::ExpectException<exception>([&] { array2   + number1 ; });
      Assert::ExpectException<exception>([&] { array2   + number2 ; });
      Assert::ExpectException<exception>([&] { array2   + numberi1; });
      Assert::ExpectException<exception>([&] { array2   + numberi2; });
      Assert::ExpectException<exception>([&] { array2   + object1 ; });
      Assert::ExpectException<exception>([&] { array2   + object2 ; });
      Assert::ExpectException<exception>([&] { array2   + string1 ; });
      Assert::ExpectException<exception>([&] { array2   + string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 + array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 + array2  ; });
      Assert::AreEqual<Json>(0i64, boolean1 + boolean1);
      Assert::AreEqual<Json>(1i64, boolean1 + boolean2);
      Assert::AreEqual<Json>(null    , boolean1 + null    );
      Assert::AreEqual<Json>(0.0, boolean1 + number1 );
      Assert::AreEqual<Json>(1.0, boolean1 + number2 );
      Assert::AreEqual<Json>(0i64, boolean1 + numberi1);
      Assert::AreEqual<Json>(1i64, boolean1 + numberi2);
      Assert::ExpectException<exception>([&] { boolean1 + object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 + object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 + string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 + string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 + array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 + array2  ; });
      Assert::AreEqual<Json>(1i64, boolean2 + boolean1);
      Assert::AreEqual<Json>(2i64, boolean2 + boolean2);
      Assert::AreEqual<Json>(null    , boolean2 + null    );
      Assert::AreEqual<Json>(1.0, boolean2 + number1 );
      Assert::AreEqual<Json>(2.0, boolean2 + number2 );
      Assert::AreEqual<Json>(1i64, boolean2 + numberi1);
      Assert::AreEqual<Json>(2i64, boolean2 + numberi2);
      Assert::ExpectException<exception>([&] { boolean2 + object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 + object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 + string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 + string2 ; });
      Assert::AreEqual<Json>(null    , null     + array1  );
      Assert::AreEqual<Json>(null    , null     + array2  );
      Assert::AreEqual<Json>(null    , null     + boolean1);
      Assert::AreEqual<Json>(null    , null     + boolean2);
      Assert::AreEqual<Json>(null    , null     + null    );
      Assert::AreEqual<Json>(null    , null     + number1 );
      Assert::AreEqual<Json>(null    , null     + number2 );
      Assert::AreEqual<Json>(null    , null     + numberi1);
      Assert::AreEqual<Json>(null    , null     + numberi2);
      Assert::AreEqual<Json>(null    , null     + object1 );
      Assert::AreEqual<Json>(null    , null     + object2 );
      Assert::AreEqual<Json>(null    , null     + string1 );
      Assert::AreEqual<Json>(null    , null     + string2 );
      Assert::ExpectException<exception>([&] { number1  + array1  ; });
      Assert::ExpectException<exception>([&] { number1  + array2  ; });
      Assert::AreEqual<Json>(0.0, number1  + boolean1);
      Assert::AreEqual<Json>(1.0, number1  + boolean2);
      Assert::AreEqual<Json>(null    , number1  + null    );
      Assert::AreEqual<Json>(0.0, number1  + number1 );
      Assert::AreEqual<Json>(1.0, number1  + number2 );
      Assert::AreEqual<Json>(0.0, number1  + numberi1);
      Assert::AreEqual<Json>(1.0, number1  + numberi2);
      Assert::ExpectException<exception>([&] { number1  + object1 ; });
      Assert::ExpectException<exception>([&] { number1  + object2 ; });
      Assert::ExpectException<exception>([&] { number1  + string1 ; });
      Assert::ExpectException<exception>([&] { number1  + string2 ; });
      Assert::ExpectException<exception>([&] { number2  + array1  ; });
      Assert::ExpectException<exception>([&] { number2  + array2  ; });
      Assert::AreEqual<Json>(1.0, number2  + boolean1);
      Assert::AreEqual<Json>(2.0, number2  + boolean2);
      Assert::AreEqual<Json>(null    , number2  + null    );
      Assert::AreEqual<Json>(1.0, number2  + number1 );
      Assert::AreEqual<Json>(2.0, number2  + number2 );
      Assert::AreEqual<Json>(1.0, number2  + numberi1);
      Assert::AreEqual<Json>(2.0, number2  + numberi2);
      Assert::ExpectException<exception>([&] { number2  + object1 ; });
      Assert::ExpectException<exception>([&] { number2  + object2 ; });
      Assert::ExpectException<exception>([&] { number2  + string1 ; });
      Assert::ExpectException<exception>([&] { number2  + string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 + array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 + array2  ; });
      Assert::AreEqual<Json>(0i64, numberi1 + boolean1);
      Assert::AreEqual<Json>(1i64, numberi1 + boolean2);
      Assert::AreEqual<Json>(null    , numberi1 + null    );
      Assert::AreEqual<Json>(0.0, numberi1 + number1 );
      Assert::AreEqual<Json>(1.0, numberi1 + number2 );
      Assert::AreEqual<Json>(0i64, numberi1 + numberi1);
      Assert::AreEqual<Json>(1i64, numberi1 + numberi2);
      Assert::ExpectException<exception>([&] { numberi1 + object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 + object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 + string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 + string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 + array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 + array2  ; });
      Assert::AreEqual<Json>(1i64, numberi2 + boolean1);
      Assert::AreEqual<Json>(2i64, numberi2 + boolean2);
      Assert::AreEqual<Json>(null    , numberi2 + null    );
      Assert::AreEqual<Json>(1.0, numberi2 + number1 );
      Assert::AreEqual<Json>(2.0, numberi2 + number2 );
      Assert::AreEqual<Json>(1i64, numberi2 + numberi1);
      Assert::AreEqual<Json>(2i64, numberi2 + numberi2);
      Assert::ExpectException<exception>([&] { numberi2 + object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 + object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 + string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 + string2 ; });
      Assert::ExpectException<exception>([&] { object1  + array1  ; });
      Assert::ExpectException<exception>([&] { object1  + array2  ; });
      Assert::ExpectException<exception>([&] { object1  + boolean1; });
      Assert::ExpectException<exception>([&] { object1  + boolean2; });
      Assert::AreEqual<Json>(null    , object1  + null    );
      Assert::ExpectException<exception>([&] { object1  + number1 ; });
      Assert::ExpectException<exception>([&] { object1  + number2 ; });
      Assert::ExpectException<exception>([&] { object1  + numberi1; });
      Assert::ExpectException<exception>([&] { object1  + numberi2; });
      Assert::ExpectException<exception>([&] { object1  + object1 ; });
      Assert::ExpectException<exception>([&] { object1  + object2 ; });
      Assert::ExpectException<exception>([&] { object1  + string1 ; });
      Assert::ExpectException<exception>([&] { object1  + string2 ; });
      Assert::ExpectException<exception>([&] { object2  + array1  ; });
      Assert::ExpectException<exception>([&] { object2  + array2  ; });
      Assert::ExpectException<exception>([&] { object2  + boolean1; });
      Assert::ExpectException<exception>([&] { object2  + boolean2; });
      Assert::AreEqual<Json>(null    , object2  + null    );
      Assert::ExpectException<exception>([&] { object2  + number1 ; });
      Assert::ExpectException<exception>([&] { object2  + number2 ; });
      Assert::ExpectException<exception>([&] { object2  + numberi1; });
      Assert::ExpectException<exception>([&] { object2  + numberi2; });
      Assert::ExpectException<exception>([&] { object2  + object1 ; });
      Assert::ExpectException<exception>([&] { object2  + object2 ; });
      Assert::ExpectException<exception>([&] { object2  + string1 ; });
      Assert::ExpectException<exception>([&] { object2  + string2 ; });
      Assert::ExpectException<exception>([&] { string1  + array1  ; });
      Assert::ExpectException<exception>([&] { string1  + array2  ; });
      Assert::ExpectException<exception>([&] { string1  + boolean1; });
      Assert::ExpectException<exception>([&] { string1  + boolean2; });
      Assert::AreEqual<Json>(null    , string1  + null    );
      Assert::ExpectException<exception>([&] { string1  + number1 ; });
      Assert::ExpectException<exception>([&] { string1  + number2 ; });
      Assert::ExpectException<exception>([&] { string1  + numberi1; });
      Assert::ExpectException<exception>([&] { string1  + numberi2; });
      Assert::ExpectException<exception>([&] { string1  + object1 ; });
      Assert::ExpectException<exception>([&] { string1  + object2 ; });
      Assert::AreEqual<Json>(L"TestString0TestString0"s, string1  + string1 );
      Assert::AreEqual<Json>(L"TestString0TestString1"s, string1  + string2 );
      Assert::ExpectException<exception>([&] { string2  + array1  ; });
      Assert::ExpectException<exception>([&] { string2  + array2  ; });
      Assert::ExpectException<exception>([&] { string2  + boolean1; });
      Assert::ExpectException<exception>([&] { string2  + boolean2; });
      Assert::AreEqual<Json>(null    , string2  + null    );
      Assert::ExpectException<exception>([&] { string2  + number1 ; });
      Assert::ExpectException<exception>([&] { string2  + number2 ; });
      Assert::ExpectException<exception>([&] { string2  + numberi1; });
      Assert::ExpectException<exception>([&] { string2  + numberi2; });
      Assert::ExpectException<exception>([&] { string2  + object1 ; });
      Assert::ExpectException<exception>([&] { string2  + object2 ; });
      Assert::AreEqual<Json>(L"TestString1TestString0"s, string2  + string1 );
      Assert::AreEqual<Json>(L"TestString1TestString1"s, string2  + string2 );
    }

    TEST_METHOD(TestOperatorAddAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value += boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(1i64, value += boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value += number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(1.0, value += number2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value += numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(1i64, value += numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value += boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(2i64, value += boolean2);
      Assert::AreEqual<Json>(2i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value += number1);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(2.0, value += number2);
      Assert::AreEqual<Json>(2.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value += numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(2i64, value += numberi2);
      Assert::AreEqual<Json>(2i64, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value += array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value += boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(1.0, value += boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value += number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(1.0, value += number2);
      Assert::AreEqual<Json>(1.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value += numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(1.0, value += numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value += boolean1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(2.0, value += boolean2);
      Assert::AreEqual<Json>(2.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value += number1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(2.0, value += number2);
      Assert::AreEqual<Json>(2.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value += numberi1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(2.0, value += numberi2);
      Assert::AreEqual<Json>(2.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value += boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(1i64, value += boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value += number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(1.0, value += number2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value += numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(1i64, value += numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value += boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(2i64, value += boolean2);
      Assert::AreEqual<Json>(2i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value += number1);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(2.0, value += number2);
      Assert::AreEqual<Json>(2.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value += numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(2i64, value += numberi2);
      Assert::AreEqual<Json>(2i64, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(L"TestString0TestString0"s, value += string1);
      Assert::AreEqual<Json>(L"TestString0TestString0"s, value);
      value = string1;
      Assert::AreEqual<Json>(L"TestString0TestString1"s, value += string2);
      Assert::AreEqual<Json>(L"TestString0TestString1"s, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(L"TestString1TestString0"s, value += string1);
      Assert::AreEqual<Json>(L"TestString1TestString0"s, value);
      value = string2;
      Assert::AreEqual<Json>(L"TestString1TestString1"s, value += string2);
      Assert::AreEqual<Json>(L"TestString1TestString1"s, value);
    }

    TEST_METHOD(TestOperatorSubtract)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   - array1  ; });
      Assert::ExpectException<exception>([&] { array1   - array2  ; });
      Assert::ExpectException<exception>([&] { array1   - boolean1; });
      Assert::ExpectException<exception>([&] { array1   - boolean2; });
      Assert::AreEqual<Json>(null , array1   - null    );
      Assert::ExpectException<exception>([&] { array1   - number1 ; });
      Assert::ExpectException<exception>([&] { array1   - number2 ; });
      Assert::ExpectException<exception>([&] { array1   - numberi1; });
      Assert::ExpectException<exception>([&] { array1   - numberi2; });
      Assert::ExpectException<exception>([&] { array1   - object1 ; });
      Assert::ExpectException<exception>([&] { array1   - object2 ; });
      Assert::ExpectException<exception>([&] { array1   - string1 ; });
      Assert::ExpectException<exception>([&] { array1   - string2 ; });
      Assert::ExpectException<exception>([&] { array2   - array1  ; });
      Assert::ExpectException<exception>([&] { array2   - array2  ; });
      Assert::ExpectException<exception>([&] { array2   - boolean1; });
      Assert::ExpectException<exception>([&] { array2   - boolean2; });
      Assert::AreEqual<Json>(null , array2   - null    );
      Assert::ExpectException<exception>([&] { array2   - number1 ; });
      Assert::ExpectException<exception>([&] { array2   - number2 ; });
      Assert::ExpectException<exception>([&] { array2   - numberi1; });
      Assert::ExpectException<exception>([&] { array2   - numberi2; });
      Assert::ExpectException<exception>([&] { array2   - object1 ; });
      Assert::ExpectException<exception>([&] { array2   - object2 ; });
      Assert::ExpectException<exception>([&] { array2   - string1 ; });
      Assert::ExpectException<exception>([&] { array2   - string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 - array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 - array2  ; });
      Assert::AreEqual<Json>( 0i64, boolean1 - boolean1);
      Assert::AreEqual<Json>(-1i64, boolean1 - boolean2);
      Assert::AreEqual<Json>(null , boolean1 - null    );
      Assert::AreEqual<Json>( 0.0 , boolean1 - number1 );
      Assert::AreEqual<Json>(-1.0 , boolean1 - number2 );
      Assert::AreEqual<Json>( 0i64, boolean1 - numberi1);
      Assert::AreEqual<Json>(-1i64, boolean1 - numberi2);
      Assert::ExpectException<exception>([&] { boolean1 - object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 - object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 - string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 - string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 - array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 - array2  ; });
      Assert::AreEqual<Json>( 1i64, boolean2 - boolean1);
      Assert::AreEqual<Json>( 0i64, boolean2 - boolean2);
      Assert::AreEqual<Json>(null , boolean2 - null    );
      Assert::AreEqual<Json>( 1.0 , boolean2 - number1 );
      Assert::AreEqual<Json>( 0.0 , boolean2 - number2 );
      Assert::AreEqual<Json>( 1i64, boolean2 - numberi1);
      Assert::AreEqual<Json>( 0i64, boolean2 - numberi2);
      Assert::ExpectException<exception>([&] { boolean2 - object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 - object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 - string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 - string2 ; });
      Assert::AreEqual<Json>(null , null     - array1  );
      Assert::AreEqual<Json>(null , null     - array2  );
      Assert::AreEqual<Json>(null , null     - boolean1);
      Assert::AreEqual<Json>(null , null     - boolean2);
      Assert::AreEqual<Json>(null , null     - null    );
      Assert::AreEqual<Json>(null , null     - number1 );
      Assert::AreEqual<Json>(null , null     - number2 );
      Assert::AreEqual<Json>(null , null     - numberi1);
      Assert::AreEqual<Json>(null , null     - numberi2);
      Assert::AreEqual<Json>(null , null     - object1 );
      Assert::AreEqual<Json>(null , null     - object2 );
      Assert::AreEqual<Json>(null , null     - string1 );
      Assert::AreEqual<Json>(null , null     - string2 );
      Assert::ExpectException<exception>([&] { number1  - array1  ; });
      Assert::ExpectException<exception>([&] { number1  - array2  ; });
      Assert::AreEqual<Json>( 0.0 , number1  - boolean1);
      Assert::AreEqual<Json>(-1.0 , number1  - boolean2);
      Assert::AreEqual<Json>(null , number1  - null    );
      Assert::AreEqual<Json>( 0.0 , number1  - number1 );
      Assert::AreEqual<Json>(-1.0 , number1  - number2 );
      Assert::AreEqual<Json>( 0.0 , number1  - numberi1);
      Assert::AreEqual<Json>(-1.0 , number1  - numberi2);
      Assert::ExpectException<exception>([&] { number1  - object1 ; });
      Assert::ExpectException<exception>([&] { number1  - object2 ; });
      Assert::ExpectException<exception>([&] { number1  - string1 ; });
      Assert::ExpectException<exception>([&] { number1  - string2 ; });
      Assert::ExpectException<exception>([&] { number2  - array1  ; });
      Assert::ExpectException<exception>([&] { number2  - array2  ; });
      Assert::AreEqual<Json>( 1.0 , number2  - boolean1);
      Assert::AreEqual<Json>( 0.0 , number2  - boolean2);
      Assert::AreEqual<Json>(null , number2  - null    );
      Assert::AreEqual<Json>( 1.0 , number2  - number1 );
      Assert::AreEqual<Json>( 0.0 , number2  - number2 );
      Assert::AreEqual<Json>( 1.0 , number2  - numberi1);
      Assert::AreEqual<Json>( 0.0 , number2  - numberi2);
      Assert::ExpectException<exception>([&] { number2  - object1 ; });
      Assert::ExpectException<exception>([&] { number2  - object2 ; });
      Assert::ExpectException<exception>([&] { number2  - string1 ; });
      Assert::ExpectException<exception>([&] { number2  - string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 - array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 - array2  ; });
      Assert::AreEqual<Json>( 0i64, numberi1 - boolean1);
      Assert::AreEqual<Json>(-1i64, numberi1 - boolean2);
      Assert::AreEqual<Json>(null , numberi1 - null    );
      Assert::AreEqual<Json>( 0.0 , numberi1 - number1 );
      Assert::AreEqual<Json>(-1.0 , numberi1 - number2 );
      Assert::AreEqual<Json>( 0i64, numberi1 - numberi1);
      Assert::AreEqual<Json>(-1i64, numberi1 - numberi2);
      Assert::ExpectException<exception>([&] { numberi1 - object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 - object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 - string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 - string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 - array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 - array2  ; });
      Assert::AreEqual<Json>( 1i64, numberi2 - boolean1);
      Assert::AreEqual<Json>( 0i64, numberi2 - boolean2);
      Assert::AreEqual<Json>(null , numberi2 - null    );
      Assert::AreEqual<Json>( 1.0 , numberi2 - number1 );
      Assert::AreEqual<Json>( 0.0 , numberi2 - number2 );
      Assert::AreEqual<Json>( 1i64, numberi2 - numberi1);
      Assert::AreEqual<Json>( 0i64, numberi2 - numberi2);
      Assert::ExpectException<exception>([&] { numberi2 - object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 - object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 - string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 - string2 ; });
      Assert::ExpectException<exception>([&] { object1  - array1  ; });
      Assert::ExpectException<exception>([&] { object1  - array2  ; });
      Assert::ExpectException<exception>([&] { object1  - boolean1; });
      Assert::ExpectException<exception>([&] { object1  - boolean2; });
      Assert::AreEqual<Json>(null , object1  - null    );
      Assert::ExpectException<exception>([&] { object1  - number1 ; });
      Assert::ExpectException<exception>([&] { object1  - number2 ; });
      Assert::ExpectException<exception>([&] { object1  - numberi1; });
      Assert::ExpectException<exception>([&] { object1  - numberi2; });
      Assert::ExpectException<exception>([&] { object1  - object1 ; });
      Assert::ExpectException<exception>([&] { object1  - object2 ; });
      Assert::ExpectException<exception>([&] { object1  - string1 ; });
      Assert::ExpectException<exception>([&] { object1  - string2 ; });
      Assert::ExpectException<exception>([&] { object2  - array1  ; });
      Assert::ExpectException<exception>([&] { object2  - array2  ; });
      Assert::ExpectException<exception>([&] { object2  - boolean1; });
      Assert::ExpectException<exception>([&] { object2  - boolean2; });
      Assert::AreEqual<Json>(null , object2  - null    );
      Assert::ExpectException<exception>([&] { object2  - number1 ; });
      Assert::ExpectException<exception>([&] { object2  - number2 ; });
      Assert::ExpectException<exception>([&] { object2  - numberi1; });
      Assert::ExpectException<exception>([&] { object2  - numberi2; });
      Assert::ExpectException<exception>([&] { object2  - object1 ; });
      Assert::ExpectException<exception>([&] { object2  - object2 ; });
      Assert::ExpectException<exception>([&] { object2  - string1 ; });
      Assert::ExpectException<exception>([&] { object2  - string2 ; });
      Assert::ExpectException<exception>([&] { string1  - array1  ; });
      Assert::ExpectException<exception>([&] { string1  - array2  ; });
      Assert::ExpectException<exception>([&] { string1  - boolean1; });
      Assert::ExpectException<exception>([&] { string1  - boolean2; });
      Assert::AreEqual<Json>(null , string1  - null    );
      Assert::ExpectException<exception>([&] { string1  - number1 ; });
      Assert::ExpectException<exception>([&] { string1  - number2 ; });
      Assert::ExpectException<exception>([&] { string1  - numberi1; });
      Assert::ExpectException<exception>([&] { string1  - numberi2; });
      Assert::ExpectException<exception>([&] { string1  - object1 ; });
      Assert::ExpectException<exception>([&] { string1  - object2 ; });
      Assert::ExpectException<exception>([&] { string1  - string1 ; });
      Assert::ExpectException<exception>([&] { string1  - string2 ; });
      Assert::ExpectException<exception>([&] { string2  - array1  ; });
      Assert::ExpectException<exception>([&] { string2  - array2  ; });
      Assert::ExpectException<exception>([&] { string2  - boolean1; });
      Assert::ExpectException<exception>([&] { string2  - boolean2; });
      Assert::AreEqual<Json>(null , string2  - null    );
      Assert::ExpectException<exception>([&] { string2  - number1 ; });
      Assert::ExpectException<exception>([&] { string2  - number2 ; });
      Assert::ExpectException<exception>([&] { string2  - numberi1; });
      Assert::ExpectException<exception>([&] { string2  - numberi2; });
      Assert::ExpectException<exception>([&] { string2  - object1 ; });
      Assert::ExpectException<exception>([&] { string2  - object2 ; });
      Assert::ExpectException<exception>([&] { string2  - string1 ; });
      Assert::ExpectException<exception>([&] { string2  - string2 ; });
    }

    TEST_METHOD(TestOperatorSubtractAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value -= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(-1i64, value -= boolean2);
      Assert::AreEqual<Json>(-1i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value -= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(-1.0, value -= number2);
      Assert::AreEqual<Json>(-1.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value -= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(-1i64, value -= numberi2);
      Assert::AreEqual<Json>(-1i64, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value -= boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value -= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value -= number1);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value -= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value -= numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value -= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value -= boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(-1.0, value -= boolean2);
      Assert::AreEqual<Json>(-1.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value -= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(-1.0, value -= number2);
      Assert::AreEqual<Json>(-1.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value -= numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(-1.0, value -= numberi2);
      Assert::AreEqual<Json>(-1.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value -= boolean1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value -= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value -= number1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value -= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value -= numberi1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value -= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value -= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(-1i64, value -= boolean2);
      Assert::AreEqual<Json>(-1i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value -= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(-1.0, value -= number2);
      Assert::AreEqual<Json>(-1.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value -= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(-1i64, value -= numberi2);
      Assert::AreEqual<Json>(-1i64, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value -= boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value -= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value -= number1);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value -= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value -= numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value -= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(string2, value);
    }

    TEST_METHOD(TestOperatorMultiply)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   * array1  ; });
      Assert::ExpectException<exception>([&] { array1   * array2  ; });
      Assert::ExpectException<exception>([&] { array1   * boolean1; });
      Assert::ExpectException<exception>([&] { array1   * boolean2; });
      Assert::AreEqual<Json>(null , array1   * null    );
      Assert::ExpectException<exception>([&] { array1   * number1 ; });
      Assert::ExpectException<exception>([&] { array1   * number2 ; });
      Assert::ExpectException<exception>([&] { array1   * numberi1; });
      Assert::ExpectException<exception>([&] { array1   * numberi2; });
      Assert::ExpectException<exception>([&] { array1   * object1 ; });
      Assert::ExpectException<exception>([&] { array1   * object2 ; });
      Assert::ExpectException<exception>([&] { array1   * string1 ; });
      Assert::ExpectException<exception>([&] { array1   * string2 ; });
      Assert::ExpectException<exception>([&] { array2   * array1  ; });
      Assert::ExpectException<exception>([&] { array2   * array2  ; });
      Assert::ExpectException<exception>([&] { array2   * boolean1; });
      Assert::ExpectException<exception>([&] { array2   * boolean2; });
      Assert::AreEqual<Json>(null , array2   * null    );
      Assert::ExpectException<exception>([&] { array2   * number1 ; });
      Assert::ExpectException<exception>([&] { array2   * number2 ; });
      Assert::ExpectException<exception>([&] { array2   * numberi1; });
      Assert::ExpectException<exception>([&] { array2   * numberi2; });
      Assert::ExpectException<exception>([&] { array2   * object1 ; });
      Assert::ExpectException<exception>([&] { array2   * object2 ; });
      Assert::ExpectException<exception>([&] { array2   * string1 ; });
      Assert::ExpectException<exception>([&] { array2   * string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 * array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 * array2  ; });
      Assert::AreEqual<Json>( 0i64, boolean1 * boolean1);
      Assert::AreEqual<Json>( 0i64, boolean1 * boolean2);
      Assert::AreEqual<Json>(null , boolean1 * null    );
      Assert::AreEqual<Json>( 0.0 , boolean1 * number1 );
      Assert::AreEqual<Json>( 0.0 , boolean1 * number2 );
      Assert::AreEqual<Json>( 0i64, boolean1 * numberi1);
      Assert::AreEqual<Json>( 0i64, boolean1 * numberi2);
      Assert::ExpectException<exception>([&] { boolean1 * object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 * object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 * string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 * string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 * array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 * array2  ; });
      Assert::AreEqual<Json>( 0i64, boolean2 * boolean1);
      Assert::AreEqual<Json>( 1i64, boolean2 * boolean2);
      Assert::AreEqual<Json>(null , boolean2 * null    );
      Assert::AreEqual<Json>( 0.0 , boolean2 * number1 );
      Assert::AreEqual<Json>( 1.0 , boolean2 * number2 );
      Assert::AreEqual<Json>( 0i64, boolean2 * numberi1);
      Assert::AreEqual<Json>( 1i64, boolean2 * numberi2);
      Assert::ExpectException<exception>([&] { boolean2 * object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 * object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 * string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 * string2 ; });
      Assert::AreEqual<Json>(null , null     * array1  );
      Assert::AreEqual<Json>(null , null     * array2  );
      Assert::AreEqual<Json>(null , null     * boolean1);
      Assert::AreEqual<Json>(null , null     * boolean2);
      Assert::AreEqual<Json>(null , null     * null    );
      Assert::AreEqual<Json>(null , null     * number1 );
      Assert::AreEqual<Json>(null , null     * number2 );
      Assert::AreEqual<Json>(null , null     * numberi1);
      Assert::AreEqual<Json>(null , null     * numberi2);
      Assert::AreEqual<Json>(null , null     * object1 );
      Assert::AreEqual<Json>(null , null     * object2 );
      Assert::AreEqual<Json>(null , null     * string1 );
      Assert::AreEqual<Json>(null , null     * string2 );
      Assert::ExpectException<exception>([&] { number1  * array1  ; });
      Assert::ExpectException<exception>([&] { number1  * array2  ; });
      Assert::AreEqual<Json>( 0.0 , number1  * boolean1);
      Assert::AreEqual<Json>( 0.0 , number1  * boolean2);
      Assert::AreEqual<Json>(null , number1  * null    );
      Assert::AreEqual<Json>( 0.0 , number1  * number1 );
      Assert::AreEqual<Json>( 0.0 , number1  * number2 );
      Assert::AreEqual<Json>( 0.0 , number1  * numberi1);
      Assert::AreEqual<Json>( 0.0 , number1  * numberi2);
      Assert::ExpectException<exception>([&] { number1  * object1 ; });
      Assert::ExpectException<exception>([&] { number1  * object2 ; });
      Assert::ExpectException<exception>([&] { number1  * string1 ; });
      Assert::ExpectException<exception>([&] { number1  * string2 ; });
      Assert::ExpectException<exception>([&] { number2  * array1  ; });
      Assert::ExpectException<exception>([&] { number2  * array2  ; });
      Assert::AreEqual<Json>( 0.0 , number2  * boolean1);
      Assert::AreEqual<Json>( 1.0 , number2  * boolean2);
      Assert::AreEqual<Json>(null , number2  * null    );
      Assert::AreEqual<Json>( 0.0 , number2  * number1 );
      Assert::AreEqual<Json>( 1.0 , number2  * number2 );
      Assert::AreEqual<Json>( 0.0 , number2  * numberi1);
      Assert::AreEqual<Json>( 1.0 , number2  * numberi2);
      Assert::ExpectException<exception>([&] { number2  * object1 ; });
      Assert::ExpectException<exception>([&] { number2  * object2 ; });
      Assert::ExpectException<exception>([&] { number2  * string1 ; });
      Assert::ExpectException<exception>([&] { number2  * string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 * array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 * array2  ; });
      Assert::AreEqual<Json>( 0i64, numberi1 * boolean1);
      Assert::AreEqual<Json>( 0i64, numberi1 * boolean2);
      Assert::AreEqual<Json>(null , numberi1 * null    );
      Assert::AreEqual<Json>( 0.0 , numberi1 * number1 );
      Assert::AreEqual<Json>( 0.0 , numberi1 * number2 );
      Assert::AreEqual<Json>( 0i64, numberi1 * numberi1);
      Assert::AreEqual<Json>( 0i64, numberi1 * numberi2);
      Assert::ExpectException<exception>([&] { numberi1 * object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 * object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 * string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 * string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 * array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 * array2  ; });
      Assert::AreEqual<Json>( 0i64, numberi2 * boolean1);
      Assert::AreEqual<Json>( 1i64, numberi2 * boolean2);
      Assert::AreEqual<Json>(null , numberi2 * null    );
      Assert::AreEqual<Json>( 0.0 , numberi2 * number1 );
      Assert::AreEqual<Json>( 1.0 , numberi2 * number2 );
      Assert::AreEqual<Json>( 0i64, numberi2 * numberi1);
      Assert::AreEqual<Json>( 1i64, numberi2 * numberi2);
      Assert::ExpectException<exception>([&] { numberi2 * object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 * object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 * string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 * string2 ; });
      Assert::ExpectException<exception>([&] { object1  * array1  ; });
      Assert::ExpectException<exception>([&] { object1  * array2  ; });
      Assert::ExpectException<exception>([&] { object1  * boolean1; });
      Assert::ExpectException<exception>([&] { object1  * boolean2; });
      Assert::AreEqual<Json>(null , object1  * null    );
      Assert::ExpectException<exception>([&] { object1  * number1 ; });
      Assert::ExpectException<exception>([&] { object1  * number2 ; });
      Assert::ExpectException<exception>([&] { object1  * numberi1; });
      Assert::ExpectException<exception>([&] { object1  * numberi2; });
      Assert::ExpectException<exception>([&] { object1  * object1 ; });
      Assert::ExpectException<exception>([&] { object1  * object2 ; });
      Assert::ExpectException<exception>([&] { object1  * string1 ; });
      Assert::ExpectException<exception>([&] { object1  * string2 ; });
      Assert::ExpectException<exception>([&] { object2  * array1  ; });
      Assert::ExpectException<exception>([&] { object2  * array2  ; });
      Assert::ExpectException<exception>([&] { object2  * boolean1; });
      Assert::ExpectException<exception>([&] { object2  * boolean2; });
      Assert::AreEqual<Json>(null , object2  * null    );
      Assert::ExpectException<exception>([&] { object2  * number1 ; });
      Assert::ExpectException<exception>([&] { object2  * number2 ; });
      Assert::ExpectException<exception>([&] { object2  * numberi1; });
      Assert::ExpectException<exception>([&] { object2  * numberi2; });
      Assert::ExpectException<exception>([&] { object2  * object1 ; });
      Assert::ExpectException<exception>([&] { object2  * object2 ; });
      Assert::ExpectException<exception>([&] { object2  * string1 ; });
      Assert::ExpectException<exception>([&] { object2  * string2 ; });
      Assert::ExpectException<exception>([&] { string1  * array1  ; });
      Assert::ExpectException<exception>([&] { string1  * array2  ; });
      Assert::ExpectException<exception>([&] { string1  * boolean1; });
      Assert::ExpectException<exception>([&] { string1  * boolean2; });
      Assert::AreEqual<Json>(null , string1  * null    );
      Assert::ExpectException<exception>([&] { string1  * number1 ; });
      Assert::ExpectException<exception>([&] { string1  * number2 ; });
      Assert::ExpectException<exception>([&] { string1  * numberi1; });
      Assert::ExpectException<exception>([&] { string1  * numberi2; });
      Assert::ExpectException<exception>([&] { string1  * object1 ; });
      Assert::ExpectException<exception>([&] { string1  * object2 ; });
      Assert::ExpectException<exception>([&] { string1  * string1 ; });
      Assert::ExpectException<exception>([&] { string1  * string2 ; });
      Assert::ExpectException<exception>([&] { string2  * array1  ; });
      Assert::ExpectException<exception>([&] { string2  * array2  ; });
      Assert::ExpectException<exception>([&] { string2  * boolean1; });
      Assert::ExpectException<exception>([&] { string2  * boolean2; });
      Assert::AreEqual<Json>(null , string2  * null    );
      Assert::ExpectException<exception>([&] { string2  * number1 ; });
      Assert::ExpectException<exception>([&] { string2  * number2 ; });
      Assert::ExpectException<exception>([&] { string2  * numberi1; });
      Assert::ExpectException<exception>([&] { string2  * numberi2; });
      Assert::ExpectException<exception>([&] { string2  * object1 ; });
      Assert::ExpectException<exception>([&] { string2  * object2 ; });
      Assert::ExpectException<exception>([&] { string2  * string1 ; });
      Assert::ExpectException<exception>([&] { string2  * string2 ; });
    }

    TEST_METHOD(TestOperatorMultiplyAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value *= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value *= boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value *= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value *= numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value *= boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value *= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value *= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value *= numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value *= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value *= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value *= boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value *= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value *= numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(string2, value);
    }

    TEST_METHOD(TestOperatorDivide)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   / array1  ; });
      Assert::ExpectException<exception>([&] { array1   / array2  ; });
      Assert::ExpectException<exception>([&] { array1   / boolean1; });
      Assert::ExpectException<exception>([&] { array1   / boolean2; });
      Assert::AreEqual<Json>(null    , array1   / null    );
      Assert::ExpectException<exception>([&] { array1   / number1 ; });
      Assert::ExpectException<exception>([&] { array1   / number2 ; });
      Assert::ExpectException<exception>([&] { array1   / numberi1; });
      Assert::ExpectException<exception>([&] { array1   / numberi2; });
      Assert::ExpectException<exception>([&] { array1   / object1 ; });
      Assert::ExpectException<exception>([&] { array1   / object2 ; });
      Assert::ExpectException<exception>([&] { array1   / string1 ; });
      Assert::ExpectException<exception>([&] { array1   / string2 ; });
      Assert::ExpectException<exception>([&] { array2   / array1  ; });
      Assert::ExpectException<exception>([&] { array2   / array2  ; });
      Assert::ExpectException<exception>([&] { array2   / boolean1; });
      Assert::ExpectException<exception>([&] { array2   / boolean2; });
      Assert::AreEqual<Json>(null    , array2   / null    );
      Assert::ExpectException<exception>([&] { array2   / number1 ; });
      Assert::ExpectException<exception>([&] { array2   / number2 ; });
      Assert::ExpectException<exception>([&] { array2   / numberi1; });
      Assert::ExpectException<exception>([&] { array2   / numberi2; });
      Assert::ExpectException<exception>([&] { array2   / object1 ; });
      Assert::ExpectException<exception>([&] { array2   / object2 ; });
      Assert::ExpectException<exception>([&] { array2   / string1 ; });
      Assert::ExpectException<exception>([&] { array2   / string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 / array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 / array2  ; });
      Assert::IsTrue(isnan((boolean1 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 / boolean2);
      Assert::AreEqual<Json>(null    , boolean1 / null    );
      Assert::IsTrue(isnan((boolean1 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 / number2 );
      Assert::IsTrue(isnan((boolean1 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 / numberi2);
      Assert::ExpectException<exception>([&] { boolean1 / object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 / object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 / string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 / string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 / array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 / array2  ; });
      Assert::IsTrue(isinf((boolean2 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(1.0, boolean2 / boolean2);
      Assert::AreEqual<Json>(null    , boolean2 / null    );
      Assert::IsTrue(isinf((boolean2 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, boolean2 / number2 );
      Assert::IsTrue(isinf((boolean2 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(1.0, boolean2 / numberi2);
      Assert::ExpectException<exception>([&] { boolean2 / object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 / object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 / string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 / string2 ; });
      Assert::AreEqual<Json>(null    , null     / array1  );
      Assert::AreEqual<Json>(null    , null     / array2  );
      Assert::AreEqual<Json>(null    , null     / boolean1);
      Assert::AreEqual<Json>(null    , null     / boolean2);
      Assert::AreEqual<Json>(null    , null     / null    );
      Assert::AreEqual<Json>(null    , null     / number1 );
      Assert::AreEqual<Json>(null    , null     / number2 );
      Assert::AreEqual<Json>(null    , null     / numberi1);
      Assert::AreEqual<Json>(null    , null     / numberi2);
      Assert::AreEqual<Json>(null    , null     / object1 );
      Assert::AreEqual<Json>(null    , null     / object2 );
      Assert::AreEqual<Json>(null    , null     / string1 );
      Assert::AreEqual<Json>(null    , null     / string2 );
      Assert::ExpectException<exception>([&] { number1  / array1  ; });
      Assert::ExpectException<exception>([&] { number1  / array2  ; });
      Assert::IsTrue(isnan((number1  / boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1  / boolean2);
      Assert::AreEqual<Json>(null    , number1  / null    );
      Assert::IsTrue(isnan((number1  / number1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1  / number2 );
      Assert::IsTrue(isnan((number1 / numberi1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1  / numberi2);
      Assert::ExpectException<exception>([&] { number1  / object1 ; });
      Assert::ExpectException<exception>([&] { number1  / object2 ; });
      Assert::ExpectException<exception>([&] { number1  / string1 ; });
      Assert::ExpectException<exception>([&] { number1  / string2 ; });
      Assert::ExpectException<exception>([&] { number2  / array1  ; });
      Assert::ExpectException<exception>([&] { number2  / array2  ; });
      Assert::IsTrue(isinf((number2  / boolean1).Get<double>()));
      Assert::AreEqual<Json>(1.0, number2  / boolean2);
      Assert::AreEqual<Json>(null    , number2  / null    );
      Assert::IsTrue(isinf((number2  / number1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, number2  / number2 );
      Assert::IsTrue(isinf((number2 / numberi1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, number2  / numberi2);
      Assert::ExpectException<exception>([&] { number2  / object1 ; });
      Assert::ExpectException<exception>([&] { number2  / object2 ; });
      Assert::ExpectException<exception>([&] { number2  / string1 ; });
      Assert::ExpectException<exception>([&] { number2  / string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 / array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 / array2  ; });
      Assert::IsTrue(isnan((numberi1 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 / boolean2);
      Assert::AreEqual<Json>(null    , numberi1 / null    );
      Assert::IsTrue(isnan((numberi1 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 / number2 );
      Assert::IsTrue(isnan((numberi1 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 / numberi2);
      Assert::ExpectException<exception>([&] { numberi1 / object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 / object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 / string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 / string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 / array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 / array2  ; });
      Assert::IsTrue(isinf((numberi2 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(1.0, numberi2 / boolean2);
      Assert::AreEqual<Json>(null    , numberi2 / null    );
      Assert::IsTrue(isinf((numberi2 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, numberi2 / number2 );
      Assert::IsTrue(isinf((numberi2 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(1.0, numberi2 / numberi2);
      Assert::ExpectException<exception>([&] { numberi2 / object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 / object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 / string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 / string2 ; });
      Assert::ExpectException<exception>([&] { object1  / array1  ; });
      Assert::ExpectException<exception>([&] { object1  / array2  ; });
      Assert::ExpectException<exception>([&] { object1  / boolean1; });
      Assert::ExpectException<exception>([&] { object1  / boolean2; });
      Assert::AreEqual<Json>(null    , object1  / null    );
      Assert::ExpectException<exception>([&] { object1  / number1 ; });
      Assert::ExpectException<exception>([&] { object1  / number2 ; });
      Assert::ExpectException<exception>([&] { object1  / numberi1; });
      Assert::ExpectException<exception>([&] { object1  / numberi2; });
      Assert::ExpectException<exception>([&] { object1  / object1 ; });
      Assert::ExpectException<exception>([&] { object1  / object2 ; });
      Assert::ExpectException<exception>([&] { object1  / string1 ; });
      Assert::ExpectException<exception>([&] { object1  / string2 ; });
      Assert::ExpectException<exception>([&] { object2  / array1  ; });
      Assert::ExpectException<exception>([&] { object2  / array2  ; });
      Assert::ExpectException<exception>([&] { object2  / boolean1; });
      Assert::ExpectException<exception>([&] { object2  / boolean2; });
      Assert::AreEqual<Json>(null    , object2  / null    );
      Assert::ExpectException<exception>([&] { object2  / number1 ; });
      Assert::ExpectException<exception>([&] { object2  / number2 ; });
      Assert::ExpectException<exception>([&] { object2  / numberi1; });
      Assert::ExpectException<exception>([&] { object2  / numberi2; });
      Assert::ExpectException<exception>([&] { object2  / object1 ; });
      Assert::ExpectException<exception>([&] { object2  / object2 ; });
      Assert::ExpectException<exception>([&] { object2  / string1 ; });
      Assert::ExpectException<exception>([&] { object2  / string2 ; });
      Assert::ExpectException<exception>([&] { string1  / array1  ; });
      Assert::ExpectException<exception>([&] { string1  / array2  ; });
      Assert::ExpectException<exception>([&] { string1  / boolean1; });
      Assert::ExpectException<exception>([&] { string1  / boolean2; });
      Assert::AreEqual<Json>(null    , string1  / null    );
      Assert::ExpectException<exception>([&] { string1  / number1 ; });
      Assert::ExpectException<exception>([&] { string1  / number2 ; });
      Assert::ExpectException<exception>([&] { string1  / numberi1; });
      Assert::ExpectException<exception>([&] { string1  / numberi2; });
      Assert::ExpectException<exception>([&] { string1  / object1 ; });
      Assert::ExpectException<exception>([&] { string1  / object2 ; });
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString0"s)), string1  / string1 );
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString1"s)), string1  / string2 );
      Assert::ExpectException<exception>([&] { string2  / array1  ; });
      Assert::ExpectException<exception>([&] { string2  / array2  ; });
      Assert::ExpectException<exception>([&] { string2  / boolean1; });
      Assert::ExpectException<exception>([&] { string2  / boolean2; });
      Assert::AreEqual<Json>(null    , string2  / null    );
      Assert::ExpectException<exception>([&] { string2  / number1 ; });
      Assert::ExpectException<exception>([&] { string2  / number2 ; });
      Assert::ExpectException<exception>([&] { string2  / numberi1; });
      Assert::ExpectException<exception>([&] { string2  / numberi2; });
      Assert::ExpectException<exception>([&] { string2  / object1 ; });
      Assert::ExpectException<exception>([&] { string2  / object2 ; });
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString0"s)), string2  / string1 );
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString1"s)), string2  / string2 );
    }

    TEST_METHOD(TestOperatorDivideAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::IsTrue(isnan((value /= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value /= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::IsTrue(isnan((value /= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value /= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::IsTrue(isnan((value /= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value /= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::IsTrue(isinf((value /= boolean1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value /= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::IsTrue(isinf((value /= number1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value /= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::IsTrue(isinf((value /= numberi1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value /= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::IsTrue(isnan((value /= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value /= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::IsTrue(isnan((value /= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value /= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::IsTrue(isnan((value /= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value /= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::IsTrue(isinf((value /= boolean1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(1.0, value /= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::IsTrue(isinf((value /= number1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(1.0, value /= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::IsTrue(isinf((value /= numberi1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(1.0, value /= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::IsTrue(isnan((value /= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value /= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::IsTrue(isnan((value /= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value /= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::IsTrue(isnan((value /= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value /= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::IsTrue(isinf((value /= boolean1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value /= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::IsTrue(isinf((value /= number1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value /= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::IsTrue(isinf((value /= numberi1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value /= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString0"s)), value /= string1);
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString0"s)), value);
      value = string1;
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString1"s)), value /= string2);
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString1"s)), value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString0"s)), value /= string1);
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString0"s)), value);
      value = string2;
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString1"s)), value /= string2);
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString1"s)), value);
    }

    TEST_METHOD(TestOperatorModulo)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   % array1  ; });
      Assert::ExpectException<exception>([&] { array1   % array2  ; });
      Assert::ExpectException<exception>([&] { array1   % boolean1; });
      Assert::ExpectException<exception>([&] { array1   % boolean2; });
      Assert::AreEqual<Json>(null, array1 % null);
      Assert::ExpectException<exception>([&] { array1   % number1 ; });
      Assert::ExpectException<exception>([&] { array1   % number2 ; });
      Assert::ExpectException<exception>([&] { array1   % numberi1; });
      Assert::ExpectException<exception>([&] { array1   % numberi2; });
      Assert::ExpectException<exception>([&] { array1   % object1 ; });
      Assert::ExpectException<exception>([&] { array1   % object2 ; });
      Assert::ExpectException<exception>([&] { array1   % string1 ; });
      Assert::ExpectException<exception>([&] { array1   % string2 ; });
      Assert::ExpectException<exception>([&] { array2   % array1  ; });
      Assert::ExpectException<exception>([&] { array2   % array2  ; });
      Assert::ExpectException<exception>([&] { array2   % boolean1; });
      Assert::ExpectException<exception>([&] { array2   % boolean2; });
      Assert::AreEqual<Json>(null, array2 % null);
      Assert::ExpectException<exception>([&] { array2   % number1 ; });
      Assert::ExpectException<exception>([&] { array2   % number2 ; });
      Assert::ExpectException<exception>([&] { array2   % numberi1; });
      Assert::ExpectException<exception>([&] { array2   % numberi2; });
      Assert::ExpectException<exception>([&] { array2   % object1 ; });
      Assert::ExpectException<exception>([&] { array2   % object2 ; });
      Assert::ExpectException<exception>([&] { array2   % string1 ; });
      Assert::ExpectException<exception>([&] { array2   % string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 % array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 % array2  ; });
      Assert::IsTrue(isnan((boolean1 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 % boolean2);
      Assert::AreEqual<Json>(null, boolean1 % null);
      Assert::IsTrue(isnan((boolean1 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 % number2);
      Assert::IsTrue(isnan((boolean1 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 % numberi2);
      Assert::ExpectException<exception>([&] { boolean1 % object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 % object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 % string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 % string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 % array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 % array2  ; });
      Assert::IsTrue(isnan((boolean2 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean2 % boolean2);
      Assert::AreEqual<Json>(null, boolean2 % null);
      Assert::IsTrue(isnan((boolean2 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean2 % number2);
      Assert::IsTrue(isnan((boolean2 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean2 % numberi2);
      Assert::ExpectException<exception>([&] { boolean2 % object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 % object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 % string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 % string2 ; });
      Assert::AreEqual<Json>(null, null % array1);
      Assert::AreEqual<Json>(null, null % array2);
      Assert::AreEqual<Json>(null, null % boolean1);
      Assert::AreEqual<Json>(null, null % boolean2);
      Assert::AreEqual<Json>(null, null % null);
      Assert::AreEqual<Json>(null, null % number1);
      Assert::AreEqual<Json>(null, null % number2);
      Assert::AreEqual<Json>(null, null % numberi1);
      Assert::AreEqual<Json>(null, null % numberi2);
      Assert::AreEqual<Json>(null, null % object1);
      Assert::AreEqual<Json>(null, null % object2);
      Assert::AreEqual<Json>(null, null % string1);
      Assert::AreEqual<Json>(null, null % string2);
      Assert::ExpectException<exception>([&] { number1  % array1  ; });
      Assert::ExpectException<exception>([&] { number1  % array2  ; });
      Assert::IsTrue(isnan((number1 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1 % boolean2);
      Assert::AreEqual<Json>(null, number1 % null);
      Assert::IsTrue(isnan((number1 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1 % number2);
      Assert::IsTrue(isnan((number1 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1 % numberi2);
      Assert::ExpectException<exception>([&] { number1  % object1 ; });
      Assert::ExpectException<exception>([&] { number1  % object2 ; });
      Assert::ExpectException<exception>([&] { number1  % string1 ; });
      Assert::ExpectException<exception>([&] { number1  % string2 ; });
      Assert::ExpectException<exception>([&] { number2  % array1  ; });
      Assert::ExpectException<exception>([&] { number2  % array2  ; });
      Assert::IsTrue(isnan((number2 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number2 % boolean2);
      Assert::AreEqual<Json>(null, number2 % null);
      Assert::IsTrue(isnan((number2 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number2 % number2);
      Assert::IsTrue(isnan((number2 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number2 % numberi2);
      Assert::ExpectException<exception>([&] { number2  % object1 ; });
      Assert::ExpectException<exception>([&] { number2  % object2 ; });
      Assert::ExpectException<exception>([&] { number2  % string1 ; });
      Assert::ExpectException<exception>([&] { number2  % string2 ; });
      Assert::ExpectException<exception>([&] { numberi1  % array1  ; });
      Assert::ExpectException<exception>([&] { numberi1  % array2  ; });
      Assert::IsTrue(isnan((numberi1 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 % boolean2);
      Assert::AreEqual<Json>(null, numberi1 % null);
      Assert::IsTrue(isnan((numberi1 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 % number2);
      Assert::IsTrue(isnan((numberi1 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 % numberi2);
      Assert::ExpectException<exception>([&] { numberi1  % object1 ; });
      Assert::ExpectException<exception>([&] { numberi1  % object2 ; });
      Assert::ExpectException<exception>([&] { numberi1  % string1 ; });
      Assert::ExpectException<exception>([&] { numberi1  % string2 ; });
      Assert::ExpectException<exception>([&] { numberi2  % array1  ; });
      Assert::ExpectException<exception>([&] { numberi2  % array2  ; });
      Assert::IsTrue(isnan((numberi2 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi2 % boolean2);
      Assert::AreEqual<Json>(null, numberi2 % null);
      Assert::IsTrue(isnan((numberi2 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi2 % number2);
      Assert::IsTrue(isnan((numberi2 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi2 % numberi2);
      Assert::ExpectException<exception>([&] { numberi2  % object1 ; });
      Assert::ExpectException<exception>([&] { numberi2  % object2 ; });
      Assert::ExpectException<exception>([&] { numberi2  % string1 ; });
      Assert::ExpectException<exception>([&] { numberi2  % string2 ; });
      Assert::ExpectException<exception>([&] { object1  % array1  ; });
      Assert::ExpectException<exception>([&] { object1  % array2  ; });
      Assert::ExpectException<exception>([&] { object1  % boolean1; });
      Assert::ExpectException<exception>([&] { object1  % boolean2; });
      Assert::AreEqual<Json>(null, object1 % null);
      Assert::ExpectException<exception>([&] { object1  % number1 ; });
      Assert::ExpectException<exception>([&] { object1  % number2 ; });
      Assert::ExpectException<exception>([&] { object1  % numberi1; });
      Assert::ExpectException<exception>([&] { object1  % numberi2; });
      Assert::ExpectException<exception>([&] { object1  % object1 ; });
      Assert::ExpectException<exception>([&] { object1  % object2 ; });
      Assert::ExpectException<exception>([&] { object1  % string1 ; });
      Assert::ExpectException<exception>([&] { object1  % string2 ; });
      Assert::ExpectException<exception>([&] { object2  % array1  ; });
      Assert::ExpectException<exception>([&] { object2  % array2  ; });
      Assert::ExpectException<exception>([&] { object2  % boolean1; });
      Assert::ExpectException<exception>([&] { object2  % boolean2; });
      Assert::AreEqual<Json>(null, object2 % null);
      Assert::ExpectException<exception>([&] { object2  % number1 ; });
      Assert::ExpectException<exception>([&] { object2  % number2 ; });
      Assert::ExpectException<exception>([&] { object2  % numberi1; });
      Assert::ExpectException<exception>([&] { object2  % numberi2; });
      Assert::ExpectException<exception>([&] { object2  % object1 ; });
      Assert::ExpectException<exception>([&] { object2  % object2 ; });
      Assert::ExpectException<exception>([&] { object2  % string1 ; });
      Assert::ExpectException<exception>([&] { object2  % string2 ; });
      Assert::ExpectException<exception>([&] { string1  % array1  ; });
      Assert::ExpectException<exception>([&] { string1  % array2  ; });
      Assert::ExpectException<exception>([&] { string1  % boolean1; });
      Assert::ExpectException<exception>([&] { string1  % boolean2; });
      Assert::AreEqual<Json>(null, string1 % null);
      Assert::ExpectException<exception>([&] { string1  % number1 ; });
      Assert::ExpectException<exception>([&] { string1  % number2 ; });
      Assert::ExpectException<exception>([&] { string1  % numberi1; });
      Assert::ExpectException<exception>([&] { string1  % numberi2; });
      Assert::ExpectException<exception>([&] { string1  % object1 ; });
      Assert::ExpectException<exception>([&] { string1  % object2 ; });
      Assert::ExpectException<exception>([&] { string1  % string1 ; });
      Assert::ExpectException<exception>([&] { string1  % string2 ; });
      Assert::ExpectException<exception>([&] { string2  % array1  ; });
      Assert::ExpectException<exception>([&] { string2  % array2  ; });
      Assert::ExpectException<exception>([&] { string2  % boolean1; });
      Assert::ExpectException<exception>([&] { string2  % boolean2; });
      Assert::AreEqual<Json>(null, string2 % null);
      Assert::ExpectException<exception>([&] { string2  % number1 ; });
      Assert::ExpectException<exception>([&] { string2  % number2 ; });
      Assert::ExpectException<exception>([&] { string2  % numberi1; });
      Assert::ExpectException<exception>([&] { string2  % numberi2; });
      Assert::ExpectException<exception>([&] { string2  % object1 ; });
      Assert::ExpectException<exception>([&] { string2  % object2 ; });
      Assert::ExpectException<exception>([&] { string2  % string1 ; });
      Assert::ExpectException<exception>([&] { string2  % string2 ; });
    }

    TEST_METHOD(TestOperatorModuloAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(string2, value);
    }

    TEST_METHOD(TestOperatorNegate)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Assert::ExpectException<exception>([&] { -array; });
      Assert::AreEqual<Json>(-1i64, -boolean);
      Assert::AreEqual<Json>(null, -null);
      Assert::AreEqual<Json>(-1.0, -number);
      Assert::AreEqual<Json>(-1i64, -numberi);
      Assert::ExpectException<exception>([&] { -object; });
      Assert::ExpectException<exception>([&] { -string; });
    }

    TEST_METHOD(TestOperatorNot)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Assert::ExpectException<exception>([&] { !array; });
      Assert::AreEqual<Json>(!true, !boolean);
      Assert::AreEqual<Json>(!nullptr, !null);
      Assert::AreEqual<Json>(!1.0, !number);
      Assert::AreEqual<Json>(!1i64, !numberi);
      Assert::ExpectException<exception>([&] { !object; });
      Assert::ExpectException<exception>([&] { !string; });
    }

    TEST_METHOD(TestOperatorPreIncrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, ++value);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(2.0, ++value);
      Assert::AreEqual<Json>(2.0, value);
      value = numberi;
      Assert::AreEqual<Json>(2i64, ++value);
      Assert::AreEqual<Json>(2i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorPostIncrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, value++);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(1.0, value++);
      Assert::AreEqual<Json>(2.0, value);
      value = numberi;
      Assert::AreEqual<Json>(1i64, value++);
      Assert::AreEqual<Json>(2i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorPreDecrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, --value);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(0.0, --value);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi;
      Assert::AreEqual<Json>(0i64, --value);
      Assert::AreEqual<Json>(0i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorPostDecrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, value--);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(1.0, value--);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi;
      Assert::AreEqual<Json>(1i64, value--);
      Assert::AreEqual<Json>(0i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorLogicalAnd)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::IsTrue (array1   && array1  );
      Assert::IsTrue (array1   && array2  );
      Assert::IsFalse(array1   && boolean1);
      Assert::IsTrue (array1   && boolean2);
      Assert::IsFalse(array1   && null    );
      Assert::IsFalse(array1   && number1 );
      Assert::IsTrue (array1   && number2 );
      Assert::IsFalse(array1   && numberi1);
      Assert::IsTrue (array1   && numberi2);
      Assert::IsTrue (array1   && object1 );
      Assert::IsTrue (array1   && object2 );
      Assert::IsTrue (array1   && string1 );
      Assert::IsTrue (array1   && string2 );
      Assert::IsTrue (array2   && array1  );
      Assert::IsTrue (array2   && array2  );
      Assert::IsFalse(array2   && boolean1);
      Assert::IsTrue (array2   && boolean2);
      Assert::IsFalse(array2   && null    );
      Assert::IsFalse(array2   && number1 );
      Assert::IsTrue (array2   && number2 );
      Assert::IsFalse(array2   && numberi1);
      Assert::IsTrue (array2   && numberi2);
      Assert::IsTrue (array2   && object1 );
      Assert::IsTrue (array2   && object2 );
      Assert::IsTrue (array2   && string1 );
      Assert::IsTrue (array2   && string2 );
      Assert::IsFalse(boolean1 && array1  );
      Assert::IsFalse(boolean1 && array2  );
      Assert::IsFalse(boolean1 && boolean1);
      Assert::IsFalse(boolean1 && boolean2);
      Assert::IsFalse(boolean1 && null    );
      Assert::IsFalse(boolean1 && number1 );
      Assert::IsFalse(boolean1 && number2 );
      Assert::IsFalse(boolean1 && numberi1);
      Assert::IsFalse(boolean1 && numberi2);
      Assert::IsFalse(boolean1 && object1 );
      Assert::IsFalse(boolean1 && object2 );
      Assert::IsFalse(boolean1 && string1 );
      Assert::IsFalse(boolean1 && string2 );
      Assert::IsTrue (boolean2 && array1  );
      Assert::IsTrue (boolean2 && array2  );
      Assert::IsFalse(boolean2 && boolean1);
      Assert::IsTrue (boolean2 && boolean2);
      Assert::IsFalse(boolean2 && null    );
      Assert::IsFalse(boolean2 && number1 );
      Assert::IsTrue (boolean2 && number2 );
      Assert::IsFalse(boolean2 && numberi1);
      Assert::IsTrue (boolean2 && numberi2);
      Assert::IsTrue (boolean2 && object1 );
      Assert::IsTrue (boolean2 && object2 );
      Assert::IsTrue (boolean2 && string1 );
      Assert::IsTrue (boolean2 && string2 );
      Assert::IsFalse(null     && array1  );
      Assert::IsFalse(null     && array2  );
      Assert::IsFalse(null     && boolean1);
      Assert::IsFalse(null     && boolean2);
      Assert::IsFalse(null     && null    );
      Assert::IsFalse(null     && number1 );
      Assert::IsFalse(null     && number2 );
      Assert::IsFalse(null     && numberi1);
      Assert::IsFalse(null     && numberi2);
      Assert::IsFalse(null     && object1 );
      Assert::IsFalse(null     && object2 );
      Assert::IsFalse(null     && string1 );
      Assert::IsFalse(null     && string2 );
      Assert::IsFalse(number1  && array1  );
      Assert::IsFalse(number1  && array2  );
      Assert::IsFalse(number1  && boolean1);
      Assert::IsFalse(number1  && boolean2);
      Assert::IsFalse(number1  && null    );
      Assert::IsFalse(number1  && number1 );
      Assert::IsFalse(number1  && number2 );
      Assert::IsFalse(number1  && numberi1);
      Assert::IsFalse(number1  && numberi2);
      Assert::IsFalse(number1  && object1 );
      Assert::IsFalse(number1  && object2 );
      Assert::IsFalse(number1  && string1 );
      Assert::IsFalse(number1  && string2 );
      Assert::IsTrue (number2  && array1  );
      Assert::IsTrue (number2  && array2  );
      Assert::IsFalse(number2  && boolean1);
      Assert::IsTrue (number2  && boolean2);
      Assert::IsFalse(number2  && null    );
      Assert::IsFalse(number2  && number1 );
      Assert::IsTrue (number2  && number2 );
      Assert::IsFalse(number2  && numberi1);
      Assert::IsTrue (number2  && numberi2);
      Assert::IsTrue (number2  && object1 );
      Assert::IsTrue (number2  && object2 );
      Assert::IsTrue (number2  && string1 );
      Assert::IsTrue (number2  && string2 );
      Assert::IsFalse(numberi1 && array1  );
      Assert::IsFalse(numberi1 && array2  );
      Assert::IsFalse(numberi1 && boolean1);
      Assert::IsFalse(numberi1 && boolean2);
      Assert::IsFalse(numberi1 && null    );
      Assert::IsFalse(numberi1 && number1 );
      Assert::IsFalse(numberi1 && number2 );
      Assert::IsFalse(numberi1 && numberi1);
      Assert::IsFalse(numberi1 && numberi2);
      Assert::IsFalse(numberi1 && object1 );
      Assert::IsFalse(numberi1 && object2 );
      Assert::IsFalse(numberi1 && string1 );
      Assert::IsFalse(numberi1 && string2 );
      Assert::IsTrue (numberi2 && array1  );
      Assert::IsTrue (numberi2 && array2  );
      Assert::IsFalse(numberi2 && boolean1);
      Assert::IsTrue (numberi2 && boolean2);
      Assert::IsFalse(numberi2 && null    );
      Assert::IsFalse(numberi2 && number1 );
      Assert::IsTrue (numberi2 && number2 );
      Assert::IsFalse(numberi2 && numberi1);
      Assert::IsTrue (numberi2 && numberi2);
      Assert::IsTrue (numberi2 && object1 );
      Assert::IsTrue (numberi2 && object2 );
      Assert::IsTrue (numberi2 && string1 );
      Assert::IsTrue (numberi2 && string2 );
      Assert::IsTrue (object1  && array1  );
      Assert::IsTrue (object1  && array2  );
      Assert::IsFalse(object1  && boolean1);
      Assert::IsTrue (object1  && boolean2);
      Assert::IsFalse(object1  && null    );
      Assert::IsFalse(object1  && number1 );
      Assert::IsTrue (object1  && number2 );
      Assert::IsFalse(object1  && numberi1);
      Assert::IsTrue (object1  && numberi2);
      Assert::IsTrue (object1  && object1 );
      Assert::IsTrue (object1  && object2 );
      Assert::IsTrue (object1  && string1 );
      Assert::IsTrue (object1  && string2 );
      Assert::IsTrue (object2  && array1  );
      Assert::IsTrue (object2  && array2  );
      Assert::IsFalse(object2  && boolean1);
      Assert::IsTrue (object2  && boolean2);
      Assert::IsFalse(object2  && null    );
      Assert::IsFalse(object2  && number1 );
      Assert::IsTrue (object2  && number2 );
      Assert::IsFalse(object2  && numberi1);
      Assert::IsTrue (object2  && numberi2);
      Assert::IsTrue (object2  && object1 );
      Assert::IsTrue (object2  && object2 );
      Assert::IsTrue (object2  && string1 );
      Assert::IsTrue (object2  && string2 );
      Assert::IsTrue (string1  && array1  );
      Assert::IsTrue (string1  && array2  );
      Assert::IsFalse(string1  && boolean1);
      Assert::IsTrue (string1  && boolean2);
      Assert::IsFalse(string1  && null    );
      Assert::IsFalse(string1  && number1 );
      Assert::IsTrue (string1  && number2 );
      Assert::IsFalse(string1  && numberi1);
      Assert::IsTrue (string1  && numberi2);
      Assert::IsTrue (string1  && object1 );
      Assert::IsTrue (string1  && object2 );
      Assert::IsTrue (string1  && string1 );
      Assert::IsTrue (string1  && string2 );
      Assert::IsTrue (string2  && array1  );
      Assert::IsTrue (string2  && array2  );
      Assert::IsFalse(string2  && boolean1);
      Assert::IsTrue (string2  && boolean2);
      Assert::IsFalse(string2  && null    );
      Assert::IsFalse(string2  && number1 );
      Assert::IsTrue (string2  && number2 );
      Assert::IsFalse(string2  && numberi1);
      Assert::IsTrue (string2  && numberi2);
      Assert::IsTrue (string2  && object1 );
      Assert::IsTrue (string2  && object2 );
      Assert::IsTrue (string2  && string1 );
      Assert::IsTrue (string2  && string2 );
    }

    TEST_METHOD(TestOperatorLogicalOr)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::IsTrue (array1   || array1  );
      Assert::IsTrue (array1   || array2  );
      Assert::IsTrue (array1   || boolean1);
      Assert::IsTrue (array1   || boolean2);
      Assert::IsTrue (array1   || null    );
      Assert::IsTrue (array1   || number1 );
      Assert::IsTrue (array1   || number2 );
      Assert::IsTrue (array1   || numberi1);
      Assert::IsTrue (array1   || numberi2);
      Assert::IsTrue (array1   || object1 );
      Assert::IsTrue (array1   || object2 );
      Assert::IsTrue (array1   || string1 );
      Assert::IsTrue (array1   || string2 );
      Assert::IsTrue (array2   || array1  );
      Assert::IsTrue (array2   || array2  );
      Assert::IsTrue (array2   || boolean1);
      Assert::IsTrue (array2   || boolean2);
      Assert::IsTrue (array2   || null    );
      Assert::IsTrue (array2   || number1 );
      Assert::IsTrue (array2   || number2 );
      Assert::IsTrue (array2   || numberi1);
      Assert::IsTrue (array2   || numberi2);
      Assert::IsTrue (array2   || object1 );
      Assert::IsTrue (array2   || object2 );
      Assert::IsTrue (array2   || string1 );
      Assert::IsTrue (array2   || string2 );
      Assert::IsTrue (boolean1 || array1  );
      Assert::IsTrue (boolean1 || array2  );
      Assert::IsFalse(boolean1 || boolean1);
      Assert::IsTrue (boolean1 || boolean2);
      Assert::IsFalse(boolean1 || null    );
      Assert::IsFalse(boolean1 || number1 );
      Assert::IsTrue (boolean1 || number2 );
      Assert::IsFalse(boolean1 || numberi1);
      Assert::IsTrue (boolean1 || numberi2);
      Assert::IsTrue (boolean1 || object1 );
      Assert::IsTrue (boolean1 || object2 );
      Assert::IsTrue (boolean1 || string1 );
      Assert::IsTrue (boolean1 || string2 );
      Assert::IsTrue (boolean2 || array1  );
      Assert::IsTrue (boolean2 || array2  );
      Assert::IsTrue (boolean2 || boolean1);
      Assert::IsTrue (boolean2 || boolean2);
      Assert::IsTrue (boolean2 || null    );
      Assert::IsTrue (boolean2 || number1 );
      Assert::IsTrue (boolean2 || number2 );
      Assert::IsTrue (boolean2 || numberi1);
      Assert::IsTrue (boolean2 || numberi2);
      Assert::IsTrue (boolean2 || object1 );
      Assert::IsTrue (boolean2 || object2 );
      Assert::IsTrue (boolean2 || string1 );
      Assert::IsTrue (boolean2 || string2 );
      Assert::IsTrue (null     || array1  );
      Assert::IsTrue (null     || array2  );
      Assert::IsFalse(null     || boolean1);
      Assert::IsTrue (null     || boolean2);
      Assert::IsFalse(null     || null    );
      Assert::IsFalse(null     || number1 );
      Assert::IsTrue (null     || number2 );
      Assert::IsFalse(null     || numberi1);
      Assert::IsTrue (null     || numberi2);
      Assert::IsTrue (null     || object1 );
      Assert::IsTrue (null     || object2 );
      Assert::IsTrue (null     || string1 );
      Assert::IsTrue (null     || string2 );
      Assert::IsTrue (number1  || array1  );
      Assert::IsTrue (number1  || array2  );
      Assert::IsFalse(number1  || boolean1);
      Assert::IsTrue (number1  || boolean2);
      Assert::IsFalse(number1  || null    );
      Assert::IsFalse(number1  || number1 );
      Assert::IsTrue (number1  || number2 );
      Assert::IsFalse(number1  || numberi1);
      Assert::IsTrue (number1  || numberi2);
      Assert::IsTrue (number1  || object1 );
      Assert::IsTrue (number1  || object2 );
      Assert::IsTrue (number1  || string1 );
      Assert::IsTrue (number1  || string2 );
      Assert::IsTrue (number2  || array1  );
      Assert::IsTrue (number2  || array2  );
      Assert::IsTrue (number2  || boolean1);
      Assert::IsTrue (number2  || boolean2);
      Assert::IsTrue (number2  || null    );
      Assert::IsTrue (number2  || number1 );
      Assert::IsTrue (number2  || number2 );
      Assert::IsTrue (number2  || numberi1);
      Assert::IsTrue (number2  || numberi2);
      Assert::IsTrue (number2  || object1 );
      Assert::IsTrue (number2  || object2 );
      Assert::IsTrue (number2  || string1 );
      Assert::IsTrue (number2  || string2 );
      Assert::IsTrue (numberi1 || array1  );
      Assert::IsTrue (numberi1 || array2  );
      Assert::IsFalse(numberi1 || boolean1);
      Assert::IsTrue (numberi1 || boolean2);
      Assert::IsFalse(numberi1 || null    );
      Assert::IsFalse(numberi1 || number1 );
      Assert::IsTrue (numberi1 || number2 );
      Assert::IsFalse(numberi1 || numberi1);
      Assert::IsTrue (numberi1 || numberi2);
      Assert::IsTrue (numberi1 || object1 );
      Assert::IsTrue (numberi1 || object2 );
      Assert::IsTrue (numberi1 || string1 );
      Assert::IsTrue (numberi1 || string2 );
      Assert::IsTrue (numberi2 || array1  );
      Assert::IsTrue (numberi2 || array2  );
      Assert::IsTrue (numberi2 || boolean1);
      Assert::IsTrue (numberi2 || boolean2);
      Assert::IsTrue (numberi2 || null    );
      Assert::IsTrue (numberi2 || number1 );
      Assert::IsTrue (numberi2 || number2 );
      Assert::IsTrue (numberi2 || numberi1);
      Assert::IsTrue (numberi2 || numberi2);
      Assert::IsTrue (numberi2 || object1 );
      Assert::IsTrue (numberi2 || object2 );
      Assert::IsTrue (numberi2 || string1 );
      Assert::IsTrue (numberi2 || string2 );
      Assert::IsTrue (object1  || array1  );
      Assert::IsTrue (object1  || array2  );
      Assert::IsTrue (object1  || boolean1);
      Assert::IsTrue (object1  || boolean2);
      Assert::IsTrue (object1  || null    );
      Assert::IsTrue (object1  || number1 );
      Assert::IsTrue (object1  || number2 );
      Assert::IsTrue (object1  || numberi1);
      Assert::IsTrue (object1  || numberi2);
      Assert::IsTrue (object1  || object1 );
      Assert::IsTrue (object1  || object2 );
      Assert::IsTrue (object1  || string1 );
      Assert::IsTrue (object1  || string2 );
      Assert::IsTrue (object2  || array1  );
      Assert::IsTrue (object2  || array2  );
      Assert::IsTrue (object2  || boolean1);
      Assert::IsTrue (object2  || boolean2);
      Assert::IsTrue (object2  || null    );
      Assert::IsTrue (object2  || number1 );
      Assert::IsTrue (object2  || number2 );
      Assert::IsTrue (object2  || numberi1);
      Assert::IsTrue (object2  || numberi2);
      Assert::IsTrue (object2  || object1 );
      Assert::IsTrue (object2  || object2 );
      Assert::IsTrue (object2  || string1 );
      Assert::IsTrue (object2  || string2 );
      Assert::IsTrue (string1  || array1  );
      Assert::IsTrue (string1  || array2  );
      Assert::IsTrue (string1  || boolean1);
      Assert::IsTrue (string1  || boolean2);
      Assert::IsTrue (string1  || null    );
      Assert::IsTrue (string1  || number1 );
      Assert::IsTrue (string1  || number2 );
      Assert::IsTrue (string1  || numberi2);
      Assert::IsTrue (string1  || numberi2);
      Assert::IsTrue (string1  || object1 );
      Assert::IsTrue (string1  || object2 );
      Assert::IsTrue (string1  || string1 );
      Assert::IsTrue (string1  || string2 );
      Assert::IsTrue (string2  || array1  );
      Assert::IsTrue (string2  || array2  );
      Assert::IsTrue (string2  || boolean1);
      Assert::IsTrue (string2  || boolean2);
      Assert::IsTrue (string2  || null    );
      Assert::IsTrue (string2  || number1 );
      Assert::IsTrue (string2  || number2 );
      Assert::IsTrue (string2  || numberi1);
      Assert::IsTrue (string2  || numberi2);
      Assert::IsTrue (string2  || object1 );
      Assert::IsTrue (string2  || object2 );
      Assert::IsTrue (string2  || string1 );
      Assert::IsTrue (string2  || string2 );
    }
  };
}