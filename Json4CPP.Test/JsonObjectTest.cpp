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
  };
}