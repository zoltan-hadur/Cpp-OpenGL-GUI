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
  };
}