#include "stdafx.h"

#include "Helper.h"
#include <locale>
#include <codecvt>

using namespace std;
using namespace std::filesystem;

namespace Json4CPP::Detail
{
  wstring ReadAllText(path const& path)
  {
    if (auto is = ifstream(path, fstream::in | fstream::binary))
    {
      stringstream ss;
      ss << is.rdbuf();
      return String2WString(ss.str());
    }
    auto message = "Could not open file: "s + path.string() + "!"s;
    throw exception(message.c_str());
  }

  void WriteAllText(path const& path, wstring const& value)
  {
    wfstream(path, wfstream::out | wfstream::binary) << WidenString(WString2String(value));
  }

  wstring EscapeString(wstring const& value)
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

  wstring WidenString(string const& value)
  {
    auto str = wstring(value.size(), L'\0');
    for (int i = 0; i < str.size(); ++i)
    {
      str[i] = value[i] & 0x00FF;
    }
    return str;
  }

  string  NarrowWString(wstring const& value)
  {
    auto str = string(value.size(), L'\0');
    for (int i = 0; i < str.size(); ++i)
    {
      str[i] = value[i] & 0x00FF;
    }
    return str;
  }

  wstring String2WString(string const& string)
  {
    return wstring_convert<codecvt_utf8_utf16<wchar_t>>().from_bytes(string);
  }

  string WString2String(const wstring& string)
  {
    return wstring_convert<codecvt_utf8_utf16<wchar_t>>().to_bytes(string);
  }

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