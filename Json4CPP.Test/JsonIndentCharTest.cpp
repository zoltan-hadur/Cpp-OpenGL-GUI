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

    TEST_METHOD(TestGetChar)
    {
      wstringstream ss;
      Assert::AreEqual(wchar_t(), JsonIndentChar::GetChar(ss));
      ss << JsonIndentChar(static_cast<wchar_t>(0xFFFF));
      Assert::AreEqual(static_cast<wchar_t>(0xFFFF), JsonIndentChar::GetChar(ss));
      ss = wstringstream();
      Assert::AreEqual(wchar_t(), JsonIndentChar::GetChar(ss));
      ss << JsonIndentChar(L'€');
      Assert::AreEqual(L'€', JsonIndentChar::GetChar(ss));
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