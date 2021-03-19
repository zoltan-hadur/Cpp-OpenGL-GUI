#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonIteratorTest)
  {
  public:
    TEST_METHOD(TestConstructor)
    {
      auto json0 = Json{ 1, 2, 3, 4 };
      auto it0 = json0.begin();
      Assert::AreSame(json0, *it0._json);
      Assert::AreEqual(0i64, it0._position);
      auto json1 = Json{ 1, 2, 3, 4 };
      auto it1 = json1.end();
      Assert::AreSame(json1, *it1._json);
      Assert::AreEqual(4i64, it1._position);

      JsonIterator def;
      Assert::IsNull(def._json);
      Assert::AreEqual(0i64, def._position);
      JsonIterator it = it0;
      Assert::AreSame(json0, *it._json);
      Assert::AreEqual(0i64, it._position);
    }

    TEST_METHOD(TestOperatorAssign)
    {
      auto json0 = Json{ 1, 2, 3, 4 };
      auto it0 = json0.begin();
      Assert::AreSame(json0, *it0._json);
      Assert::AreEqual(0i64, it0._position);
      auto it1 = JsonIterator(nullptr, 1337);
      Assert::AreEqual<void*>(nullptr, it1._json);
      Assert::AreEqual(1337i64, it1._position);
      auto it = it1 = it0;
      Assert::AreEqual<void*>(&json0, it1._json);
      Assert::AreEqual(0i64, it1._position);
      Assert::AreEqual<void*>(&json0, it._json);
      Assert::AreEqual(0i64, it._position);
    }

    TEST_METHOD(TestOperatorEqual)
    {
      auto json = Json{ 1, 2, 3, 4 };
      Assert::IsTrue (json.begin() == json.begin());
      Assert::IsFalse(json.begin() == json.end  ());
      Assert::IsFalse(json.end  () == json.begin());
      Assert::IsTrue (json.end  () == json.end  ());
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      auto json = Json{ 1, 2, 3, 4 };
      Assert::IsFalse(json.begin() != json.begin());
      Assert::IsTrue (json.begin() != json.end  ());
      Assert::IsTrue (json.end  () != json.begin());
      Assert::IsFalse(json.end  () != json.end  ());
    }

    TEST_METHOD(TestOperatorDereference)
    {
      auto array = Json{ 1, 3, 3, 7 };
      auto json = Json{ array, 2, 3, 4 };
      auto it = json.begin();
      Assert::AreEqual(array, *it);
    }

    TEST_METHOD(TestOperatorMemberAccess)
    {
      auto array = Json{ 1, 3, 3, 7 };
      auto json = Json{ array, 2, 3, 4 };
      auto it = json.begin();
      Assert::AreEqual(4i64, it->Size());
      Assert::AreEqual<Json>(7, it->At(3));
    }

    TEST_METHOD(TestOperatorPreIncrement)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      auto it1 = ++it0;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
    }

    TEST_METHOD(TestOperatorPostIncrement)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      auto it1 = it0++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(1, *it1);
    }

    TEST_METHOD(TestOperatorPreDecrement)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      it0++;
      auto it1 = --it0;
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(1, *it1);
    }

    TEST_METHOD(TestOperatorPostDecrement)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      it0++;
      auto it1 = it0--;
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(2, *it1);
    }

    TEST_METHOD(TestOperatorAddAssign)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      auto it1 = it0 += 2;
      Assert::AreEqual<Json>(3, *it0);
      Assert::AreEqual<Json>(3, *it1);
    }

    TEST_METHOD(TestOperatorAdd)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      auto it1 = it0 + 2;
      auto it2 = 2 + it0;
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(3, *it1);
      Assert::AreEqual<Json>(3, *it2);
    }

    TEST_METHOD(TestOperatorSubtractAssign)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      it0 += 2;
      auto it1 = it0 -= 1;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
    }

    TEST_METHOD(TestOperatorSubtract)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      it0 += 2;
      auto it1 = it0 - 1;
      Assert::AreEqual<Json>(3, *it0);
      Assert::AreEqual<Json>(2, *it1);
      auto it2 = json.end();
      Assert::AreEqual(3i64, it2 - it1);
      Assert::AreEqual(2i64, it2 - it0);
    }

    TEST_METHOD(TestOperatorSubscript)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(1, it0[0]);
      Assert::AreEqual<Json>(2, it0[1]);
      Assert::AreEqual<Json>(3, it0[2]);
      Assert::AreEqual<Json>(4, it0[3]);
      Assert::AreEqual<Json>(1, *it0);
      it0++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, it0[0]);
      Assert::AreEqual<Json>(3, it0[1]);
      Assert::AreEqual<Json>(4, it0[2]);
      Assert::AreEqual<Json>(2, *it0);
    }

    TEST_METHOD(TestOperatorLessThan)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      auto it1 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(1, *it1);
      Assert::IsFalse(it0 < it1);
      it1++;
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsTrue(it0 < it1);
      it0++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsFalse(it0 < it1);
      it0++;
      Assert::AreEqual<Json>(3, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsFalse(it0 < it1);
    }

    TEST_METHOD(TestOperatorLessThanOrEqual)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      auto it1 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(1, *it1);
      Assert::IsTrue(it0 <= it1);
      it1++;
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsTrue(it0 <= it1);
      it0++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsTrue(it0 <= it1);
      it0++;
      Assert::AreEqual<Json>(3, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsFalse(it0 <= it1);
    }

    TEST_METHOD(TestOperatorGreaterThan)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      auto it1 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(1, *it1);
      Assert::IsFalse(it0 > it1);
      it0++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(1, *it1);
      Assert::IsTrue(it0 > it1);
      it1++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsFalse(it0 > it1);
      it1++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(3, *it1);
      Assert::IsFalse(it0 > it1);
    }

    TEST_METHOD(TestOperatorGreaterThanOrEqual)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      auto it1 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(1, *it1);
      Assert::IsTrue(it0 >= it1);
      it0++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(1, *it1);
      Assert::IsTrue(it0 >= it1);
      it1++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
      Assert::IsTrue(it0 >= it1);
      it1++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(3, *it1);
      Assert::IsFalse(it0 >= it1);
    }

    TEST_METHOD(TestSize)
    {
      auto json = Json{ 1, 2, 3, 4 };
      auto it = json.begin();
      Assert::AreEqual(4i64, it.Size());
    }

    TEST_METHOD(TestKey)
    {
      auto json = Json{
        { L"Key1", 1 },
        { L"Key2", 2 }
      };
      auto it = json.begin();
      Assert::AreEqual<Json>(L"Key1", it.Key());
      it++;
      Assert::AreEqual<Json>(L"Key2", it.Key());

      json = Json{ 1, 3, 3, 7 };
      it = json.begin();
      ExpectException<exception>([&]() { it.Key(); }, "Key() is only defined for JsonObject!");
    }

    TEST_METHOD(TestValue)
    {
      auto json = Json{
        { L"Key1", 1 },
        { L"Key2", 2 }
      };
      auto it = json.begin();
      Assert::AreEqual<Json>(1, it.Value());
      Json value = 1337;
      it.Value(value);
      Assert::AreEqual<Json>(1337, it.Value());
      it++;
      Assert::AreEqual<Json>(2, it.Value());
      value = L"Test"s;
      it.Value(move(value));
      Assert::AreEqual<Json>(L""s, value);
      Assert::AreEqual<Json>(L"Test"s, it.Value());
      Assert::AreEqual<Json>(1337, json[L"Key1"]);
      Assert::AreEqual<Json>(L"Test"s, json[L"Key2"]);

      json = Json{ 1, 3, 3, 7 };
      it = json.begin();
      ExpectException<exception>([&]() { it.Value(); }, "Value() is only defined for JsonObject!");
      ExpectException<exception>([&]() { it.Value(json); }, "Value(Json const& json) is only defined for JsonObject!");
      ExpectException<exception>([&]() { it.Value(nullptr); }, "Value(Json && json) is only defined for JsonObject!");
    }

    TEST_METHOD(TestRangeBasedFor)
    {
      auto values = vector<Json>();
      auto json = Json{ 1, 2, 3, 4 };
      for (auto value : json)
      {
        value = value + 1;
        values.push_back(value);
      }
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(2, json[1]);
      Assert::AreEqual<Json>(3, json[2]);
      Assert::AreEqual<Json>(4, json[3]);
      Assert::AreEqual<Json>(2, values[0]);
      Assert::AreEqual<Json>(3, values[1]);
      Assert::AreEqual<Json>(4, values[2]);
      Assert::AreEqual<Json>(5, values[3]);
      values.clear();
      for (auto& value : json)
      {
        value = value + 1;
        values.push_back(value);
      }
      Assert::AreEqual<Json>(2, json[0]);
      Assert::AreEqual<Json>(3, json[1]);
      Assert::AreEqual<Json>(4, json[2]);
      Assert::AreEqual<Json>(5, json[3]);
      Assert::AreEqual<Json>(2, values[0]);
      Assert::AreEqual<Json>(3, values[1]);
      Assert::AreEqual<Json>(4, values[2]);
      Assert::AreEqual<Json>(5, values[3]);
    }

    TEST_METHOD(TestFor)
    {
      auto values = vector<Json>();
      auto json = Json{ 1, 2, 3, 4 };
      for (auto it = json.begin(); it != json.end(); ++it)
      {
        *it = *it + 1;
        values.push_back(*it);
      }
      Assert::AreEqual<Json>(2, json[0]);
      Assert::AreEqual<Json>(3, json[1]);
      Assert::AreEqual<Json>(4, json[2]);
      Assert::AreEqual<Json>(5, json[3]);
      Assert::AreEqual<Json>(2, values[0]);
      Assert::AreEqual<Json>(3, values[1]);
      Assert::AreEqual<Json>(4, values[2]);
      Assert::AreEqual<Json>(5, values[3]);
    }
  };
}