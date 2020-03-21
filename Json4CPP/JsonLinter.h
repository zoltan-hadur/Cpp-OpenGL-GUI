#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonToken.h"

#include <variant>
#include <string>
#include <deque>
#include <utility>
#include <sstream>

namespace Json4CPP::Detail
{
  using VALUE_TOKEN = std::variant<std::nullptr_t, std::wstring, bool, double>;
  using TOKEN_COLLECTION = std::deque<std::pair<JsonToken, VALUE_TOKEN>>;

  class JSON_API JsonLinter
  {
  private:
    static std::nullptr_t ParseNull   (std::wistream& is);
    static std::wstring   ParseString (std::wistream& is);
    static bool           ParseBoolean(std::wistream& is);
    static double         ParseNumber (std::wistream& is);
    static void           ParseObject (std::wistream& is, TOKEN_COLLECTION& tokens, int& level);
    static void           ParseArray  (std::wistream& is, TOKEN_COLLECTION& tokens, int& level);

    static void           Read        (std::wistream& is, TOKEN_COLLECTION& tokens, int& level);

    static std::wostream& Write       (std::wostream& os, JsonToken const& token, VALUE_TOKEN const& value);
    static std::wostream& WriteObject (std::wostream& os, TOKEN_COLLECTION& tokens, uint8_t indentation, uint64_t level);
    static std::wostream& WriteArray  (std::wostream& os, TOKEN_COLLECTION& tokens, uint8_t indentation, uint64_t level);
  public:
    static TOKEN_COLLECTION Read(std::wistream     & is   );
    static TOKEN_COLLECTION Read(std::wstring const& value);

    static std::wostream& Write(std::wostream& os, TOKEN_COLLECTION& tokens, uint8_t indentation, uint64_t level);

    static std::wstring Dump(VALUE_TOKEN value);
  };
}