#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <string>
#include <istream>

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

  JSON_API std::wstring EscapeString(std::wstring const& value);
  JSON_API std::wstring WidenString(std::string  const& value);
  JSON_API std::string  NarrowString(std::wstring const& value);

  JSON_API std::wstring String2WString(std::string  const& string);
  JSON_API std::string  WString2String(std::wstring const& string);

  // Returns the {Line, Column} pair of the specified position in the stream.
  JSON_API std::pair<uint64_t, uint64_t> GetStreamPosition(std::wistream& is, std::wistream::pos_type pos);

  JSON_API std::wstring GetFormattedStreamPosition (std::wistream& is, std::wistream::pos_type pos);
  JSON_API std::string  GetFormattedStreamPositionA(std::wistream& is, std::wistream::pos_type pos);
}