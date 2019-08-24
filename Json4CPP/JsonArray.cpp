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
  void JsonArray::Dump(wstringstream& os, int indentation, int level) const
  {
    auto indent = wstring(indentation * level, L' ');
    auto single = wstring(indentation, L' ');
    auto newLine = indentation == 0 ? L"" : L"\r\n";
    auto size = _values.size();

    os << L"[" << newLine;
    for (int i = 0; i < size; ++i)
    {
      auto& value = _values[i];
      os << indent << single;
      switch (value.Type())
      {
      case JsonType::Object: value.Get<JsonObject>().Dump(os, indentation, level + 1); break;
      case JsonType::Array : value.Get<JsonArray >().Dump(os, indentation, level + 1); break;
      default: os << value; break;
      }
      if (i < size - 1)
      {
        os << L",";
      }
      os << newLine;
    }
    os << indent << L"]";
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
      throw exception();
    }
  }

  JsonArray::JsonArray(std::initializer_list<JsonBuilder> builders) : JsonArray(JsonBuilder(builders)) {}

  JsonArray::JsonArray(JsonArray const& array)
  {
    _values = array._values;
  }

  wstring JsonArray::Dump(int indentation) const
  {
    wstringstream buffer;
    Dump(buffer, indentation, 0);
    return buffer.str();
  }

  void JsonArray::AddValue(Json value)
  {
    _values.push_back(value);
  }

  Json& JsonArray::operator[](int const& index)
  {
    return _values[index];
  }

  wostream& operator<<(wostream& os, JsonArray const& array)
  {
    return os << array.Dump(JsonDefault::Indentation);
  }

  wistream& operator>>(wistream& is, JsonArray& array)
  {
    is >> ws;
    if (is.peek() == L'[')
    {
      is.get();
      is >> ws;
      if (is.peek() != L']')
      {
        array._values.push_back(Value::ParseJson(is));
        is >> ws;
        while (is.peek() == L',')
        {
          is.get();
          is >> ws;
          array._values.push_back(Value::ParseJson(is));
          is >> ws;
        }
        if (is.peek() == L']')
        {
          is.get();
        }
        else
        {
          auto message = "Expected ']' at position " + to_string(is.tellg());
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
      auto message = "Expected '[' at position " + to_string(is.tellg());
      throw exception(message.c_str());
    }
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