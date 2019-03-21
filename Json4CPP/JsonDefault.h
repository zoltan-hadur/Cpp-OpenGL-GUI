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
    static int Indentation;
  };
}