#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

namespace Json4CPP
{
  // Available values: Null, String, Boolean, Real, Integer, Number, Object, Array, Simple, Complex, Invalid.
  enum class JSON_API JsonType
  {
    Null    = 1 << 0,                             // Value is std::nullptr.
    String  = 1 << 1,                             // Value is std::wstring.
    Boolean = 1 << 2,                             // Value is bool.
    Real    = 1 << 3,                             // Value is double.
    Integer = 1 << 4,                             // Value is int64_t.
    Number  = Real | Integer,                     // Value is Real | Integer.
    Object  = 1 << 5,                             // Value is JsonObject.
    Array   = 1 << 6,                             // Value is JsonArray.
    Simple  = Null | String | Boolean | Number,   // Value is Null | String | Boolean | Number.
    Complex = Object | Array,                     // Value is Object | Array.
    Invalid = 1 << 31
  };

  JSON_API std::wostream& operator<<(std::wostream& os, JsonType type);

  JSON_API bool operator==(JsonType left, JsonType right);
  JSON_API bool operator!=(JsonType left, JsonType right);
}