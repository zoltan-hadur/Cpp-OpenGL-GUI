#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

namespace Json4CPP
{
  Json operator""_json(const wchar_t* value, std::size_t size)
  {
    return ParseJson(wstring(value));
  }

  void Json::Dump(wstringstream& os, int indentation, int level) const
  {
    switch (GetType(_value))
    {
    case JsonType::Object:  get<JsonObject> (_value).Dump(os, indentation, level); break;
    case JsonType::Array:   get<JsonArray>  (_value).Dump(os, indentation, level); break;
    default: ValueWrite(os, _value); break;
    }
  }

  Json::Json()
  {
    _value = JsonObject();
  }

  Json::Json(JsonBuilder value)
  {
    switch (value.Type())
    {
    case JsonBuilderType::Object: _value = JsonObject (value); break;
    case JsonBuilderType::Array:  _value = JsonArray  (value); break;
    default:
      visit(Overload{
        [&](auto arg) { _value = arg; },
        [&](vector<JsonBuilder> const& arg)
        {
          switch (value.Type())
          {
          case JsonBuilderType::Empty: case JsonBuilderType::Object:  _value = JsonObject (arg); break;
          case JsonBuilderType::Array: case JsonBuilderType::Pair:    _value = JsonArray  (arg); break;
          default: throw exception("Should not be possible!");
          }
        }
      }, value._value);
      break;
    }
  }

  Json::Json(initializer_list<JsonBuilder> values) : Json(JsonBuilder(values)) {}

  Json::Json(Json const& json)
  {
    _value = json._value;
  }

  JsonType Json::Type() const
  {
    return GetType(_value);
  }

  bool Json::Is(JsonType type) const
  {
    return Type() == type;
  }

  wstring Json::Dump(int indentation) const
  {
    wstringstream buffer;
    Dump(buffer, indentation, 0);
    return buffer.str();
  }

  Json Json::Read(path filePath)
  {
    return ParseJson(wfstream(filePath, wfstream::in));
  }

  void Json::Write(path filePath) const
  {
    wfstream(filePath, wfstream::out) << *this;
  }

#pragma region Implicit conversion operators
#pragma warning(push)
#pragma warning(disable : 4244)
  Json::operator std::nullptr_t()
  {
    if (Is(JsonType::Null)) return nullptr;
    throw exception("Invalid conversion!");
  }

  Json::operator std::wstring()
  {
    if (Is(JsonType::String)) return Get<std::wstring>();
    throw exception("Invalid conversion!");
  }

  Json::operator bool()
  {
    switch (Type())
    {
    case JsonType::Null: return false;
    case JsonType::Boolean: return Get<bool>();
    default: return true;
    }
  }

  Json::operator char()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int8_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint8_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int16_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint16_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int32_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint32_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int64_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint64_t()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator float()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator double()
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonObject()
  {
    if (Is(JsonType::Object)) return Get<JsonObject>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonArray()
  {
    if (Is(JsonType::Array)) return Get<JsonArray>();
    throw exception("Invalid conversion!");
  }

  Json::operator std::nullptr_t() const
  {
    if (Is(JsonType::Null)) return nullptr;
    throw exception("Invalid conversion!");
  }

  Json::operator std::wstring() const
  {
    if (Is(JsonType::String)) return Get<std::wstring>();
    throw exception("Invalid conversion!");
  }

  Json::operator bool() const
  {
    switch (Type())
    {
    case JsonType::Null: return false;
    case JsonType::Boolean: return Get<bool>();
    default: return true;
    }
  }

  Json::operator char() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int8_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint8_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int16_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint16_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int32_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint32_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator int64_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator uint64_t() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator float() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator double() const
  {
    if (Is(JsonType::Number)) return Get<double>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonObject() const
  {
    if (Is(JsonType::Object)) return Get<JsonObject>();
    throw exception("Invalid conversion!");
  }

  Json::operator JsonArray() const
  {
    if (Is(JsonType::Array)) return Get<JsonArray>();
    throw exception("Invalid conversion!");
  }

  Json::operator std::nullptr_t&()
  {
    return Get<std::nullptr_t>();
  }

  Json::operator std::wstring&()
  {
    return Get<std::wstring>();
  }

  Json::operator bool&()
  {
    return Get<bool>();
  }

  Json::operator double&()
  {
    return Get<double>();
  }

  Json::operator JsonObject&()
  {
    return Get<JsonObject>();
  }

  Json::operator JsonArray&()
  {
    return Get<JsonArray>();
  }
#pragma warning(pop)
#pragma endregion

  Json& Json::operator[](NAME const& name)
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value)[name];
    default: throw exception("Operator[NAME] is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](const wchar_t* name)
  {
    return (*this)[wstring(name)];
  }

  Json& Json::operator[](int const& index)
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value)[index];
    default: throw exception("Operator[int] is only defined for JsonArray!");
    }
  }

  Json& Json::operator=(std::nullptr_t                      value) { _value = value;               return *this; }
  Json& Json::operator=(const wchar_t*                      value) { _value = std::wstring(value); return *this; }
  Json& Json::operator=(std::wstring                        value) { _value = value;               return *this; }
  Json& Json::operator=(bool                                value) { _value = value;               return *this; }
  Json& Json::operator=(char                                value) { _value = double(value);       return *this; }
  Json& Json::operator=(int8_t                              value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint8_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(int16_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint16_t                            value) { _value = double(value);       return *this; }
  Json& Json::operator=(int32_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint32_t                            value) { _value = double(value);       return *this; }
  Json& Json::operator=(int64_t                             value) { _value = double(value);       return *this; }
  Json& Json::operator=(uint64_t                            value) { _value = double(value);       return *this; }
  Json& Json::operator=(float                               value) { _value = double(value);       return *this; }
  Json& Json::operator=(double                              value) { _value = value;               return *this; }
  Json& Json::operator=(JsonObject                          value) { _value = value;               return *this; }
  Json& Json::operator=(JsonArray                           value) { _value = value;               return *this; }
  Json& Json::operator=(JsonBuilder                         value) { _value = Json(value)._value;  return *this; }
  Json& Json::operator=(std::initializer_list<JsonBuilder> values) { _value = Json(values)._value; return *this; }

  wostream& operator<<(wostream& os, Json const& json)
  {
    return os << json.Dump(JsonDefault::Indentation);
  }

  wistream& operator>>(wistream&is, Json& json)
  {
    return ValueRead(is, json._value);
  }

  bool  operator==(Json const& left, Json const& right) { return ValueEqual(left._value, right._value);                }
  bool  operator!=(Json const& left, Json const& right) { return ValueNotEqual(left._value, right._value);             }
  bool  operator< (Json const& left, Json const& right) { return ValueLessThan(left._value, right._value);             }
  bool  operator<=(Json const& left, Json const& right) { return ValueLessThanOrEqual(left._value, right._value);      }
  bool  operator> (Json const& left, Json const& right) { return ValueGreaterThan(left._value, right._value);          }
  bool  operator>=(Json const& left, Json const& right) { return ValueGreaterThanOrEqual(left._value, right._value);   }
  Json  operator+ (Json const& left, Json const& right) { return Json(ValueAdd(left._value, right._value));            }
  Json& operator+=(Json&       left, Json const& right) { ValueAddAssign(left._value, right._value); return left;      }
  Json  operator- (Json const& left, Json const& right) { return Json(ValueSubtract(left._value, right._value));       }
  Json& operator-=(Json&       left, Json const& right) { ValueSubtractAssign(left._value, right._value); return left; }
  Json  operator* (Json const& left, Json const& right) { return Json(ValueMultiply(left._value, right._value));       }
  Json& operator*=(Json&       left, Json const& right) { ValueMultiplyAssign(left._value, right._value); return left; }
  Json  operator/ (Json const& left, Json const& right) { return Json(ValueDivide(left._value, right._value));         }
  Json& operator/=(Json&       left, Json const& right) { ValueDivideAssign(left._value, right._value); return left;   }
  Json  operator% (Json const& left, Json const& right) { return Json(ValueModulo(left._value, right._value));         }
  Json& operator%=(Json&       left, Json const& right) { ValueModuloAssign(left._value, right._value); return left;   }
  Json  operator- (Json const& value)                   { return Json(ValueNegate(value._value));                      }
  bool  operator! (Json const& value)                   { return ValueNot(value._value);                               }
  Json& operator++(Json&       value)                   { ValuePreIncrement(value._value); return value;               }
  Json  operator++(Json&       value, int)              { return Json(ValuePostIncrement(value._value));               }
  Json& operator--(Json&       value)                   { ValuePreDecrement(value._value); return value;               }
  Json  operator--(Json&       value, int)              { return Json(ValuePostDecrement(value._value));               }
  bool  operator&&(Json const& left, Json const& right) { return ValueLogicalAnd(left._value, right._value);           }
  bool  operator||(Json const& left, Json const& right) { return ValueLogicalOr(left._value, right._value);            }
}