#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <iostream>

namespace Json4CPP
{
  // Non-sticky output manipulator for setting the indentation char before outputting a Json.
  class JSON_API JsonIndentChar
  {
  private:
    inline static int _activeIndex = std::ios_base::xalloc();
    inline static int _charIndex   = std::ios_base::xalloc();
    wchar_t _char;
  public:
    JsonIndentChar(wchar_t c);
    static bool IsActive(std::wostream& os);
    static wchar_t GetChar(std::wostream& os);
    static void ResetState(std::wostream& os);
    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonIndentChar const& jsonIndentChar);
  };
}