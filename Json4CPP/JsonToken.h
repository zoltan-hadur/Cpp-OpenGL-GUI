#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

namespace Json4CPP::Detail
{
  enum class JSON_API JsonToken
  {
    Undefined,
    Null,
    String,
    Boolean,
    Number,
    PropertyName,
    StartObject,
    EndObject,
    StartArray,
    EndArray,
  };
}