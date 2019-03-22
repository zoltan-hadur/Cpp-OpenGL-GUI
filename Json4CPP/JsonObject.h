#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "Value.h"

#include <vector>
#include <utility>
#include <sstream>
#include <iostream>
#include <initializer_list>
#include <string>

namespace Json4CPP
{
  class JSON_API JsonArray;
  class JSON_API Json;
  class JSON_API JsonBuilder;

  class JSON_API JsonObject
  {
  private:
    friend class JsonArray;
    friend class Json;
    friend class JsonBuilder;
#pragma warning(suppress: 4251)
    std::vector<std::pair<NAME, Json>> _pairs;

    void Dump(std::wstringstream& os, int indentation, int level) const;
  public:
    JsonObject() = default;
    JsonObject(JsonBuilder builder);
    JsonObject(std::initializer_list<JsonBuilder> builders);
    JsonObject(JsonObject const& object);

    std::wstring Dump(int indentation = 0) const;

    void AddPair(std::pair<NAME, Json> pair);

    Json& operator[](NAME const& name);

    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonObject const& object);
    JSON_API friend std::wistream& operator>>(std::wistream& is, JsonObject      & object);

    JSON_API friend bool operator==(JsonObject const& left, JsonObject const& right);
    JSON_API friend bool operator!=(JsonObject const& left, JsonObject const& right);
  };
}