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

using namespace std::string_literals;

namespace Json4CPP
{
  Json operator""_Json(wchar_t const* value, size_t size)
  {
    return Json::Parse(value);
  }

  Json Json::Read(std::deque<Detail::TOKEN>& tokens)
  {
    auto& [token, value] = tokens.front();
    switch (token)
    {
    using enum Detail::JsonTokenType;
    case StartObject: return JsonObject::Read(tokens);
    case StartArray : return JsonArray ::Read(tokens);
    default:
      auto message = Helper::WString2String(L"Invalid token: "s + Json::Stringify(token) + L", with invalid data: "s + Detail::JsonLinter::Dump(value) + L"!"s);
      throw std::exception(message.c_str());
    }
  }

  Json Json::Read(std::deque<Detail::TOKEN> && tokens)
  {
    return Read(tokens);
  }

  std::deque<Detail::TOKEN>& Json::Write(Json const& json, std::deque<Detail::TOKEN>& tokens)
  {
    switch (json.Type())
    {
    using enum JsonType;
    case Null   : tokens.push_back({ Detail::JsonTokenType::Null   , std::get<std::nullptr_t>(json._value) }); break;
    case String : tokens.push_back({ Detail::JsonTokenType::String , std::get<std::wstring  >(json._value) }); break;
    case Boolean: tokens.push_back({ Detail::JsonTokenType::Boolean, std::get<bool          >(json._value) }); break;
    case Real   : tokens.push_back({ Detail::JsonTokenType::Real   , std::get<double        >(json._value) }); break;
    case Integer: tokens.push_back({ Detail::JsonTokenType::Integer, std::get<int64_t       >(json._value) }); break;
    case Object : JsonObject::Write(std::get<JsonObject>(json._value), tokens); break;
    case Array  : JsonArray ::Write(std::get<JsonArray >(json._value), tokens); break;
    default:
      auto message = Helper::WString2String(L"Invalid type: "s + Json::Stringify(json.Type()) + L"!"s);
      throw std::exception(message.c_str());
    }
    return tokens;
  }

  std::deque<Detail::TOKEN> && Json::Write(Json const& json, std::deque<Detail::TOKEN> && tokens)
  {
    return std::move(Write(json, tokens));
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

  Json::Json(Detail::JsonBuilder const& value)
  {
    switch (value.Type())
    {
    using enum Detail::JsonBuilderType;
    case Empty :
    case Object:
      _value = JsonObject(value);
      break;
    case Pair  :
    case Array :
      _value = JsonArray (value);
      break;
    default:
      std::visit(Helper::Overload{
        [&](std::nullptr_t const& arg) { _value = arg; },
        [&](std::wstring   const& arg) { _value = arg; },
        [&](bool           const& arg) { _value = arg; },
        [&](double         const& arg) { _value = arg; },
        [&](int64_t        const& arg) { _value = arg; },
        [&](auto const& arg)
        {
          throw std::exception("Should not be possible!");
        }
      }, value._value);
      break;
    }
  }

  Json::Json(Detail::JsonBuilder && value)
  {
    switch (value.Type())
    {
    using enum Detail::JsonBuilderType;
    case Empty :
    case Object:
      _value = JsonObject(std::move(value));
      break;
    case Pair  :
    case Array :
      _value = JsonArray (std::move(value));
      break;
    default:
      std::visit(Helper::Overload{
        [&](std::nullptr_t && arg) { _value = std::move(arg); },
        [&](std::wstring   && arg) { _value = std::move(arg); },
        [&](bool           && arg) { _value = std::move(arg); },
        [&](double         && arg) { _value = std::move(arg); },
        [&](int64_t        && arg) { _value = std::move(arg); },
        [&](auto && arg)
        {
          throw std::exception("Should not be possible!");
        }
      }, std::move(value._value));
      break;
    }
  }

  Json::Json(std::initializer_list<Detail::JsonBuilder> values) : Json(Detail::JsonBuilder(values))
  {

  }

  Json::Json(std::nullptr_t      value) { _value =                      value ; }
  Json::Json(wchar_t      const* value) { _value =         std::wstring(value); }
  Json::Json(std::wstring const& value) { _value =                      value ; }
  Json::Json(std::wstring     && value) { _value =            std::move(value); }
  Json::Json(bool                value) { _value =                      value ; }
  Json::Json(char                value) { _value = static_cast<int64_t>(value); }
  Json::Json(int8_t              value) { _value = static_cast<int64_t>(value); }
  Json::Json(uint8_t             value) { _value = static_cast<int64_t>(value); }
  Json::Json(int16_t             value) { _value = static_cast<int64_t>(value); }
  Json::Json(uint16_t            value) { _value = static_cast<int64_t>(value); }
  Json::Json(int32_t             value) { _value = static_cast<int64_t>(value); }
  Json::Json(uint32_t            value) { _value = static_cast<int64_t>(value); }
  Json::Json(int64_t             value) { _value =                      value ; }
  Json::Json(uint64_t            value) { _value = static_cast<int64_t>(value); }
  Json::Json(float               value) { _value = static_cast<double >(value); }
  Json::Json(double              value) { _value =                      value ; }
  Json::Json(JsonObject   const& value) { _value =                      value ; }
  Json::Json(JsonObject       && value) { _value =            std::move(value); }
  Json::Json(JsonArray    const& value) { _value =                      value ; }
  Json::Json(JsonArray        && value) { _value =            std::move(value); }

  JsonType Json::Type() const
  {
    JsonType result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = JsonType::Null   ; },
      [&](std::wstring   const& v) { result = JsonType::String ; },
      [&](bool           const& v) { result = JsonType::Boolean; },
      [&](double         const& v) { result = JsonType::Real   ; },
      [&](int64_t        const& v) { result = JsonType::Integer; },
      [&](JsonObject     const& v) { result = JsonType::Object ; },
      [&](JsonArray      const& v) { result = JsonType::Array  ; },
      [&](auto           const& v) { result = JsonType::Invalid; }
    }, _value);
    return result;
  }

  bool Json::Is(JsonType type) const
  {
    return Type() == type;
  }

  std::wstring Json::Dump(uint8_t indentSize, wchar_t indentChar) const
  {
    std::wstringstream os;
    Detail::JsonLinter::Write(os, Json::Write(*this, std::deque<Detail::TOKEN>()), indentSize, indentChar);
    return os.str();
  }

  Json Json::Parse(std::string const& string)
  {
    return Json::Read(Detail::JsonLinter::Read(Helper::String2WString(string)));
  }

  Json Json::Parse(std::wstring const& wstring)
  {
    return Json::Read(Detail::JsonLinter::Read(wstring));
  }

  Json Json::Parse(std::u32string const& u32string)
  {
    return Json::Read(Detail::JsonLinter::Read(Helper::U32String2WString(u32string)));
  }

  Json Json::Read(std::filesystem::path filePath)
  {
    return Json::Read(Detail::JsonLinter::Read(Helper::ReadAllText(filePath)));
  }

  void Json::Write(std::filesystem::path filePath) const
  {
    Helper::WriteAllText(filePath, Dump(JsonDefault::IndentSize, JsonDefault::IndentChar));
  }

  void Json::Write(std::filesystem::path filePath, uint8_t indentSize) const
  {
    Helper::WriteAllText(filePath, Dump(indentSize, JsonDefault::IndentChar));
  }

  void Json::Write(std::filesystem::path filePath, wchar_t indentChar) const
  {
    Helper::WriteAllText(filePath, Dump(JsonDefault::IndentSize, indentChar));
  }

  void Json::Write(std::filesystem::path filePath, uint8_t indentSize, wchar_t indentChar) const
  {
    Helper::WriteAllText(filePath, Dump(indentSize, indentChar));
  }

  int64_t Json::Size() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::get<JsonObject>(_value).Size();
    case Array : return std::get<JsonArray >(_value).Size();
    default: throw std::exception("Size() is only defined for JsonObject and JsonArray!");
    }
  }

  int64_t Json::Count(std::wstring const& key) const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::get<JsonObject>(_value).Count(key);
    default: throw std::exception("Count(wstring const& key) is only defined for JsonObject!");
    }
  }

  bool Json::Contains(std::wstring const& key) const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::get<JsonObject>(_value).Contains(key);
    default: throw std::exception("Contains(wstring const& key) is only defined for JsonObject!");
    }
  }

  void Json::Resize(int64_t size)
  {
    switch (Type())
    {
    using enum JsonType;
    case Array: std::get<JsonArray>(_value).Resize(size); break;
    default: throw std::exception("Resize(int64_t size) is only defined for JsonArray!");
    }
  }

  void Json::Resize(int64_t size, Json const& json)
  {
    switch (Type())
    {
    using enum JsonType;
    case Array: std::get<JsonArray>(_value).Resize(size, json); break;
    default: throw std::exception("Resize(int64_t size, Json const& json) is only defined for JsonArray!");
    }
  }

  void Json::Clear()
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: std::get<JsonObject>(_value).Clear(); break;
    case Array : std::get<JsonArray >(_value).Clear(); break;
    default: throw std::exception("Clear() is only defined for JsonObject and JsonArray!");
    }
  }

  void Json::PushBack(Json const& value)
  {
    switch (Type())
    {
    using enum JsonType;
    case Null: _value = JsonArray(); [[fallthrough]];
    case Array: std::get<JsonArray>(_value).PushBack(value); break;
    default: throw std::exception("PushBack(Json const& value) is only defined for JsonArray!");
    }
  }

  void Json::PushBack(Json && value)
  {
    switch (Type())
    {
    using enum JsonType;
    case Null: _value = JsonArray(); [[fallthrough]];
    case Array: std::get<JsonArray>(_value).PushBack(std::move(value)); break;
    default: throw std::exception("PushBack(Json && value) is only defined for JsonArray!");
    }
  }

  bool Json::Insert(std::pair<std::wstring, Json> const& pair)
  {
    switch (Type())
    {
    using enum JsonType;
    case Null  : _value = JsonObject(); [[fallthrough]];
    case Object: return std::get<JsonObject>(_value).Insert(pair);
    default: throw std::exception("Insert(pair<wstring, Json> const& pair) is only defined for JsonObject!");
    }
  }

  bool Json::Insert(std::pair<std::wstring, Json> && pair)
  {
    switch (Type())
    {
    using enum JsonType;
    case Null: _value = JsonObject(); [[fallthrough]];
    case Object: return std::get<JsonObject>(_value).Insert(std::move(pair));
    default: throw std::exception("Insert(pair<wstring, Json> && pair) is only defined for JsonObject!");
    }
  }

  void Json::Insert(int64_t index, Json const& value)
  {
    switch (Type())
    {
    using enum JsonType;
    case Null  : _value = JsonArray(); [[fallthrough]];
    case Array: return std::get<JsonArray>(_value).Insert(index, value);
    default: throw std::exception("Insert(int64_t index, Json const& value) is only defined for JsonArray!");
    }
  }

  void Json::Insert(int64_t index, Json && value)
  {
    switch (Type())
    {
    using enum JsonType;
    case Null: _value = JsonArray(); [[fallthrough]];
    case Array: return std::get<JsonArray>(_value).Insert(index, std::move(value));
    default: throw std::exception("Insert(int64_t index, Json && value) is only defined for JsonArray!");
    }
  }

  void Json::Erase(std::wstring const& key)
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: std::get<JsonObject>(_value).Erase(key); break;
    default: throw std::exception("Erase(wstring const& key) is only defined for JsonObject!");
    }
  }

  void Json::Erase(int64_t index)
  {
    switch (Type())
    {
    using enum JsonType;
    case Array: std::get<JsonArray>(_value).Erase(index); break;
    default: throw std::exception("Erase(int64_t index) is only defined for JsonArray!");
    }
  }

  std::vector<std::wstring> Json::Keys() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::get<JsonObject>(_value).Keys();
    default: throw std::exception("Keys() is only defined for JsonObject!");
    }
  }

  std::vector<std::reference_wrapper<const std::wstring>> Json::KeysView() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::get<JsonObject>(_value).KeysView();
    default: throw std::exception("KeysView() is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](JsonPointer const& ptr)
  {
    switch (Type())
    {
    using enum JsonType;
    case Object:
    case Array :
      return ptr.Navigate(*this);
    default: throw std::exception("Operator[](JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
    }
  }

  Json& Json::operator[](wchar_t const* key)
  {
    return (*this)[std::wstring(key)];
  }

  Json& Json::operator[](std::wstring const& key)
  {
    switch (Type())
    {
    using enum JsonType;
    case Null  : _value = JsonObject(); [[fallthrough]];
    case Object: return std::get<JsonObject>(_value)[key];
    default: throw std::exception("Operator[](wstring const& key) is only defined for JsonObject!");
    }
  }

  Json& Json::operator[](int64_t index)
  {
    switch (Type())
    {
    using enum JsonType;
    case Array: return std::get<JsonArray>(_value)[index];
    default: throw std::exception("Operator[](int64_t index) is only defined for JsonArray!");
    }
  }

  Json& Json::At(JsonPointer const& ptr)
  {
    return const_cast<Json&>(std::as_const(*this).At(ptr));
  }

  Json const& Json::At(JsonPointer const& ptr) const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object:
    case Array :
      return ptr.Navigate(*this);
    default: throw std::exception("At(JsonPointer const& ptr) is only defined for JsonObject and JsonArray!");
    }
  }

  Json& Json::At(wchar_t const* key)
  {
    return const_cast<Json&>(std::as_const(*this).At(key));
  }

  Json const& Json::At(wchar_t const* key) const
  {
    return At(std::wstring(key));
  }

  Json& Json::At(std::wstring const& key)
  {
    return const_cast<Json&>(std::as_const(*this).At(key));
  }

  Json const& Json::At(std::wstring const& key) const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::get<JsonObject>(_value).At(key);
    default: throw std::exception("At(wstring const& key) is only defined for JsonObject!");
    }
  }

  Json& Json::At(int64_t index)
  {
    return const_cast<Json&>(std::as_const(*this).At(index));
  }

  Json const& Json::At(int64_t index) const
  {
    switch (Type())
    {
    using enum JsonType;
    case Array: return std::get<JsonArray>(_value).At(index);
    default: throw std::exception("At(int64_t index) is only defined for JsonArray!");
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
  Json::operator std::nullptr_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Null: return nullptr;
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Null) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator std::wstring const&() const
  {
    switch (Type())
    {
    using enum JsonType;
    case String: return std::get<std::wstring>(_value);
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::String) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator std::wstring && ()
  {
    switch (Type())
    {
    using enum JsonType;
    case String: return std::move(std::get<std::wstring>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::String) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator bool() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Null   : return false;
    case Boolean: return std::get<bool   >(_value);
    case Real   : return std::get<double >(_value);
    case Integer: return std::get<int64_t>(_value);
    default: return true;
    }
  }

  Json::operator char() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<char>(std::get<double >(_value));
    case Integer: return static_cast<char>(std::get<int64_t>(_value));
    default: 
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator int8_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<int8_t>(std::get<double >(_value));
    case Integer: return static_cast<int8_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator uint8_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<uint8_t>(std::get<double >(_value));
    case Integer: return static_cast<uint8_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator int16_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<int16_t>(std::get<double >(_value));
    case Integer: return static_cast<int16_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator uint16_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<uint16_t>(std::get<double >(_value));
    case Integer: return static_cast<uint16_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator int32_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<int32_t>(std::get<double >(_value));
    case Integer: return static_cast<int32_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator uint32_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<uint32_t>(std::get<double >(_value));
    case Integer: return static_cast<uint32_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator int64_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<int64_t>(std::get<double >(_value));
    case Integer: return static_cast<int64_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator uint64_t() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<uint64_t>(std::get<double >(_value));
    case Integer: return static_cast<uint64_t>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Integer) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator float() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<float>(std::get<double >(_value));
    case Integer: return static_cast<float>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Real) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator double() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Real   : return static_cast<double>(std::get<double >(_value));
    case Integer: return static_cast<double>(std::get<int64_t>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Real) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator JsonObject const&() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::get<JsonObject>(_value);
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Object) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator JsonObject &&()
  {
    switch (Type())
    {
    using enum JsonType;
    case Object: return std::move(std::get<JsonObject>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Object) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator JsonArray const&() const
  {
    switch (Type())
    {
    using enum JsonType;
    case Array: return std::get<JsonArray>(_value);
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Array) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }

  Json::operator JsonArray &&()
  {
    switch (Type())
    {
    using enum JsonType;
    case Array: return std::move(std::get<JsonArray>(_value));
    default:
      auto message = Helper::WString2String(L"Invalid conversion: Cannot convert type '"s + Json::Stringify(Type()) + L"' to '"s + Json::Stringify(JsonType::Array) + L"'!"s);
      throw std::exception(message.c_str());
    }
  }
#pragma endregion

  Json& Json::operator=(std::nullptr_t                             value )          { _value =                          value          ;          return *this; }
  Json& Json::operator=(wchar_t             const*                 value )          { _value =             std::wstring(value)         ;          return *this; }
  Json& Json::operator=(std::wstring        const&                 value )          { _value =                          value          ;          return *this; }
  Json& Json::operator=(std::wstring            &&                 value )          { _value =                std::move(value)         ;          return *this; }
  Json& Json::operator=(bool                                       value )          { _value =                          value          ;          return *this; }
  Json& Json::operator=(char                                       value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(int8_t                                     value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(uint8_t                                    value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(int16_t                                    value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(uint16_t                                   value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(int32_t                                    value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(uint32_t                                   value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(int64_t                                    value )          { _value =                          value          ;          return *this; }
  Json& Json::operator=(uint64_t                                   value )          { _value =     static_cast<int64_t>(value)         ;          return *this; }
  Json& Json::operator=(float                                      value )          { _value =     static_cast<double> (value)         ;          return *this; }
  Json& Json::operator=(double                                     value )          { _value =                          value          ;          return *this; }
  Json& Json::operator=(Json                const&                 value )          { _value =                          value._value   ;          return *this; }
  Json& Json::operator=(Json                    &&                 value ) noexcept { _value =                std::move(value._value)  ;          return *this; }
  Json& Json::operator=(JsonObject          const&                 value )          { _value =                          value          ;          return *this; }
  Json& Json::operator=(JsonObject              &&                 value )          { _value =                std::move(value)         ;          return *this; }
  Json& Json::operator=(JsonArray           const&                 value )          { _value =                          value          ;          return *this; }
  Json& Json::operator=(JsonArray               &&                 value )          { _value =                std::move(value)         ;          return *this; }
  Json& Json::operator=(Detail::JsonBuilder const&                 value )          { _value = std::move(Json(          value )._value);          return *this; }
  Json& Json::operator=(Detail::JsonBuilder     &&                 value )          { _value = std::move(Json(std::move(value))._value);          return *this; }
  Json& Json::operator=(std::initializer_list<Detail::JsonBuilder> values)          { _value = std::move(Json(          values)._value);          return *this; }

  std::wostream& operator<<(std::wostream& os, Json const& json)
  {
    auto indentSizeActive = JsonIndentSize::IsActive(os);
    auto indentCharActive = JsonIndentChar::IsActive(os);
    Detail::JsonLinter::Write(os,
                              Json::Write(json, std::deque<Detail::TOKEN>()),
                              indentSizeActive ? JsonIndentSize::GetSize(os) : JsonDefault::IndentSize,
                              indentCharActive ? JsonIndentChar::GetChar(os) : JsonDefault::IndentChar);
    if (indentSizeActive) JsonIndentSize::ResetState(os);
    if (indentCharActive) JsonIndentChar::ResetState(os);
    return os;
  }

  std::wistream& operator>>(std::wistream&is, Json& json)
  {
    json = Json::Read(Detail::JsonLinter::Read(is));
    return is;
  }

#pragma warning(push)
#pragma warning(disable : 4804 4805)
  bool operator==(Json const& left, Json const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l == r; },
      [&](bool           const& l, bool           const& r) { result = l == r; },
      [&](bool           const& l, double         const& r) { result = l == r; },
      [&](bool           const& l, int64_t        const& r) { result = l == r; },
      [&](double         const& l, bool           const& r) { result = l == r; },
      [&](double         const& l, double         const& r) { result = l == r; },
      [&](double         const& l, int64_t        const& r) { result = l == r; },
      [&](int64_t        const& l, bool           const& r) { result = l == r; },
      [&](int64_t        const& l, double         const& r) { result = l == r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l == r; },
      [&](JsonObject     const& l, JsonObject     const& r) { result = l == r; },
      [&](JsonArray      const& l, JsonArray      const& r) { result = l == r; },
      [&](auto           const& l, auto           const& r) { result = false;  }
    }, left._value, right._value);
    return result;
  }

  bool operator!=(Json const& left, Json const& right)
  {
    return !(left == right);
  }

  bool operator<(Json const& left, Json const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = false; },
      [&](auto           const& l, std::nullptr_t const& r) { result = true;  },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = false; },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l < r; },
      [&](bool           const& l, bool           const& r) { result = l < r; },
      [&](bool           const& l, double         const& r) { result = l < r; },
      [&](bool           const& l, int64_t        const& r) { result = l < r; },
      [&](double         const& l, bool           const& r) { result = l < r; },
      [&](double         const& l, double         const& r) { result = l < r; },
      [&](double         const& l, int64_t        const& r) { result = l < r; },
      [&](int64_t        const& l, bool           const& r) { result = l < r; },
      [&](int64_t        const& l, double         const& r) { result = l < r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l < r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator< is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  bool operator<=(Json const& left, Json const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = false;  },
      [&](auto           const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l <= r; },
      [&](bool           const& l, bool           const& r) { result = l <= r; },
      [&](bool           const& l, double         const& r) { result = l <= r; },
      [&](bool           const& l, int64_t        const& r) { result = l <= r; },
      [&](double         const& l, bool           const& r) { result = l <= r; },
      [&](double         const& l, double         const& r) { result = l <= r; },
      [&](double         const& l, int64_t        const& r) { result = l <= r; },
      [&](int64_t        const& l, bool           const& r) { result = l <= r; },
      [&](int64_t        const& l, double         const& r) { result = l <= r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l <= r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator<= is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  bool operator>(Json const& left, Json const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = true;  },
      [&](auto           const& l, std::nullptr_t const& r) { result = false; },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = false; },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l > r; },
      [&](bool           const& l, bool           const& r) { result = l > r; },
      [&](bool           const& l, double         const& r) { result = l > r; },
      [&](bool           const& l, int64_t        const& r) { result = l > r; },
      [&](double         const& l, bool           const& r) { result = l > r; },
      [&](double         const& l, double         const& r) { result = l > r; },
      [&](double         const& l, int64_t        const& r) { result = l > r; },
      [&](int64_t        const& l, bool           const& r) { result = l > r; },
      [&](int64_t        const& l, double         const& r) { result = l > r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l > r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator> is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  bool operator>=(Json const& left, Json const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = true;   },
      [&](auto           const& l, std::nullptr_t const& r) { result = false;  },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l >= r; },
      [&](bool           const& l, bool           const& r) { result = l >= r; },
      [&](bool           const& l, double         const& r) { result = l >= r; },
      [&](bool           const& l, int64_t        const& r) { result = l >= r; },
      [&](double         const& l, bool           const& r) { result = l >= r; },
      [&](double         const& l, double         const& r) { result = l >= r; },
      [&](double         const& l, int64_t        const& r) { result = l >= r; },
      [&](int64_t        const& l, bool           const& r) { result = l >= r; },
      [&](int64_t        const& l, double         const& r) { result = l >= r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l >= r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator>= is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  Json operator+(Json const& left, Json const& right)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                     },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l +                      r ; },
      [&](bool           const& l, bool           const& r) { result = l + static_cast<int64_t>(r); },
      [&](bool           const& l, double         const& r) { result = l +                      r ; },
      [&](bool           const& l, int64_t        const& r) { result = l +                      r ; },
      [&](double         const& l, bool           const& r) { result = l +                      r ; },
      [&](double         const& l, double         const& r) { result = l +                      r ; },
      [&](double         const& l, int64_t        const& r) { result = l +                      r ; },
      [&](int64_t        const& l, bool           const& r) { result = l +                      r ; },
      [&](int64_t        const& l, double         const& r) { result = l +                      r ; },
      [&](int64_t        const& l, int64_t        const& r) { result = l +                      r ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator+ is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  Json& operator+=(Json& left, Json const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                     },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::wstring   & l, std::wstring   const& r) { left = l +                      r ; },
      [&](bool           & l, bool           const& r) { left = l + static_cast<int64_t>(r); },
      [&](bool           & l, double         const& r) { left = l +                      r ; },
      [&](bool           & l, int64_t        const& r) { left = l +                      r ; },
      [&](double         & l, bool           const& r) { left = l +                      r ; },
      [&](double         & l, double         const& r) { left = l +                      r ; },
      [&](double         & l, int64_t        const& r) { left = l +                      r ; },
      [&](int64_t        & l, bool           const& r) { left = l +                      r ; },
      [&](int64_t        & l, double         const& r) { left = l +                      r ; },
      [&](int64_t        & l, int64_t        const& r) { left = l +                      r ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator+= is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return left;
  }

  Json operator-(Json const& left, Json const& right)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                     },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](bool           const& l, bool           const& r) { result = l - static_cast<int64_t>(r); },
      [&](bool           const& l, double         const& r) { result = l -                      r ; },
      [&](bool           const& l, int64_t        const& r) { result = l -                      r ; },
      [&](double         const& l, bool           const& r) { result = l -                      r ; },
      [&](double         const& l, double         const& r) { result = l -                      r ; },
      [&](double         const& l, int64_t        const& r) { result = l -                      r ; },
      [&](int64_t        const& l, bool           const& r) { result = l -                      r ; },
      [&](int64_t        const& l, double         const& r) { result = l -                      r ; },
      [&](int64_t        const& l, int64_t        const& r) { result = l -                      r ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator- is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  Json& operator-=(Json& left, Json const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                     },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](bool           & l, bool           const& r) { left = l - static_cast<int64_t>(r); },
      [&](bool           & l, double         const& r) { left = l -                      r ; },
      [&](bool           & l, int64_t        const& r) { left = l -                      r ; },
      [&](double         & l, bool           const& r) { left = l -                      r ; },
      [&](double         & l, double         const& r) { left = l -                      r ; },
      [&](double         & l, int64_t        const& r) { left = l -                      r ; },
      [&](int64_t        & l, bool           const& r) { left = l -                      r ; },
      [&](int64_t        & l, double         const& r) { left = l -                      r ; },
      [&](int64_t        & l, int64_t        const& r) { left = l -                      r ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator-= is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return left;
  }

  Json operator*(Json const& left, Json const& right)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                     },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](bool           const& l, bool           const& r) { result = l * static_cast<int64_t>(r); },
      [&](bool           const& l, double         const& r) { result = l *                      r ; },
      [&](bool           const& l, int64_t        const& r) { result = l *                      r ; },
      [&](double         const& l, bool           const& r) { result = l *                      r ; },
      [&](double         const& l, double         const& r) { result = l *                      r ; },
      [&](double         const& l, int64_t        const& r) { result = l *                      r ; },
      [&](int64_t        const& l, bool           const& r) { result = l *                      r ; },
      [&](int64_t        const& l, double         const& r) { result = l *                      r ; },
      [&](int64_t        const& l, int64_t        const& r) { result = l *                      r ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator* is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  Json& operator*=(Json& left, Json const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                     },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](bool           & l, bool           const& r) { left = l * static_cast<int64_t>(r); },
      [&](bool           & l, double         const& r) { left = l *                      r ; },
      [&](bool           & l, int64_t        const& r) { left = l *                      r ; },
      [&](double         & l, bool           const& r) { left = l *                      r ; },
      [&](double         & l, double         const& r) { left = l *                      r ; },
      [&](double         & l, int64_t        const& r) { left = l *                      r ; },
      [&](int64_t        & l, bool           const& r) { left = l *                      r ; },
      [&](int64_t        & l, double         const& r) { left = l *                      r ; },
      [&](int64_t        & l, int64_t        const& r) { left = l *                      r ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator*= is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return left;
  }

  Json operator/(Json const& left, Json const& right)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                                                           },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                                                           },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                                                           },
      [&](std::wstring   const& l, std::wstring   const& r) { result = std::wstring(std::filesystem::path(l) / std::filesystem::path(r)); },
      [&](bool           const& l, bool           const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](bool           const& l, double         const& r) { result =                                    l  /                       r  ; },
      [&](bool           const& l, int64_t        const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](double         const& l, bool           const& r) { result =                                    l  /                       r  ; },
      [&](double         const& l, double         const& r) { result =                                    l  /                       r  ; },
      [&](double         const& l, int64_t        const& r) { result =                                    l  /                       r  ; },
      [&](int64_t        const& l, bool           const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](int64_t        const& l, double         const& r) { result =                                    l  /                       r  ; },
      [&](int64_t        const& l, int64_t        const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator/ is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  Json& operator/=(Json& left, Json const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                                                           },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                                                           },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                                                           },
      [&](std::wstring   & l, std::wstring   const& r) { left = std::wstring(std::filesystem::path(l) / std::filesystem::path(r)); },
      [&](bool           & l, bool           const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](bool           & l, double         const& r) { left =                                    l  /                       r  ; },
      [&](bool           & l, int64_t        const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](double         & l, bool           const& r) { left =                                    l  /                       r  ; },
      [&](double         & l, double         const& r) { left =                                    l  /                       r  ; },
      [&](double         & l, int64_t        const& r) { left =                                    l  /                       r  ; },
      [&](int64_t        & l, bool           const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](int64_t        & l, double         const& r) { left =                                    l  /                       r  ; },
      [&](int64_t        & l, int64_t        const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator/= is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return left;
  }

  Json operator%(Json const& left, Json const& right)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;         },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;         },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;         },
      [&](bool           const& l, bool           const& r) { result = std::fmod(l, r); },
      [&](bool           const& l, double         const& r) { result = std::fmod(l, r); },
      [&](bool           const& l, int64_t        const& r) { result = std::fmod(l, r); },
      [&](double         const& l, bool           const& r) { result = std::fmod(l, r); },
      [&](double         const& l, double         const& r) { result = std::fmod(l, r); },
      [&](double         const& l, int64_t        const& r) { result = std::fmod(l, r); },
      [&](int64_t        const& l, bool           const& r) { result = std::fmod(l, r); },
      [&](int64_t        const& l, double         const& r) { result = std::fmod(l, r); },
      [&](int64_t        const& l, int64_t        const& r) { result = std::fmod(l, r); },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator% is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return result;
  }

  Json& operator%=(Json& left, Json const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;         },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;         },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;         },
      [&](bool           & l, bool           const& r) { left = std::fmod(l, r); },
      [&](bool           & l, double         const& r) { left = std::fmod(l, r); },
      [&](bool           & l, int64_t        const& r) { left = std::fmod(l, r); },
      [&](double         & l, bool           const& r) { left = std::fmod(l, r); },
      [&](double         & l, double         const& r) { left = std::fmod(l, r); },
      [&](double         & l, int64_t        const& r) { left = std::fmod(l, r); },
      [&](int64_t        & l, bool           const& r) { left = std::fmod(l, r); },
      [&](int64_t        & l, double         const& r) { left = std::fmod(l, r); },
      [&](int64_t        & l, int64_t        const& r) { left = std::fmod(l, r); },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator%= is not defined for types \""s + Json::Stringify(left.Type()) + L"\" and \""s + Json::Stringify(right.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left._value, right._value);
    return left;
  }

  Json operator-(Json const& value)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = nullptr;                  },
      [&](bool           const& v) { result = -static_cast<int64_t>(v); },
      [&](double         const& v) { result = -v;                       },
      [&](int64_t        const& v) { result = -v;                       },
      [&](auto           const& v)
      {
        auto message = Helper::WString2String(L"Operator- is not defined for type \""s + Json::Stringify(value.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value._value);
    return result;
  }

  bool operator!(Json const& value)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = true; },
      [&](bool           const& v) { result = !v; },
      [&](double         const& v) { result = !v; },
      [&](int64_t        const& v) { result = !v; },
      [&](auto           const& v)
      {
        auto message = Helper::WString2String(L"Operator! is not defined for type \""s + Json::Stringify(value.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
      }, value._value);
    return result;
  }

  Json& operator++(Json& value)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { value = nullptr; },
      [&](double         & v) { value = ++v;     },
      [&](int64_t        & v) { value = ++v;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator++ (pre-increment) is not defined for type \""s + Json::Stringify(value.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value._value);
    return value;
  }

  Json operator++(Json& value, int)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { result = nullptr; },
      [&](double         & v) { result = v++;     },
      [&](int64_t        & v) { result = v++;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator++ (post-increment) is not defined for type \""s + Json::Stringify(value.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value._value);
    return result;
  }

  Json& operator--(Json& value)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { value = nullptr; },
      [&](double         & v) { value = --v;     },
      [&](int64_t        & v) { value = --v;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator-- (pre-decrement) is not defined for type \""s + Json::Stringify(value.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value._value);
    return value;
  }

  Json operator--(Json& value, int)
  {
    Json result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { result = nullptr; },
      [&](double         & v) { result = v--;     },
      [&](int64_t        & v) { result = v--;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator-- (post-decrement) is not defined for type \""s + Json::Stringify(value.Type()) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value._value);
    return result;
  }

  bool operator&&(Json const& left, Json const& right)
  {
    return static_cast<bool>(left) && static_cast<bool>(right);
  }

  bool operator||(Json const& left, Json const& right)
  {
    return static_cast<bool>(left) || static_cast<bool>(right);
  }
#pragma warning(pop)
}