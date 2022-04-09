#include "stdafx.h"

#include "JsonType.h"

using namespace std::string_literals;

namespace Json4CPP
{
  std::wostream& operator<<(std::wostream& os, JsonType type)
  {
    switch (type)
    {
    case JsonType::Null   : return os << L"Null"s   ;
    case JsonType::String : return os << L"String"s ;
    case JsonType::Boolean: return os << L"Boolean"s;
    case JsonType::Real   : return os << L"Real"s   ;
    case JsonType::Integer: return os << L"Integer"s;
    case JsonType::Number : return os << L"Number"s ;
    case JsonType::Object : return os << L"Object"s ;
    case JsonType::Array  : return os << L"Array"s  ;
    case JsonType::Simple : return os << L"Simple"s ;
    case JsonType::Complex: return os << L"Complex"s;
    default:
    case JsonType::Invalid: return os << L"Invalid"s;
    }
  }

  bool operator==(JsonType left, JsonType right)
  {
    return static_cast<int>(left) & static_cast<int>(right);
  }

  bool operator!=(JsonType left, JsonType right)
  {
    return !(left == right);
  }
}