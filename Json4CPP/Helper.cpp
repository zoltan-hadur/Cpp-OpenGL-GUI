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

  wstring String2WString(string const& string, uint32_t codePage)
  {
    return CA2W(string.c_str(), codePage);
  }

  string WString2String(const wstring& string, uint32_t codePage)
  {
    return CW2A(string.c_str(), codePage);
  }
}