#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonBuilderType.h"
#include "Value.h"

#include <string>
#include <initializer_list>
#include <vector>

namespace Json4CPP
{
  class JSON_API JsonObject;
  class JSON_API JsonArray;
  class JSON_API Json;

  namespace Detail
  {
    class JSON_API JsonBuilder
    {
    private:
      friend class JsonObject;
      friend class JsonArray;
      friend class Json;
  #pragma warning(suppress: 4251)
      VALUE_BUILDER _value;

    public:
      JsonBuilder();

      JsonBuilder(VALUE          const& value);
      JsonBuilder(VALUE              && value);
      JsonBuilder(std::nullptr_t const& value);
      JsonBuilder(const wchar_t*        value);
      JsonBuilder(std::wstring   const& value);
      JsonBuilder(std::wstring       && value);
      JsonBuilder(bool           const& value);
      JsonBuilder(char           const& value);
      JsonBuilder(int8_t         const& value);
      JsonBuilder(uint8_t        const& value);
      JsonBuilder(int16_t        const& value);
      JsonBuilder(uint16_t       const& value);
      JsonBuilder(int32_t        const& value);
      JsonBuilder(uint32_t       const& value);
      JsonBuilder(int64_t        const& value);
      JsonBuilder(uint64_t       const& value);
      JsonBuilder(float          const& value);
      JsonBuilder(double         const& value);
      JsonBuilder(JsonObject     const& value);
      JsonBuilder(JsonObject         && value);
      JsonBuilder(JsonArray      const& value);
      JsonBuilder(JsonArray          && value);
      JsonBuilder(Json           const& value);
      JsonBuilder(Json               && value);
      JsonBuilder(std::initializer_list<JsonBuilder> values);
      JsonBuilder(std::vector<JsonBuilder> const&    values);
      JsonBuilder(std::vector<JsonBuilder>     &&    values);

      template<typename T>
      JsonBuilder(T value)
      {
        TypeDebug<T>(); // Type T is not supported. See output error C2027 for the exact type.
      }

      JsonBuilderType Type() const;
      bool Is(JsonBuilderType type) const;
    };
  }
}