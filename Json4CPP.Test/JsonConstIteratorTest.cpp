#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonConstIteratorTest)
  {
  public:
    TEST_METHOD(TestConstructor)
    {
      auto const json0 = Json{ 1, 2, 3, 4 };
      auto it0 = json0.begin();
      Assert::AreSame(json0, *it0._json);
      Assert::AreEqual(0i64, it0._position);
      auto const json1 = Json{ 1, 2, 3, 4 };
      auto it1 = json1.end();
      Assert::AreSame(json1, *it1._json);
      Assert::AreEqual(4i64, it1._position);
    }

    TEST_METHOD(TestSize)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      auto it = json.begin();
      Assert::AreEqual(4i64, it.Size());
    }

    TEST_METHOD(TestOperatorAssign)
    {
      auto const json0 = Json{ 1, 2, 3, 4 };
      auto it0 = json0.begin();
      Assert::AreSame(json0, *it0._json);
      Assert::AreEqual(0i64, it0._position);
      auto it1 = JsonConstIterator(nullptr, 1337);
      Assert::AreEqual<void const*>(nullptr, it1._json);
      Assert::AreEqual(1337i64, it1._position);
      auto it = it1 = it0;
      Assert::AreEqual<void const*>(&json0, it1._json);
      Assert::AreEqual(0i64, it1._position);
      Assert::AreEqual<void const*>(&json0, it._json);
      Assert::AreEqual(0i64, it._position);
    }

    TEST_METHOD(TestOperatorEqual)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      Assert::IsTrue (json.begin() == json.begin());
      Assert::IsFalse(json.begin() == json.end  ());
      Assert::IsFalse(json.end  () == json.begin());
      Assert::IsTrue (json.end  () == json.end  ());
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      Assert::IsFalse(json.begin() != json.begin());
      Assert::IsTrue (json.begin() != json.end  ());
      Assert::IsTrue (json.end  () != json.begin());
      Assert::IsFalse(json.end  () != json.end  ());
    }

    TEST_METHOD(TestOperatorDereference)
    {
      auto const array = Json{ 1, 3, 3, 7 };
      auto const json = Json{ array, 2, 3, 4 };
      auto it = json.begin();
      Assert::AreEqual(array, *it);
    }

    TEST_METHOD(TestOperatorMemberAccess)
    {
      auto const array = Json{ 1, 3, 3, 7 };
      auto const json = Json{ array, 2, 3, 4 };
      auto it = json.begin();
      Assert::AreEqual(4i64, it->Size());
      Assert::AreEqual<Json>(7, it->At(3));
    }

    TEST_METHOD(TestOperatorPreIncrement)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      auto it1 = ++it0;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
    }

    TEST_METHOD(TestOperatorPostIncrement)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      auto it1 = it0++;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(1, *it1);
    }

    TEST_METHOD(TestOperatorPreDecrement)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      it0++;
      auto it1 = --it0;
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(1, *it1);
    }

    TEST_METHOD(TestOperatorPostDecrement)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      it0++;
      auto it1 = it0--;
      Assert::AreEqual<Json>(1, *it0);
      Assert::AreEqual<Json>(2, *it1);
    }

    TEST_METHOD(TestOperatorAddAssign)
    {
      auto const json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      auto it1 = it0 += 2;
      Assert::AreEqual<Json>(3, *it0);
      Assert::AreEqual<Json>(3, *it1);
    }

    TEST_METHOD(TestOperatorAdd)
    {
      auto const json = Json{ 1, 2, 3, 4 };
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
      auto const json = Json{ 1, 2, 3, 4 };
      auto it0 = json.begin();
      Assert::AreEqual<Json>(1, *it0);
      it0 += 2;
      auto it1 = it0 -= 1;
      Assert::AreEqual<Json>(2, *it0);
      Assert::AreEqual<Json>(2, *it1);
    }

    TEST_METHOD(TestOperatorSubtract)
    {
      auto const json = Json{ 1, 2, 3, 4 };
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
      auto const json = Json{ 1, 2, 3, 4 };
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
      auto const json = Json{ 1, 2, 3, 4 };
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
      auto const json = Json{ 1, 2, 3, 4 };
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
      auto const json = Json{ 1, 2, 3, 4 };
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
      auto const json = Json{ 1, 2, 3, 4 };
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

    TEST_METHOD(TestKey)
    {
      auto const json = Json{
        { L"Key1", 1 },
        { L"Key2", 2 }
      };
      auto it = json.begin();
      Assert::AreEqual<Json>(L"Key1", it.Key());
      it++;
      Assert::AreEqual<Json>(L"Key2", it.Key());
    }

    TEST_METHOD(TestValue)
    {
      auto const json = Json{
        { L"Key1", 1 },
        { L"Key2", 2 }
      };
      auto it = json.begin();
      Assert::AreEqual<Json>(1, it.Value());
      it++;
      Assert::AreEqual<Json>(2, it.Value());
      Assert::AreEqual<Json>(1, json.At(L"Key1"));
      Assert::AreEqual<Json>(2, json.At(L"Key2"));
    }

    TEST_METHOD(TestRangeBasedFor)
    {
      auto values = vector<Json>();
      auto const json = Json{ 1, 2, 3, 4 };
      for (auto value : json)
      {
        value = value + 1;
        values.push_back(value);
      }
      Assert::AreEqual<Json>(1, json.At(0));
      Assert::AreEqual<Json>(2, json.At(1));
      Assert::AreEqual<Json>(3, json.At(2));
      Assert::AreEqual<Json>(4, json.At(3));
      Assert::AreEqual<Json>(2, values[0]);
      Assert::AreEqual<Json>(3, values[1]);
      Assert::AreEqual<Json>(4, values[2]);
      Assert::AreEqual<Json>(5, values[3]);
      values.clear();
      for (auto& value : json)
      {
        static_assert(is_const_v<remove_reference_t<decltype(value)>>, "Type of value must be const!");
        values.push_back(value);
      }
      Assert::AreEqual<Json>(1, json.At(0));
      Assert::AreEqual<Json>(2, json.At(1));
      Assert::AreEqual<Json>(3, json.At(2));
      Assert::AreEqual<Json>(4, json.At(3));
      Assert::AreEqual<Json>(1, values[0]);
      Assert::AreEqual<Json>(2, values[1]);
      Assert::AreEqual<Json>(3, values[2]);
      Assert::AreEqual<Json>(4, values[3]);
    }

    TEST_METHOD(TestFor)
    {
      auto values = vector<Json>();
      auto const json = Json{ 1, 2, 3, 4 };
      for (auto it = json.begin(); it != json.end(); ++it)
      {
        static_assert(is_const_v<remove_reference_t<decltype(*it)>>, "Type of value must be const!");
        values.push_back(*it);
      }
      Assert::AreEqual<Json>(1, json.At(0));
      Assert::AreEqual<Json>(2, json.At(1));
      Assert::AreEqual<Json>(3, json.At(2));
      Assert::AreEqual<Json>(4, json.At(3));
      Assert::AreEqual<Json>(1, values[0]);
      Assert::AreEqual<Json>(2, values[1]);
      Assert::AreEqual<Json>(3, values[2]);
      Assert::AreEqual<Json>(4, values[3]);
    }
  };
}