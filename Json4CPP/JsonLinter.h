#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonTokenType.h"

#include <variant>
#include <string>
#include <deque>
#include <utility>
#include <sstream>

namespace Json4CPP::Detail
{
  using VALUE_TOKEN = std::variant<std::nullptr_t, std::wstring, bool, double, int64_t>;
  using TOKEN = std::pair<JsonTokenType, VALUE_TOKEN>;
  using NUMBER = std::variant<double, int64_t>;

  class JSON_API JsonLinter
  {
  private:
    static std::nullptr_t ParseNull   (std::wistream& is);
    static std::wstring   ParseString (std::wistream& is);
    static bool           ParseBoolean(std::wistream& is);
    static NUMBER         ParseNumber (std::wistream& is);
    static void           ParseObject (std::wistream& is, std::deque<TOKEN>& tokens, uint8_t depth);
    static void           ParseArray  (std::wistream& is, std::deque<TOKEN>& tokens, uint8_t depth);
    static void           Read        (std::wistream& is, std::deque<TOKEN>& tokens, uint8_t depth);

    static std::wostream& WriteNumber (std::wostream& os, NUMBER number);
    static std::wostream& WriteObject (std::wostream& os, std::deque<TOKEN>& tokens, uint8_t indentation, uint8_t depth);
    static std::wostream& WriteArray  (std::wostream& os, std::deque<TOKEN>& tokens, uint8_t indentation, uint8_t depth);
    static std::wostream& Write(std::wostream& os, JsonTokenType const& token, VALUE_TOKEN const& value);
  public:
    static std::deque<TOKEN> Read(std::wistream     & is   );
    static std::deque<TOKEN> Read(std::wstring const& value);

    static std::wostream& Write(std::wostream& os, std::deque<TOKEN>& tokens, uint8_t indentation);

    static std::wstring Dump(VALUE_TOKEN value);
  };
}