#include "stdafx.h"

#include "Json.h"
#include "JsonBuilder.h"
#include "Helper.h"

using namespace std::string_literals;

namespace Json4CPP::Detail
{
  JsonBuilder::JsonBuilder() : JsonBuilder(nullptr) {}

  JsonBuilder::JsonBuilder(VALUE        const& value) { std::visit([&](auto const& arg) { _value =           arg ; },           value) ; }
  JsonBuilder::JsonBuilder(VALUE            && value) { std::visit([&](auto     && arg) { _value = std::move(arg); }, std::move(value)); }
  JsonBuilder::JsonBuilder(std::nullptr_t      value) {                                   _value =                              value  ; }
  JsonBuilder::JsonBuilder(wchar_t      const* value) {                                   _value =                 std::wstring(value) ; }
  JsonBuilder::JsonBuilder(std::wstring const& value) {                                   _value =                              value  ; }
  JsonBuilder::JsonBuilder(std::wstring     && value) {                                   _value =                    std::move(value) ; }
  JsonBuilder::JsonBuilder(bool                value) {                                   _value =                              value  ; }
  JsonBuilder::JsonBuilder(char                value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int8_t              value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(uint8_t             value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int16_t             value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(uint16_t            value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int32_t             value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(uint32_t            value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int64_t             value) {                                   _value =                              value  ; }
  JsonBuilder::JsonBuilder(uint64_t            value) {                                   _value =         static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(float               value) {                                   _value =         static_cast<double >(value) ; }
  JsonBuilder::JsonBuilder(double              value) {                                   _value =                              value  ; }
  JsonBuilder::JsonBuilder(JsonObject   const& value) {                                   _value =                              value  ; }
  JsonBuilder::JsonBuilder(JsonObject       && value) {                                   _value =                    std::move(value) ; }
  JsonBuilder::JsonBuilder(JsonArray    const& value) {                                   _value =                              value  ; }
  JsonBuilder::JsonBuilder(JsonArray        && value) {                                   _value =                    std::move(value) ; }
  JsonBuilder::JsonBuilder(Json         const& value) : JsonBuilder(          value._value)  { }
  JsonBuilder::JsonBuilder(Json             && value) : JsonBuilder(std::move(value._value)) { }
  JsonBuilder::JsonBuilder(std::initializer_list<JsonBuilder>        values) { _value = std::vector<JsonBuilder>(values); }
  JsonBuilder::JsonBuilder(std::vector          <JsonBuilder> const& values) { _value =                          values ; }
  JsonBuilder::JsonBuilder(std::vector          <JsonBuilder>     && values) { _value =                std::move(values); }

  JsonBuilderType JsonBuilder::Type() const
  {
    JsonBuilderType result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = JsonBuilderType::Null;    },
      [&](std::wstring   const& v) { result = JsonBuilderType::String;  },
      [&](bool           const& v) { result = JsonBuilderType::Boolean; },
      [&](double         const& v) { result = JsonBuilderType::Real;    },
      [&](int64_t        const& v) { result = JsonBuilderType::Integer; },
      [&](JsonObject     const& v) { result = JsonBuilderType::Object;  },
      [&](JsonArray      const& v) { result = JsonBuilderType::Array;   },
      [&](std::vector<JsonBuilder> const& arg)
      {
        if (arg.size() == 0)
          result = JsonBuilderType::Empty;
        else if (arg.size() == 2 && arg[0].Type() == JsonBuilderType::String)
          result = JsonBuilderType::Pair;
        else if (std::all_of(arg.begin(), arg.end(), [](JsonBuilder const& value) { return value.Is(JsonBuilderType::Pair); }))
          result = JsonBuilderType::Object;
        else
          result = JsonBuilderType::Array;
      },
      [&](auto const& v) { result = JsonBuilderType::Invalid; }
    }, _value);
    return result;
  }

  bool JsonBuilder::Is(JsonBuilderType type) const
  {
    return Type() == type;
  }

  std::wostream& operator<<(std::wostream& os, JsonBuilder const& builder)
  {
    std::visit(Helper::Overload{
      [&](auto const& v) { os << Json::Stringify(v); },
      [&](std::vector<JsonBuilder> const& arg)
      {
        os << L"[";
        for (int i = 0; i < arg.size(); ++i)
        {
          os << arg[i];
          if (i < arg.size() - 1)
          {
            os << L",";
          }
        }
        os << L"]";
      }
    }, builder._value);
    return os;
  }

  bool operator==(JsonBuilder const& left, JsonBuilder const& right)
  {
    return left._value == right._value;
  }

  bool operator!=(JsonBuilder const& left, JsonBuilder const& right)
  {
    return left._value != right._value;
  }
}