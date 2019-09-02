#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "Value.h"

#include <vector>
#include <unordered_map>
#include <utility>
#include <sstream>
#include <iostream>
#include <initializer_list>
#include <string>

namespace Json4CPP
{
  namespace Detail
  {
    class JSON_API JsonBuilder;
  }
  class JSON_API JsonArray;
  class JSON_API Json;

  class JSON_API JsonObject
  {
  private:
    friend class JsonArray;
    friend class Json;
    friend class Detail::JsonBuilder;
#pragma warning(suppress: 4251)
    std::vector<std::pair<KEY, Json>> _pairs;
#pragma warning(suppress: 4251)
    std::unordered_map<KEY, int64_t> _indexes;

    void _Dump(std::wstringstream& os, uint8_t indentation, uint64_t level) const;
  public:
    JsonObject() = default;
    JsonObject(Detail::JsonBuilder builder);
    JsonObject(std::initializer_list<Detail::JsonBuilder> builders);
    JsonObject(JsonObject const& object);

    std::wstring Dump(uint8_t indentation = 0) const;

    int64_t Size() const;
    void Clear();
    bool Insert(std::pair<KEY, Json> pair);
    void Erase(KEY key);
    std::vector<KEY> Keys() const;
    Json& operator[](KEY const& key);
    Json At(KEY const& key) const;
    Json& At(KEY const& key);

    std::vector<std::pair<KEY, Json>>::iterator begin();
    std::vector<std::pair<KEY, Json>>::iterator end();
    std::vector<std::pair<KEY, Json>>::const_iterator begin() const;
    std::vector<std::pair<KEY, Json>>::const_iterator end() const;

    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonObject const& object);
    JSON_API friend std::wistream& operator>>(std::wistream& is, JsonObject      & object);

    JSON_API friend bool operator==(JsonObject const& left, JsonObject const& right);
    JSON_API friend bool operator!=(JsonObject const& left, JsonObject const& right);
  };
}