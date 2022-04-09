#include "stdafx.h"

#include "JsonType.h"

using namespace std::string_literals;

namespace Json4CPP
{
  std::wostream& operator<<(std::wostream& os, JsonType type)
  {
    switch (type)
    {
    using enum JsonType;
    case Null   : return os << L"Null"s   ;
    case String : return os << L"String"s ;
    case Boolean: return os << L"Boolean"s;
    case Real   : return os << L"Real"s   ;
    case Integer: return os << L"Integer"s;
    case Number : return os << L"Number"s ;
    case Object : return os << L"Object"s ;
    case Array  : return os << L"Array"s  ;
    case Simple : return os << L"Simple"s ;
    case Complex: return os << L"Complex"s;
    default:
    case Invalid: return os << L"Invalid"s;
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