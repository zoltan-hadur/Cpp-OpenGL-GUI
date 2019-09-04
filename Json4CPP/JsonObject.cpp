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
        auto pair = get<vector<JsonBuilder>>(builder._value);
        auto key = get<KEY>(pair[0]._value);
        auto value = Json(pair[1]);
        Insert({ key, value });
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
    is >> ws;
    if (is.peek() == L'{')
    {
      is.get();
      is >> ws;
      if (is.peek() != L'}')
      {
        auto key = Value::ParseString(is);
        is >> ws;
        if (is.peek() == L':')
        {
          is.get();
          is >> ws;
        }
        else
        {
          auto message = "Expected ':' at position " + to_string(is.tellg());
          throw exception(message.c_str());
        }
        auto value = Value::ParseJson(is);
        is >> ws;
        object.Insert({ key, value });
        while (is.peek() == L',')
        {
          is.get();
          is >> ws;
          key = Value::ParseString(is);
          is >> ws;
          if (is.peek() == L':')
          {
            is.get();
            is >> ws;
          }
          else
          {
            auto message = "Expected ':' at position " + to_string(is.tellg());
            throw exception(message.c_str());
          }
          auto value = Value::ParseJson(is);
          is >> ws;
          object.Insert({ key, value });
        }
        if (is.peek() == L'}')
        {
          is.get();
        }
        else
        {
          auto message = "Expected '}' at position " + to_string(is.tellg());
          throw exception(message.c_str());
        }
      }
      else
      {
        is.get();
      }
    }
    else
    {
      auto message = "Expected '{' at position " + to_string(is.tellg());
      throw exception(message.c_str());
    }
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