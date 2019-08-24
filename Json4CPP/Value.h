#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonType.h"
#include "JsonBuilderType.h"

#include <variant>
#include <cstddef>
#include <string>
#include <vector>
#include <ostream>
#include <type_traits>

namespace Json4CPP
{
  class JSON_API JsonObject;
  class JSON_API JsonArray;
  class JSON_API Json;
  class JSON_API JsonBuilder;

  template<typename ...Args>
  using VALUE_EXT = std::variant<std::nullptr_t, std::wstring, bool, double, JsonObject, JsonArray, Args...>;

  using KEY = std::wstring;
  using VALUE = VALUE_EXT<>;
  using VALUE_BUILDER = VALUE_EXT<std::vector<JsonBuilder>>;

  JSON_API JsonType         GetType(VALUE         const& value);
  JSON_API JsonBuilderType  GetType(VALUE_BUILDER const& value);

  JSON_API std::nullptr_t ParseNull       (std::wistream     & is   );
  JSON_API std::nullptr_t ParseNull       (std::wstring const& value);
  JSON_API std::wstring   ParseString     (std::wistream     & is   );
  JSON_API std::wstring   ParseString     (std::wstring const& value);
  JSON_API bool           ParseBoolean    (std::wistream     & is   );
  JSON_API bool           ParseBoolean    (std::wstring const& value);
  JSON_API double         ParseNumber     (std::wistream     & is   );
  JSON_API double         ParseNumber     (std::wstring const& value);
  JSON_API JsonObject     ParseJsonObject (std::wistream     & is   );
  JSON_API JsonObject     ParseJsonObject (std::wstring const& value);
  JSON_API JsonArray      ParseJsonArray  (std::wistream     & is   );
  JSON_API JsonArray      ParseJsonArray  (std::wstring const& value);
  JSON_API Json           ParseJson       (std::wistream     & is   );
  JSON_API Json           ParseJson       (std::wstring const& value);

  JSON_API std::wostream& ValueWrite(std::wostream& os, VALUE const& value);
  JSON_API std::wistream& ValueRead (std::wistream& is, VALUE      & value);

  JSON_API bool   ValueEqual              (VALUE const& left, VALUE const& right);
  JSON_API bool   ValueNotEqual           (VALUE const& left, VALUE const& right);
  JSON_API bool   ValueLessThan           (VALUE const& left, VALUE const& right);
  JSON_API bool   ValueLessThanOrEqual    (VALUE const& left, VALUE const& right);
  JSON_API bool   ValueGreaterThan        (VALUE const& left, VALUE const& right);
  JSON_API bool   ValueGreaterThanOrEqual (VALUE const& left, VALUE const& right);
  JSON_API VALUE  ValueAdd                (VALUE const& left, VALUE const& right);
  JSON_API VALUE& ValueAddAssign          (VALUE      & left, VALUE const& right);
  JSON_API VALUE  ValueSubtract           (VALUE const& left, VALUE const& right);
  JSON_API VALUE& ValueSubtractAssign     (VALUE      & left, VALUE const& right);
  JSON_API VALUE  ValueMultiply           (VALUE const& left, VALUE const& right);
  JSON_API VALUE& ValueMultiplyAssign     (VALUE      & left, VALUE const& right);
  JSON_API VALUE  ValueDivide             (VALUE const& left, VALUE const& right);
  JSON_API VALUE& ValueDivideAssign       (VALUE      & left, VALUE const& right);
  JSON_API VALUE  ValueModulo             (VALUE const& left, VALUE const& right);
  JSON_API VALUE& ValueModuloAssign       (VALUE      & left, VALUE const& right);
  JSON_API VALUE  ValueNegate             (VALUE const& value                   );
  JSON_API bool   ValueNot                (VALUE const& value                   );
  JSON_API VALUE& ValuePreIncrement       (VALUE      & value                   );
  JSON_API VALUE  ValuePostIncrement      (VALUE      & value                   );
  JSON_API VALUE& ValuePreDecrement       (VALUE      & value                   );
  JSON_API VALUE  ValuePostDecrement      (VALUE      & value                   );
  JSON_API bool   ValueLogicalAnd         (VALUE const& left, VALUE const& right);
  JSON_API bool   ValueLogicalOr          (VALUE const& left, VALUE const& right);
}