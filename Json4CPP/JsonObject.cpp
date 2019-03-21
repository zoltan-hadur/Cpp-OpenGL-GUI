#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"

using namespace std;

namespace Json4CPP
{
  void JsonObject::Dump(wstringstream& os, int indentation, int level) const
  {
    auto indent = wstring(indentation * level, L' ');
    auto single = wstring(indentation, L' ');
    auto newLine = indentation == 0 ? L"" : L"\n";
    auto space = indentation == 0 ? L"" : L" ";
    auto size = _pairs.size();

    os << L"{" << newLine;
    for (int i = 0; i < size; ++i)
    {
      auto& [name, value] = _pairs[i];
      os << indent << single;
      os << L"\"" << name << L"\":" << space;
      switch (value.Type())
      {
      case JsonType::Object:  value.Get<JsonObject> ().Dump(os, indentation, level + 1); break;
      case JsonType::Array:   value.Get<JsonArray>  ().Dump(os, indentation, level + 1); break;
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
        auto name = get<NAME>(pair[0]._value);
        auto value = Json(pair[1]);
        _pairs.push_back({ name, value });
      }
    }
    else
    {
      throw exception();
    }
  }

  JsonObject::JsonObject(std::initializer_list<JsonBuilder> builders) : JsonObject(JsonBuilder(builders)) {}

  JsonObject::JsonObject(JsonObject const& object)
  {
    _pairs = object._pairs;
  }

  wstring JsonObject::Dump(int indentation) const
  {
    wstringstream buffer;
    Dump(buffer, indentation, 0);
    return buffer.str();
  }

  Json& JsonObject::operator[](NAME const& name)
  {
    for (auto& [key, value] : _pairs)
    {
      if (key == name)
        return value;
    }
    return _pairs.emplace_back(make_pair(name, Json{})).second;
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
        auto name = ParseString(is);
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
        auto value = ParseJson(is);
        is >> ws;
        object._pairs.push_back({ name, value });
        while (is.peek() == L',')
        {
          is.get();
          is >> ws;
          auto name = ParseString(is);
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
          auto value = ParseJson(is);
          is >> ws;
          object._pairs.push_back({ name, value });
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