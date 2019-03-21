#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

namespace Json4CPP
{
  enum class JSON_API JsonType
  {
    Null    = 1 << 0,                             // Value is std::nullptr.
    String  = 1 << 1,                             // Value is std::wstring.
    Boolean = 1 << 2,                             // Value is bool.
    Number  = 1 << 3,                             // Value is double.
    Object  = 1 << 4,                             // Value is JsonObject.
    Array   = 1 << 5,                             // Value is JsonArray.
    Simple  = Null | String | Boolean | Number,   // Value is Null | String | Boolean | Number.
    Complex = Object | Array,                     // Value is Object | Array.
    Invalid = 1 << 31
  };

  inline bool operator==(JsonType left, JsonType right)
  {
    return (int)left & (int)right;
  }

  inline bool operator!=(JsonType left, JsonType right)
  {
    return !(left == right);
  }

  inline JsonType operator|(JsonType left, JsonType right)
  {
    return static_cast<JsonType>(static_cast<std::underlying_type_t<JsonType>>(left) | static_cast<std::underlying_type_t<JsonType>>(right));
  }

  inline JsonType operator&(JsonType left, JsonType right)
  {
    return static_cast<JsonType>(static_cast<std::underlying_type_t<JsonType>>(left) & static_cast<std::underlying_type_t<JsonType>>(right));
  }
}