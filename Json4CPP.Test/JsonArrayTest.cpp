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
  };
}