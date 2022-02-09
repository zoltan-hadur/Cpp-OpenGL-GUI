#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

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
            [&](int     const& l, int     const& r) { number = (float)l + (float)r; },
            [&](int     const& l, float   const& r) { number = (float)l +        r; },
            [&](float   const& l, int     const& r) { number =        l + (float)r; },
            [&](float   const& l, float   const& r) { number =        l +        r; },
            [&](wstring const& l, wstring const& r) { text   =        l +        r; },
            [&](auto    const& l, auto    const& r) { invalid = true;               }
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

    TEST_METHOD(TestReadAllText)
    {
      auto input = L"名前:前田あゆみ\\n第一印象:なんか怖っ！\\n今の印象:とりあえずキモい。噛み合わない\\n好きなところ:ぶすでキモいとこ😋✨✨\\n思い出:んーーー、ありすぎ😊❤️\\nLINE交換できる？:あぁ……ごめん✋\\nトプ画をみて:照れますがな😘✨"s;
      auto excepted = ReadAllText(L"UTF-8.txt");
      Assert::AreEqual(excepted, input);
      Assert::ExpectException<exception>([]() -> void { ReadAllText(L"NonExistent.file"); });
    }

    TEST_METHOD(TestWriteAllText)
    {
      auto input = L"名前:前田あゆみ\\n第一印象:なんか怖っ！\\n今の印象:とりあえずキモい。噛み合わない\\n好きなところ:ぶすでキモいとこ😋✨✨\\n思い出:んーーー、ありすぎ😊❤️\\nLINE交換できる？:あぁ……ごめん✋\\nトプ画をみて:照れますがな😘✨"s;
      WriteAllText(L"UTF-8_copy.txt", input);
      auto is1 = wifstream(L"UTF-8.txt"     , wifstream::binary | wifstream::ate);
      auto is2 = wifstream(L"UTF-8_copy.txt", wifstream::binary | wifstream::ate);
      if (is1.fail() || is2.fail() || is1.tellg() != is2.tellg()) Assert::Fail();
      is1.seekg(0, wifstream::beg);
      is2.seekg(0, wifstream::beg);
      Assert::IsTrue(equal(istreambuf_iterator<wchar_t>(is1.rdbuf()), istreambuf_iterator<wchar_t>(), istreambuf_iterator<wchar_t>(is2.rdbuf())));
    }

    TEST_METHOD(TestEscapeString)
    {
      auto input    =   L"test\r\n\t\"test\"\\test\\"s;
      auto expected = LR"(test\r\n\t\"test\"\\test\\)"s;
      auto output = EscapeString(input);
      Assert::AreEqual(expected, output);
    }

    TEST_METHOD(TestWidenString)
    {
      auto expected = wstring{ ((wchar_t)-100) & 0x00FF, ((wchar_t)-50) & 0x00FF, 50, 100 };
      auto input    = string { -100, -50, 50, 100 };
      auto actual = WidenString(input);
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(TestNarrowWString)
    {
      auto expected = string { (char)0x00CD, (char)0x000D, (char)0x00AD, -100   };
      auto input    = wstring{       0xABCD,       0xF00D,       0xBAAD, 0x009C };
      auto actual = NarrowWString(input);
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(TestString2WString)
    {
      auto expected = L"💰, A, Á, B, C, Cs, D, Dz, Dzs, E, É, F, G, Gy, H, I, Í, J, K, L, Ly, M, N, Ny, O, Ó, Ö, Ő, P, Q, R, S, Sz, T, Ty, U, Ú, Ü, Ű, V, W, X, Y, Z, Zs, a, á, b, c, cs, d, dz, dzs, e, é, f, g, gy, h, i, í, j, k, l, ly, m, n, ny, o, ó, ö, ő, p, q, r, s, sz, t, ty, u, ú, ü, ű, v, w, x, y, z, zs"s;
      auto input = "ðŸ’°, A, Ã, B, C, Cs, D, Dz, Dzs, E, Ã‰, F, G, Gy, H, I, Ã, J, K, L, Ly, M, N, Ny, O, Ã“, Ã–, Å, P, Q, R, S, Sz, T, Ty, U, Ãš, Ãœ, Å°, V, W, X, Y, Z, Zs, a, Ã¡, b, c, cs, d, dz, dzs, e, Ã©, f, g, gy, h, i, Ã­, j, k, l, ly, m, n, ny, o, Ã³, Ã¶, Å‘, p, q, r, s, sz, t, ty, u, Ãº, Ã¼, Å±, v, w, x, y, z, zs"s;
      auto actual = String2WString(input);
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(TestWString2String)
    {
      auto expected = "ðŸ’°, A, Ã, B, C, Cs, D, Dz, Dzs, E, Ã‰, F, G, Gy, H, I, Ã, J, K, L, Ly, M, N, Ny, O, Ã“, Ã–, Å, P, Q, R, S, Sz, T, Ty, U, Ãš, Ãœ, Å°, V, W, X, Y, Z, Zs, a, Ã¡, b, c, cs, d, dz, dzs, e, Ã©, f, g, gy, h, i, Ã­, j, k, l, ly, m, n, ny, o, Ã³, Ã¶, Å‘, p, q, r, s, sz, t, ty, u, Ãº, Ã¼, Å±, v, w, x, y, z, zs"s;
      auto input = L"💰, A, Á, B, C, Cs, D, Dz, Dzs, E, É, F, G, Gy, H, I, Í, J, K, L, Ly, M, N, Ny, O, Ó, Ö, Ő, P, Q, R, S, Sz, T, Ty, U, Ú, Ü, Ű, V, W, X, Y, Z, Zs, a, á, b, c, cs, d, dz, dzs, e, é, f, g, gy, h, i, í, j, k, l, ly, m, n, ny, o, ó, ö, ő, p, q, r, s, sz, t, ty, u, ú, ü, ű, v, w, x, y, z, zs"s;
      auto actual = WString2String(input);
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(TestU32String2WString)
    {
      auto str = string{ (char)0xF0, (char)0xA4, (char)0xAD, (char)0xA2 };
      auto str2 = u8"𤭢"s;
      auto wstr = wstring{ 0xD852, 0xDF62 };
      auto wstr2 = L"𤭢"s;
      u32string ustr = u32string{ 0x00024B62 };
      auto ustr2 = U"𤭢"s;
      for (int i = 0; i < 4; ++i)
      {
        Assert::AreEqual(str.c_str()[i], (char)str2.c_str()[i]);
      }
      Assert::AreEqual(wstr2, wstr);
      Assert::AreEqual(ustr2, ustr);
      Assert::AreEqual(wstr, String2WString(str));
      Assert::AreEqual(wstr, U32String2WString(ustr));
    }

    TEST_METHOD(TestGetStreamPosition)
    {
      auto ss = wstringstream(L"abc\r\n"
                              L"def\r\n"
                              L"ghi"s);
      auto tuples = deque<tuple<wchar_t, uint64_t, uint64_t>>
      {
        { L'a', 1, 1 }, { L'b', 1, 2 }, { L'c', 1, 3 },
        { L'd', 2, 1 }, { L'e', 2, 2 }, { L'f', 2, 3 },
        { L'g', 3, 1 }, { L'h', 3, 2 }, { L'i', 3, 3 }
      };
      while(tuples.size())
      {
        wchar_t actualChar = ss.get();
        if (actualChar == L'\r' || actualChar == L'\n') continue;
        auto [actualLine, actualColumn] = GetStreamPosition(ss, ss.tellg());
        auto [expectedChar, expectedLine, expectedColumn] = tuples.front();
        tuples.pop_front();
        Assert::AreEqual(expectedChar, actualChar);
        Assert::AreEqual(expectedLine, actualLine);
        Assert::AreEqual(expectedColumn, actualColumn);
        Assert::AreEqual(GetFormattedStreamPosition (ss, ss.tellg()), L"Line: " + to_wstring(actualLine) + L" Column: " + to_wstring(actualColumn));
        Assert::AreEqual(GetFormattedStreamPositionA(ss, ss.tellg()),  "Line: " + to_string (actualLine) +  " Column: " + to_string (actualColumn));
      }
    }
  };
}