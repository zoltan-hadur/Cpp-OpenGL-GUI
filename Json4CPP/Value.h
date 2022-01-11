#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonType.h"
#include "JsonBuilderType.h"

#include <variant>
#include <string>
#include <vector>
#include <ostream>
#include <type_traits>

namespace Json4CPP
{
  class JSON_API JsonObject;
  class JSON_API JsonArray;
  class JSON_API Json;

  namespace Detail
  {
    class JSON_API JsonBuilder;

    // Holds one of the following types: std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray, or any other types specified in Args.
    template<typename ...Args>
    using VALUE_EXT = std::variant<std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray, Args...>;

    // Holds one of the following types: std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray.
    using VALUE = VALUE_EXT<>;
    // Holds one of the following types: std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray, std::vector<JsonBuilder>.
    using VALUE_BUILDER = VALUE_EXT<std::vector<JsonBuilder>>;

    namespace Value
    {
      JSON_API JsonType         GetType(VALUE         const& value);
      JSON_API JsonBuilderType  GetType(VALUE_BUILDER const& value);

      JSON_API bool   Equal              (VALUE const& left, VALUE const& right);
      JSON_API bool   NotEqual           (VALUE const& left, VALUE const& right);
      JSON_API bool   LessThan           (VALUE const& left, VALUE const& right);
      JSON_API bool   LessThanOrEqual    (VALUE const& left, VALUE const& right);
      JSON_API bool   GreaterThan        (VALUE const& left, VALUE const& right);
      JSON_API bool   GreaterThanOrEqual (VALUE const& left, VALUE const& right);
      JSON_API VALUE  Add                (VALUE const& left, VALUE const& right);
      JSON_API VALUE& AddAssign          (VALUE      & left, VALUE const& right);
      JSON_API VALUE  Subtract           (VALUE const& left, VALUE const& right);
      JSON_API VALUE& SubtractAssign     (VALUE      & left, VALUE const& right);
      JSON_API VALUE  Multiply           (VALUE const& left, VALUE const& right);
      JSON_API VALUE& MultiplyAssign     (VALUE      & left, VALUE const& right);
      JSON_API VALUE  Divide             (VALUE const& left, VALUE const& right);
      JSON_API VALUE& DivideAssign       (VALUE      & left, VALUE const& right);
      JSON_API VALUE  Modulo             (VALUE const& left, VALUE const& right);
      JSON_API VALUE& ModuloAssign       (VALUE      & left, VALUE const& right);
      JSON_API VALUE  Negate             (VALUE const& value                   );
      JSON_API bool   Not                (VALUE const& value                   );
      JSON_API VALUE& PreIncrement       (VALUE      & value                   );
      JSON_API VALUE  PostIncrement      (VALUE      & value                   );
      JSON_API VALUE& PreDecrement       (VALUE      & value                   );
      JSON_API VALUE  PostDecrement      (VALUE      & value                   );
      JSON_API bool   LogicalAnd         (VALUE const& left, VALUE const& right);
      JSON_API bool   LogicalOr          (VALUE const& left, VALUE const& right);
    }
  }
}