#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonLinter.h"

using namespace std;
using namespace Json4CPP::Detail;

namespace Json4CPP
{
  JsonObject JsonObject::Read(deque<pair<JsonToken, VALUE_TOKEN>>& tokens)
  {
    if (tokens.empty())
    {
      auto message = WString2String(L"Expected token: " + Json::Stringify(JsonToken::StartObject) + L"!");
      throw exception(message.c_str());
    }

    auto object = JsonObject();
    auto property = L""s;
    auto counter = 0;
    auto& [token, value] = tokens.front();
    if (token == JsonToken::StartObject)
    {
      tokens.pop_front();
    }
    else
    {
      auto message = WString2String(L"Expected token: " + Json::Stringify(JsonToken::StartObject) + L"!");
      throw exception(message.c_str());
    }

    while (tokens.size())
    {
      tie(token, value) = tokens.front();
      // Property then value then property then value etc...
      if (counter++ % 2 == 0 && token != JsonToken::PropertyName)
      {
        auto message = WString2String(L"Expected token: " + Json::Stringify(JsonToken::PropertyName) + L"!");
        throw exception(message.c_str());
      }
      switch (token)
      {
      case JsonToken::PropertyName: property = get<wstring>(value);                                     tokens.pop_front(); break;
      case JsonToken::Null        : object._pairs.push_back({ property, Json(get<nullptr_t>(value)) }); tokens.pop_front(); break;
      case JsonToken::String      : object._pairs.push_back({ property, Json(get<wstring  >(value)) }); tokens.pop_front(); break;
      case JsonToken::Boolean     : object._pairs.push_back({ property, Json(get<bool     >(value)) }); tokens.pop_front(); break;
      case JsonToken::Number      : object._pairs.push_back({ property, Json(get<double   >(value)) }); tokens.pop_front(); break;
      case JsonToken::StartObject : object._pairs.push_back({ property, JsonObject::Read   (tokens) });                     break;
      case JsonToken::StartArray  : object._pairs.push_back({ property, JsonArray::Read    (tokens) });                     break;
      case JsonToken::EndObject   : tokens.pop_front(); return object;
      default:
      {
        auto message = WString2String(L"Invalid token: " + Json::Stringify(token) + L"!");
        throw exception(message.c_str());
      }
      }
    }
    auto message = WString2String(L"Expected token: " + Json::Stringify(JsonToken::EndObject) + L"!");
    throw exception(message.c_str());
  }

  void JsonObject::Write(JsonObject const& object, deque<pair<JsonToken, VALUE_TOKEN>>& tokens)
  {
    tokens.push_back({ JsonToken::StartObject, L"{"s });
    for (auto& [key, value] : object._pairs)
    {
      tokens.push_back({ JsonToken::PropertyName, key });
      Json::Write(value, tokens);
    }
    tokens.push_back({ JsonToken::EndObject, L"}"s });
  }

  void JsonObject::_Dump(wstringstream& os, uint8_t indentation, uint64_t level) const
  {
    auto indent = wstring(indentation * level, L' ');
    auto single = wstring(indentation, L' ');
    auto newLine = indentation == 0 ? L"" : L"\r\n";
    auto space = indentation == 0 ? L"" : L" ";
    auto size = _pairs.size();

    os << L"{" << newLine;
    for (int i = 0; i < size; ++i)
    {
      auto& [key, value] = _pairs[i];
      os << indent << single;
      os << L"\"" << key << L"\":" << space;
      switch (value.Type())
      {
      case JsonType::Object: value.Get<JsonObject>()._Dump(os, indentation, level + 1); break;
      case JsonType::Array : value.Get<JsonArray >()._Dump(os, indentation, level + 1); break;
      default: os << value; break;
      }
      if (i < size - 1)
      {
        os << L",";
      }
      os << newLine;
    }
    os << indent << L"}";
  }

  JsonObject::JsonObject(JsonBuilder builder)
  {
    if (auto object = get_if<JsonObject>(&builder._value))
    {
      *this = *object;
    }
    else if (auto builders = get_if<vector<JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        if (auto pair = get_if<vector<JsonBuilder>>(&builder._value))
        {
          auto key = get<KEY>((*pair)[0]._value);
          auto value = Json((*pair)[1]);
          Insert({ key, value });
        }
        else
        {
          VALUE value;
          visit(Overload{
            [&](auto const& arg) { value = arg; },
            [&](vector<JsonBuilder> const& arg) { },  // Empty as it is impossible
          }, builder._value);
          auto message = WString2String(L"JsonObject(JsonBuilder builder) is not defined for type " + Json::Stringify(builder.Type()) + L"!" +
                                        L" Error at: " + Json::Stringify(Json(value)) + L".");
          throw exception(message.c_str());
        }
      }
    }
    else
    {
      auto message = WString2String(L"JsonObject(JsonBuilder builder) is not defined for type " + Json::Stringify(builder.Type()) + L"!");
      throw exception(message.c_str());
    }
  }

  JsonObject::JsonObject(std::initializer_list<JsonBuilder> builders) : JsonObject(JsonBuilder(builders)) {}

  JsonObject::JsonObject(JsonObject const& object)
  {
    _pairs = object._pairs;
    for (int i = 0; i < _pairs.size(); ++i)
    {
      _indexes[_pairs[i].first] = i;
    }
  }

  wstring JsonObject::Dump(uint8_t indentation) const
  {
    wstringstream buffer;
    _Dump(buffer, indentation, 0);
    return buffer.str();
  }

  int64_t JsonObject::Size() const
  {
    return _pairs.size();
  }

  void JsonObject::Clear()
  {
    _pairs.clear();
    _indexes.clear();
  }

  bool JsonObject::Insert(pair<KEY, Json> pair)
  {
    if (_indexes.count(pair.first)) return false;
    _pairs.push_back(pair);
    _indexes[pair.first] = _pairs.size() - 1;
    return true;
  }

  void JsonObject::Erase(KEY key)
  {
    if (!_indexes.count(key)) return;
    _pairs.erase(std::remove_if(_pairs.begin(), _pairs.end(), [&](std::pair<KEY, Json> const& pair) { return pair.first == key; }), _pairs.end());
    _indexes.clear();
    for (int i = 0; i < _pairs.size(); ++i)
    {
      _indexes[_pairs[i].first] = i;
    }
  }

  std::vector<KEY> JsonObject::Keys() const
  {
    vector<KEY> keys;
    transform(_pairs.begin(), _pairs.end(), back_inserter(keys), [](pair<KEY, Json> const& pair) { return pair.first; });
    return keys;
  }

  Json& JsonObject::operator[](KEY const& key)
  {
    if (!_indexes.count(key)) Insert({ key, Json{} });
    return _pairs[_indexes.at(key)].second;
  }

  Json JsonObject::At(KEY const& key) const
  {
    return _pairs[_indexes.at(key)].second;
  }

  Json& JsonObject::At(KEY const& key)
  {
    return _pairs[_indexes.at(key)].second;
  }

  std::vector<std::pair<KEY, Json>>::iterator JsonObject::begin()
  {
    return _pairs.begin();
  }

  std::vector<std::pair<KEY, Json>>::iterator JsonObject::end()
  {
    return _pairs.end();
  }

  std::vector<std::pair<KEY, Json>>::const_iterator JsonObject::begin() const
  {
    return _pairs.begin();
  }

  std::vector<std::pair<KEY, Json>>::const_iterator JsonObject::end() const
  {
    return _pairs.end();
  }

  wostream& operator<<(wostream& os, JsonObject const& object)
  {
    return os << object.Dump(JsonDefault::Indentation);
  }

  wistream& operator>>(wistream& is, JsonObject& object)
  {
    object = JsonObject::Read(JsonLinter::Read(is));
    return is;
  }

  bool operator==(JsonObject const& left, JsonObject const& right)
  {
    return left._pairs == right._pairs;
  }

  bool operator!=(JsonObject const& left, JsonObject const& right)
  {
    return left._pairs != right._pairs;
  }
}