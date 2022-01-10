#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonIndentSizeTest)
  {
  public:
    TEST_METHOD(TestIsActive)
    {
      wstringstream ss;
      Assert::IsFalse(JsonIndentSize::IsActive(ss));
      ss << JsonIndentSize(uint8_t());
      Assert::IsTrue(JsonIndentSize::IsActive(ss));
    }

    TEST_METHOD(TestGetSize)
    {
      wstringstream ss;
      Assert::AreEqual(0ui8, JsonIndentSize::GetSize(ss));
      ss << JsonIndentSize(250ui8);
      Assert::AreEqual(250ui8, JsonIndentSize::GetSize(ss));
    }

    TEST_METHOD(TestResetState)
    {
      wstringstream ss;
      Assert::IsFalse(JsonIndentSize::IsActive(ss));
      Assert::AreEqual(0ui8, JsonIndentSize::GetSize(ss));
      ss << JsonIndentSize(250ui8);
      Assert::IsTrue(JsonIndentSize::IsActive(ss));
      Assert::AreEqual(250ui8, JsonIndentSize::GetSize(ss));
      JsonIndentSize::ResetState(ss);
      Assert::IsFalse(JsonIndentSize::IsActive(ss));
      Assert::AreEqual(0ui8, JsonIndentSize::GetSize(ss));
    }
  };
}