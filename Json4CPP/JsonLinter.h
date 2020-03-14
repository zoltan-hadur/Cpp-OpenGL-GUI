#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonToken.h"

#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <variant>

namespace Json4CPP::Detail
{
  using VALUE_TOKEN = std::variant<std::nullptr_t, std::wstring, bool, double>;

  class JSON_API JsonLinter
  {
  private:
    static std::nullptr_t ParseNull   (std::wistream& is);
    static std::wstring   ParseString (std::wistream& is);
    static bool           ParseBoolean(std::wistream& is);
    static double         ParseNumber (std::wistream& is);
    static std::vector<std::pair<JsonToken, VALUE_TOKEN>> ParseObject (std::wistream& is);
    static std::vector<std::pair<JsonToken, VALUE_TOKEN>> ParseArray  (std::wistream& is);
  public:
    static std::vector<std::pair<JsonToken, VALUE_TOKEN>> Read(std::wistream     & is   );
    static std::vector<std::pair<JsonToken, VALUE_TOKEN>> Read(std::wstring const& value);
  };
}