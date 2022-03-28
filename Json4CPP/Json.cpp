#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"
#include "JsonPatch.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace Json4CPP::Detail;

namespace Json4CPP
{
  Json operator""_Json(wchar_t const* value, size_t size)
  {
    return Json::Parse(value);
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

  Json Json::Read(deque<TOKEN> && tokens)
  {
    return Read(tokens);
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

  deque<TOKEN> && Json::Write(Json const& json, deque<TOKEN> && tokens)
  {
    return move(Write(json, tokens));
  }

  Json::Json()
  {
    _value = nullptr;
  }

  Json::Json(Json const& json)
  {
    _value = json._value;
  }

  Json::Json(Json && json) noexcept
  {
    _value = move(json._value);
  }

  Json::Json(JsonBuilder const& value)
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

  Json::Json(JsonBuilder && value)
  {
    switch (value.Type())
    {
    case JsonBuilderType::Empty :
    case JsonBuilderType::Object:
      _value = JsonObject(move(value));
      break;
    case JsonBuilderType::Pair  :
    case JsonBuilderType::Array :
      _value = JsonArray (move(value));
      break;
    default:
      visit(Overload{
        [&](nullptr_t && arg) { _value = move(arg); },
        [&](wstring   && arg) { _value = move(arg); },
        [&](bool      && arg) { _value = move(arg); },
        [&](double    && arg) { _value = move(arg); },
        [&](int64_t   && arg) { _value = move(arg); },
        [&](auto && arg)
        {
          throw exception("Should not be possible!");
        }
      }, move(value._value));
      break;
    }
  }

  Json::Json(initializer_list<JsonBuilder> values) : Json(JsonBuilder(values))
  {

  }

  Json::Json(nullptr_t         value) { _value =                      value  ; }
  Json::Json(wchar_t    const* value) { _value =              wstring(value) ; }
  Json::Json(wstring    const& value) { _value =                      value  ; }
  Json::Json(wstring        && value) { _value =                 move(value) ; }
  Json::Json(bool              value) { _value =                      value  ; }
  Json::Json(char              value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(int8_t            value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(uint8_t           value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(int16_t           value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(uint16_t          value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(int32_t           value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(uint32_t          value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(int64_t           value) { _value =                      value  ; }
  Json::Json(uint64_t          value) { _value = static_cast<int64_t>(value) ; }
  Json::Json(float             value) { _value = static_cast<double >(value) ; }
  Json::Json(double            value) { _value =                      value  ; }
  Json::Json(JsonObject const& value) { _value =                      value  ; }
  Json::Json(JsonObject     && value) { _value =                 move(value) ; }
  Json::Json(JsonArray  const& value) { _value =                      value  ; }
  Json::Json(JsonArray      && value) { _value =                 move(value) ; }

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

  Json Json::Parse(string const& string)
  {
    return Json::Read(JsonLinter::Read(String2WString(string)));
  }

  Json Json::Parse(wstring const& wstring)
  {
    return Json::Read(JsonLinter::Read(wstring));
  }

  Json Json::Parse(u32string const& u32string)
  {
    return Json::Read(JsonLinter::Read(U32String2WString(u32string)));
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

  int64_t Json::Count(wstring const& key) const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).Count(key);
    default: throw exception("Count(wstring const& key) is only defined for JsonObject!");
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

  void Json::Resize(int64_t size, Json const& json)
  {
    switch (Type())
    {
    case JsonType::Array: get<JsonArray>(_value).Resize(size, json); break;
    default: throw exception("Resize(int64_t size, Json const& json) is only defined for JsonArray!");
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

  void Json::PushBack(Json const& value)
  {
    switch (Type())
    {
    case JsonType::Null: _value = JsonArray(); [[fallthrough]];
    case JsonType::Array: get<JsonArray>(_value).PushBack(value); break;
    default: throw exception("PushBack(Json const& value) is only defined for JsonArray!");
    }
  }

  void Json::PushBack(Json && value)
  {
    switch (Type())
    {
    case JsonType::Null: _value = JsonArray(); [[fallthrough]];
    case JsonType::Array: get<JsonArray>(_value).PushBack(move(value)); break;
    default: throw exception("PushBack(Json && value) is only defined for JsonArray!");
    }
  }

  bool Json::Insert(pair<wstring, Json> const& pair)
  {
    switch (Type())
    {
    case JsonType::Null  : _value = JsonObject(); [[fallthrough]];
    case JsonType::Object: return get<JsonObject>(_value).Insert(pair);
    default: throw exception("Insert(pair<wstring, Json> const& pair) is only defined for JsonObject!");
    }
  }

  bool Json::Insert(pair<wstring, Json> && pair)
  {
    switch (Type())
    {
    case JsonType::Null: _value = JsonObject(); [[fallthrough]];
    case JsonType::Object: return get<JsonObject>(_value).Insert(move(pair));
    default: throw exception("Insert(pair<wstring, Json> && pair) is only defined for JsonObject!");
    }
  }

  void Json::Insert(int64_t index, Json const& value)
  {
    switch (Type())
    {
    case JsonType::Null  : _value = JsonArray(); [[fallthrough]];
    case JsonType::Array: return get<JsonArray>(_value).Insert(index, value);
    default: throw exception("Insert(int64_t index, Json const& value) is only defined for JsonArray!");
    }
  }

  void Json::Insert(int64_t index, Json && value)
  {
    switch (Type())
    {
    case JsonType::Null: _value = JsonArray(); [[fallthrough]];
    case JsonType::Array: return get<JsonArray>(_value).Insert(index, move(value));
    default: throw exception("Insert(int64_t index, Json && value) is only defined for JsonArray!");
    }
  }

  void Json::Erase(wstring const& key)
  {
    switch (Type())
    {
    case JsonType::Object: get<JsonObject>(_value).Erase(key); break;
    default: throw exception("Erase(wstring const& key) is only defined for JsonObject!");
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

  vector<wstring> Json::Keys() const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).Keys();
    default: throw exception("Keys() is only defined for JsonObject!");
    }
  }

  vector<reference_wrapper<const wstring>> Json::KeysView() const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).KeysView();
    default: throw exception("KeysView() is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](JsonPointer const& ptr)
  {
    switch (Type())
    {
    case JsonType::Object:
    case JsonType::Array :
      return ptr.Navigate(*this);
    default: throw exception("Operator[](JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
    }
  }

  Json& Json::operator[](wchar_t const* key)
  {
    return (*this)[wstring(key)];
  }

  Json& Json::operator[](wstring const& key)
  {
    switch (Type())
    {
    case JsonType::Null  : _value = JsonObject(); [[fallthrough]];
    case JsonType::Object: return get<JsonObject>(_value)[key];
    default: throw exception("Operator[](wstring const& key) is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](int64_t index)
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value)[index];
    default: throw exception("Operator[](int64_t index) is only defined for JsonArray!");
    }
  }

  Json& Json::At(JsonPointer const& ptr)
  {
    return const_cast<Json&>(as_const(*this).At(ptr));
  }

  Json const& Json::At(JsonPointer const& ptr) const
  {
    switch (Type())
    {
    case JsonType::Object:
    case JsonType::Array :
      return ptr.Navigate(*this);
    default: throw exception("At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
    }
  }

  Json& Json::At(wchar_t const* key)
  {
    return const_cast<Json&>(as_const(*this).At(key));
  }

  Json const& Json::At(wchar_t const* key) const
  {
    return At(wstring(key));
  }

  Json& Json::At(wstring const& key)
  {
    return const_cast<Json&>(as_const(*this).At(key));
  }

  Json const& Json::At(wstring const& key) const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value).At(key);
    default: throw exception("At(wstring const& key) is only defined for JsonObject!");
    }
  }

  Json& Json::At(int64_t index)
  {
    return const_cast<Json&>(as_const(*this).At(index));
  }

  Json const& Json::At(int64_t index) const
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value).At(index);
    default: throw exception("At(int64_t index) is only defined for JsonArray!");
    }
  }

  Json Json::Patch(JsonPatch const& patch)
  {
    return patch.Apply(*this);
  }

  JsonIterator Json::begin()
  {
    return JsonIterator(this, 0);
  }

  JsonIterator Json::end()
  {
    return JsonIterator(this, Size());
  }

  JsonConstIterator Json::begin() const
  {
    return JsonConstIterator(this, 0);
  }

  JsonConstIterator Json::end() const
  {
    return JsonConstIterator(this, Size());
  }

#pragma region Conversion operators
  Json::operator nullptr_t() const
  {
    switch (Type())
    {
    case JsonType::Null: return nullptr;
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Null) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator wstring const&() const
  {
    switch (Type())
    {
    case JsonType::String: return get<wstring>(_value);
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::String) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator wstring && ()
  {
    switch (Type())
    {
    case JsonType::String: return move(get<wstring>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::String) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator bool() const
  {
    switch (Type())
    {
    case JsonType::Null   : return false;
    case JsonType::Boolean: return get<bool   >(_value);
    case JsonType::Real   : return get<double >(_value);
    case JsonType::Integer: return get<int64_t>(_value);
    default: return true;
    }
  }

  Json::operator char() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<char>(get<double >(_value));
    case JsonType::Integer: return static_cast<char>(get<int64_t>(_value));
    default: 
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator int8_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<int8_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<int8_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator uint8_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<uint8_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<uint8_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator int16_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<int16_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<int16_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator uint16_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<uint16_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<uint16_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator int32_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<int32_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<int32_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator uint32_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<uint32_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<uint32_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator int64_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<int64_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<int64_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator uint64_t() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<uint64_t>(get<double >(_value));
    case JsonType::Integer: return static_cast<uint64_t>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator float() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<float>(get<double >(_value));
    case JsonType::Integer: return static_cast<float>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Real) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator double() const
  {
    switch (Type())
    {
    case JsonType::Real   : return static_cast<double>(get<double >(_value));
    case JsonType::Integer: return static_cast<double>(get<int64_t>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Real) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator JsonObject const&() const
  {
    switch (Type())
    {
    case JsonType::Object: return get<JsonObject>(_value);
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Object) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator JsonObject &&()
  {
    switch (Type())
    {
    case JsonType::Object: return move(get<JsonObject>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Object) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator JsonArray const&() const
  {
    switch (Type())
    {
    case JsonType::Array: return get<JsonArray>(_value);
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Array) + L"'!"s);
      throw exception(message.c_str());
    }
  }

  Json::operator JsonArray &&()
  {
    switch (Type())
    {
    case JsonType::Array: return move(get<JsonArray>(_value));
    default:
      auto message = WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Array) + L"'!"s);
      throw exception(message.c_str());
    }
  }
#pragma endregion

  Json& Json::operator=(nullptr_t                     value )          { _value =                      value ;          return *this; }
  Json& Json::operator=(wchar_t const*                value )          { _value =              wstring(value);          return *this; }
  Json& Json::operator=(wstring     const&            value )          { _value =                      value ;          return *this; }
  Json& Json::operator=(wstring         &&            value )          { _value =                 move(value);          return *this; }
  Json& Json::operator=(bool                          value )          { _value =                      value ;          return *this; }
  Json& Json::operator=(char                          value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(int8_t                        value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(uint8_t                       value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(int16_t                       value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(uint16_t                      value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(int32_t                       value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(uint32_t                      value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(int64_t                       value )          { _value =                      value ;          return *this; }
  Json& Json::operator=(uint64_t                      value )          { _value = static_cast<int64_t>(value);          return *this; }
  Json& Json::operator=(float                         value )          { _value = static_cast<double> (value);          return *this; }
  Json& Json::operator=(double                        value )          { _value =                      value ;          return *this; }
  Json& Json::operator=(Json        const&            value )          { _value =                      value._value ;   return *this; }
  Json& Json::operator=(Json            &&            value ) noexcept { _value =                 move(value._value);   return *this; }
  Json& Json::operator=(JsonObject  const&            value )          { _value =                      value ;          return *this; }
  Json& Json::operator=(JsonObject      &&            value )          { _value =                 move(value);          return *this; }
  Json& Json::operator=(JsonArray   const&            value )          { _value =                      value ;          return *this; }
  Json& Json::operator=(JsonArray       &&            value )          { _value =                 move(value);          return *this; }
  Json& Json::operator=(JsonBuilder const&            value )          { _value =       move(Json(     value )._value); return *this; }
  Json& Json::operator=(JsonBuilder     &&            value )          { _value =       move(Json(move(value))._value); return *this; }
  Json& Json::operator=(initializer_list<JsonBuilder> values)          { _value =       move(Json(     values)._value); return *this; }

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