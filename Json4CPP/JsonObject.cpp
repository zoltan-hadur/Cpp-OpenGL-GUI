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
  JsonObject JsonObject::Read(deque<TOKEN>& tokens)
  {
    if (tokens.empty())
    {
      auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::StartObject) + L"!"s);
      throw exception(message.c_str());
    }

    auto object = JsonObject();
    auto property = L""s;
    auto counter = 0;
    auto& [token, value] = tokens.front();
    if (token == JsonTokenType::StartObject)
    {
      tokens.pop_front();
    }
    else
    {
      auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::StartObject) + L"!"s);
      throw exception(message.c_str());
    }

    while (tokens.size())
    {
      tie(token, value) = tokens.front();
      // Property then value then property then value etc...
      if (counter++ % 2 == 0 && token != JsonTokenType::PropertyName && token != JsonTokenType::EndObject)
      {
        auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::PropertyName) + L"!"s);
        throw exception(message.c_str());
      }
      switch (token)
      {
      case JsonTokenType::PropertyName: property = get<wstring>(value);                                     tokens.pop_front(); break;
      case JsonTokenType::Null        : object._pairs.push_back({ property, Json(get<nullptr_t>(value)) }); tokens.pop_front(); break;
      case JsonTokenType::String      : object._pairs.push_back({ property, Json(get<wstring  >(value)) }); tokens.pop_front(); break;
      case JsonTokenType::Boolean     : object._pairs.push_back({ property, Json(get<bool     >(value)) }); tokens.pop_front(); break;
      case JsonTokenType::Number      : object._pairs.push_back({ property, Json(get<double   >(value)) }); tokens.pop_front(); break;
      case JsonTokenType::StartObject : object._pairs.push_back({ property, JsonObject::Read   (tokens) });                     break;
      case JsonTokenType::StartArray  : object._pairs.push_back({ property, JsonArray::Read    (tokens) });                     break;
      case JsonTokenType::EndObject   : tokens.pop_front(); return object;
      default:
      {
        auto message = WString2String(L"Invalid token: "s + Json::Stringify(token) + L"!"s);
        throw exception(message.c_str());
      }
      }
    }
    auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::EndObject) + L"!"s);
    throw exception(message.c_str());
  }

  void JsonObject::Write(JsonObject const& object, deque<TOKEN>& tokens)
  {
    tokens.push_back({ JsonTokenType::StartObject, L"{"s });
    for (auto& [key, value] : object)
    {
      tokens.push_back({ JsonTokenType::PropertyName, key });
      Json::Write(value, tokens);
    }
    tokens.push_back({ JsonTokenType::EndObject, L"}"s });
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
          auto message = WString2String(L"JsonObject(JsonBuilder builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s +
                                        L" Error at: "s + Json::Stringify(Json(value)) + L"."s);
          throw exception(message.c_str());
        }
      }
    }
    else
    {
      auto message = WString2String(L"JsonObject(JsonBuilder builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw exception(message.c_str());
    }
  }

  JsonObject::JsonObject(initializer_list<JsonBuilder> builders) : JsonObject(JsonBuilder(builders))
  {

  }

  JsonObject::JsonObject(JsonObject const& object)
  {
    _pairs = object._pairs;
    _indexes = object._indexes;
  }

  wstring JsonObject::Dump(uint8_t indentation) const
  {
    auto tokens = deque<TOKEN>();
    JsonObject::Write(*this, tokens);
    wstringstream os;
    JsonLinter::Write(os, tokens, indentation, 0);
    return os.str();
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
    _pairs.erase(remove_if(_pairs.begin(), _pairs.end(), [&](pair<KEY, Json> const& pair) { return pair.first == key; }), _pairs.end());
    _indexes.clear();
    for (int i = 0; i < _pairs.size(); ++i)
    {
      _indexes[_pairs[i].first] = i;
    }
  }

  vector<KEY> JsonObject::Keys() const
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

  vector<pair<KEY, Json>>::iterator JsonObject::begin()
  {
    return _pairs.begin();
  }

  vector<pair<KEY, Json>>::iterator JsonObject::end()
  {
    return _pairs.end();
  }

  vector<pair<KEY, Json>>::const_iterator JsonObject::begin() const
  {
    return _pairs.begin();
  }

  vector<pair<KEY, Json>>::const_iterator JsonObject::end() const
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