#include "stdafx.h"

#include "Helper.h"

using namespace std;

namespace Json4CPP::Detail
{
  wstring EscapeString(wstring value)
  {
    wostringstream os;
    for (auto c = value.cbegin(); c != value.cend(); ++c)
    {
      switch (*c)
      {
      case L'"' : os << L"\\\""; break;
      case L'\\': os << L"\\\\"; break;
      case L'\b': os << L"\\b" ; break;
      case L'\f': os << L"\\f" ; break;
      case L'\n': os << L"\\n" ; break;
      case L'\r': os << L"\\r" ; break;
      case L'\t': os << L"\\t" ; break;
      default:
        if (L'\x00' <= *c && *c <= L'\x1f')
        {
          os << L"\\u" << hex << setw(4) << setfill(L'0') << (int)*c;
        }
        else
        {
          os << *c;
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
    auto [line, column] = GetStreamPosition(is, is.tellg());
    return L"Line: " + to_wstring(line) + L" Column: " + to_wstring(column);
  }

  string GetFormattedStreamPositionA(wistream& is, wistream::pos_type pos)
  {
    auto [line, column] = GetStreamPosition(is, is.tellg());
    return "Line: " + to_string(line) + " Column: " + to_string(column);
  }
}