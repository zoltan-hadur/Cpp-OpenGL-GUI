#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonTest)
  {
  public:
    TEST_METHOD(TestConstructorDefault)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::AreEqual<Json>(nullptr, json);
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      Json json;

      json = JsonBuilder();
      Assert::AreEqual<VALUE>(nullptr, json._value);

      // JsonBuilder from VALUE
      json = JsonBuilder(VALUE(nullptr_t()));
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(VALUE(L"TestString"s));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(VALUE(true));
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder(VALUE(13.37));
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(VALUE(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);

      // JsonBuilder constructors
      json = JsonBuilder(nullptr_t());
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(L"TestString");
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(L"TestString"s);
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(true);
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder((char)1);
      Assert::AreEqual<VALUE>(1.0, json._value);
      json = JsonBuilder((int8_t)2);
      Assert::AreEqual<VALUE>(2.0, json._value);
      json = JsonBuilder((uint8_t)3);
      Assert::AreEqual<VALUE>(3.0, json._value);
      json = JsonBuilder((int16_t)4);
      Assert::AreEqual<VALUE>(4.0, json._value);
      json = JsonBuilder((uint16_t)5);
      Assert::AreEqual<VALUE>(5.0, json._value);
      json = JsonBuilder((int32_t)6);
      Assert::AreEqual<VALUE>(6.0, json._value);
      json = JsonBuilder((uint32_t)7);
      Assert::AreEqual<VALUE>(7.0, json._value);
      json = JsonBuilder((int64_t)8);
      Assert::AreEqual<VALUE>(8.0, json._value);
      json = JsonBuilder((uint64_t)9);
      Assert::AreEqual<VALUE>(9.0, json._value);
      json = JsonBuilder(13.37f);
      Assert::AreEqual<VALUE>((double)13.37f, json._value);
      json = JsonBuilder(13.37);
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(JsonArray({ 1337, L"1337", true }));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      json = JsonBuilder(Json());
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(Json(nullptr_t()));
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(Json(L"TestString"));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(Json(L"TestString"s));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(Json(true));
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder(Json((char)1));
      Assert::AreEqual<VALUE>(1.0, json._value);
      json = JsonBuilder(Json((int8_t)2));
      Assert::AreEqual<VALUE>(2.0, json._value);
      json = JsonBuilder(Json((uint8_t)3));
      Assert::AreEqual<VALUE>(3.0, json._value);
      json = JsonBuilder(Json((int16_t)4));
      Assert::AreEqual<VALUE>(4.0, json._value);
      json = JsonBuilder(Json((uint16_t)5));
      Assert::AreEqual<VALUE>(5.0, json._value);
      json = JsonBuilder(Json((int32_t)6));
      Assert::AreEqual<VALUE>(6.0, json._value);
      json = JsonBuilder(Json((uint32_t)7));
      Assert::AreEqual<VALUE>(7.0, json._value);
      json = JsonBuilder(Json((int64_t)8));
      Assert::AreEqual<VALUE>(8.0, json._value);
      json = JsonBuilder(Json((uint64_t)9));
      Assert::AreEqual<VALUE>(9.0, json._value);
      json = JsonBuilder(Json(13.37f));
      Assert::AreEqual<VALUE>((double)13.37f, json._value);
      json = JsonBuilder(Json(13.37));
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(Json(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      json = JsonBuilder(Json(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
    }

    TEST_METHOD(TestLiteral)
    {
      Assert::AreEqual<Json>(nullptr, L"null"_json);
      Assert::AreEqual<Json>(L"TestString", L"\"TestString\""_json);
      Assert::AreEqual<Json>(true, L"true"_json);
      Assert::AreEqual<Json>(13.37, L"13.37"_json);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, L"{ \"Key1\": 1, \"Key2\": 2 }"_json);
      Assert::AreEqual<Json>({ 1, 2, 3 }, L"[ 1, 2, 3 ]"_json);
    }
  };
}