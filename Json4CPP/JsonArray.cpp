#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"

using namespace std;
using namespace Json4CPP::Detail;

namespace Json4CPP
{
  JsonArray JsonArray::Read(deque<TOKEN>& tokens)
  {
    if (tokens.empty())
    {
      auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::StartArray) + L"!"s);
      throw exception(message.c_str());
    }

    auto array = JsonArray();
    auto& [token, value] = tokens.front();
    if (token == JsonTokenType::StartArray)
    {
      tokens.pop_front();
    }
    else
    {
      auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::StartArray) + L"!"s);
      throw exception(message.c_str());
    }

    while (tokens.size())
    {
      tie(token, value) = tokens.front();
      switch (token)
      {
      case JsonTokenType::Null       : array._values.push_back(     get<nullptr_t>(value)) ; tokens.pop_front(); break;
      case JsonTokenType::String     : array._values.push_back(move(get<wstring  >(value))); tokens.pop_front(); break;
      case JsonTokenType::Boolean    : array._values.push_back(     get<bool     >(value)) ; tokens.pop_front(); break;
      case JsonTokenType::Real       : array._values.push_back(     get<double   >(value)) ; tokens.pop_front(); break;
      case JsonTokenType::Integer    : array._values.push_back(     get<int64_t  >(value)) ; tokens.pop_front(); break;
      case JsonTokenType::StartObject: array._values.push_back(JsonObject::Read   (tokens));                     break;
      case JsonTokenType::StartArray : array._values.push_back(JsonArray ::Read   (tokens));                     break;
      case JsonTokenType::EndArray   : tokens.pop_front(); return array;
      default:
      {
        auto message = WString2String(L"Invalid token: "s + Json::Stringify(token) + L"!"s);
        throw exception(message.c_str());
      }
      }
    }
    auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::EndArray) + L"!"s);
    throw exception(message.c_str());
  }

  JsonArray JsonArray::Read(deque<TOKEN> && tokens)
  {
    return Read(tokens);
  }

  deque<TOKEN>& JsonArray::Write(JsonArray const& array, deque<TOKEN>& tokens)
  {
    tokens.push_back({ JsonTokenType::StartArray, L"["s });
    for (auto& value : array)
    {
      Json::Write(value, tokens);
    }
    tokens.push_back({ JsonTokenType::EndArray, L"]"s });
    return tokens;
  }

  deque<TOKEN> && JsonArray::Write(JsonArray const& array, deque<TOKEN> && tokens)
  {
    return move(Write(array, tokens));
  }

  JsonArray::JsonArray(Json const& json)
  {
    *this = json.operator const Json4CPP::JsonArray & ();
  }

  JsonArray::JsonArray(Json && json)
  {
    *this = move(json.operator Json4CPP::JsonArray && ());
  }

  JsonArray::JsonArray(JsonBuilder const& builder)
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
      auto message = WString2String(L"JsonArray(JsonBuilder const& builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw exception(message.c_str());
    }
  }

  JsonArray::JsonArray(JsonBuilder && builder)
  {
    if (auto array = get_if<JsonArray>(&builder._value))
    {
      *this = move(*array);
    }
    else if (auto builders = get_if<vector<JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        _values.push_back(Json(move(builder)));
      }
    }
    else
    {
      auto message = WString2String(L"JsonArray(JsonBuilder && builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw exception(message.c_str());
    }
  }

  JsonArray::JsonArray(initializer_list<JsonBuilder> builders) : JsonArray(JsonBuilder(builders))
  {

  }

  wstring JsonArray::Dump(uint8_t indentSize, wchar_t indentChar) const
  {
    wstringstream os;
    JsonLinter::Write(os, JsonArray::Write(*this, deque<TOKEN>()), indentSize, indentChar);
    return os.str();
  }

  int64_t JsonArray::Size() const
  {
    return _values.size();
  }

  void JsonArray::Resize(int64_t size)
  {
    _values.resize(size);
  }

  void JsonArray::Resize(int64_t size, Json const& json)
  {
    _values.resize(size, json);
  }

  void JsonArray::Clear()
  {
    _values.clear();
  }

  void JsonArray::PushBack(Json const& value)
  {
    _values.push_back(value);
  }

  void JsonArray::PushBack(Json && value)
  {
    _values.push_back(move(value));
  }

  void JsonArray::Insert(int64_t index, Json const& value)
  {
    _values.insert(_values.begin() + index, value);
  }

  void JsonArray::Insert(int64_t index, Json && value)
  {
    _values.insert(_values.begin() + index, move(value));
  }
  
  void JsonArray::Erase(int64_t index)
  {
    _values.erase(_values.begin() + index);
  }

  Json& JsonArray::operator[](int64_t index)
  {
    return _values[index];
  }

  Json& JsonArray::At(int64_t index)
  {
    return _values[index];
  }

  Json const& JsonArray::At(int64_t index) const
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
    auto indentSizeActive = JsonIndentSize::IsActive(os);
    auto indentCharActive = JsonIndentChar::IsActive(os);
    JsonLinter::Write(os, JsonArray::Write(array, deque<TOKEN>()), indentSizeActive ? JsonIndentSize::GetSize(os) : JsonDefault::IndentSize,
                                                                   indentCharActive ? JsonIndentChar::GetChar(os) : JsonDefault::IndentChar);
    if (indentSizeActive) JsonIndentSize::ResetState(os);
    if (indentCharActive) JsonIndentChar::ResetState(os);
    return os;
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