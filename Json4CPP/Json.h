#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonType.h"
#include "JsonBuilderType.h"
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
  class JSON_API JsonObject;
  class JSON_API JsonArray;
  class JSON_API JsonBuilder;

  JSON_API Json operator""_json(const wchar_t* value, std::size_t size);

  class JSON_API Json
  {
  private:
    friend class JsonObject;
    friend class JsonArray;
    friend class JsonBuilder;
#pragma warning(suppress: 4251)
    VALUE _value;

    void Dump(std::wstringstream& os, int indentation, int level) const;
  public:
    Json();
    Json(JsonBuilder value);
    Json(std::initializer_list<JsonBuilder> values);
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

    std::wstring Dump(int indentation = 0) const;

    static Json Read(std::filesystem::path filePath);
    void Write(std::filesystem::path filePath) const;

    template<typename T>
    T Get() const
    {
      return std::get<T>(_value);
    }

    template<typename T>
    T& Get()
    {
      return std::get<T>(_value);
    }

    operator std::nullptr_t   ();
    operator std::wstring     ();
    operator bool             ();
    operator char             ();
    operator int8_t           ();
    operator uint8_t          ();
    operator int16_t          ();
    operator uint16_t         ();
    operator int32_t          ();
    operator uint32_t         ();
    operator int64_t          ();
    operator uint64_t         ();
    operator float            ();
    operator double           ();
    operator JsonObject       ();
    operator JsonArray        ();

    operator std::nullptr_t   () const;
    operator std::wstring     () const;
    operator bool             () const;
    operator char             () const;
    operator int8_t           () const;
    operator uint8_t          () const;
    operator int16_t          () const;
    operator uint16_t         () const;
    operator int32_t          () const;
    operator uint32_t         () const;
    operator int64_t          () const;
    operator uint64_t         () const;
    operator float            () const;
    operator double           () const;
    operator JsonObject       () const;
    operator JsonArray        () const;

    operator std::nullptr_t & ();
    operator std::wstring   & ();
    operator bool           & ();
    operator double         & ();
    operator JsonObject     & ();
    operator JsonArray      & ();

    Json& operator[](NAME const&                        name  );
    Json& operator[](const wchar_t*                     name  );
    Json& operator[](int const&                         index );

    Json& operator= (std::nullptr_t                     value );
    Json& operator= (const wchar_t*                     value );
    Json& operator= (std::wstring                       value );
    Json& operator= (bool                               value );
    Json& operator= (char                               value );
    Json& operator= (int8_t                             value );
    Json& operator= (uint8_t                            value );
    Json& operator= (int16_t                            value );
    Json& operator= (uint16_t                           value );
    Json& operator= (int32_t                            value );
    Json& operator= (uint32_t                           value );
    Json& operator= (int64_t                            value );
    Json& operator= (uint64_t                           value );
    Json& operator= (float                              value );
    Json& operator= (double                             value );
    Json& operator= (JsonObject                         value );
    Json& operator= (JsonArray                          value );
    Json& operator= (JsonBuilder                        value );
    Json& operator= (std::initializer_list<JsonBuilder> values);

    template<typename T>
    Json& operator=(T const& value)
    {
      TypeDebug<T>(); // Type T is not supported. See output error C2027 for the exact type.
    }

    JSON_API friend std::wostream& operator<<(std::wostream& os, Json const& json);
    JSON_API friend std::wistream& operator>>(std::wistream& is, Json      & json);

    JSON_API              friend bool   operator==(Json const& left, Json const& right);
    template<typename T>  friend bool   operator==(Json const& left, T    const& right) { return left == Json(right); }
    template<typename T>  friend bool   operator==(T    const& left, Json const& right) { return Json(left) == right; }

    JSON_API              friend bool   operator!=(Json const& left, Json const& right);
    template<typename T>  friend bool   operator!=(Json const& left, T    const& right) { return left != Json(right); }
    template<typename T>  friend bool   operator!=(T    const& left, Json const& right) { return Json(left) != right; }

    JSON_API              friend bool   operator< (Json const& left, Json const& right);
    template<typename T>  friend bool   operator< (Json const& left, T    const& right) { return left < Json(right); }
    template<typename T>  friend bool   operator< (T    const& left, Json const& right) { return Json(left) < right; }

    JSON_API              friend bool   operator<=(Json const& left, Json const& right);
    template<typename T>  friend bool   operator<=(Json const& left, T    const& right) { return left <= Json(right); }
    template<typename T>  friend bool   operator<=(T    const& left, Json const& right) { return Json(left) <= right; }

    JSON_API              friend bool   operator> (Json const& left, Json const& right);
    template<typename T>  friend bool   operator> (Json const& left, T    const& right) { return left > Json(right); }
    template<typename T>  friend bool   operator> (T    const& left, Json const& right) { return Json(left) > right; }

    JSON_API              friend bool   operator>=(Json const& left, Json const& right);
    template<typename T>  friend bool   operator>=(Json const& left, T    const& right) { return left >= Json(right); }
    template<typename T>  friend bool   operator>=(T    const& left, Json const& right) { return Json(left) >= right; }

    JSON_API              friend Json   operator+ (Json const& left, Json const& right);
    template<typename T>  friend Json   operator+ (Json const& left, T    const& right) { return left + Json(right); }
    template<typename T>  friend Json   operator+ (T    const& left, Json const& right) { return Json(left) + right; }

    JSON_API              friend Json&  operator+=(Json      & left, Json const& right);
    template<typename T>  friend Json&  operator+=(Json      & left, T    const& right) { left += Json(right); return left; }

    JSON_API              friend Json   operator- (Json const& left, Json const& right);
    template<typename T>  friend Json   operator- (Json const& left, T    const& right) { return left - Json(right); }
    template<typename T>  friend Json   operator- (T    const& left, Json const& right) { return Json(left) - right; }

    JSON_API              friend Json&  operator-=(Json      & left, Json const& right);
    template<typename T>  friend Json&  operator-=(Json      & left, T    const& right) { left -= Json(right); return left; }

    JSON_API              friend Json   operator* (Json const& left, Json const& right);
    template<typename T>  friend Json   operator* (Json const& left, T    const& right) { return left * Json(right); }
    template<typename T>  friend Json   operator* (T    const& left, Json const& right) { return Json(left) * right; }

    JSON_API              friend Json&  operator*=(Json      & left, Json const& right);
    template<typename T>  friend Json&  operator*=(Json      & left, T    const& right) { left *= Json(right); return left; }

    JSON_API              friend Json   operator/ (Json const& left, Json const& right);
    template<typename T>  friend Json   operator/ (Json const& left, T    const& right) { return left / Json(right); }
    template<typename T>  friend Json   operator/ (T    const& left, Json const& right) { return Json(left) / right; }

    JSON_API              friend Json&  operator/=(Json      & left, Json const& right);
    template<typename T>  friend Json&  operator/=(Json      & left, T    const& right) { left /= Json(right); return left; }

    JSON_API              friend Json   operator% (Json const& left, Json const& right);
    template<typename T>  friend Json   operator% (Json const& left, T    const& right) { return left % Json(right); }
    template<typename T>  friend Json   operator% (T    const& left, Json const& right) { return Json(left) % right; }

    JSON_API              friend Json&  operator%=(Json      & left, Json const& right);
    template<typename T>  friend Json&  operator%=(Json      & left, T    const& right) { left %= Json(right); return left; }

    JSON_API              friend Json   operator- (Json const& value);

    JSON_API              friend bool   operator! (Json const& value);

    JSON_API              friend Json&  operator++(Json      & value);

    JSON_API              friend Json   operator++(Json      & value, int);

    JSON_API              friend Json&  operator--(Json      & value);

    JSON_API              friend Json   operator--(Json      & value, int);

    JSON_API              friend bool   operator&&(Json const& left, Json const& right);
    template<typename T>  friend bool   operator&&(Json const& left, T    const& right) { return left && Json(right); }
    template<typename T>  friend bool   operator&&(T    const& left, Json const& right) { return Json(left) && right; }

    JSON_API              friend bool   operator||(Json const& left, Json const& right);
    template<typename T>  friend bool   operator||(Json const& left, T    const& right) { return left || Json(right); }
    template<typename T>  friend bool   operator||(T    const& left, Json const& right) { return Json(left) || right; }
  };
}