#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <string>

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

  JSON_API std::wstring EscapeString(std::wstring value);

  JSON_API std::wstring String2WString(std::string const& string, uint32_t codePage = 1252);
  JSON_API std::string WString2String(std::wstring const& string, uint32_t codePage = 1252);

}