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