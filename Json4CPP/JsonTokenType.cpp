#include "stdafx.h"

#include "JsonTokenType.h"

using namespace std::string_literals;

namespace Json4CPP::Detail
{
  std::wostream& operator<<(std::wostream& os, JsonTokenType token)
  {
    switch (token)
    {
    using enum JsonTokenType;
    case Null        : return os << L"Null"s        ;
    case String      : return os << L"String"s      ;
    case Boolean     : return os << L"Boolean"s     ;
    case Real        : return os << L"Real"s        ;
    case Integer     : return os << L"Integer"s     ;
    case PropertyName: return os << L"PropertyName"s;
    case StartObject : return os << L"StartObject"s ;
    case EndObject   : return os << L"EndObject"s   ;
    case StartArray  : return os << L"StartArray"s  ;
    case EndArray    : return os << L"EndArray"s    ;
    default:
    case Undefined   : return os << L"Undefined"s   ;
    }
  }
}