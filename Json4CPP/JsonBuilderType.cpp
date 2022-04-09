#include "stdafx.h"

#include "JsonBuilderType.h"

using namespace std::string_literals;

namespace Json4CPP::Detail
{
  std::wostream& operator<<(std::wostream& os, JsonBuilderType type)
  {
    switch (type)
    {
    using enum JsonBuilderType;
    case Null   : return os << L"Null"s   ;
    case String : return os << L"String"s ;
    case Boolean: return os << L"Boolean"s;
    case Real   : return os << L"Real"s   ;
    case Integer: return os << L"Integer"s;
    case Number : return os << L"Number"s ;
    case Object : return os << L"Object"s ;
    case Array  : return os << L"Array"s  ;
    case Pair   : return os << L"Pair"s   ;
    case Empty  : return os << L"Empty"s  ;
    case Simple : return os << L"Simple"s ;
    case Complex: return os << L"Complex"s;
    default:
    case Invalid: return os << L"Invalid"s;
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