#include "stdafx.h"
#include "CppUnitTest.h"
#include <optional>

#include "..\Json4CPP\Json.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;

namespace Json4CPP::Test
{		
	TEST_CLASS(HelperTest)
	{
	public:
		TEST_METHOD(TestOverload)
		{
      variant<int, float, wstring> left;
      variant<int, float, wstring> right;
      variant<int, float, wstring> input[] = { 1, 0.5f, L"a" };
      for (int i = 0; i < 3; ++i)
      {
        for (int j = 0; j < 3; ++j)
        {
          left  = input[i];
          right = input[j];
          optional<wstring> text = nullopt;
          optional<float> number = nullopt;
          optional<bool> invalid = nullopt;
          visit(Overload{
            [&](int     const& l, int     const& r) { number = l + r; },
            [&](int     const& l, float   const& r) { number = l + r; },
            [&](float   const& l, int     const& r) { number = l + r; },
            [&](float   const& l, float   const& r) { number = l + r; },
            [&](wstring const& l, wstring const& r) { text   = l + r; },
            [&](auto    const& l, auto    const& r) { invalid = true; }
          }, left, right);
          Assert::AreEqual(1, text.has_value() + number.has_value() + invalid.has_value());
          if (text)
          {
            Assert::AreEqual(L"aa"s, text.value());
          }
          else if (number)
          {
            if (i == 0 && j == 0)       // Both int
            {
              Assert::AreEqual(2.0f, number.value());
            }
            else if (i == 1 && j == 1)  // Both float
            {
              Assert::AreEqual(1.0f, number.value());
            }
            else
            {
              Assert::AreEqual(1.5f, number.value());
            }
          }
          else if (invalid)
          {
            // (left is int or float while right is wstring) OR (left is wstring while right is int or float)
            Assert::IsTrue(((i == 0 || i == 1) && j == 2) || (i == 2 && (j == 0 || j == 1)));
          }
        }
      }
		}

    TEST_METHOD(TestEscapeString)
    {
      auto input    =   L"test\r\n\t\"test\"\\test\\"s;
      auto expected = LR"(test\r\n\t\"test\"\\test\\)"s;
      auto output = EscapeString(input);
      Assert::AreEqual(expected, output);
    }
	};
}