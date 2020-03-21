#include "stdafx.h"

#include "Helper.h"

using namespace std;

namespace Json4CPP::Detail
{
  wstring EscapeString(wstring value)
  {
    wostringstream os;
    for (auto& c : value)
    {
      switch (c)
      {
      case L'"' : os << L"\\\""s; break;
      case L'\\': os << L"\\\\"s; break;
      case L'\b': os << L"\\b"s ; break;
      case L'\f': os << L"\\f"s ; break;
      case L'\n': os << L"\\n"s ; break;
      case L'\r': os << L"\\r"s ; break;
      case L'\t': os << L"\\t"s ; break;
      default:
        if (L'\x00' <= c && c <= L'\x1f')
        {
          os << L"\\u"s << hex << setw(4) << setfill(L'0') << (int)c;
        }
        else
        {
          os << c;
        }
      }
    }
    return os.str();
  }

#pragma warning(disable : 4927)
  wstring String2WString(string const& string, uint32_t codePage)
  {
    return CA2W(string.c_str(), codePage);
  }

  string WString2String(const wstring& string, uint32_t codePage)
  {
    return CW2A(string.c_str(), codePage);
  }
#pragma warning(push)

  pair<uint64_t, uint64_t> GetStreamPosition(wistream& is, wistream::pos_type pos)
  {
    auto state = is.rdstate();
    is.clear();
    is.seekg(is.beg);
    uint64_t column = 0;
    uint64_t line = 1;
    auto carriageReturn = false;
    while (is.tellg() != pos)
    {
      if (carriageReturn && is.get() == L'\n')
      {
        carriageReturn = false;
        column = 0;
        line++;
      }
      else
      {
        carriageReturn = is.get() == L'\r';
        column++;
      }
    }
    is.setstate(state);
    return { line, column };
  }

  wstring GetFormattedStreamPosition(wistream& is, wistream::pos_type pos)
  {
    auto [line, column] = GetStreamPosition(is, pos);
    return L"Line: "s + to_wstring(line) + L" Column: "s + to_wstring(column);
  }

  string GetFormattedStreamPositionA(wistream& is, wistream::pos_type pos)
  {
    auto [line, column] = GetStreamPosition(is, pos);
    return "Line: "s + to_string(line) + " Column: "s + to_string(column);
  }
}