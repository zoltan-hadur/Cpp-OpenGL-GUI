#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonIndentCharTest)
  {
  public:
    TEST_METHOD(TestIsActive)
    {
      wstringstream ss;
      Assert::IsFalse(JsonIndentChar::IsActive(ss));
      ss << JsonIndentChar(wchar_t());
      Assert::IsTrue(JsonIndentChar::IsActive(ss));
    }

    TEST_METHOD(TestGetSize)
    {
      wstringstream ss;
      Assert::AreEqual(wchar_t(), JsonIndentChar::GetChar(ss));
      ss << JsonIndentChar(L'\t');
      Assert::AreEqual(L'\t', JsonIndentChar::GetChar(ss));
    }

    TEST_METHOD(TestResetState)
    {
      wstringstream ss;
      Assert::IsFalse(JsonIndentChar::IsActive(ss));
      Assert::AreEqual(wchar_t(), JsonIndentChar::GetChar(ss));
      ss << JsonIndentChar(L'\t');
      Assert::IsTrue(JsonIndentChar::IsActive(ss));
      Assert::AreEqual(L'\t', JsonIndentChar::GetChar(ss));
      JsonIndentChar::ResetState(ss);
      Assert::IsFalse(JsonIndentChar::IsActive(ss));
      Assert::AreEqual(wchar_t(), JsonIndentChar::GetChar(ss));
    }
  };
}