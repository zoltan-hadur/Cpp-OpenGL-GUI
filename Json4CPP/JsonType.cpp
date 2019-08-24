#include "stdafx.h"

#include "JsonType.h"

using namespace std;

namespace Json4CPP
{
  wostream& operator<<(wostream& os, JsonType const& type)
  {
    switch (type)
    {
    case JsonType::Null:    return os << L"Null";
    case JsonType::String:  return os << L"String";
    case JsonType::Boolean: return os << L"Boolean";
    case JsonType::Number:  return os << L"Number";
    case JsonType::Object:  return os << L"Object";
    case JsonType::Array:   return os << L"Array";
    case JsonType::Simple:  return os << L"Simple";
    case JsonType::Complex: return os << L"Complex";
    default:
    case JsonType::Invalid: return os << L"Invalid";
    }
  }

  bool operator==(JsonType left, JsonType right)
  {
    return (int)left & (int)right;
  }

  bool operator!=(JsonType left, JsonType right)
  {
    return !(left == right);
  }

  JsonType operator|(JsonType left, JsonType right)
  {
    return static_cast<JsonType>(static_cast<std::underlying_type_t<JsonType>>(left) | static_cast<std::underlying_type_t<JsonType>>(right));
  }

  JsonType operator&(JsonType left, JsonType right)
  {
    return static_cast<JsonType>(static_cast<std::underlying_type_t<JsonType>>(left) & static_cast<std::underlying_type_t<JsonType>>(right));
  }
}