#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <vector>
#include <sstream>
#include <iostream>
#include <initializer_list>
#include <string>

namespace Json4CPP
{
  class JSON_API JsonObject;
  class JSON_API Json;
  class JSON_API JsonBuilder;

  class JSON_API JsonArray
  {
  private:
    friend class JsonObject;
    friend class Json;
    friend class JsonBuilder;
#pragma warning(suppress: 4251)
    std::vector<Json> _values;

    void Dump(std::wstringstream& os, int indentation, int level) const;
  public:
    JsonArray() = default;
    JsonArray(JsonBuilder builder);
    JsonArray(std::initializer_list<JsonBuilder> builders);
    JsonArray(JsonArray const& array);

    std::wstring Dump(int indentation = 0) const;

    Json& operator[](int const& index);

    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonArray const& array);
    JSON_API friend std::wistream& operator>>(std::wistream& is, JsonArray      & array);

    JSON_API friend bool operator==(JsonArray const& left, JsonArray const& right);
    JSON_API friend bool operator!=(JsonArray const& left, JsonArray const& right);
  };
}