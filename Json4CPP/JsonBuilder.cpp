#include "stdafx.h"

#include "Json.h"
#include "JsonBuilder.h"
#include "Helper.h"

using namespace std;

namespace Json4CPP::Detail
{
  JsonBuilder::JsonBuilder() : JsonBuilder(nullptr) {}

  JsonBuilder::JsonBuilder(VALUE      const& value) { visit([&](auto const& arg) { _value =      arg ; },        value) ; }
  JsonBuilder::JsonBuilder(VALUE          && value) { visit([&](auto     && arg) { _value = move(arg); },   move(value)); }
  JsonBuilder::JsonBuilder(nullptr_t         value) {                              _value =                      value  ; }
  JsonBuilder::JsonBuilder(wchar_t    const* value) {                              _value =              wstring(value) ; }
  JsonBuilder::JsonBuilder(wstring    const& value) {                              _value =                      value  ; }
  JsonBuilder::JsonBuilder(wstring        && value) {                              _value =                 move(value) ; }
  JsonBuilder::JsonBuilder(bool              value) {                              _value =                      value  ; }
  JsonBuilder::JsonBuilder(char              value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int8_t            value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(uint8_t           value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int16_t           value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(uint16_t          value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int32_t           value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(uint32_t          value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(int64_t           value) {                              _value =                      value  ; }
  JsonBuilder::JsonBuilder(uint64_t          value) {                              _value = static_cast<int64_t>(value) ; }
  JsonBuilder::JsonBuilder(float             value) {                              _value = static_cast<double >(value) ; }
  JsonBuilder::JsonBuilder(double            value) {                              _value =                      value  ; }
  JsonBuilder::JsonBuilder(JsonObject const& value) {                              _value =                      value  ; }
  JsonBuilder::JsonBuilder(JsonObject     && value) {                              _value =                 move(value) ; }
  JsonBuilder::JsonBuilder(JsonArray  const& value) {                              _value =                      value  ; }
  JsonBuilder::JsonBuilder(JsonArray      && value) {                              _value =                 move(value) ; }
  JsonBuilder::JsonBuilder(Json       const& value) : JsonBuilder(     value._value)  { }
  JsonBuilder::JsonBuilder(Json           && value) : JsonBuilder(move(value._value)) { }
  JsonBuilder::JsonBuilder(initializer_list<JsonBuilder>        values) { _value = vector<JsonBuilder>(values); }
  JsonBuilder::JsonBuilder(vector          <JsonBuilder> const& values) { _value =                     values ; }
  JsonBuilder::JsonBuilder(vector          <JsonBuilder>     && values) { _value =                move(values); }

  JsonBuilderType JsonBuilder::Type() const
  {
    return Value::GetType(_value);
  }

  bool JsonBuilder::Is(JsonBuilderType type) const
  {
    return Type() == type;
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