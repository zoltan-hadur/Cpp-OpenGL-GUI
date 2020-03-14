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
        auto message = "Expected \"null\" at position " + GetFormattedStreamPositionA(is, pos) + "!";
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
                  auto message = "Expected a hexadecimal digit at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
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
              auto message = "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
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
            auto message = "Invalid character found at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
            throw exception(message.c_str());
          }
        }
        // If no closing quote, just eof
        if (is.eof())
        {
          auto message = "Expected '\"' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
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
      auto message = "Expected '\"' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
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
          auto message = "Expected \"" + WString2String(expected) + "\" at position " + GetFormattedStreamPositionA(is, pos) + "!";
          throw exception(message.c_str());
        }
      }
      return expected == L"true"s;
    }
    auto message = "Expected \"true\" or \"false\" at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
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
      auto message = "Expected digit at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
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
        auto message = "Expected digit at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
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
        auto message = "Expected digit at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
        throw exception(message.c_str());
      }
    }

    double number;
    wstringstream(text) >> number;
    return number;
  }

  vector<pair<JsonToken, VALUE_TOKEN>> JsonLinter::ParseObject(wistream& is)
  {
    auto tokens = vector<pair<JsonToken, VALUE_TOKEN>>();
    if (is.peek() == L'{')
    {
      tokens.push_back({ JsonToken::StartObject, L""s + (wchar_t)is.get() });
      is >> ws;
      if (is.peek() != L'}')
      {
        tokens.push_back({ JsonToken::PropertyName, ParseString(is) });
        is >> ws;
        if (is.get() == L':')
        {
          is >> ws;
        }
        else
        {
          auto message = "Expected ':' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
          throw exception(message.c_str());
        }
        for (auto& token : Read(is))
        {
          tokens.push_back(token);
        }
        is >> ws;
        while (is.peek() == L',')
        {
          is.get();
          is >> ws;
          tokens.push_back({ JsonToken::PropertyName, ParseString(is) });
          is >> ws;
          if (is.get() == L':')
          {
            is >> ws;
          }
          else
          {
            auto message = "Expected ':' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
            throw exception(message.c_str());
          }
          for (auto& token : Read(is))
          {
            tokens.push_back(token);
          }
          is >> ws;
        }
        if (is.peek() == L'}')
        {
          tokens.push_back({ JsonToken::EndObject, L""s + (wchar_t)is.get() });
        }
        else
        {
          auto message = "Expected '}' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
          throw exception(message.c_str());
        }
      }
      else
      {
        tokens.push_back({ JsonToken::EndObject, L""s + (wchar_t)is.get() });
      }
    }
    else
    {
      auto message = "Expected '{' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
      throw exception(message.c_str());
    }
    return tokens;
  }

  vector<pair<JsonToken, VALUE_TOKEN>> JsonLinter::ParseArray(wistream& is)
  {
    auto tokens = vector<pair<JsonToken, VALUE_TOKEN>>();
    if (is.peek() == L'[')
    {
      tokens.push_back({ JsonToken::StartArray, L""s + (wchar_t)is.get() });
      is >> ws;
      if (is.peek() == L',')
      {
        is.get();
        auto message = "Unexpected ',' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
        throw exception(message.c_str());
      }
      if (is.peek() != L']')
      {
        for (auto& token : Read(is))
        {
          tokens.push_back(token);
        }
        is >> ws;
        while (is.peek() == L',')
        {
          is.get();
          is >> ws;
          for (auto& token : Read(is))
          {
            tokens.push_back(token);
          }
          is >> ws;
        }
        if (is.peek() == L']')
        {
          tokens.push_back({ JsonToken::EndArray, L""s + (wchar_t)is.get() });
        }
        else
        {
          is.get();
          auto message = "Expected ']' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
          throw exception(message.c_str());
        }
      }
      else
      {
        tokens.push_back({ JsonToken::EndArray, L""s + (wchar_t)is.get() });
      }
    }
    else
    {
      auto message = "Expected '[' at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
      throw exception(message.c_str());
    }
    return tokens;
  }


  vector<pair<JsonToken, VALUE_TOKEN>> JsonLinter::Read(wistream     & is   )
  {
    auto tokens = vector<pair<JsonToken, VALUE_TOKEN>>();
    is >> ws;
    switch (is.peek())
    {
    case L'n':
    {
      tokens.push_back({ JsonToken::Null   , ParseNull(is)    });
      break;
    }
    case L'\"':
    {
      tokens.push_back({ JsonToken::String , ParseString(is)  });
      break;
    }
    case L't':
    case L'f':
    {
      tokens.push_back({ JsonToken::Boolean, ParseBoolean(is) });
      break;
    }
    default:
    {
      tokens.push_back({ JsonToken::Number   , ParseNumber(is)  });
      break;
    }
    case L'{':
    {
      for (auto& token : ParseObject(is))
      {
        tokens.push_back(token);
      }
      break;
    }
    case L'[':
    {
      for (auto& token : ParseArray(is))
      {
        tokens.push_back(token);
      }
      break;
    }
    }
    return tokens;
  }

  vector<pair<JsonToken, VALUE_TOKEN>> JsonLinter::Read(wstring const& value)
  {
    return Read(wstringstream(value));
  }
}