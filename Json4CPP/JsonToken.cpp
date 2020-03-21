#include "stdafx.h"

#include "JsonToken.h"

using namespace std;

namespace Json4CPP::Detail
{
  wostream& operator<<(wostream& os, JsonToken const& token)
  {
    switch (token)
    {
    case JsonToken::Null        : return os << L"Null"s;
    case JsonToken::String      : return os << L"String"s;
    case JsonToken::Boolean     : return os << L"Boolean"s;
    case JsonToken::Number      : return os << L"Number"s;
    case JsonToken::PropertyName: return os << L"PropertyName"s;
    case JsonToken::StartObject : return os << L"StartObject"s;
    case JsonToken::EndObject   : return os << L"EndObject"s;
    case JsonToken::StartArray  : return os << L"StartArray"s;
    case JsonToken::EndArray    : return os << L"EndArray"s;
    default:
    case JsonToken::Undefined   : return os << L"Undefined"s;
    }
  }

  wstring Dump(JsonToken const& token)
  {
    wostringstream os;
    os << token;
    return os.str();
  }
}