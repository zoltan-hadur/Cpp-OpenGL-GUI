#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonTokenType.h"
#include "Value.h"

#include <variant>
#include <string>
#include <deque>
#include <utility>
#include <sstream>

namespace Json4CPP::Detail
{
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

    /// <summary>
    /// Escapes ", \, \b, \f, \n, \r, \t characters and any other character between 0x00 and 0x1f.
    /// </summary>
    static std::wstring EscapeString  (std::wstring const& value);

    static std::wostream& WriteNumber (std::wostream& os, NUMBER number);
    static std::wostream& WriteObject (std::wostream& os, std::deque<TOKEN>& tokens, uint8_t indentSize, wchar_t indentChar, uint8_t depth);
    static std::wostream& WriteArray  (std::wostream& os, std::deque<TOKEN>& tokens, uint8_t indentSize, wchar_t indentChar, uint8_t depth);
    static std::wostream& Write(std::wostream& os, JsonTokenType token, VALUE_TOKEN const& value);
  public:
    static std::deque<TOKEN> Read(std::wistream     & is   );
    static std::deque<TOKEN> Read(std::wistream    && is   );
    static std::deque<TOKEN> Read(std::wstring const& value);

    static std::wostream& Write(std::wostream& os, std::deque<TOKEN>  & tokens, uint8_t indentSize, wchar_t indentChar);
    static std::wostream& Write(std::wostream& os, std::deque<TOKEN> && tokens, uint8_t indentSize, wchar_t indentChar);

    static std::wstring Dump(VALUE_TOKEN const& value);
  };
}