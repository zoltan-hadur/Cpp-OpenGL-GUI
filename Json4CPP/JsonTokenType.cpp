#include "stdafx.h"

#include "JsonTokenType.h"

using namespace std;

namespace Json4CPP::Detail
{
  wostream& operator<<(wostream& os, JsonTokenType const& token)
  {
    switch (token)
    {
    case JsonTokenType::Null        : return os << L"Null"s;
    case JsonTokenType::String      : return os << L"String"s;
    case JsonTokenType::Boolean     : return os << L"Boolean"s;
    case JsonTokenType::Real        : return os << L"Real"s;
    case JsonTokenType::Integer     : return os << L"Integer"s;
    case JsonTokenType::PropertyName: return os << L"PropertyName"s;
    case JsonTokenType::StartObject : return os << L"StartObject"s;
    case JsonTokenType::EndObject   : return os << L"EndObject"s;
    case JsonTokenType::StartArray  : return os << L"StartArray"s;
    case JsonTokenType::EndArray    : return os << L"EndArray"s;
    default:
    case JsonTokenType::Undefined   : return os << L"Undefined"s;
    }
  }

  wstring Dump(JsonTokenType const& token)
  {
    wostringstream os;
    os << token;
    return os.str();
  }
}