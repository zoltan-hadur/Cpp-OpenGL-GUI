#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonType.h"
#include "Value.h"
#include "JsonArray.h"
#include "JsonObject.h"

#include <variant>
#include <string>
#include <vector>
#include <utility>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace Json4CPP
{
  namespace Detail
  {
    class JSON_API JsonBuilder;
  }
  class JSON_API JsonObject;
  class JSON_API JsonArray;

  JSON_API Json operator""_json(const wchar_t* value, std::size_t size);

  class JSON_API Json
  {
  private:
    friend class JsonObject;
    friend class JsonArray;
    friend class Detail::JsonBuilder;
#pragma warning(suppress: 4251)
    Detail::VALUE _value;

    void _Dump(std::wstringstream& os, uint8_t indentation, uint64_t level) const;
  public:
    Json();
    Json(Detail::JsonBuilder value);
    Json(std::initializer_list<Detail::JsonBuilder> values);
    Json(Json const& json);

    Json(std::nullptr_t value);
    Json(const wchar_t* value);
    Json(std::wstring   value);
    Json(bool           value);
    Json(char           value);
    Json(int8_t         value);
    Json(uint8_t        value);
    Json(int16_t        value);
    Json(uint16_t       value);
    Json(int32_t        value);
    Json(uint32_t       value);
    Json(int64_t        value);
    Json(uint64_t       value);
    Json(float          value);
    Json(double         value);
    Json(JsonObject     value);
    Json(JsonArray      value);

    JsonType Type() const;
    bool Is(JsonType type) const;

    std::wstring Dump(uint8_t indentation = 0) const;

    static Json Read(std::filesystem::path filePath);
    void Write(std::filesystem::path filePath) const;

    template<typename T>
    auto const& Get() const
    {
      return std::get<T>(_value);
    }

    template<typename T>
    auto GetIf()
    {
      return std::get_if<T>(&_value);
    }

    template<typename T>
    static std::wstring Stringify(T const& value)
    {
      std::wostringstream os;
      os << value;
      return os.str();
    }

    int64_t Size();
    void Resize(int64_t size);
    void Clear();
    bool Insert(std::pair<KEY, Json> pair);
    void PushBack(Json value);
    void Erase(KEY key);
    void Erase(int64_t index);
    Json& operator[](KEY const& key);
    Json& operator[](int const& index);

    explicit operator std::nullptr_t () const;
    explicit operator std::wstring   () const;
    explicit operator bool           () const;
    explicit operator char           () const;
    explicit operator int8_t         () const;
    explicit operator uint8_t        () const;
    explicit operator int16_t        () const;
    explicit operator uint16_t       () const;
    explicit operator int32_t        () const;
    explicit operator uint32_t       () const;
    explicit operator int64_t        () const;
    explicit operator uint64_t       () const;
    explicit operator float          () const;
    explicit operator double         () const;
    explicit operator JsonObject     () const;
    explicit operator JsonArray      () const;

    Json& operator= (std::nullptr_t      value);
    Json& operator= (const wchar_t*      value);
    Json& operator= (std::wstring        value);
    Json& operator= (bool                value);
    Json& operator= (char                value);
    Json& operator= (int8_t              value);
    Json& operator= (uint8_t             value);
    Json& operator= (int16_t             value);
    Json& operator= (uint16_t            value);
    Json& operator= (int32_t             value);
    Json& operator= (uint32_t            value);
    Json& operator= (int64_t             value);
    Json& operator= (uint64_t            value);
    Json& operator= (float               value);
    Json& operator= (double              value);
    Json& operator= (Json                value);
    Json& operator= (JsonObject          value);
    Json& operator= (JsonArray           value);
    Json& operator= (Detail::JsonBuilder value);
    Json& operator= (std::initializer_list<Detail::JsonBuilder> values);

    template<typename T>
    Json& operator=(T const& value)
    {
      Detail::TypeDebug<T>(); // Type T is not supported. See output error C2027 for the exact type.
    }

    JSON_API friend std::wostream& operator<<(std::wostream& os, Json const& json);
    JSON_API friend std::wistream& operator>>(std::wistream& is, Json      & json);

    JSON_API friend bool   operator==(Json const& left, Json const& right);
    JSON_API friend bool   operator!=(Json const& left, Json const& right);
    JSON_API friend bool   operator< (Json const& left, Json const& right);
    JSON_API friend bool   operator<=(Json const& left, Json const& right);
    JSON_API friend bool   operator> (Json const& left, Json const& right);
    JSON_API friend bool   operator>=(Json const& left, Json const& right);
    JSON_API friend Json   operator+ (Json const& left, Json const& right);
    JSON_API friend Json&  operator+=(Json      & left, Json const& right);
    JSON_API friend Json   operator- (Json const& left, Json const& right);
    JSON_API friend Json&  operator-=(Json      & left, Json const& right);
    JSON_API friend Json   operator* (Json const& left, Json const& right);
    JSON_API friend Json&  operator*=(Json      & left, Json const& right);
    JSON_API friend Json   operator/ (Json const& left, Json const& right);
    JSON_API friend Json&  operator/=(Json      & left, Json const& right);
    JSON_API friend Json   operator% (Json const& left, Json const& right);
    JSON_API friend Json&  operator%=(Json      & left, Json const& right);
    JSON_API friend Json   operator- (Json const& value                  );
    JSON_API friend bool   operator! (Json const& value                  );
    JSON_API friend Json&  operator++(Json      & value                  );
    JSON_API friend Json   operator++(Json      & value, int             );
    JSON_API friend Json&  operator--(Json      & value                  );
    JSON_API friend Json   operator--(Json      & value, int             );
    JSON_API friend bool   operator&&(Json const& left, Json const& right);
    JSON_API friend bool   operator||(Json const& left, Json const& right);
  };
}