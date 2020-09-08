#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace Json4CPP::Detail;

namespace Json4CPP
{
  Json operator""_json(const wchar_t* value, size_t size)
  {
    return Json::Read(JsonLinter::Read(value));
  }

  Json Json::Read(deque<TOKEN>& tokens)
  {
    auto& [token, value] = tokens.front();
    switch (token)
    {
    case JsonTokenType::StartObject: return JsonObject::Read(tokens);
    case JsonTokenType::StartArray : return JsonArray ::Read(tokens);
    default:
      auto message = WString2String(L"Invalid token: "s + Json::Stringify(token) + L", with invalid data: "s + JsonLinter::Dump(value) + L"!"s);
      throw exception(message.c_str());
    }
  }

  deque<TOKEN>& Json::Write(Json const& json, deque<TOKEN>& tokens)
  {
    switch (Value::GetType(json._value))
    {
    case JsonType::Null   : tokens.push_back({ JsonTokenType::Null   , get<nullptr_t>(json._value) }); break;
    case JsonType::String : tokens.push_back({ JsonTokenType::String , get<wstring  >(json._value) }); break;
    case JsonType::Boolean: tokens.push_back({ JsonTokenType::Boolean, get<bool     >(json._value) }); break;
    case JsonType::Real   : tokens.push_back({ JsonTokenType::Real   , get<double   >(json._value) }); break;
    case JsonType::Integer: tokens.push_back({ JsonTokenType::Integer, get<int64_t  >(json._value) }); break;
    case JsonType::Object : JsonObject::Write(get<JsonObject>(json._value), tokens); break;
    case JsonType::Array  : JsonArray ::Write(get<JsonArray >(json._value), tokens); break;
    default:
      auto message = WString2String(L"Invalid type: "s + Json::Stringify(Value::GetType(json._value)) + L"!"s);
      throw exception(message.c_str());
    }
    return tokens;
  }

  Json::Json()
  {
    _value = nullptr;
  }

  Json::Json(JsonBuilder value)
  {
    switch (value.Type())
    {
    case JsonBuilderType::Empty :
    case JsonBuilderType::Object:
      _value = JsonObject(value);
      break;
    case JsonBuilderType::Pair  :
    case JsonBuilderType::Array :
      _value = JsonArray (value);
      break;
    default:
      visit(Overload{
        [&](nullptr_t const& arg) { _value = arg; },
        [&](wstring   const& arg) { _value = arg; },
        [&](bool      const& arg) { _value = arg; },
        [&](double    const& arg) { _value = arg; },
        [&](int64_t   const& arg) { _value = arg; },
        [&](auto const& arg)
        {
          throw exception("Should not be possible!");
        }
      }, value._value);
      break;
    }
  }

  Json::Json(initializer_list<JsonBuilder> values) : Json(JsonBuilder(values))
  {

  }

  Json::Json(Json const& json)
  {
    _value = json._value;
  }

  Json::Json(nullptr_t      value) { _value =         value;  }
  Json::Json(const wchar_t* value) { _value = wstring(value); }
  Json::Json(wstring        value) { _value =         value;  }
  Json::Json(bool           value) { _value =         value;  }
  Json::Json(char           value) { _value = int64_t(value); }
  Json::Json(int8_t         value) { _value = int64_t(value); }
  Json::Json(uint8_t        value) { _value = int64_t(value); }
  Json::Json(int16_t        value) { _value = int64_t(value); }
  Json::Json(uint16_t       value) { _value = int64_t(value); }
  Json::Json(int32_t        value) { _value = int64_t(value); }
  Json::Json(uint32_t       value) { _value = int64_t(value); }
  Json::Json(int64_t        value) { _value =         value;  }
  Json::Json(uint64_t       value) { _value = int64_t(value); }
  Json::Json(float          value) { _value = double (value); }
  Json::Json(double         value) { _value =         value;  }
  Json::Json(JsonObject     value) { _value =         value;  }
  Json::Json(JsonArray      value) { _value =         value;  }

  JsonType Json::Type() const
  {
    return Value::GetType(_value);
  }

  bool Json::Is(JsonType type) const
  {
    return Type() == type;
  }

  wstring Json::Dump(uint8_t indentSize, wchar_t indentChar) const
  {
    wstringstream os;
    JsonLinter::Write(os, Json::Write(*this, deque<TOKEN>()), indentSize, indentChar);
    return os.str();
  }

  Json Json::Read(path filePath)
  {
    return Json::Read(JsonLinter::Read(ReadAllText(filePath)));
  }

  void Json::Write(path filePath) const
  {
    WriteAllText(filePath, Dump(JsonDefault::IndentSize, JsonDefault::IndentChar));
  }

  void Json::Write(path filePath, uint8_t indentSize) const
  {
    WriteAllText(filePath, Dump(indentSize, JsonDefault::IndentChar));
  }

  void Json::Write(path filePath, wchar_t indentChar) const
  {
    WriteAllText(filePath, Dump(JsonDefault::IndentSize, indentChar));
  }

  void Json::Write(path filePath, uint8_t indentSize, wchar_t indentChar) const
  {
    WriteAllText(filePath, Dump(indentSize, indentChar));
  }

  int64_t Json::Size() const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).Size();
    case JsonType::Array : return get<JsonArray >(_value).Size();
    default: throw exception("Size() is only defined for JsonObject and JsonArray!");
    }
  }

  void Json::Resize(int64_t size)
  {
    switch (Type())
    {
    case JsonType::Array: get<JsonArray>(_value).Resize(size); break;
    default: throw exception("Resize(int64_t size) is only defined for JsonArray!");
    }
  }

  void Json::Clear()
  {
    switch (Type())
    {
    case JsonType::Object: get<JsonObject>(_value).Clear(); break;
    case JsonType::Array : get<JsonArray >(_value).Clear(); break;
    default: throw exception("Clear() is only defined for JsonObject and JsonArray!");
    }
  }

  void Json::PushBack(Json value)
  {
    switch (Type())
    {
    case JsonType::Null: _value = JsonArray(); [[fallthrough]];
    case JsonType::Array: get<JsonArray>(_value).PushBack(value); break;
    default: throw exception("PushBack(Json value) is only defined for JsonArray!");
    }
  }

  bool Json::Insert(pair<KEY, Json> pair)
  {
    switch (Type())
    {
    case JsonType::Null  : _value = JsonObject(); [[fallthrough]];
    case JsonType::Object: return get<JsonObject>(_value).Insert(pair);
    default: throw exception("Insert(pair<KEY, Json> pair) is only defined for JsonObject!");
    }
  }

  void Json::Insert(int64_t index, Json value)
  {
    switch (Type())
    {
    case JsonType::Null  : _value = JsonArray(); [[fallthrough]];
    case JsonType::Array: return get<JsonArray>(_value).Insert(index, value);
    default: throw exception("Insert(Json value, int64_t index) is only defined for JsonArray!");
    }
  }

  void Json::Erase(KEY key)
  {
    switch (Type())
    {
    case JsonType::Object: get<JsonObject>(_value).Erase(key); break;
    default: throw exception("Erase(KEY key) is only defined for JsonObject!");
    }
  }

  void Json::Erase(int64_t index)
  {
    switch (Type())
    {
    case JsonType::Array: get<JsonArray>(_value).Erase(index); break;
    default: throw exception("Erase(int64_t index) is only defined for JsonArray!");
    }
  }

  vector<KEY> Json::Keys() const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).Keys();
    default: throw exception("Keys() is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](KEY const& key)
  {
    switch (Type())
    {
    case JsonType::Null  : _value = JsonObject(); [[fallthrough]];
    case JsonType::Object: return get<JsonObject>(_value)[key];
    default: throw exception("Operator[KEY] is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](int64_t const& index)
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value)[index];
    default: throw exception("Operator[int] is only defined for JsonArray!");
    }
  }

  Json Json::At(KEY const& key) const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).At(key);
    default: throw exception("At(KEY key) is only defined for JsonObject!");
    }
  }

  Json& Json::At(KEY const& key)
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).At(key);
    default: throw exception("At(KEY key) is only defined for JsonObject!");
    }
  }

  Json Json::At(int64_t const& index) const
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value).At(index);
    default: throw exception("At(int index) is only defined for JsonArray!");
    }
  }

  Json& Json::At(int64_t const& index)
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value).At(index);
    default: throw exception("At(int index) is only defined for JsonArray!");
    }
  }

#pragma region Conversion operators
#pragma warning(push)
#pragma warning(disable : 4244)
  Json::operator nullptr_t() const
  {
    switch (Type())
    {
    case JsonType::Null: return nullptr;
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator wstring() const
  {
    switch (Type())
    {
    case JsonType::String: return Get<wstring>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator bool() const
  {
    switch (Type())
    {
    case JsonType::Null   : return false;
    case JsonType::Boolean: return Get<bool   >();
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: return true;
    }
  }

  Json::operator char() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator int8_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator uint8_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator int16_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator uint16_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator int32_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator uint32_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator int64_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator uint64_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator float() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator double() const
  {
    switch (Type())
    {
    case JsonType::Real   : return Get<double >();
    case JsonType::Integer: return Get<int64_t>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator JsonObject() const
  {
    switch (Type())
    {
    case JsonType::Object: return Get<JsonObject>();
    default: throw exception("Invalid conversion!");
    }
  }

  Json::operator JsonArray() const
  {
    switch (Type())
    {
    case JsonType::Array: return Get<JsonArray>();
    default: throw exception("Invalid conversion!");
    }
  }
#pragma warning(pop)
#pragma endregion

  Json& Json::operator=(nullptr_t                     value ) { _value =         value;          return *this; }
  Json& Json::operator=(const wchar_t*                value ) { _value = wstring(value);         return *this; }
  Json& Json::operator=(wstring                       value ) { _value =         value;          return *this; }
  Json& Json::operator=(bool                          value ) { _value =         value;          return *this; }
  Json& Json::operator=(char                          value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(int8_t                        value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(uint8_t                       value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(int16_t                       value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(uint16_t                      value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(int32_t                       value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(uint32_t                      value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(int64_t                       value ) { _value =         value;          return *this; }
  Json& Json::operator=(uint64_t                      value ) { _value = int64_t(value);         return *this; }
  Json& Json::operator=(float                         value ) { _value = double (value);         return *this; }
  Json& Json::operator=(double                        value ) { _value =         value;          return *this; }
  Json& Json::operator=(Json                          value ) { _value =         value._value;   return *this; }
  Json& Json::operator=(JsonObject                    value ) { _value =         value;          return *this; }
  Json& Json::operator=(JsonArray                     value ) { _value =         value;          return *this; }
  Json& Json::operator=(JsonBuilder                   value ) { _value = Json   (value )._value; return *this; }
  Json& Json::operator=(initializer_list<JsonBuilder> values) { _value = Json   (values)._value; return *this; }

  wostream& operator<<(wostream& os, Json const& json)
  {
    auto indentSizeActive = JsonIndentSize::IsActive(os);
    auto indentCharActive = JsonIndentChar::IsActive(os);
    JsonLinter::Write(os, Json::Write(json, deque<TOKEN>()), indentSizeActive ? JsonIndentSize::GetSize(os) : JsonDefault::IndentSize,
                                                             indentCharActive ? JsonIndentChar::GetChar(os) : JsonDefault::IndentChar);
    if (indentSizeActive) JsonIndentSize::ResetState(os);
    if (indentCharActive) JsonIndentChar::ResetState(os);
    return os;
  }

  wistream& operator>>(wistream&is, Json& json)
  {
    json = Json::Read(JsonLinter::Read(is));
    return is;
  }

  bool  operator==(Json const& left , Json const& right) { return      Value::             Equal( left._value, right._value) ;               }
  bool  operator!=(Json const& left , Json const& right) { return      Value::          NotEqual( left._value, right._value) ;               }
  bool  operator< (Json const& left , Json const& right) { return      Value::   LessThan       ( left._value, right._value) ;               }
  bool  operator<=(Json const& left , Json const& right) { return      Value::   LessThanOrEqual( left._value, right._value) ;               }
  bool  operator> (Json const& left , Json const& right) { return      Value::GreaterThan       ( left._value, right._value) ;               }
  bool  operator>=(Json const& left , Json const& right) { return      Value::GreaterThanOrEqual( left._value, right._value) ;               }
  Json  operator+ (Json const& left , Json const& right) { return Json(Value::         Add      ( left._value, right._value));               }
  Json& operator+=(Json      & left , Json const& right) {             Value::         AddAssign( left._value, right._value) ; return left ; }
  Json  operator- (Json const& left , Json const& right) { return Json(Value::    Subtract      ( left._value, right._value));               }
  Json& operator-=(Json      & left , Json const& right) {             Value::    SubtractAssign( left._value, right._value) ; return left ; }
  Json  operator* (Json const& left , Json const& right) { return Json(Value::    Multiply      ( left._value, right._value));               }
  Json& operator*=(Json      & left , Json const& right) {             Value::    MultiplyAssign( left._value, right._value) ; return left ; }
  Json  operator/ (Json const& left , Json const& right) { return Json(Value::      Divide      ( left._value, right._value));               }
  Json& operator/=(Json      & left , Json const& right) {             Value::      DivideAssign( left._value, right._value) ; return left ; }
  Json  operator% (Json const& left , Json const& right) { return Json(Value::      Modulo      ( left._value, right._value));               }
  Json& operator%=(Json      & left , Json const& right) {             Value::      ModuloAssign( left._value, right._value) ; return left ; }
  Json  operator- (Json const& value                   ) { return Json(Value::Negate            (value._value              ));               }
  bool  operator! (Json const& value                   ) { return      Value::Not               (value._value              ) ;               }
  Json& operator++(Json      & value                   ) {             Value:: PreIncrement     (value._value              ) ; return value; }
  Json  operator++(Json      & value, int              ) { return Json(Value::PostIncrement     (value._value              ));               }
  Json& operator--(Json      & value                   ) {             Value:: PreDecrement     (value._value              ) ; return value; }
  Json  operator--(Json      & value, int              ) { return Json(Value::PostDecrement     (value._value              ));               }
  bool  operator&&(Json const& left , Json const& right) { return      Value::LogicalAnd        ( left._value, right._value) ;               }
  bool  operator||(Json const& left , Json const& right) { return      Value::LogicalOr         ( left._value, right._value) ;               }
}