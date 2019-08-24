#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

namespace Json4CPP::Detail
{
  enum class JSON_API JsonBuilderType
  {
    Null    = 1 << 0,                             // Value is std::nullptr.
    String  = 1 << 1,                             // Value is std::wstring.
    Boolean = 1 << 2,                             // Value is bool.
    Number  = 1 << 3,                             // Value is double.
    Object  = 1 << 4,                             // Value is JsonObject.
    Array   = 1 << 5,                             // Value is JsonArray.
    Pair    = 1 << 6,                             // Value is { key: value } pair.
    Empty   = 1 << 7,                             // Value is {}.
    Simple  = Null | String | Boolean | Number,   // Value is Null | String | Boolean | Number.
    Complex = Object | Array | Pair | Empty,      // Value is Object | Array | Pair | Empty.
    Invalid = 1 << 31
  };

  inline bool operator==(JsonBuilderType left, JsonBuilderType right)
  {
    return (int)left & (int)right;
  }

  inline bool operator!=(JsonBuilderType left, JsonBuilderType right)
  {
    return !(left == right);
  }

  inline JsonBuilderType operator|(JsonBuilderType left, JsonBuilderType right)
  {
    return static_cast<JsonBuilderType>(static_cast<std::underlying_type_t<JsonBuilderType>>(left) | static_cast<std::underlying_type_t<JsonBuilderType>>(right));
  }

  inline JsonBuilderType operator&(JsonBuilderType left, JsonBuilderType right)
  {
    return static_cast<JsonBuilderType>(static_cast<std::underlying_type_t<JsonBuilderType>>(left) & static_cast<std::underlying_type_t<JsonBuilderType>>(right));
  }
}