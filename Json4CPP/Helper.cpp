#include "stdafx.h"

#include "Helper.h"
#include <locale>
#include <codecvt>

using namespace std::string_literals;

namespace Json4CPP::Helper
{
  std::wstring ReadAllText(std::filesystem::path const& path)
  {
    if (auto is = std::ifstream(path, std::fstream::in | std::fstream::binary))
    {
      std::stringstream ss;
      ss << is.rdbuf();
      return String2WString(ss.str());
    }
    auto message = "Could not open file: "s + path.string() + "!"s;
    throw std::exception(message.c_str());
  }

  void WriteAllText(std::filesystem::path const& path, std::wstring const& value)
  {
    std::wfstream(path, std::wfstream::out | std::wfstream::binary) << WidenString(WString2String(value));
  }

  std::wstring WidenString(std::string const& value)
  {
    auto str = std::wstring(value.size(), L'\0');
    for (int i = 0; i < str.size(); ++i)
    {
      str[i] = value[i] & 0x00FF;
    }
    return str;
  }

  std::string  NarrowWString(std::wstring const& value)
  {
    auto str = std::string(value.size(), L'\0');
    for (int i = 0; i < str.size(); ++i)
    {
      str[i] = value[i] & 0x00FF;
    }
    return str;
  }

  std::wstring String2WString(std::string const& string)
  {
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(string);
  }

  std::string WString2String(std::wstring const& string)
  {
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(string);
  }

  std::wstring U32String2WString(std::u32string const& string)
  {
    return String2WString(std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().to_bytes(string));
  }

  std::pair<uint64_t, uint64_t> GetStreamPosition(std::wistream& is, std::wistream::pos_type pos)
  {
    auto state = is.rdstate();
    is.clear();
    is.seekg(is.beg);
    uint64_t column = 0;
    uint64_t line = 1;
    while (is.tellg() != pos)
    {
      if (is.get() == L'\n')
      {
        column = 0;
        line++;
      }
      else
      {
        column++;
      }
    }
    is.setstate(state);
    return { line, column };
  }

  std::wstring GetFormattedStreamPosition(std::wistream& is, std::wistream::pos_type pos)
  {
    auto [line, column] = GetStreamPosition(is, pos);
    return L"Line: "s + std::to_wstring(line) + L" Column: "s + std::to_wstring(column);
  }

  std::string GetFormattedStreamPositionA(std::wistream& is, std::wistream::pos_type pos)
  {
    return WString2String(GetFormattedStreamPosition(is, pos));
  }
}