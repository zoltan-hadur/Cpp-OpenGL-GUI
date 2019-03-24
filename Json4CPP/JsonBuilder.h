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

    JsonBuilder(VALUE                               value );
    JsonBuilder(std::nullptr_t                      value );
    JsonBuilder(const wchar_t*                      value );
    JsonBuilder(std::wstring                        value );
    JsonBuilder(bool                                value );
    JsonBuilder(char                                value );
    JsonBuilder(int8_t                              value );
    JsonBuilder(uint8_t                             value );
    JsonBuilder(int16_t                             value );
    JsonBuilder(uint16_t                            value );
    JsonBuilder(int32_t                             value );
    JsonBuilder(uint32_t                            value );
    JsonBuilder(int64_t                             value );
    JsonBuilder(uint64_t                            value );
    JsonBuilder(float                               value );
    JsonBuilder(double                              value );
    JsonBuilder(JsonObject                          value );
    JsonBuilder(JsonArray                           value );
    JsonBuilder(Json                                value );
    JsonBuilder(std::initializer_list<JsonBuilder>  values);
    JsonBuilder(std::vector<JsonBuilder>            values);

    template<typename T>
    JsonBuilder(T value)
    {
      TypeDebug<T>(); // Type T is not supported. See output error C2027 for the exact type.
    }

    JsonBuilderType Type() const;
    bool Is(JsonBuilderType type) const;
  };
}