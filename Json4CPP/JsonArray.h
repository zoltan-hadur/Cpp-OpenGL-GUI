#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "Value.h"
#include "JsonLinter.h"

#include <vector>
#include <deque>
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
  class JSON_API JsonObject;
  class JSON_API Json;
  class JSON_API JsonIterator;
  class JSON_API JsonConstIterator;

  class JSON_API JsonArray
  {
  private:
    friend class JsonObject;
    friend class Json;
    friend class JsonIterator;
    friend class JsonConstIterator;
    friend class Detail::JsonBuilder;
#pragma warning(suppress: 4251)
    std::vector<Json> _values;

    static JsonArray                  Read (                        std::deque<Detail::TOKEN>& tokens);
    static std::deque<Detail::TOKEN>& Write(JsonArray const& array, std::deque<Detail::TOKEN>& tokens);
  public:
    JsonArray() = default;
    JsonArray(Detail::JsonBuilder builder);
    JsonArray(std::initializer_list<Detail::JsonBuilder> builders);
    JsonArray(JsonArray const& array);

    std::wstring Dump(uint8_t indentSize = 0, wchar_t indentChar = L' ') const;

    int64_t Size() const;
    void Resize(int64_t const& size);
    void Resize(int64_t const& size, Json const& json);
    void Clear();
    void PushBack(Json const& value);
    void PushBack(Json && value);
    void Insert  (int64_t index, Json value);
    void Erase   (int64_t index            );
    Json      & operator[](int64_t const& index);
    Json const& operator[](int64_t const& index) const;
    Json      & At(int64_t const& index);
    Json const& At(int64_t const& index) const;

    std::vector<Json>::      iterator begin();
    std::vector<Json>::      iterator end  ();
    std::vector<Json>::const_iterator begin() const;
    std::vector<Json>::const_iterator end  () const;

    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonArray const& array);
    JSON_API friend std::wistream& operator>>(std::wistream& is, JsonArray      & array);

    JSON_API friend bool operator==(JsonArray const& left, JsonArray const& right);
    JSON_API friend bool operator!=(JsonArray const& left, JsonArray const& right);
  };
}