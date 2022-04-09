#include "stdafx.h"

#include "JsonBuilderType.h"

using namespace std::string_literals;

namespace Json4CPP::Detail
{
  std::wostream& operator<<(std::wostream& os, JsonBuilderType type)
  {
    switch (type)
    {
    case JsonBuilderType::Null   : return os << L"Null"s   ;
    case JsonBuilderType::String : return os << L"String"s ;
    case JsonBuilderType::Boolean: return os << L"Boolean"s;
    case JsonBuilderType::Real   : return os << L"Real"s   ;
    case JsonBuilderType::Integer: return os << L"Integer"s;
    case JsonBuilderType::Number : return os << L"Number"s ;
    case JsonBuilderType::Object : return os << L"Object"s ;
    case JsonBuilderType::Array  : return os << L"Array"s  ;
    case JsonBuilderType::Pair   : return os << L"Pair"s   ;
    case JsonBuilderType::Empty  : return os << L"Empty"s  ;
    case JsonBuilderType::Simple : return os << L"Simple"s ;
    case JsonBuilderType::Complex: return os << L"Complex"s;
    default:
    case JsonBuilderType::Invalid: return os << L"Invalid"s;
    }
  }

  bool operator==(JsonBuilderType left, JsonBuilderType right)
  {
    return static_cast<int>(left) & static_cast<int>(right);
  }

  bool operator!=(JsonBuilderType left, JsonBuilderType right)
  {
    return !(left == right);
  }
}