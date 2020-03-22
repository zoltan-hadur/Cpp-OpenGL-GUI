#include "stdafx.h"

#include "JsonLinter.h"
#include "Helper.h"

using namespace std;

namespace Json4CPP::Detail
{
  nullptr_t JsonLinter::ParseNull(wistream& is)
  {
    auto expected = L"null"s;
    streampos pos;
    for (int i = 0; i < 4; ++i)
    {
      auto c = is.get();
      if (i == 0) pos = is.tellg();
      if (c != expected[i])
      {
        auto message = "Expected \"null\" at position "s + GetFormattedStreamPositionA(is, pos) + "!"s;
        throw exception(message.c_str());
      }
    }
    return nullptr;
  }

  wstring JsonLinter::ParseString(wistream& is)
  {
    auto text = L""s;
    // A string must start with a quote
    if (is.get() == L'\"')
    {
      // Check if it's an empty string in which case it's immediately ends with a quote
      if (is.peek() == L'\"')
      {
        is.get();
      }
      else
      {
        // Read the characters until the closing quote
        while (is.peek() != L'\"' && !is.eof())
        {
          auto c = is.get();
          if (c == L'\\')
          {
            switch (is.get())
            {
            case L'b' : text.push_back(L'\b'); break;
            case L'f' : text.push_back(L'\f'); break;
            case L'n' : text.push_back(L'\n'); break;
            case L'r' : text.push_back(L'\r'); break;
            case L't' : text.push_back(L'\t'); break;
            case L'\"': text.push_back(L'\"'); break;
            case L'\\': text.push_back(L'\\'); break;
            case L'/' : text.push_back(L'/');  break;
            case L'u' :
            {
              auto hexCode = L""s;
              for (int i = 0; i < 4; ++i)
              {
                c = is.get();
                if (iswxdigit(c))
                {
                  hexCode.push_back(c);
                }
                else
                {
                  auto message = "Expected a hexadecimal digit at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
                  throw exception(message.c_str());
                }
              }
              wstringstream ss;
              ss << hex << hexCode;
              unsigned int number;
              ss >> number;
              text.push_back((wchar_t)number);
              break;
            }
            default:
            {
              is.get();
              auto message = "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
              throw exception(message.c_str());
              break;
            }
            }
          }
          else if (!iswcntrl(c))
          {
            text.push_back(c);
          }
          else
          {
            auto message = "Invalid character found at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
            throw exception(message.c_str());
          }
        }
        // If no closing quote, just eof
        if (is.eof())
        {
          auto message = "Expected '\"' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
          throw exception(message.c_str());
        }
        // Otherwise it must be a closing quote, so get it
        else
        {
          is.get();
        }
      }
    }
    else
    {
      auto message = "Expected '\"' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
      throw exception(message.c_str());
    }
    return text;
  }

  bool JsonLinter::ParseBoolean(wistream& is)
  {
    auto expected = is.peek() == L't' ? L"true"s : is.peek() == L'f' ? L"false"s : L""s;
    if (!expected.empty())
    {
      streampos pos;
      for (int i = 0; i < expected.size(); ++i)
      {
        auto c = is.get();
        if (i == 0) pos = is.tellg();
        if (c != expected[i])
        {
          auto message = "Expected \""s + WString2String(expected) + "\" at position "s + GetFormattedStreamPositionA(is, pos) + "!"s;
          throw exception(message.c_str());
        }
      }
      return expected == L"true"s;
    }
    auto message = "Expected \"true\" or \"false\" at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
    throw exception(message.c_str());
  }

  double JsonLinter::ParseNumber(wistream& is)
  {
    wstring text;

    // Can start with '-'
    if (is.peek() == L'-')
    {
      text.push_back(is.get());
    }

    // Either continues with '0'
    if (is.peek() == L'0')
    {
      text.push_back(is.get());
    }
    // Or with a digit between '1' and '9'
    else if (L'1' <= is.peek() && is.peek() <= L'9')
    {
      // And then in that case it can continue with zero or more digit
      while (iswdigit(is.peek()))
      {
        text.push_back(is.get());
      }
    }
    // Else it's not a number
    else
    {
      is.get();
      auto message = "Expected digit at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
      throw exception(message.c_str());
    }

    // Can contain '.'
    if (is.peek() == L'.')
    {
      text.push_back(is.get());
      // And then it contains at least one digit
      auto hasAtLeastOneDigit = false;
      while (iswdigit(is.peek()))
      {
        text.push_back(is.get());
        hasAtLeastOneDigit = true;
      }
      if (!hasAtLeastOneDigit)
      {
        is.get();
        auto message = "Expected digit at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
        throw exception(message.c_str());
      }
    }

    // Can contain 'e' or 'E'
    if (is.peek() == L'e' || is.peek() == L'E')
    {
      text.push_back(is.get());
      // And then it can contain either '+' or '-'
      if (is.peek() == L'+' || is.peek() == L'-')
      {
        text.push_back(is.get());
      }
      // And then it contains at least one digit
      auto hasAtLeastOneDigit = false;
      while (iswdigit(is.peek()))
      {
        text.push_back(is.get());
        hasAtLeastOneDigit = true;
      }
      if (!hasAtLeastOneDigit)
      {
        is.get();
        auto message = "Expected digit at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
        throw exception(message.c_str());
      }
    }

    double number;
    wstringstream(text) >> number;
    return number;
  }

  void JsonLinter::ParseObject(wistream& is, std::deque<TOKEN>& tokens, uint8_t level, uint8_t& maxLevel)
  {
    if (is.peek() == L'{')
    {
      tokens.push_back({ JsonTokenType::StartObject, L""s + (wchar_t)is.get() });
      is >> ws;
      if (is.peek() != L'}')
      {
        tokens.push_back({ JsonTokenType::PropertyName, ParseString(is) });
        is >> ws;
        if (is.get() == L':')
        {
          is >> ws;
        }
        else
        {
          auto message = "Expected ':' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
          throw exception(message.c_str());
        }
        Read(is, tokens, level, maxLevel);
        is >> ws;
        while (is.peek() == L',')
        {
          is.get();
          is >> ws;
          tokens.push_back({ JsonTokenType::PropertyName, ParseString(is) });
          is >> ws;
          if (is.get() == L':')
          {
            is >> ws;
          }
          else
          {
            auto message = "Expected ':' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
            throw exception(message.c_str());
          }
          Read(is, tokens, level, maxLevel);
          is >> ws;
        }
        if (is.peek() == L'}')
        {
          tokens.push_back({ JsonTokenType::EndObject, L""s + (wchar_t)is.get() });
        }
        else
        {
          auto message = "Expected '}' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
          throw exception(message.c_str());
        }
      }
      else
      {
        tokens.push_back({ JsonTokenType::EndObject, L""s + (wchar_t)is.get() });
      }
    }
    else
    {
      auto message = "Expected '{' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
      throw exception(message.c_str());
    }
  }

  void JsonLinter::ParseArray(wistream& is, std::deque<TOKEN>& tokens, uint8_t level, uint8_t& maxLevel)
  {
    if (is.peek() == L'[')
    {
      tokens.push_back({ JsonTokenType::StartArray, L""s + (wchar_t)is.get() });
      is >> ws;
      if (is.peek() == L',')
      {
        is.get();
        auto message = "Unexpected ',' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
        throw exception(message.c_str());
      }
      if (is.peek() != L']')
      {
        Read(is, tokens, level, maxLevel);
        is >> ws;
        while (is.peek() == L',')
        {
          is.get();
          is >> ws;
          Read(is, tokens, level, maxLevel);
          is >> ws;
        }
        if (is.peek() == L']')
        {
          tokens.push_back({ JsonTokenType::EndArray, L""s + (wchar_t)is.get() });
        }
        else
        {
          is.get();
          auto message = "Expected ']' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
          throw exception(message.c_str());
        }
      }
      else
      {
        tokens.push_back({ JsonTokenType::EndArray, L""s + (wchar_t)is.get() });
      }
    }
    else
    {
      auto message = "Expected '[' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
      throw exception(message.c_str());
    }
  }

  void JsonLinter::Read(wistream& is, std::deque<TOKEN>& tokens, uint8_t level, uint8_t& maxLevel)
  {
    is >> ws;
    switch (is.peek())
    {
    case L'n':
      tokens.push_back({ JsonTokenType::Null   , ParseNull   (is) });
      break;

    case L'\"':
      tokens.push_back({ JsonTokenType::String , ParseString (is) });
      break;

    case L't':
    case L'f':
      tokens.push_back({ JsonTokenType::Boolean, ParseBoolean(is) });
      break;

    default:
      tokens.push_back({ JsonTokenType::Number , ParseNumber (is) });
      break;

    case L'{':
      maxLevel = max(++level, maxLevel);
      ParseObject(is, tokens, level, maxLevel);
      break;

    case L'[':
      maxLevel = max(++level, maxLevel);
      ParseArray(is, tokens, level, maxLevel);
      break;
    }
  }

  wostream& JsonLinter::Write(wostream& os, JsonTokenType const& token, VALUE_TOKEN const& value)
  {
    switch (token)
    {
    case JsonTokenType::Null:
      os << L"null"s;
      break;

    case JsonTokenType::String:
    case JsonTokenType::PropertyName:
      os << L"\""s << EscapeString(get<wstring>(value)) << L"\""s;
      break;

    case JsonTokenType::Boolean:
      os << (get<bool>(value) ? L"true"s : L"false"s);
      break;

    case JsonTokenType::Number:
      os << +(get<double>(value));
      break;

    case JsonTokenType::StartObject:
    case JsonTokenType::EndObject:
    case JsonTokenType::StartArray:
    case JsonTokenType::EndArray:
      os << get<wstring>(value);
      break;

    case JsonTokenType::Undefined:
    default:
      visit(Overload{
        [&](nullptr_t  const& v) { os << L"null"s;                            },
        [&](wstring    const& v) { os << L"\""s << EscapeString(v) << L"\""s; },
        [&](bool       const& v) { os << (v ? L"true"s : L"false"s);          },
        [&](double     const& v) { os << +v;                                  },
        [&](auto       const& v) { throw exception("Invalid type"s)           }
      }, value);
      break;
    }
    return os;
  }

  wostream& JsonLinter::WriteObject(wostream& os, std::deque<TOKEN>& tokens, uint8_t indentation, uint8_t level)
  {
    if (tokens.empty())
    {
      auto message = WString2String(L"Expected token: "s + Detail::Dump(JsonTokenType::StartObject) + L"!"s);
      throw exception(message.c_str());
    }

    auto indent = wstring(indentation * level, L' ');
    auto single = wstring(indentation, L' ');
    auto newLine = indentation == 0 ? L""s : L"\r\n"s;
    auto space = indentation == 0 ? L""s : L" "s;

    // Write StartObject and a new line if needed
    auto& [token, value] = tokens.front();
    if (token == JsonTokenType::StartObject)
    {
      // But first, check if it is an empty object
      if (tokens.size() >= 2 && tokens[1].first == JsonTokenType::EndObject)
      {
        Write(os, token, value);
        tokens.pop_front();
        tie(token, value) = tokens.front();
        Write(os, token, value);
        tokens.pop_front();
        return os;
      }
      else
      {
        Write(os, token, value) << newLine;
        tokens.pop_front();
      }
    }
    else
    {
      auto message = WString2String(L"Expected token: "s + Detail::Dump(JsonTokenType::StartObject) + L"!"s);
      throw exception(message.c_str());
    }

    // While there are tokens
    while (tokens.size())
    {
      // Write "key"
      os << indent << single;
      tie(token, value) = tokens.front();
      if (token == JsonTokenType::PropertyName)
      {
        Write(os, token, value) << L":"s << space;
        tokens.pop_front();
      }
      else
      {
        auto message = WString2String(L"Expected token: "s + Detail::Dump(JsonTokenType::PropertyName) + L"!"s);
        throw exception(message.c_str());
      }

      // If there are still more tokens
      if(tokens.size())
      {
        // Write "value"
        tie(token, value) = tokens.front();
        switch (token)
        {
        case JsonTokenType::StartObject:
          WriteObject(os, tokens, indentation, level + 1);
          break;
        case JsonTokenType::StartArray:
          WriteArray(os, tokens, indentation, level + 1);
          break;
        case JsonTokenType::Null:
        case JsonTokenType::String:
        case JsonTokenType::Boolean:
        case JsonTokenType::Number:
          Write(os, token, value);
          tokens.pop_front();
          break;
        default:
          auto message = WString2String(L"Expected one of the following tokens: "s +
            Detail::Dump(JsonTokenType::Null       ) + L", "s   + Detail::Dump(JsonTokenType::String    ) + L", "s +
            Detail::Dump(JsonTokenType::Boolean    ) + L", "s   + Detail::Dump(JsonTokenType::Number    ) + L", "s +
            Detail::Dump(JsonTokenType::StartObject) + L" or "s + Detail::Dump(JsonTokenType::StartArray) + L"!"s);
          throw exception(message.c_str());
        }

        // If there are even more tokens
        if (tokens.size())
        {
          // Write EndObject or a coma
          tie(token, value) = tokens.front();
          if (token == JsonTokenType::EndObject)
          {
            Write(os << newLine << indent, token, value);
            tokens.pop_front();
            return os;
          }
          else
          {
            os << L","s << newLine;
          }
        }
      }
      else
      {
        auto message = WString2String(L"Expected one of the following tokens: "s +
          Detail::Dump(JsonTokenType::Null       ) + L", "s   + Detail::Dump(JsonTokenType::String    ) + L", "s +
          Detail::Dump(JsonTokenType::Boolean    ) + L", "s   + Detail::Dump(JsonTokenType::Number    ) + L", "s +
          Detail::Dump(JsonTokenType::StartObject) + L" or "s + Detail::Dump(JsonTokenType::StartArray) + L"!"s);
        throw exception(message.c_str());
      }
    }
    auto message = WString2String(L"Expected token: "s + Detail::Dump(JsonTokenType::EndObject) + L"!"s);
    throw exception(message.c_str());
  }

  wostream& JsonLinter::WriteArray(wostream& os, std::deque<TOKEN>& tokens, uint8_t indentation, uint8_t level)
  {
    if (tokens.empty())
    {
      auto message = WString2String(L"Expected token: "s + Detail::Dump(JsonTokenType::StartArray) + L"!"s);
      throw exception(message.c_str());
    }

    auto indent = wstring(indentation * level, L' ');
    auto single = wstring(indentation, L' ');
    auto newLine = indentation == 0 ? L""s : L"\r\n"s;

    // Write StartArray and a new line if needed
    auto& [token, value] = tokens.front();
    if (token == JsonTokenType::StartArray)
    {
      // But first, check if it is an empty array
      if (tokens.size() >= 2 && tokens[1].first == JsonTokenType::EndArray)
      {
        Write(os, token, value);
        tokens.pop_front();
        tie(token, value) = tokens.front();
        Write(os, token, value);
        tokens.pop_front();
        return os;
      }
      else
      {
        Write(os, token, value) << newLine;
        tokens.pop_front();
      }
    }
    else
    {
      auto message = WString2String(L"Expected token: "s + Detail::Dump(JsonTokenType::StartArray) + L"!"s);
      throw exception(message.c_str());
    }

    // While there are tokens
    while (tokens.size())
    {
      // Write "value"
      os << indent << single;
      tie(token, value) = tokens.front();
      switch (token)
      {
      case JsonTokenType::StartObject:
        WriteObject(os, tokens, indentation, level + 1);
        break;
      case JsonTokenType::StartArray:
        WriteArray(os, tokens, indentation, level + 1);
        break;
      case JsonTokenType::Null:
      case JsonTokenType::String:
      case JsonTokenType::Boolean:
      case JsonTokenType::Number:
        Write(os, token, value);
        tokens.pop_front();
        break;
      default:
        auto message = WString2String(L"Expected one of the following tokens: "s +
          Detail::Dump(JsonTokenType::Null       ) + L", "s   + Detail::Dump(JsonTokenType::String    ) + L", "s +
          Detail::Dump(JsonTokenType::Boolean    ) + L", "s   + Detail::Dump(JsonTokenType::Number    ) + L", "s +
          Detail::Dump(JsonTokenType::StartObject) + L" or "s + Detail::Dump(JsonTokenType::StartArray) + L"!"s);
        throw exception(message.c_str());
      }

      // If there are still more tokens
      if (tokens.size())
      {
        // Write EndArray or a coma
        tie(token, value) = tokens.front();
        if (token == JsonTokenType::EndArray)
        {
          Write(os << newLine << indent, token, value);
          tokens.pop_front();
          return os;
        }
        else
        {
          os << L","s << newLine;
        }
      }
    }
    auto message = WString2String(L"Expected token: "s + Detail::Dump(JsonTokenType::EndArray) + L"!"s);
    throw exception(message.c_str());
  }

  std::deque<TOKEN> JsonLinter::Read(wistream     & is   )
  {
    auto maxLevel = 0ui8;
    auto tokens = std::deque<TOKEN>();
    Read(is, tokens, 0, maxLevel);
    if (maxLevel >= 20)
    {
      auto message = "Depth is greater or equal to the maximum 20: "s + to_string(maxLevel) + "!"s;
      throw exception(message.c_str());
    }
    if (is.peek(), is.eof())
    {
      return tokens;
    }
    auto c = WString2String(L""s + (wchar_t)is.get());
    auto message = "Unexpected '"s + c + "' at position "s + GetFormattedStreamPositionA(is, is.tellg()) + "!"s;
    throw exception(message.c_str());
  }

  std::deque<TOKEN> JsonLinter::Read(wstring const& value)
  {
    return Read(wstringstream(value));
  }

  wostream& JsonLinter::Write(wostream& os, std::deque<TOKEN>& tokens, uint8_t indentation, uint8_t level)
  {
    auto& [token, value] = tokens.front();
    switch (token)
    {
    case JsonTokenType::StartObject: WriteObject(os, tokens, indentation, level); break;
    case JsonTokenType::StartArray : WriteArray (os, tokens, indentation, level); break;
    default                    : Write(os, token, value);                     break;
    }
    return os;
  }

  wstring JsonLinter::Dump(VALUE_TOKEN value)
  {
    wostringstream os;
    Write(os, JsonTokenType::Undefined, value);
    return os.str();
  }
}