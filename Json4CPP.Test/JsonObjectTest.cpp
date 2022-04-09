#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;
using namespace Json4CPP::Helper;

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

    TEST_METHOD(TestConstructorJson)
    {
      JsonObject object;

      auto json = Json();
      Assert::ExpectException<exception>([&]() { object = json; });
      Assert::AreEqual<Json>(nullptr, json);
      json = nullptr;
      Assert::ExpectException<exception>([&]() { object = json; });
      Assert::AreEqual<Json>(nullptr, json);
      json = L"Test"s;
      Assert::ExpectException<exception>([&]() { object = json; });
      Assert::AreEqual<Json>(L"Test"s, json);
      json = true;
      Assert::ExpectException<exception>([&]() { object = json; });
      Assert::AreEqual<Json>(true, json);
      json = 1337;
      Assert::ExpectException<exception>([&]() { object = json; });
      Assert::AreEqual<Json>(1337, json);
      json = 13.37;
      Assert::ExpectException<exception>([&]() { object = json; });
      Assert::AreEqual<Json>(13.37, json);
      json = { { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } };
      object = json;
      Assert::AreEqual<Json>({ { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } }, object);
      Assert::AreEqual<Json>({ { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } }, json);
      json = { 1, 3, 3, 7 };
      Assert::ExpectException<exception>([&]() { object = json; });
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);

      json = Json();
      Assert::ExpectException<exception>([&]() { object = move(json); });
      Assert::AreEqual<Json>(nullptr, json);
      json = nullptr;
      Assert::ExpectException<exception>([&]() { object = move(json); });
      Assert::AreEqual<Json>(nullptr, json);
      json = L"Test"s;
      Assert::ExpectException<exception>([&]() { object = move(json); });
      Assert::AreEqual<Json>(L"Test"s, json);
      json = true;
      Assert::ExpectException<exception>([&]() { object = move(json); });
      Assert::AreEqual<Json>(true, json);
      json = 1337;
      Assert::ExpectException<exception>([&]() { object = move(json); });
      Assert::AreEqual<Json>(1337, json);
      json = 13.37;
      Assert::ExpectException<exception>([&]() { object = move(json); });
      Assert::AreEqual<Json>(13.37, json);
      json = { { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } };
      object = move(json);
      Assert::AreEqual<Json>({ { L"Key1"s, L"Value1"s, }, { L"Key2"s, L"Value2"s } }, object);
      Assert::AreEqual(0i64, json.Size());
      json = { 1, 3, 3, 7 };
      Assert::ExpectException<exception>([&]() { object = move(json); });
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      JsonObject object;

      auto builder = JsonBuilder();
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);

      // JsonBuilder from VALUE const&
      auto value = VALUE(nullptr);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"Test"s);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual<VALUE>(L"Test"s, value);
      value = VALUE(true);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(1337, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(value);
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual<VALUE>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, value);
      value = VALUE(JsonArray{ 1, 3, 3, 7 });
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<VALUE>(JsonArray{ 1, 3, 3, 7 }, value);

      // JsonBuilder from VALUE const&
      value = VALUE(nullptr);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"Test"s);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual<VALUE>(L""s, value);
      value = VALUE(true);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(1337, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(move(value));
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual<VALUE>(JsonObject(), value);
      value = VALUE(JsonArray{ 1, 3, 3, 7 });
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<VALUE>(JsonArray(), value);

      // JsonBuilder constructors
      builder = JsonBuilder(nullptr);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(L"Test");
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test", builder);
      auto str = L"Test"s;
      builder = JsonBuilder(str);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual(L"Test"s, str);
      str = L"Test"s;
      builder = JsonBuilder(move(str));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual(L""s, str);
      builder = JsonBuilder(true);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder('A');
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>('A', builder);
      builder = JsonBuilder(1i8);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(1i8, builder);
      builder = JsonBuilder(2ui8);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(2ui8, builder);
      builder = JsonBuilder(3i16);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(3i16, builder);
      builder = JsonBuilder(4ui16);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(4ui16, builder);
      builder = JsonBuilder(5i32);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(5i32, builder);
      builder = JsonBuilder(6ui32);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(6ui32, builder);
      builder = JsonBuilder(7i64);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(7i64, builder);
      builder = JsonBuilder(8ui64);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(8ui64, builder);
      builder = JsonBuilder(13.37f);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      builder = JsonBuilder(13.37);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      auto obj = JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(obj);
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, obj);
      obj = JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(move(obj));
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual<JsonObject>(JsonObject(), obj);
      auto arr = JsonArray{ 1, 3, 3, 7 };
      builder = JsonBuilder(arr);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 3, 3, 7 }, arr);
      arr = JsonArray{ 1, 3, 3, 7 };
      builder = JsonBuilder(move(arr));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<JsonArray>(JsonArray(), arr);
      auto json = Json();
      builder = JsonBuilder(json);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(Json(), builder);
      Assert::AreEqual(Json(), json);
      json = Json();
      builder = JsonBuilder(move(json));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(Json(), builder);
      Assert::AreEqual(Json(), json);
      json = Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(json);
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }), builder);
      Assert::AreEqual(Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }), json);
      json = Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(move(json));
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }), builder);
      Assert::AreEqual(Json(JsonObject()), json);

      // JsonBuilder from initializer_list
      builder = JsonBuilder({ L"Key1", L"Value1" });
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>({ L"Key1", L"Value1" }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" } });
      object = builder;
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1", L"Value2" } });
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1", L"Value2" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } });
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>({ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } }, builder);

      // JsonBuilder from vector
      auto vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(vec);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"Key1", L"Value1" }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(vec);
      object = builder;
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" } }, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value3" } };
      builder = JsonBuilder(vec);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value3" }}, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(vec);
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(vec);
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(move(vec));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"Key1", L"Value1" }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(move(vec));
      object = builder;
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value3" } };
      builder = JsonBuilder(move(vec));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value3" }}, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(move(vec));
      object = builder;
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(move(vec));
      Assert::ExpectException<exception>([&]() { object = builder; });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } }, builder);
      Assert::AreEqual(0ui64, vec.size());

      // Move
      builder = JsonBuilder();
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);

      // JsonBuilder from VALUE const&
      value = VALUE(nullptr);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"Test"s);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual<VALUE>(L"Test"s, value);
      value = VALUE(true);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(1337, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(value);
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<VALUE>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, value);
      value = VALUE(JsonArray{ 1, 3, 3, 7 });
      builder = JsonBuilder(value);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<VALUE>(JsonArray{ 1, 3, 3, 7 }, value);

      // JsonBuilder from VALUE const&
      value = VALUE(nullptr);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      Assert::AreEqual<VALUE>(nullptr, value);
      value = VALUE(L"Test"s);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual<VALUE>(L""s, value);
      value = VALUE(true);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(true, builder);
      Assert::AreEqual<VALUE>(true, value);
      value = VALUE(13.37);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      Assert::AreEqual<VALUE>(13.37, value);
      value = VALUE(1337i64);
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(1337, builder);
      Assert::AreEqual<VALUE>(1337i64, value);
      value = VALUE(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(move(value));
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<VALUE>(JsonObject(), value);
      value = VALUE(JsonArray{ 1, 3, 3, 7 });
      builder = JsonBuilder(move(value));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<VALUE>(JsonArray(), value);

      // JsonBuilder constructors
      builder = JsonBuilder(nullptr);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(nullptr, builder);
      builder = JsonBuilder(L"Test");
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test", builder);
      str = L"Test"s;
      builder = JsonBuilder(str);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual(L"Test"s, str);
      str = L"Test"s;
      builder = JsonBuilder(move(str));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(L"Test"s, builder);
      Assert::AreEqual(L""s, str);
      builder = JsonBuilder(true);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(true, builder);
      builder = JsonBuilder('A');
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>('A', builder);
      builder = JsonBuilder(1i8);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(1i8, builder);
      builder = JsonBuilder(2ui8);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(2ui8, builder);
      builder = JsonBuilder(3i16);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(3i16, builder);
      builder = JsonBuilder(4ui16);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(4ui16, builder);
      builder = JsonBuilder(5i32);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(5i32, builder);
      builder = JsonBuilder(6ui32);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(6ui32, builder);
      builder = JsonBuilder(7i64);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(7i64, builder);
      builder = JsonBuilder(8ui64);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(8ui64, builder);
      builder = JsonBuilder(13.37f);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(13.37f, builder);
      builder = JsonBuilder(13.37);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(13.37, builder);
      obj = JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(obj);
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, obj);
      obj = JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(move(obj));
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(JsonObject(), builder);
      Assert::AreEqual<JsonObject>(JsonObject(), obj);
      arr = JsonArray{ 1, 3, 3, 7 };
      builder = JsonBuilder(arr);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 3, 3, 7 }, arr);
      arr = JsonArray{ 1, 3, 3, 7 };
      builder = JsonBuilder(move(arr));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(JsonArray{ 1, 3, 3, 7 }, builder);
      Assert::AreEqual<JsonArray>(JsonArray(), arr);
      json = Json();
      builder = JsonBuilder(json);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(Json(), builder);
      Assert::AreEqual(Json(), json);
      json = Json();
      builder = JsonBuilder(move(json));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(Json(), builder);
      Assert::AreEqual(Json(), json);
      json = Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(json);
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(Json(JsonObject()), builder);
      Assert::AreEqual(Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }), json);
      json = Json(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      builder = JsonBuilder(move(json));
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(Json(JsonObject()), builder);
      Assert::AreEqual(Json(JsonObject()), json);

      // JsonBuilder from initializer_list
      builder = JsonBuilder({ L"Key1", L"Value1" });
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>({ L"", L"Value1" }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" } });
      object = move(builder);
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<JsonBuilder>({ { L"", L"" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1", L"Value2" } });
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>({ vector<JsonBuilder>{  } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } });
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>({ { L"", L"" }, { L"", L"" } }, builder);
      builder = JsonBuilder({ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } });
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>({ { L"", L"" }, vector<JsonBuilder>{ } }, builder);

      // JsonBuilder from vector
      vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(vec);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"", L"Value1" }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(vec);
      object = move(builder);
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" } }, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value3" } };
      builder = JsonBuilder(vec);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>({ vector<JsonBuilder>{ } }, builder);
      Assert::AreEqual(1ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(vec);
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, { L"", L"" } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(vec);
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, vector<JsonBuilder>{ } }, builder);
      Assert::AreEqual(2ui64, vec.size());
      vec = vector<JsonBuilder>{ L"Key1", L"Value1" };
      builder = JsonBuilder(move(vec));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ L"", L"Value1" }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" } };
      builder = JsonBuilder(move(vec));
      object = move(builder);
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1", L"Value3" } };
      builder = JsonBuilder(move(vec));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>({ vector<JsonBuilder>{ } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } };
      builder = JsonBuilder(move(vec));
      object = move(builder);
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(L"Value1", object[L"Key1"]);
      Assert::AreEqual<Json>(L"Value2", object[L"Key2"]);
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, { L"", L"" } }, builder);
      Assert::AreEqual(0ui64, vec.size());
      vec = vector<JsonBuilder>{ { L"Key1", L"Value1" }, { L"Key2", L"Value2", L"Value3" } };
      builder = JsonBuilder(move(vec));
      Assert::ExpectException<exception>([&]() { object = move(builder); });
      Assert::AreEqual<JsonBuilder>(vector<JsonBuilder>{ { L"", L"" }, vector<JsonBuilder>{ } }, builder);
      Assert::AreEqual(0ui64, vec.size());
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      JsonObject object;

      object = { { L"Key1", 1 } };
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);

      Assert::ExpectException<exception>([&]() { object = { { L"Key1", 1, 2 } }; });

      object = { { L"Key1", 1 }, { L"Key2", 2 } };
      Assert::AreEqual(2i64, object.Size());
      Assert::AreEqual(L"Key1"s, object.Keys()[0]);
      Assert::AreEqual(L"Key2"s, object.Keys()[1]);
      Assert::AreEqual<Json>(1, object[L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Key2"]);

      Assert::ExpectException<exception>([&]() { object = { { L"Key1", 1 }, { L"Key2", 2, 3 } }; });

      object = {
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
      Assert::AreEqual(7i64, object.Size());
      Assert::AreEqual(L"Null"s, object.Keys()[0]);
      Assert::AreEqual(L"String"s, object.Keys()[1]);
      Assert::AreEqual(L"Boolean"s, object.Keys()[2]);
      Assert::AreEqual(L"Real"s, object.Keys()[3]);
      Assert::AreEqual(L"Integer"s, object.Keys()[4]);
      Assert::AreEqual(L"Object"s, object.Keys()[5]);
      Assert::AreEqual(L"Array"s, object.Keys()[6]);
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(L"Test", object[L"String"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(13.37, object[L"Real"]);
      Assert::AreEqual<Json>(1337, object[L"Integer"]);
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

    TEST_METHOD(TestDump)
    {
      JsonObject object = {
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

      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Real\":13.37,\"Integer\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump());
      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Real\":13.37,\"Integer\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump(0));
      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Real\":13.37,\"Integer\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump(0, L' '));
      Assert::AreEqual(L"{\"Null\":null,\"String\":\"Test\",\"Boolean\":true,\"Real\":13.37,\"Integer\":1337,\"Object\":{\"Key1\":1,\"Key2\":2},\"Array\":[1,2,3]}"s, object.Dump(0, L'\t'));

      Assert::AreEqual(
        L"{"                            "\r\n"
         " \"Null\": null,"             "\r\n"
         " \"String\": \"Test\","       "\r\n"
         " \"Boolean\": true,"          "\r\n"
         " \"Real\": 13.37,"            "\r\n"
         " \"Integer\": 1337,"          "\r\n"
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
         " \"Real\": 13.37,"            "\r\n"
         " \"Integer\": 1337,"          "\r\n"
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
         "\t\"Real\": 13.37,"           "\r\n"
         "\t\"Integer\": 1337,"         "\r\n"
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
         "  \"Real\": 13.37,"           "\r\n"
         "  \"Integer\": 1337,"         "\r\n"
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
         "  \"Real\": 13.37,"           "\r\n"
         "  \"Integer\": 1337,"         "\r\n"
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
         "\t\t\"Real\": 13.37,"         "\r\n"
         "\t\t\"Integer\": 1337,"       "\r\n"
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
      Assert::AreEqual(4i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 } }.Size());
      Assert::AreEqual(5i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 }, { L"Integer", 1337 } }.Size());
      Assert::AreEqual(6i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 }, { L"Integer", 1337 }, { L"Object", {{ L"Key1", 1 }, { L"Key2", 2 } } } }.Size());
      Assert::AreEqual(7i64, JsonObject{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 }, { L"Integer", 1337 }, { L"Object", {{ L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } }.Size());
    }

    TEST_METHOD(TestCount)
    {
      JsonObject object = {
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

      Assert::AreEqual(0i64, object.Count(L"asd"));
      Assert::AreEqual(0i64, object.Count(L"asd"s));
      for (auto& key : object.Keys())
      {
        Assert::AreEqual(1i64, object.Count(key));
      }
    }

    TEST_METHOD(TestClear)
    {
      auto object = JsonObject{ { L"Key1", 1 }, { L"Key2", 2 } };
      Assert::AreEqual(2i64, object.Size());
      object.Clear();
      Assert::AreEqual(0i64, object.Size());
    }

    TEST_METHOD(TestInsert)
    {
      JsonObject object;
      pair<wstring, Json> null = { L"Null"s, nullptr };
      pair<wstring, Json> str = { L"String"s, L"Test"s };
      pair<wstring, Json> boolean = { L"Boolean"s, true };
      pair<wstring, Json> real = { L"Real"s, 13.37 };
      pair<wstring, Json> integer = { L"Integer"s, 1337 };
      pair<wstring, Json> obj = { L"Object"s, { { L"Key1", 1 }, { L"Key2", 2 } } };
      pair<wstring, Json> arr = { L"Array"s, { 1, 2, 3 } };

      Assert::IsTrue (object.Insert(null));
      Assert::IsFalse(object.Insert(null));
      Assert::IsTrue (object.Insert(str));
      Assert::IsFalse(object.Insert(str));
      Assert::IsTrue (object.Insert(boolean));
      Assert::IsFalse(object.Insert(boolean));
      Assert::IsTrue (object.Insert(real));
      Assert::IsFalse(object.Insert(real));
      Assert::IsTrue (object.Insert(integer));
      Assert::IsFalse(object.Insert(integer));
      Assert::IsTrue (object.Insert(obj));
      Assert::IsFalse(object.Insert(obj));
      Assert::IsTrue (object.Insert(arr));
      Assert::IsFalse(object.Insert(arr));
      Assert::AreEqual(7i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(L"Test"s, object[L"String"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(13.37, object[L"Real"]);
      Assert::AreEqual<Json>(1337, object[L"Integer"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
      Assert::AreEqual<pair<wstring, Json>>({ L"Null"s, nullptr }, null);
      Assert::AreEqual<pair<wstring, Json>>({ L"String"s, L"Test"s }, str);
      Assert::AreEqual<pair<wstring, Json>>({ L"Boolean"s, true }, boolean);
      Assert::AreEqual<pair<wstring, Json>>({ L"Real"s, 13.37 }, real);
      Assert::AreEqual<pair<wstring, Json>>({ L"Integer"s, 1337 }, integer);
      Assert::AreEqual<pair<wstring, Json>>({ L"Object"s, { { L"Key1", 1 }, { L"Key2", 2 } } }, obj);
      Assert::AreEqual<pair<wstring, Json>>({ L"Array"s, { 1, 2, 3 } }, arr);

      Assert::IsFalse(object.Insert(move(null)));
      Assert::IsFalse(object.Insert(move(str)));
      Assert::IsFalse(object.Insert(move(boolean)));
      Assert::IsFalse(object.Insert(move(real)));
      Assert::IsFalse(object.Insert(move(integer)));
      Assert::IsFalse(object.Insert(move(obj)));
      Assert::IsFalse(object.Insert(move(arr)));
      object.Clear();
      Assert::IsTrue(object.Insert(move(null)));
      Assert::IsTrue(object.Insert(move(str)));
      Assert::IsTrue(object.Insert(move(boolean)));
      Assert::IsTrue(object.Insert(move(real)));
      Assert::IsTrue(object.Insert(move(integer)));
      Assert::IsTrue(object.Insert(move(obj)));
      Assert::IsTrue(object.Insert(move(arr)));
      Assert::AreEqual(7i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(L"Test"s, object[L"String"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(13.37, object[L"Real"]);
      Assert::AreEqual<Json>(1337, object[L"Integer"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
      Assert::AreEqual<pair<wstring, Json>>({ L""s, nullptr }, null);
      Assert::AreEqual<pair<wstring, Json>>({ L""s, L""s }, str);
      Assert::AreEqual<pair<wstring, Json>>({ L""s, true }, boolean);
      Assert::AreEqual<pair<wstring, Json>>({ L""s, 13.37 }, real);
      Assert::AreEqual<pair<wstring, Json>>({ L""s, 1337 }, integer);
      Assert::AreEqual<pair<wstring, Json>>({ L""s, JsonObject{ } }, obj);
      Assert::AreEqual<pair<wstring, Json>>({ L""s, JsonArray{ } }, arr);
    }

    TEST_METHOD(TestErase)
    {
      JsonObject object = {
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
      object.Erase(L"String");
      object.Erase(L"Real");
      Assert::AreEqual(5i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(1337, object[L"Integer"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
    }

    TEST_METHOD(TestKeys)
    {
      JsonObject object = {
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
      Assert::AreEqual(7i64, object.Size());
      Assert::AreEqual(7ui64, object.Keys().size());
      Assert::AreEqual(L"Null"s, object.Keys()[0]);
      Assert::AreEqual(L"String"s, object.Keys()[1]);
      Assert::AreEqual(L"Boolean"s, object.Keys()[2]);
      Assert::AreEqual(L"Real"s, object.Keys()[3]);
      Assert::AreEqual(L"Integer"s, object.Keys()[4]);
      Assert::AreEqual(L"Object"s, object.Keys()[5]);
      Assert::AreEqual(L"Array"s, object.Keys()[6]);

      Assert::AreEqual(7ui64, object.KeysView().size());
      Assert::AreEqual(L"Null"s, object.KeysView()[0].get());
      Assert::AreEqual(L"String"s, object.KeysView()[1].get());
      Assert::AreEqual(L"Boolean"s, object.KeysView()[2].get());
      Assert::AreEqual(L"Real"s, object.KeysView()[3].get());
      Assert::AreEqual(L"Integer"s, object.KeysView()[4].get());
      Assert::AreEqual(L"Object"s, object.KeysView()[5].get());
      Assert::AreEqual(L"Array"s, object.KeysView()[6].get());
    }

    TEST_METHOD(TestOperatorSubscript)
    {
      JsonObject object = {
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
      Assert::AreEqual(7i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object[L"Null"]);
      Assert::AreEqual<Json>(L"Test"s, object[L"String"]);
      Assert::AreEqual<Json>(true, object[L"Boolean"]);
      Assert::AreEqual<Json>(13.37, object[L"Real"]);
      Assert::AreEqual<Json>(1337, object[L"Integer"]);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object[L"Object"]);
      Assert::AreEqual<Json>({ 1, 2, 3 }, object[L"Array"]);
      Assert::AreEqual<Json>(1, object[L"Object"][L"Key1"]);
      Assert::AreEqual<Json>(2, object[L"Object"][L"Key2"]);
      Assert::AreEqual<Json>(1, object[L"Array"][0]);
      Assert::AreEqual<Json>(2, object[L"Array"][1]);
      Assert::AreEqual<Json>(3, object[L"Array"][2]);

      for (auto& key : object.Keys())
      {
        object[key] = 1337;
        Assert::AreEqual<Json>(1337, object[key]);
      }

      object.Clear();
      Assert::AreEqual(0i64, object.Size());
      object[L"1337"] = 1337;
      Assert::AreEqual(1i64, object.Size());
      Assert::AreEqual<Json>(1337, object[L"1337"]);
    }

    TEST_METHOD(TestAt)
    {
      auto object = JsonObject{
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
      Assert::AreEqual(7i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object.At(L"Null"));
      Assert::AreEqual<Json>(L"Test"s, object.At(L"String"));
      Assert::AreEqual<Json>(true, object.At(L"Boolean"));
      Assert::AreEqual<Json>(13.37, object.At(L"Real"));
      Assert::AreEqual<Json>(1337, object.At(L"Integer"));
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object.At(L"Object"));
      Assert::AreEqual<Json>({ 1, 2, 3 }, object.At(L"Array"));
      Assert::AreEqual<Json>(1, object.At(L"Object").At(L"Key1"));
      Assert::AreEqual<Json>(2, object.At(L"Object").At(L"Key2"));
      Assert::AreEqual<Json>(1, object.At(L"Array").At(0));
      Assert::AreEqual<Json>(2, object.At(L"Array").At(1));
      Assert::AreEqual<Json>(3, object.At(L"Array").At(2));

      for (auto& key : object.Keys())
      {
        object.At(key) = 9999;
        Assert::AreEqual<Json>(9999, object.At(key));
      }
    }

    TEST_METHOD(TestAtConst)
    {
      auto const object = JsonObject{
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
      Assert::AreEqual(7i64, object.Size());
      Assert::AreEqual<Json>(nullptr, object.At(L"Null"));
      Assert::AreEqual<Json>(L"Test"s, object.At(L"String"));
      Assert::AreEqual<Json>(true, object.At(L"Boolean"));
      Assert::AreEqual<Json>(13.37, object.At(L"Real"));
      Assert::AreEqual<Json>(1337, object.At(L"Integer"));
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, object.At(L"Object"));
      Assert::AreEqual<Json>({ 1, 2, 3 }, object.At(L"Array"));
      Assert::AreEqual<Json>(1, object.At(L"Object").At(L"Key1"));
      Assert::AreEqual<Json>(2, object.At(L"Object").At(L"Key2"));
      Assert::AreEqual<Json>(1, object.At(L"Array").At(0));
      Assert::AreEqual<Json>(2, object.At(L"Array").At(1));
      Assert::AreEqual<Json>(3, object.At(L"Array").At(2));

      static_assert(is_const<remove_reference<decltype(object.At(L"Null"))>::type>::value, "JsonObject::At(KEY) return type must be Json const&");
    }

    TEST_METHOD(TestIterator)
    {
      auto object = JsonObject{
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
      auto map = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 }, { L"Integer", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto& [key, value] : object)
      {
        value = 1337;
      }
      for (auto& key : object.Keys())
      {
        Assert::AreEqual<Json>(1337, object[key]);
      }

      object = JsonObject{
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
      map = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 }, { L"Integer", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto [key, value] : object)
      {
        value = 1337;
      }
      for (auto& key : object.Keys())
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
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      auto const map = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 }, { L"Integer", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto [key, value] : object)
      {
        value = 1337;
      }
      for (auto& key : object.Keys())
      {
        Assert::AreEqual<Json>(map.at(key), object.At(key));
      }

      auto const& object2 = JsonObject{
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
      auto const& map2 = std::map<wstring, Json>{ { L"Null", nullptr }, { L"String", L"Test" }, { L"Boolean", true }, { L"Real", 13.37 }, { L"Integer", 1337 }, { L"Object", { { L"Key1", 1 }, { L"Key2", 2 } } }, { L"Array", { 1, 2, 3 } } };
      for (auto [key, value] : object2)
      {
        value = 1337;
      }
      for (auto& key : object2.Keys())
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
        { L"Real", 13.37 },
        { L"Integer", 1337 },
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
         "  \"Real\": 13.37,"           "\r\n"
         "  \"Integer\": 1337,"         "\r\n"
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
                    "  \"Real\": 13.37,"           "\r\n"
                    "  \"Integer\": 1337,"         "\r\n"
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
        { L"Real", 13.37 },
        { L"Integer", 1337 },
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
      for (auto& key : expected.Keys())
      {
        Assert::AreEqual(expected[key], result[key]);
      }

      ExpectException<exception>([&]() { wstringstream(L"0"s) >> result; }, "Expected token: StartObject!");
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
      JsonObject numberi1 = { { L"Key", 0 } };
      JsonObject numberi2 = { { L"Key", 1 } };
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
      JsonObject numberi1 = { { L"Key", 0 } };
      JsonObject numberi2 = { { L"Key", 1 } };
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

      Assert::IsTrue (JsonObject{ { L"Key", JsonArray{ 1, 2 } } } != JsonObject{ { L"Key", JsonArray{ 1 } } });
      Assert::IsFalse(JsonObject{ { L"Key", JsonArray{ 1, 2 } } } != JsonObject{ { L"Key", JsonArray{ 1, 2 } } });
      Assert::IsTrue (JsonObject{ { L"Key", JsonArray{ 1, 2 } } } != JsonObject{ { L"Key", JsonArray{ 1, 2, 3 } } });
    }
  };
}