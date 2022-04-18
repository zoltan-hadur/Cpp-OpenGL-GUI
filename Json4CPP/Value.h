#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <variant>
#include <string>
#include <vector>

namespace Json4CPP
{
  class JSON_API JsonObject;
  class JSON_API JsonArray;

  namespace Detail
  {
    class JSON_API JsonBuilder;

    /// <summary>
    /// Holds one of the following types: std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray.
    /// </summary>
    using VALUE = std::variant<std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray>;

    /// <summary>
    /// Holds one of the following types: std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray, std::vector&lt;JsonBuilder&gt;.
    /// </summary>
    using VALUE_BUILDER = std::variant<std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray, std::vector<JsonBuilder>>;

    /// <summary>
    /// Holds one of the following types: std::nullptr_t, std::wstring, bool, double, int64_t.
    /// </summary>
    using VALUE_TOKEN = std::variant<std::nullptr_t, std::wstring, bool, double, int64_t>;
  }
}