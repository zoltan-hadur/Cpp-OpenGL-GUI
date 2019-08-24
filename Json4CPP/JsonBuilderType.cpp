#include "stdafx.h"

#include "JsonBuilderType.h"

using namespace std;

namespace Json4CPP::Detail
{
  wostream& operator<<(wostream& os, JsonBuilderType const& type)
  {
    switch (type)
    {
    case JsonBuilderType::Null:    return os << L"Null";
    case JsonBuilderType::String:  return os << L"String";
    case JsonBuilderType::Boolean: return os << L"Boolean";
    case JsonBuilderType::Number:  return os << L"Number";
    case JsonBuilderType::Object:  return os << L"Object";
    case JsonBuilderType::Array:   return os << L"Array";
    case JsonBuilderType::Pair:    return os << L"Pair";
    case JsonBuilderType::Empty:   return os << L"Empty";
    case JsonBuilderType::Simple:  return os << L"Simple";
    case JsonBuilderType::Complex: return os << L"Complex";
    default:
    case JsonBuilderType::Invalid: return os << L"Invalid";
    }
  }

  bool operator==(JsonBuilderType left, JsonBuilderType right)
  {
    return (int)left & (int)right;
  }

  bool operator!=(JsonBuilderType left, JsonBuilderType right)
  {
    return !(left == right);
  }

  JsonBuilderType operator|(JsonBuilderType left, JsonBuilderType right)
  {
    return static_cast<JsonBuilderType>(static_cast<std::underlying_type_t<JsonBuilderType>>(left) | static_cast<std::underlying_type_t<JsonBuilderType>>(right));
  }

  JsonBuilderType operator&(JsonBuilderType left, JsonBuilderType right)
  {
    return static_cast<JsonBuilderType>(static_cast<std::underlying_type_t<JsonBuilderType>>(left) & static_cast<std::underlying_type_t<JsonBuilderType>>(right));
  }
}