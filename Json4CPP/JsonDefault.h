#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

namespace Json4CPP
{
  struct JSON_API JsonDefault
  {
    inline static uint8_t IndentSize = 2;
    inline static wchar_t IndentChar = L' ';
    inline static uint8_t MaxDepth = 20;
  };
}