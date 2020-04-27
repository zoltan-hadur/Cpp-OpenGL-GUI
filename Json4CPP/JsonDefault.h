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
    static uint8_t Indentation;
    static uint8_t MaxDepth;
  };
}