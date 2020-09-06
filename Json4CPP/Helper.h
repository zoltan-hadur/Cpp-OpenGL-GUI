#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <string>
#include <istream>
#include <filesystem>

namespace Json4CPP::Detail
{
  template<typename T>
  struct TypeDebug;

  template<typename ...Args>
  struct Overload : Args...
  {
    using Args::operator()...;
  };
  template<typename ...Args>
  Overload(Args...)->Overload<Args...>;

  // Reads UTF-8 without BOM encoded file into UTF-16 encoded wstring.
  JSON_API std::wstring ReadAllText (std::filesystem::path const& path);

  // Writes UTF-16 encoded wstring into UTF-8 without BOM encoded file.
  JSON_API void         WriteAllText(std::filesystem::path const& path, std::wstring const& value);

  // Escapes ", \, \b, \f, \n, \r, \t characters and any other character between 0x00 and 0x1f.
  JSON_API std::wstring EscapeString  (std::wstring const& value);

  // Widens the string character by character, for example: 'a' (0x61) -> L'a' (0x0061) or (char)-100 -> (wchar_t)156.
  JSON_API std::wstring WidenString   (std::string  const& value);

  // Narrows the wstring character by character, for example: L'a' (0x0061) -> 'a' (0x61) or (wchar_t)156 -> (char)-100.
  JSON_API std::string  NarrowWString (std::wstring const& value);

  // Converts UTF-8 encoded string into UTF-16 encoded wstring.
  JSON_API std::wstring String2WString(std::string  const& string);

  // Converts UTF-16 encoded wstring into UTF-8 encoded string.
  JSON_API std::string  WString2String(std::wstring const& string);

  // Returns the { line, column } pair of the specified position in the stream.
  // Line endings are handled as \r\n.
  JSON_API std::pair<uint64_t, uint64_t> GetStreamPosition(std::wistream& is, std::wistream::pos_type pos);

  // Returns the { line, column } data as L"Line: {line} Column: {column}"s of the specified position in the stream.
  // Line endings are handled as \r\n.
  JSON_API std::wstring GetFormattedStreamPosition (std::wistream& is, std::wistream::pos_type pos);

  // Returns the { line, column } data as "Line: {line} Column: {column}"s of the specified position in the stream.
  // Line endings are handled as \r\n.
  JSON_API std::string  GetFormattedStreamPositionA(std::wistream& is, std::wistream::pos_type pos);
}