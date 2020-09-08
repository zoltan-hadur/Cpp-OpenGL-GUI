#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <iostream>

namespace Json4CPP
{
  class JSON_API JsonIndentSize
  {
  private:
    inline static int _valueIndex = std::ios_base::xalloc();
    uint8_t _size;
  public:
    JsonIndentSize(uint8_t size);
    static bool IsActive(std::wostream& os);
    static uint8_t GetSize(std::wostream& os);
    static void ResetState(std::wostream& os);
    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonIndentSize const& jsonIndentSize);
  };
}