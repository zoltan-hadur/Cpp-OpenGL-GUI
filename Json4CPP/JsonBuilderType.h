#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

namespace Json4CPP::Detail
{
  // Available values: Null, String, Boolean, Real, Integer, Number, Object, Array, Pair, Empty, Simple, Complex, Invalid.
  enum class JSON_API JsonBuilderType
  {
    Null    = 1 << 0,                             // Value is std::nullptr.
    String  = 1 << 1,                             // Value is std::wstring.
    Boolean = 1 << 2,                             // Value is bool.
    Real    = 1 << 3,                             // Value is double.
    Integer = 1 << 4,                             // Value is int64_t.
    Number  = Real | Integer,                     // Value is Real | Integer.
    Object  = 1 << 5,                             // Value is JsonObject.
    Array   = 1 << 6,                             // Value is JsonArray.
    Pair    = 1 << 7,                             // Value is { key: value } pair.
    Empty   = 1 << 8,                             // Value is {}.
    Simple  = Null | String | Boolean | Number,   // Value is Null | String | Boolean | Number.
    Complex = Object | Array | Pair | Empty,      // Value is Object | Array | Pair | Empty.
    Invalid = 1 << 31
  };

  JSON_API std::wostream& operator<<(std::wostream& os, JsonBuilderType type);

  JSON_API bool operator==(JsonBuilderType left, JsonBuilderType right);
  JSON_API bool operator!=(JsonBuilderType left, JsonBuilderType right);
}