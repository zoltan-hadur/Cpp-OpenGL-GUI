#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonLinter.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"

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
    {
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
    }

    while (tokens.size())
    {
      auto& [token, value] = tokens.front();
      // Property then value then property then value etc...
      if (counter++ % 2 == 0 && token != JsonTokenType::PropertyName && token != JsonTokenType::EndObject)
      {
        auto message = WString2String(L"Expected token: "s + Json::Stringify(JsonTokenType::PropertyName) + L"!"s);
        throw exception(message.c_str());
      }
      switch (token)
      {
      case JsonTokenType::PropertyName:                      property =      move(get<wstring  >(value))    ; tokens.pop_front(); break;
      case JsonTokenType::Null        : object.Insert({ move(property),           get<nullptr_t>(value)   }); tokens.pop_front(); break;
      case JsonTokenType::String      : object.Insert({ move(property),      move(get<wstring  >(value))  }); tokens.pop_front(); break;
      case JsonTokenType::Boolean     : object.Insert({ move(property),           get<bool     >(value)   }); tokens.pop_front(); break;
      case JsonTokenType::Real        : object.Insert({ move(property),           get<double   >(value)   }); tokens.pop_front(); break;
      case JsonTokenType::Integer     : object.Insert({ move(property),           get<int64_t  >(value)   }); tokens.pop_front(); break;
      case JsonTokenType::StartObject : object.Insert({ move(property), JsonObject::Read(tokens)          });                     break;
      case JsonTokenType::StartArray  : object.Insert({ move(property), JsonArray ::Read(tokens)          });                     break;
      case JsonTokenType::EndObject   :                                                                       tokens.pop_front(); return object;
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

  JsonObject JsonObject::Read(deque<TOKEN> && tokens)
  {
    return Read(tokens);
  }

  deque<TOKEN>& JsonObject::Write(JsonObject const& object, deque<TOKEN>& tokens)
  {
    tokens.push_back({ JsonTokenType::StartObject, L"{"s });
    for (auto& [key, value] : object)
    {
      tokens.push_back({ JsonTokenType::PropertyName, key });
      Json::Write(value, tokens);
    }
    tokens.push_back({ JsonTokenType::EndObject, L"}"s });
    return tokens;
  }

  deque<TOKEN> && JsonObject::Write(JsonObject const& object, deque<TOKEN> && tokens)
  {
    return move(Write(object, tokens));
  }

  JsonObject::JsonObject(Json const& json)
  {
    *this = json.operator const Json4CPP::JsonObject & ();
  }

  JsonObject::JsonObject(Json && json)
  {
    *this = move(json.operator Json4CPP::JsonObject && ());
  }

  JsonObject::JsonObject(JsonBuilder const& builder)
  {
    if (auto object = get_if<JsonObject>(&builder._value))
    {
      *this = *object;
    }
    else if (auto builders = get_if<vector<JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        if (builder.Is(JsonBuilderType::Pair))
        {
          auto pair = get_if<vector<JsonBuilder>>(&builder._value);
          auto key = get<wstring>((*pair)[0]._value);
          auto value = Json((*pair)[1]);
          Insert({ move(key), move(value) });
        }
        else
        {
          VALUE value;
          visit(Overload{
            [&](auto const& arg) { value = arg; },
            [&](vector<JsonBuilder> const& arg) { value = JsonArray(arg); },
          }, builder._value);
          auto message = WString2String(L"JsonObject(JsonBuilder const& builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s +
                                        L" Error at: "s + Json::Stringify(Json(value)) + L"."s);
          throw exception(message.c_str());
        }
      }
    }
    else
    {
      auto message = WString2String(L"JsonObject(JsonBuilder const& builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw exception(message.c_str());
    }
  }

  JsonObject::JsonObject(JsonBuilder && builder)
  {
    if (auto object = get_if<JsonObject>(&builder._value))
    {
      *this = move(*object);
    }
    else if (auto builders = get_if<vector<JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        if (builder.Is(JsonBuilderType::Pair))
        {
          auto pair = get_if<vector<JsonBuilder>>(&builder._value);
          auto key = get<wstring>(move((*pair)[0]._value));
          auto value = Json(move((*pair)[1]));
          Insert({ move(key), move(value) });
        }
        else
        {
          VALUE value;
          visit(Overload{
            [&](auto const& arg) { value = move(arg); },
            [&](vector<JsonBuilder> const& arg) { value = JsonArray(move(arg)); },
          }, builder._value);
          auto message = WString2String(L"JsonObject(JsonBuilder && builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s +
                                        L" Error at: "s + Json::Stringify(Json(value)) + L"."s);
          throw exception(message.c_str());
        }
      }
    }
    else
    {
      auto message = WString2String(L"JsonObject(JsonBuilder && builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw exception(message.c_str());
    }
  }

  JsonObject::JsonObject(initializer_list<JsonBuilder> builders) : JsonObject(JsonBuilder(builders))
  {

  }

  wstring JsonObject::Dump(uint8_t indentSize, wchar_t indentChar) const
  {
    wstringstream os;
    JsonLinter::Write(os, JsonObject::Write(*this, deque<TOKEN>()), indentSize, indentChar);
    return os.str();
  }

  int64_t JsonObject::Size() const
  {
    return _pairs.size();
  }

  int64_t JsonObject::Count(wstring const& key) const
  {
    return _indexes.count(key);
  }

  void JsonObject::Clear()
  {
    _pairs.clear();
    _indexes.clear();
  }

  bool JsonObject::Insert(pair<wstring, Json> const& pair)
  {
    if (_indexes.count(pair.first)) return false;
    _indexes[pair.first] = _pairs.size();
    _pairs.push_back(pair);
    return true;
  }

  bool JsonObject::Insert(pair<wstring, Json> && pair)
  {
    if (_indexes.count(pair.first)) return false;
    _indexes[pair.first] = _pairs.size();
    _pairs.push_back(move(pair));
    return true;
  }

  void JsonObject::Erase(wstring const& key)
  {
    if (!_indexes.count(key)) return;
    _pairs.erase(remove_if(_pairs.begin(), _pairs.end(), [&](pair<wstring, Json> const& pair) { return pair.first == key; }), _pairs.end());
    _indexes.clear();
    for (int i = 0; i < _pairs.size(); ++i)
    {
      _indexes[_pairs[i].first] = i;
    }
  }

  vector<wstring> JsonObject::Keys() const
  {
    vector<wstring> keys;
    transform(_pairs.begin(), _pairs.end(), back_inserter(keys), [](pair<wstring, Json> const& pair) { return pair.first; });
    return keys;
  }

  vector<reference_wrapper<const wstring>> JsonObject::KeysView() const
  {
    vector<reference_wrapper<const wstring>> keys;
    transform(_pairs.begin(), _pairs.end(), back_inserter(keys), [](pair<wstring, Json> const& pair) { return ref(pair.first); });
    return keys;
  }

  Json& JsonObject::operator[](wstring const& key)
  {
    if (!_indexes.count(key)) Insert({ key, Json{} });
    return _pairs[_indexes[key]].second;
  }

  Json& JsonObject::At(wstring const& key)
  {
    return const_cast<Json&>(as_const(*this).At(key));
  }

  Json const& JsonObject::At(wstring const& key) const
  {
    return _pairs.at(_indexes.at(key)).second;
  }

  vector<pair<wstring, Json>>::iterator JsonObject::begin()
  {
    return _pairs.begin();
  }

  vector<pair<wstring, Json>>::iterator JsonObject::end()
  {
    return _pairs.end();
  }

  vector<pair<wstring, Json>>::const_iterator JsonObject::begin() const
  {
    return _pairs.begin();
  }

  vector<pair<wstring, Json>>::const_iterator JsonObject::end() const
  {
    return _pairs.end();
  }

  wostream& operator<<(wostream& os, JsonObject const& object)
  {
    auto isIndentSizeActive = JsonIndentSize::IsActive(os);
    auto isIndentCharActive = JsonIndentChar::IsActive(os);
    JsonLinter::Write(os, JsonObject::Write(object, deque<TOKEN>()), isIndentSizeActive ? JsonIndentSize::GetSize(os) : JsonDefault::IndentSize,
                                                                     isIndentCharActive ? JsonIndentChar::GetChar(os) : JsonDefault::IndentChar);
    if (isIndentSizeActive) JsonIndentSize::ResetState(os);
    if (isIndentCharActive) JsonIndentChar::ResetState(os);
    return os;
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