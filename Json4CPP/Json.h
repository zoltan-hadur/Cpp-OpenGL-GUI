#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "Helper.h"
#include "JsonType.h"
#include "Value.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonLinter.h"
#include "JsonTokenType.h"
#include "JsonIterator.h"
#include "JsonConstIterator.h"
#include "JsonPointer.h"
#include "JsonBuilder.h"

#include <variant>
#include <string>
#include <vector>
#include <utility>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <iterator>

namespace Json4CPP
{
  namespace Test
  {
    class JsonTest;
  }
  namespace Detail
  {
    class JSON_API JsonBuilder;
  }
  class JSON_API JsonObject;
  class JSON_API JsonArray;
  class JSON_API JsonIterator;
  class JSON_API JsonConstIterator;
  class JSON_API JsonPatch;

  JSON_API Json operator""_Json(wchar_t const* value, std::size_t size);

  class JSON_API Json
  {
  private:
    friend class JsonObject;
    friend class JsonArray;
    friend class JsonIterator;
    friend class JsonConstIterator;
    friend class Detail::JsonBuilder;
#pragma warning(suppress: 4251)
    Detail::VALUE _value;

    static Json                         Read (                  std::deque<Detail::TOKEN>  & tokens);
    static Json                         Read (                  std::deque<Detail::TOKEN> && tokens);
    static std::deque<Detail::TOKEN>  & Write(Json const& json, std::deque<Detail::TOKEN>  & tokens);
    static std::deque<Detail::TOKEN> && Write(Json const& json, std::deque<Detail::TOKEN> && tokens);
  public:
    Json();
    Json(Json const& json);
    Json(Json     && json) noexcept;
    Json(Detail::JsonBuilder const& value);
    Json(Detail::JsonBuilder     && value);
    Json(std::initializer_list<Detail::JsonBuilder> values);

    Json(std::nullptr_t      value);
    Json(wchar_t      const* value);
    Json(std::wstring const& value);
    Json(std::wstring     && value);
    Json(bool                value);
    Json(char                value);
    Json(int8_t              value);
    Json(uint8_t             value);
    Json(int16_t             value);
    Json(uint16_t            value);
    Json(int32_t             value);
    Json(uint32_t            value);
    Json(int64_t             value);
    Json(uint64_t            value);
    Json(float               value);
    Json(double              value);
    Json(JsonObject   const& value);
    Json(JsonObject       && value);
    Json(JsonArray    const& value);
    Json(JsonArray        && value);

    JsonType Type() const;
    bool Is(JsonType type) const;

    std::wstring Dump(uint8_t indentSize = 0, wchar_t indentChar = L' ') const;

    static Json Parse(std::string    const&    string);
    static Json Parse(std::wstring   const&   wstring);
    static Json Parse(std::u32string const& u32string);

    template<typename It,
      std::enable_if_t<std::is_integral_v<typename std::iterator_traits<It>::value_type> &&
                       ((sizeof(typename std::iterator_traits<It>::value_type)) == 1 ||
                        (sizeof(typename std::iterator_traits<It>::value_type)) == 2 ||
                        (sizeof(typename std::iterator_traits<It>::value_type)) == 4)>* = nullptr>
    static Json Parse(It begin, It end)
    {
      if constexpr (sizeof(typename std::iterator_traits<It>::value_type) == 1)
      {
        return Json::Parse(std::string(begin, end));
      }
      else if constexpr (sizeof(typename std::iterator_traits<It>::value_type) == 2)
      {
        return Json::Parse(std::wstring(begin, end));
      }
      else if constexpr (sizeof(typename std::iterator_traits<It>::value_type) == 4)
      {
        return Json::Parse(std::u32string(begin, end));
      }
    }

    template<typename Iterable,
      std::enable_if_t<std::is_integral_v<typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type> &&         // Has begin() and it is integral type
                       std::is_integral_v<typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type> &&         // Has end() and it is integral type
                       ((sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type)) == 1 &&             // The integral type's size is 1 byte
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type)) == 1 ||             // Or
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type)) == 2 &&             // The integral type's size is 2 byte
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type)) == 2 ||             // Or
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().begin())>::value_type)) == 4 &&             // The integral type's size is 4 byte
                        (sizeof(typename std::iterator_traits<decltype(std::declval<Iterable>().end  ())>::value_type)) == 4)>* = nullptr>
    static Json Parse(Iterable values)
    {
      return Parse(values.begin(), values.end());
    }

    static Json Read(std::filesystem::path filePath);
    void Write(std::filesystem::path filePath) const;
    void Write(std::filesystem::path filePath, uint8_t indentSize) const;
    void Write(std::filesystem::path filePath, wchar_t indentChar) const;
    void Write(std::filesystem::path filePath, uint8_t indentSize, wchar_t indentChar) const;

    template<typename T>
    auto const& Get() const
    {
      if (auto result = std::get_if<T>(&_value))
      {
        return *result;
      }
      else
      {
        using namespace std::string_literals;
        auto message = "Invalid conversion: Cannot convert type '"s + Json4CPP::Helper::WString2String(Json::Stringify(Type())) + "' to '"s + typeid(T).name() + "'!"s;
        throw std::exception(message.c_str());
      }
    }

    template<typename T>
    auto GetIf()
    {
      return std::get_if<T>(&_value);
    }

    template<typename T>
    static std::wstring Stringify(T && value)
    {
      if constexpr (std::is_same_v<std::decay_t<T>, Json>
                 || std::is_same_v<std::decay_t<T>, Detail::JsonBuilder>
                 || std::is_same_v<std::decay_t<T>, std::initializer_list<Detail::JsonBuilder>>
                 || std::is_same_v<std::decay_t<T>, std::nullptr_t>
                 || std::is_same_v<std::decay_t<T>, wchar_t const*>
                 || std::is_same_v<std::decay_t<T>, std::wstring>
                 || std::is_same_v<std::decay_t<T>, bool>
                 || std::is_same_v<std::decay_t<T>, char>
                 || std::is_same_v<std::decay_t<T>, int8_t>
                 || std::is_same_v<std::decay_t<T>, uint8_t>
                 || std::is_same_v<std::decay_t<T>, int16_t>
                 || std::is_same_v<std::decay_t<T>, uint16_t>
                 || std::is_same_v<std::decay_t<T>, int32_t>
                 || std::is_same_v<std::decay_t<T>, uint32_t>
                 || std::is_same_v<std::decay_t<T>, int64_t>
                 || std::is_same_v<std::decay_t<T>, uint64_t>
                 || std::is_same_v<std::decay_t<T>, float>
                 || std::is_same_v<std::decay_t<T>, double>
                 || std::is_same_v<std::decay_t<T>, JsonObject>
                 || std::is_same_v<std::decay_t<T>, JsonArray>)
      {
        return Json(std::forward<T>(value)).Dump();
      }
      else
      {
        std::wostringstream os;
        os << value;
        return os.str();
      }
    }

    int64_t Size() const;
    int64_t Count(std::wstring const& key) const;
    bool Contains(std::wstring const& key) const;
    void Resize(int64_t size);
    void Resize(int64_t size, Json const& json);
    void Clear();
    void PushBack(Json const& value);
    void PushBack(Json     && value);
    bool Insert(std::pair<std::wstring, Json> const& pair);
    bool Insert(std::pair<std::wstring, Json>     && pair);
    void Insert(int64_t index, Json const& value);
    void Insert(int64_t index, Json     && value);
    void Erase(std::wstring const& key);
    void Erase(int64_t index);
    std::vector<std::wstring> Keys() const;
    std::vector<std::reference_wrapper<const std::wstring>> KeysView() const;
    Json      & operator[](JsonPointer const& ptr);
    Json      & operator[](wchar_t      const* key);
    Json      & operator[](std::wstring const& key);
    Json      & operator[](int64_t index);
    template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
    Json      & operator[](T index)
    {
      return (*this)[static_cast<int64_t>(index)];
    }
    Json      & At(JsonPointer const& ptr);
    Json const& At(JsonPointer const& ptr) const;
    Json      & At(wchar_t      const* key);
    Json const& At(wchar_t      const* key) const;
    Json      & At(std::wstring const& key);
    Json const& At(std::wstring const& key) const;
    Json      & At(int64_t index);
    Json const& At(int64_t index) const;
    template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
    Json      & At(T index)
    {
      return At(static_cast<int64_t>(index));
    }
    template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
    Json const& At(T index) const
    {
      return At(static_cast<int64_t>(index));
    }
    Json Patch(JsonPatch const& patch);

    JsonIterator begin();
    JsonIterator end  ();
    JsonConstIterator begin() const;
    JsonConstIterator end  () const;

    explicit operator std::nullptr_t      () const;
    explicit operator std::wstring const& () const;
    explicit operator std::wstring     && ();
    explicit operator bool                () const;
    explicit operator char                () const;
    explicit operator int8_t              () const;
    explicit operator uint8_t             () const;
    explicit operator int16_t             () const;
    explicit operator uint16_t            () const;
    explicit operator int32_t             () const;
    explicit operator uint32_t            () const;
    explicit operator int64_t             () const;
    explicit operator uint64_t            () const;
    explicit operator float               () const;
    explicit operator double              () const;
    explicit operator JsonObject   const& () const;
    explicit operator JsonObject       && ();
    explicit operator JsonArray    const& () const;
    explicit operator JsonArray        && ();

    Json& operator= (std::nullptr_t             value);
    Json& operator= (wchar_t const*             value);
    Json& operator= (std::wstring        const& value);
    Json& operator= (std::wstring            && value);
    Json& operator= (bool                       value);
    Json& operator= (char                       value);
    Json& operator= (int8_t                     value);
    Json& operator= (uint8_t                    value);
    Json& operator= (int16_t                    value);
    Json& operator= (uint16_t                   value);
    Json& operator= (int32_t                    value);
    Json& operator= (uint32_t                   value);
    Json& operator= (int64_t                    value);
    Json& operator= (uint64_t                   value);
    Json& operator= (float                      value);
    Json& operator= (double                     value);
    Json& operator= (Json                const& value);
    Json& operator= (Json                    && value) noexcept;
    Json& operator= (JsonObject          const& value);
    Json& operator= (JsonObject              && value);
    Json& operator= (JsonArray           const& value);
    Json& operator= (JsonArray               && value);
    Json& operator= (Detail::JsonBuilder const& value);
    Json& operator= (Detail::JsonBuilder     && value);
    Json& operator= (std::initializer_list<Detail::JsonBuilder> values);

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