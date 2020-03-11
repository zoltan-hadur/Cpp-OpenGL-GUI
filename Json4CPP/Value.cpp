#include "stdafx.h"

#include "Value.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Helper.h"

using namespace std;
using namespace std::filesystem;

namespace Json4CPP::Detail::Value
{
#pragma warning(push)
#pragma warning(disable : 4804 4805)
  JsonType GetType(VALUE const& value)
  {
    JsonType result;
    visit(Overload{
      [&](nullptr_t  const& v) { result = JsonType::Null;    },
      [&](wstring    const& v) { result = JsonType::String;  },
      [&](bool       const& v) { result = JsonType::Boolean; },
      [&](double     const& v) { result = JsonType::Number;  },
      [&](JsonObject const& v) { result = JsonType::Object;  },
      [&](JsonArray  const& v) { result = JsonType::Array;   },
      [&](auto       const& v) { result = JsonType::Invalid; }
    }, value);
    return result;
  }

  JsonBuilderType GetType(VALUE_BUILDER const& value)
  {
    JsonBuilderType result;
    visit(Overload{
      [&](nullptr_t  const& v) { result = JsonBuilderType::Null;    },
      [&](wstring    const& v) { result = JsonBuilderType::String;  },
      [&](bool       const& v) { result = JsonBuilderType::Boolean; },
      [&](double     const& v) { result = JsonBuilderType::Number;  },
      [&](JsonObject const& v) { result = JsonBuilderType::Object;  },
      [&](JsonArray  const& v) { result = JsonBuilderType::Array;   },
      [&](vector<JsonBuilder> const& arg)
      {
        if (arg.size() == 0)
          result = JsonBuilderType::Empty;
        else if (arg.size() == 2 && arg[0].Type() == JsonBuilderType::String)
          result = JsonBuilderType::Pair;
        else if (all_of(arg.begin(), arg.end(), [](JsonBuilder const& value) { return value.Is(JsonBuilderType::Pair); }))
          result = JsonBuilderType::Object;
        else
          result = JsonBuilderType::Array;
      },
      [&](auto       const& v) { result = JsonBuilderType::Invalid; }
    }, value);
    return result;
  }

  nullptr_t ParseNull(wistream& is)
  {
    is >> ws;
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

  nullptr_t ParseNull(wstring const& value)
  {
    return ParseNull(wstringstream(value));
  }

  wstring ParseString(wistream& is)
  {
    is >> ws;
    auto text = L""s;
    // A string must start with a quote
    if (is.peek() == L'\"')
    {
      is.get();
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
          if (is.peek() == L'\\')
          {
            is.get();
            auto c = is.peek();
            switch (c)
            {
            case L'b' : text.push_back(L'\b'); is.get(); break;
            case L'f' : text.push_back(L'\f'); is.get(); break;
            case L'n' : text.push_back(L'\n'); is.get(); break;
            case L'r' : text.push_back(L'\r'); is.get(); break;
            case L't' : text.push_back(L'\t'); is.get(); break;
            case L'\"':
            case L'\\':
            case L'/' :
            {
              text.push_back(is.get());
              break;
            }
            case L'u' :
            {
              is.get();
              auto hexCode = L""s;
              for (int i = 0; i < 4; ++i)
              {
                if (iswxdigit(is.peek()))
                {
                  hexCode.push_back(is.get());
                }
                else
                {
                  auto message = "Expected a hexadecimal digit at position " + to_string(is.tellg());
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
              auto message = "Expected one of the following characters: \", \\, /, b, f, n, r, t, u at position " + to_string(is.tellg());
              throw exception(message.c_str());
              break;
            }
            }
          }
          else if (!iswcntrl(is.peek()))
          {
            text.push_back(is.get());
          }
          else
          {
            auto message = "Invalid character found at position " + to_string(is.tellg());
            throw exception(message.c_str());
          }
        }
        if (is.eof())
        {
          auto message = "Expected '\"' at position " + to_string(is.tellg());
          throw exception(message.c_str());
        }
        else
        {
          is.get();
        }
      }
    }
    else
    {
      auto message = "Expected '\"' at position " + to_string(is.tellg());
      throw exception(message.c_str());
    }
    return text;
  }

  wstring ParseString(wstring const& value)
  {
    return ParseString(wstringstream(value));
  }

  bool ParseBoolean(wistream& is)
  {
    is >> ws;
    wstring expected = is.peek() == L't' ? L"true" : is.peek() == L'f' ? L"false" : L"";
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
      return expected == L"true";
    }
    auto message = "Expected \"true\" or \"false\" at position " + GetFormattedStreamPositionA(is, is.tellg()) + "!";
    throw exception(message.c_str());
  }

  bool ParseBoolean(wstring const& value)
  {
    return ParseBoolean(wstringstream(value));
  }

  double ParseNumber(wistream& is)
  {
    is >> ws;
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
      auto message = "Expected '-' or digit at position " + to_string(is.tellg());
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
        auto message = "Expected digit at position " + to_string(is.tellg());
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
        auto message = "Expected digit at position " + to_string(is.tellg());
        throw exception(message.c_str());
      }
    }

    double number;
    wstringstream(text) >> number;
    return number;
  }

  double ParseNumber(wstring const& value)
  {
    return ParseNumber(wstringstream(value));
  }

  JsonObject ParseJsonObject(wistream& is)
  {
    JsonObject object;
    is >> object;
    return object;
  }

  JsonObject ParseJsonObject(wstring const& value)
  {
    return ParseJsonObject(wstringstream(value));
  }

  JsonArray ParseJsonArray(wistream& is)
  {
    JsonArray array;
    is >> array;
    return array;
  }

  JsonArray ParseJsonArray(wstring const& value)
  {
    return ParseJsonArray(wstringstream(value));
  }

  Json ParseJson(wistream& is)
  {
    Json json;
    is >> json;
    return json;
  }

  Json ParseJson(wstring const& value)
  {
    return ParseJson(wstringstream(value));
  }

  wostream& Write(wostream& os, VALUE const& value)
  {
    visit(Overload{
      [&](nullptr_t  const& v) { os << L"null";                           },
      [&](wstring    const& v) { os << L"\"" << EscapeString(v) << L"\""; },
      [&](bool       const& v) { os << (v ? L"true" : L"false");          },
      [&](double     const& v) { os << +v;                                },
      [&](JsonObject const& v) { os << v;                                 },
      [&](JsonArray  const& v) { os << v;                                 }
    }, value);
    return os;
  }

  wistream& Read(wistream& is, VALUE& value)
  {
    is >> ws;
    switch (is.peek())
    {
    case L'n':            value = ParseNull(is); break;
    case L'\"':           value = ParseString(is); break;
    case L't': case L'f': value = ParseBoolean(is); break;
    default:              value = ParseNumber(is); break;
    case L'{':            value = ParseJsonObject (is); break;
    case L'[':            value = ParseJsonArray  (is); break;
    }
    return is;
  }

  bool Equal(VALUE const& left, VALUE const& right)
  {
    bool result;
    visit(Overload{
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = true;   },
      [&](wstring    const& l, wstring    const& r) { result = l == r; },
      [&](bool       const& l, bool       const& r) { result = l == r; },
      [&](bool       const& l, double     const& r) { result = l == r; },
      [&](double     const& l, bool       const& r) { result = l == r; },
      [&](double     const& l, double     const& r) { result = l == r; },
      [&](JsonObject const& l, JsonObject const& r) { result = l == r; },
      [&](JsonArray  const& l, JsonArray  const& r) { result = l == r; },
      [&](auto       const& l, auto       const& r) { result = false;  }
    }, left, right);
    return result;
  }

  bool NotEqual(VALUE const& left, VALUE const& right)
  {
    return !Equal(left, right);
  }

  bool LessThan(VALUE const& left, VALUE const& right)
  {
    bool result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = false; },
      [&](auto       const& l, nullptr_t  const& r) { result = true;  },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = false; },
      [&](wstring    const& l, wstring    const& r) { result = l < r; },
      [&](bool       const& l, bool       const& r) { result = l < r; },
      [&](bool       const& l, double     const& r) { result = l < r; },
      [&](double     const& l, bool       const& r) { result = l < r; },
      [&](double     const& l, double     const& r) { result = l < r; },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator< is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  bool LessThanOrEqual(VALUE const& left, VALUE const& right)
  {
    bool result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = false;  },
      [&](auto       const& l, nullptr_t  const& r) { result = true;   },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = true;   },
      [&](wstring    const& l, wstring    const& r) { result = l <= r; },
      [&](bool       const& l, bool       const& r) { result = l <= r; },
      [&](bool       const& l, double     const& r) { result = l <= r; },
      [&](double     const& l, bool       const& r) { result = l <= r; },
      [&](double     const& l, double     const& r) { result = l <= r; },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator<= is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  bool GreaterThan(VALUE const& left, VALUE const& right)
  {
    bool result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = true;  },
      [&](auto       const& l, nullptr_t  const& r) { result = false; },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = false; },
      [&](wstring    const& l, wstring    const& r) { result = l > r; },
      [&](bool       const& l, bool       const& r) { result = l > r; },
      [&](bool       const& l, double     const& r) { result = l > r; },
      [&](double     const& l, bool       const& r) { result = l > r; },
      [&](double     const& l, double     const& r) { result = l > r; },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator> is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  bool GreaterThanOrEqual(VALUE const& left, VALUE const& right)
  {
    bool result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = true;   },
      [&](auto       const& l, nullptr_t  const& r) { result = false;  },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = true;   },
      [&](wstring    const& l, wstring    const& r) { result = l >= r; },
      [&](bool       const& l, bool       const& r) { result = l >= r; },
      [&](bool       const& l, double     const& r) { result = l >= r; },
      [&](double     const& l, bool       const& r) { result = l >= r; },
      [&](double     const& l, double     const& r) { result = l >= r; },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator>= is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE Add(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = nullptr;               },
      [&](auto       const& l, nullptr_t  const& r) { result = nullptr;               },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = nullptr;               },
      [&](wstring    const& l, wstring    const& r) { result = l + r;                 },
      [&](bool       const& l, bool       const& r) { result = double(l) + double(r); },
      [&](bool       const& l, double     const& r) { result = l + r;                 },
      [&](double     const& l, bool       const& r) { result = l + r;                 },
      [&](double     const& l, double     const& r) { result = l + r;                 },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator+ is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& AddAssign(VALUE& left, VALUE const& right)
  {
    visit(Overload{
      [&](nullptr_t  & l, auto       const& r) { left = nullptr;               },
      [&](auto       & l, nullptr_t  const& r) { left = nullptr;               },
      [&](nullptr_t  & l, nullptr_t  const& r) { left = nullptr;               },
      [&](wstring    & l, wstring    const& r) { left = l + r;                 },
      [&](bool       & l, bool       const& r) { left = double(l) + double(r); },
      [&](bool       & l, double     const& r) { left = l + r;                 },
      [&](double     & l, bool       const& r) { left = l + r;                 },
      [&](double     & l, double     const& r) { left = l + r;                 },
      [&](auto       & l, auto       const& r)
      {
        auto message = "Operator+= is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Subtract(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = nullptr;               },
      [&](auto       const& l, nullptr_t  const& r) { result = nullptr;               },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = nullptr;               },
      [&](bool       const& l, bool       const& r) { result = double(l) - double(r); },
      [&](bool       const& l, double     const& r) { result = l - r;                 },
      [&](double     const& l, bool       const& r) { result = l - r;                 },
      [&](double     const& l, double     const& r) { result = l - r;                 },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator- is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& SubtractAssign(VALUE& left, VALUE const& right)
  {
    visit(Overload{
      [&](nullptr_t  & l, auto       const& r) { left = nullptr;               },
      [&](auto       & l, nullptr_t  const& r) { left = nullptr;               },
      [&](nullptr_t  & l, nullptr_t  const& r) { left = nullptr;               },
      [&](bool       & l, bool       const& r) { left = double(l) - double(r); },
      [&](bool       & l, double     const& r) { left = l - r;                 },
      [&](double     & l, bool       const& r) { left = l - r;                 },
      [&](double     & l, double     const& r) { left = l - r;                 },
      [&](auto       & l, auto       const& r)
      {
        auto message = "Operator-= is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Multiply(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = nullptr;               },
      [&](auto       const& l, nullptr_t  const& r) { result = nullptr;               },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = nullptr;               },
      [&](bool       const& l, bool       const& r) { result = double(l) * double(r); },
      [&](bool       const& l, double     const& r) { result = l * r;                 },
      [&](double     const& l, bool       const& r) { result = l * r;                 },
      [&](double     const& l, double     const& r) { result = l * r;                 },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator* is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& MultiplyAssign(VALUE& left, VALUE const& right)
  {
    visit(Overload{
      [&](nullptr_t  & l, auto       const& r) { left = nullptr;               },
      [&](auto       & l, nullptr_t  const& r) { left = nullptr;               },
      [&](nullptr_t  & l, nullptr_t  const& r) { left = nullptr;               },
      [&](bool       & l, bool       const& r) { left = double(l) * double(r); },
      [&](bool       & l, double     const& r) { left = l * r;                 },
      [&](double     & l, bool       const& r) { left = l * r;                 },
      [&](double     & l, double     const& r) { left = l * r;                 },
      [&](auto       & l, auto       const& r)
      {
        auto message = "Operator*= is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Divide(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = nullptr;                    },
      [&](auto       const& l, nullptr_t  const& r) { result = nullptr;                    },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = nullptr;                    },
      [&](wstring    const& l, wstring    const& r) { result = wstring(path(l) / path(r)); },
      [&](bool       const& l, bool       const& r) { result = double(l) / double(r);      },
      [&](bool       const& l, double     const& r) { result = l / r;                      },
      [&](double     const& l, bool       const& r) { result = l / r;                      },
      [&](double     const& l, double     const& r) { result = l / r;                      },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator/ is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& DivideAssign(VALUE& left, VALUE const& right)
  {
    visit(Overload{
      [&](nullptr_t  & l, auto       const& r) { left = nullptr;                    },
      [&](auto       & l, nullptr_t  const& r) { left = nullptr;                    },
      [&](nullptr_t  & l, nullptr_t  const& r) { left = nullptr;                    },
      [&](wstring    & l, wstring    const& r) { left = wstring(path(l) / path(r)); },
      [&](bool       & l, bool       const& r) { left = double(l) / double(r);      },
      [&](bool       & l, double     const& r) { left = l / r;                      },
      [&](double     & l, bool       const& r) { left = l / r;                      },
      [&](double     & l, double     const& r) { left = l / r;                      },
      [&](auto       & l, auto       const& r)
      {
        auto message = "Operator/= is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Modulo(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  const& l, auto       const& r) { result = nullptr;    },
      [&](auto       const& l, nullptr_t  const& r) { result = nullptr;    },
      [&](nullptr_t  const& l, nullptr_t  const& r) { result = nullptr;    },
      [&](bool       const& l, bool       const& r) { result = fmod(l, r); },
      [&](bool       const& l, double     const& r) { result = fmod(l, r); },
      [&](double     const& l, bool       const& r) { result = fmod(l, r); },
      [&](double     const& l, double     const& r) { result = fmod(l, r); },
      [&](auto       const& l, auto       const& r)
      {
        auto message = "Operator% is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& ModuloAssign(VALUE& left, VALUE const& right)
  {
    visit(Overload{
      [&](nullptr_t  & l, auto       const& r) { left = nullptr;    },
      [&](auto       & l, nullptr_t  const& r) { left = nullptr;    },
      [&](nullptr_t  & l, nullptr_t  const& r) { left = nullptr;    },
      [&](bool       & l, bool       const& r) { left = fmod(l, r); },
      [&](bool       & l, double     const& r) { left = fmod(l, r); },
      [&](double     & l, bool       const& r) { left = fmod(l, r); },
      [&](double     & l, double     const& r) { left = fmod(l, r); },
      [&](auto       & l, auto       const& r)
      {
        auto message = "Operator%= is not defined for types " + string(typeid(l).name()) + " and " + string(typeid(r).name()) + "!";
        throw exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Negate(VALUE const& value)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  const& v) { result = nullptr;    },
      [&](bool       const& v) { result = -double(v); },
      [&](double     const& v) { result = -v;         },
      [&](auto       const& v)
      {
        auto message = "Operator- is not defined for type " + string(typeid(v).name()) + "!";
        throw exception(message.c_str());
      }
    }, value);
    return result;
  }

  bool Not(VALUE const& value)
  {
    bool result;
    visit(Overload{
      [&](nullptr_t  const& v) { result = true; },
      [&](bool       const& v) { result = !v;   },
      [&](double     const& v) { result = !v;   },
      [&](auto       const& v)
      {
        auto message = "Operator! is not defined for type " + string(typeid(v).name()) + "!";
        throw exception(message.c_str());
      }
    }, value);
    return result;
  }

  VALUE& PreIncrement(VALUE& value)
  {
    visit(Overload{
      [&](nullptr_t  & v) { value = nullptr; },
      [&](double     & v) { value = ++v;     },
      [&](auto       & v)
      {
        auto message = "Operator++ (pre-increment) is not defined for type " + string(typeid(v).name()) + "!";
        throw exception(message.c_str());
      }
    }, value);
    return value;
  }

  VALUE PostIncrement(VALUE& value)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  & v) { result = nullptr; },
      [&](double     & v) { result = v++;     },
      [&](auto       & v)
      {
        auto message = "Operator++ (post-increment) is not defined for type " + string(typeid(v).name()) + "!";
        throw exception(message.c_str());
      }
    }, value);
    return result;
  }

  VALUE& PreDecrement(VALUE& value)
  {
    visit(Overload{
      [&](nullptr_t  & v) { value = nullptr; },
      [&](double     & v) { value = --v;     },
      [&](auto       & v)
      {
        auto message = "Operator-- (pre-decrement) is not defined for type " + string(typeid(v).name()) + "!";
        throw exception(message.c_str());
      }
    }, value);
    return value;
  }

  VALUE PostDecrement(VALUE& value)
  {
    VALUE result;
    visit(Overload{
      [&](nullptr_t  & v) { result = nullptr; },
      [&](double     & v) { result = v--;     },
      [&](auto       & v)
      {
        auto message = "Operator-- (post-decrement) is not defined for type " + string(typeid(v).name()) + "!";
        throw exception(message.c_str());
      }
    }, value);
    return result;
  }

  bool LogicalAnd(VALUE const& left, VALUE const& right)
  {
    return (bool)Json(left) && (bool)Json(right);
  }

  bool LogicalOr(VALUE const& left, VALUE const& right)
  {
    return (bool)Json(left) || (bool)Json(right);
  }
#pragma warning(pop)
}