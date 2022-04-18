#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "Value.h"

#include <sstream>
#include <string>

namespace Json4CPP::Detail
{
  // Available values: Undefined, Null, String, Boolean, Real, Integer, PropertyName, StartObject, EndObject, StartArray, EndArray.
  enum class JSON_API JsonTokenType
  {
    Undefined,
    Null,
    String,
    Boolean,
    Real,
    Integer,
    PropertyName,
    StartObject,
    EndObject,
    StartArray,
    EndArray,
  };

  JSON_API std::wostream& operator<<(std::wostream& os, JsonTokenType token);
}