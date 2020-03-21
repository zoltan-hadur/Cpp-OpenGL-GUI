#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"

using namespace std;
using namespace Json4CPP::Detail;

namespace Json4CPP
{
  JsonArray JsonArray::Read(TOKEN_COLLECTION& tokens)
  {
    if (tokens.empty())
    {
      auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonToken::StartArray) + L"!"s);
      throw exception(message.c_str());
    }

    auto array = JsonArray();
    auto& [token, value] = tokens.front();
    if (token == JsonToken::StartArray)
    {
      tokens.pop_front();
    }
    else
    {
      auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonToken::StartArray) + L"!"s);
      throw exception(message.c_str());
    }

    while (tokens.size())
    {
      tie(token, value) = tokens.front();
      switch (token)
      {
      case JsonToken::Null       : array._values.push_back(Json(get<nullptr_t>(value))); tokens.pop_front(); break;
      case JsonToken::String     : array._values.push_back(Json(get<wstring  >(value))); tokens.pop_front(); break;
      case JsonToken::Boolean    : array._values.push_back(Json(get<bool     >(value))); tokens.pop_front(); break;
      case JsonToken::Number     : array._values.push_back(Json(get<double   >(value))); tokens.pop_front(); break;
      case JsonToken::StartObject: array._values.push_back(JsonObject::Read   (tokens));                     break;
      case JsonToken::StartArray : array._values.push_back(JsonArray::Read    (tokens));                     break;
      case JsonToken::EndArray   : tokens.pop_front(); return array;
      default:
      {
        auto message = WString2String(L"Invalid token: "s + Json::Stringify(token) + L"!"s);
        throw exception(message.c_str());
      }
      }
    }
    auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonToken::EndArray) + L"!"s);
    throw exception(message.c_str());
  }

  void JsonArray::Write(JsonArray const& array, TOKEN_COLLECTION& tokens)
  {
    tokens.push_back({ JsonToken::StartArray, L"["s });
    for (auto& value : array._values)
    {
      Json::Write(value, tokens);
    }
    tokens.push_back({ JsonToken::EndArray, L"]"s });
  }

  void JsonArray::_Dump(wstringstream& os, uint8_t indentation, uint64_t level) const
  {
    auto indent = wstring(indentation * level, L' ');
    auto single = wstring(indentation, L' ');
    auto newLine = indentation == 0 ? L""s : L"\r\n"s;
    auto size = _values.size();

    os << L"["s << newLine;
    for (int i = 0; i < size; ++i)
    {
      auto& value = _values[i];
      os << indent << single;
      switch (value.Type())
      {
      case JsonType::Object: value.Get<JsonObject>()._Dump(os, indentation, level + 1); break;
      case JsonType::Array : value.Get<JsonArray >()._Dump(os, indentation, level + 1); break;
      default: os << value; break;
      }
      if (i < size - 1)
      {
        os << L","s;
      }
      os << newLine;
    }
    os << indent << L"]"s;
  }

  JsonArray::JsonArray(JsonBuilder builder)
  {
    if (auto array = get_if<JsonArray>(&builder._value))
    {
      *this = *array;
    }
    else if (auto builders = get_if<vector<JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        _values.push_back(Json(builder));
      }
    }
    else
    {
      auto message = WString2String(L"JsonArray(JsonBuilder builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw exception(message.c_str());
    }
  }

  JsonArray::JsonArray(initializer_list<JsonBuilder> builders) : JsonArray(JsonBuilder(builders))
  {

  }

  JsonArray::JsonArray(JsonArray const& array)
  {
    _values = array._values;
  }

  wstring JsonArray::Dump(uint8_t indentation) const
  {
    wstringstream buffer;
    _Dump(buffer, indentation, 0);
    return buffer.str();
  }

  int64_t JsonArray::Size() const
  {
    return _values.size();
  }

  void JsonArray::Resize(int64_t size)
  {
    _values.resize(size);
  }

  void JsonArray::Clear()
  {
    _values.clear();
  }

  void JsonArray::PushBack(Json value)
  {
    _values.push_back(value);
  }

  void JsonArray::Insert(Json value, int64_t index)
  {
    _values.insert(_values.begin() + index, value);
  }
  
  void JsonArray::Erase(int64_t index)
  {
    _values.erase(_values.begin() + index);
  }

  Json& JsonArray::operator[](int64_t const& index)
  {
    return _values[index];
  }

  Json JsonArray::At(int64_t const& index) const
  {
    return _values[index];
  }

  Json& JsonArray::At(int64_t const& index)
  {
    return _values[index];
  }

  vector<Json>::iterator JsonArray::begin()
  {
    return _values.begin();
  }

  vector<Json>::iterator JsonArray::end()
  {
    return _values.end();
  }

  vector<Json>::const_iterator JsonArray::begin() const
  {
    return _values.begin();
  }

  vector<Json>::const_iterator JsonArray::end() const
  {
    return _values.end();
  }

  wostream& operator<<(wostream& os, JsonArray const& array)
  {
    return os << array.Dump(JsonDefault::Indentation);
  }

  wistream& operator>>(wistream& is, JsonArray& array)
  {
    array = JsonArray::Read(JsonLinter::Read(is));
    return is;
  }

  bool operator==(JsonArray const& left, JsonArray const& right)
  {
    return left._values == right._values;
  }

  bool operator!=(JsonArray const& left, JsonArray const& right)
  {
    return left._values != right._values;
  }
}