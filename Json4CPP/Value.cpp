#include "stdafx.h"

#include "Value.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Helper.h"

using namespace std::string_literals;

namespace Json4CPP::Detail::Value
{
#pragma warning(push)
#pragma warning(disable : 4804 4805)
  JsonType GetType(VALUE const& value)
  {
    JsonType result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = JsonType::Null;    },
      [&](std::wstring   const& v) { result = JsonType::String;  },
      [&](bool           const& v) { result = JsonType::Boolean; },
      [&](double         const& v) { result = JsonType::Real;    },
      [&](int64_t        const& v) { result = JsonType::Integer; },
      [&](JsonObject     const& v) { result = JsonType::Object;  },
      [&](JsonArray      const& v) { result = JsonType::Array;   },
      [&](auto           const& v) { result = JsonType::Invalid; }
    }, value);
    return result;
  }

  JsonBuilderType GetType(VALUE_BUILDER const& value)
  {
    JsonBuilderType result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = JsonBuilderType::Null;    },
      [&](std::wstring   const& v) { result = JsonBuilderType::String;  },
      [&](bool           const& v) { result = JsonBuilderType::Boolean; },
      [&](double         const& v) { result = JsonBuilderType::Real;    },
      [&](int64_t        const& v) { result = JsonBuilderType::Integer; },
      [&](JsonObject     const& v) { result = JsonBuilderType::Object;  },
      [&](JsonArray      const& v) { result = JsonBuilderType::Array;   },
      [&](std::vector<JsonBuilder> const& arg)
      {
        if (arg.size() == 0)
          result = JsonBuilderType::Empty;
        else if (arg.size() == 2 && arg[0].Type() == JsonBuilderType::String)
          result = JsonBuilderType::Pair;
        else if (std::all_of(arg.begin(), arg.end(), [](JsonBuilder const& value) { return value.Is(JsonBuilderType::Pair); }))
          result = JsonBuilderType::Object;
        else
          result = JsonBuilderType::Array;
      },
      [&](auto const& v) { result = JsonBuilderType::Invalid; }
    }, value);
    return result;
  }

  bool Equal(VALUE const& left, VALUE const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l == r; },
      [&](bool           const& l, bool           const& r) { result = l == r; },
      [&](bool           const& l, double         const& r) { result = l == r; },
      [&](bool           const& l, int64_t        const& r) { result = l == r; },
      [&](double         const& l, bool           const& r) { result = l == r; },
      [&](double         const& l, double         const& r) { result = l == r; },
      [&](double         const& l, int64_t        const& r) { result = l == r; },
      [&](int64_t        const& l, bool           const& r) { result = l == r; },
      [&](int64_t        const& l, double         const& r) { result = l == r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l == r; },
      [&](JsonObject     const& l, JsonObject     const& r) { result = l == r; },
      [&](JsonArray      const& l, JsonArray      const& r) { result = l == r; },
      [&](auto           const& l, auto           const& r) { result = false;  }
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
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = false; },
      [&](auto           const& l, std::nullptr_t const& r) { result = true;  },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = false; },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l < r; },
      [&](bool           const& l, bool           const& r) { result = l < r; },
      [&](bool           const& l, double         const& r) { result = l < r; },
      [&](bool           const& l, int64_t        const& r) { result = l < r; },
      [&](double         const& l, bool           const& r) { result = l < r; },
      [&](double         const& l, double         const& r) { result = l < r; },
      [&](double         const& l, int64_t        const& r) { result = l < r; },
      [&](int64_t        const& l, bool           const& r) { result = l < r; },
      [&](int64_t        const& l, double         const& r) { result = l < r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l < r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator< is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  bool LessThanOrEqual(VALUE const& left, VALUE const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = false;  },
      [&](auto           const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l <= r; },
      [&](bool           const& l, bool           const& r) { result = l <= r; },
      [&](bool           const& l, double         const& r) { result = l <= r; },
      [&](bool           const& l, int64_t        const& r) { result = l <= r; },
      [&](double         const& l, bool           const& r) { result = l <= r; },
      [&](double         const& l, double         const& r) { result = l <= r; },
      [&](double         const& l, int64_t        const& r) { result = l <= r; },
      [&](int64_t        const& l, bool           const& r) { result = l <= r; },
      [&](int64_t        const& l, double         const& r) { result = l <= r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l <= r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator<= is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  bool GreaterThan(VALUE const& left, VALUE const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = true;  },
      [&](auto           const& l, std::nullptr_t const& r) { result = false; },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = false; },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l > r; },
      [&](bool           const& l, bool           const& r) { result = l > r; },
      [&](bool           const& l, double         const& r) { result = l > r; },
      [&](bool           const& l, int64_t        const& r) { result = l > r; },
      [&](double         const& l, bool           const& r) { result = l > r; },
      [&](double         const& l, double         const& r) { result = l > r; },
      [&](double         const& l, int64_t        const& r) { result = l > r; },
      [&](int64_t        const& l, bool           const& r) { result = l > r; },
      [&](int64_t        const& l, double         const& r) { result = l > r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l > r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator> is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  bool GreaterThanOrEqual(VALUE const& left, VALUE const& right)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = true;   },
      [&](auto           const& l, std::nullptr_t const& r) { result = false;  },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = true;   },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l >= r; },
      [&](bool           const& l, bool           const& r) { result = l >= r; },
      [&](bool           const& l, double         const& r) { result = l >= r; },
      [&](bool           const& l, int64_t        const& r) { result = l >= r; },
      [&](double         const& l, bool           const& r) { result = l >= r; },
      [&](double         const& l, double         const& r) { result = l >= r; },
      [&](double         const& l, int64_t        const& r) { result = l >= r; },
      [&](int64_t        const& l, bool           const& r) { result = l >= r; },
      [&](int64_t        const& l, double         const& r) { result = l >= r; },
      [&](int64_t        const& l, int64_t        const& r) { result = l >= r; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator>= is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE Add(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                     },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::wstring   const& l, std::wstring   const& r) { result = l +                      r ; },
      [&](bool           const& l, bool           const& r) { result = l + static_cast<int64_t>(r); },
      [&](bool           const& l, double         const& r) { result = l +                      r ; },
      [&](bool           const& l, int64_t        const& r) { result = l +                      r ; },
      [&](double         const& l, bool           const& r) { result = l +                      r ; },
      [&](double         const& l, double         const& r) { result = l +                      r ; },
      [&](double         const& l, int64_t        const& r) { result = l +                      r ; },
      [&](int64_t        const& l, bool           const& r) { result = l +                      r ; },
      [&](int64_t        const& l, double         const& r) { result = l +                      r ; },
      [&](int64_t        const& l, int64_t        const& r) { result = l +                      r ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator+ is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& AddAssign(VALUE& left, VALUE const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                     },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::wstring   & l, std::wstring   const& r) { left = l +                      r ; },
      [&](bool           & l, bool           const& r) { left = l + static_cast<int64_t>(r); },
      [&](bool           & l, double         const& r) { left = l +                      r ; },
      [&](bool           & l, int64_t        const& r) { left = l +                      r ; },
      [&](double         & l, bool           const& r) { left = l +                      r ; },
      [&](double         & l, double         const& r) { left = l +                      r ; },
      [&](double         & l, int64_t        const& r) { left = l +                      r ; },
      [&](int64_t        & l, bool           const& r) { left = l +                      r ; },
      [&](int64_t        & l, double         const& r) { left = l +                      r ; },
      [&](int64_t        & l, int64_t        const& r) { left = l +                      r ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator+= is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Subtract(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                     },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](bool           const& l, bool           const& r) { result = l - static_cast<int64_t>(r); },
      [&](bool           const& l, double         const& r) { result = l -                      r ; },
      [&](bool           const& l, int64_t        const& r) { result = l -                      r ; },
      [&](double         const& l, bool           const& r) { result = l -                      r ; },
      [&](double         const& l, double         const& r) { result = l -                      r ; },
      [&](double         const& l, int64_t        const& r) { result = l -                      r ; },
      [&](int64_t        const& l, bool           const& r) { result = l -                      r ; },
      [&](int64_t        const& l, double         const& r) { result = l -                      r ; },
      [&](int64_t        const& l, int64_t        const& r) { result = l -                      r ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator- is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& SubtractAssign(VALUE& left, VALUE const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                     },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](bool           & l, bool           const& r) { left = l - static_cast<int64_t>(r); },
      [&](bool           & l, double         const& r) { left = l -                      r ; },
      [&](bool           & l, int64_t        const& r) { left = l -                      r ; },
      [&](double         & l, bool           const& r) { left = l -                      r ; },
      [&](double         & l, double         const& r) { left = l -                      r ; },
      [&](double         & l, int64_t        const& r) { left = l -                      r ; },
      [&](int64_t        & l, bool           const& r) { left = l -                      r ; },
      [&](int64_t        & l, double         const& r) { left = l -                      r ; },
      [&](int64_t        & l, int64_t        const& r) { left = l -                      r ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator-= is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Multiply(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                     },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                     },
      [&](bool           const& l, bool           const& r) { result = l * static_cast<int64_t>(r); },
      [&](bool           const& l, double         const& r) { result = l *                      r ; },
      [&](bool           const& l, int64_t        const& r) { result = l *                      r ; },
      [&](double         const& l, bool           const& r) { result = l *                      r ; },
      [&](double         const& l, double         const& r) { result = l *                      r ; },
      [&](double         const& l, int64_t        const& r) { result = l *                      r ; },
      [&](int64_t        const& l, bool           const& r) { result = l *                      r ; },
      [&](int64_t        const& l, double         const& r) { result = l *                      r ; },
      [&](int64_t        const& l, int64_t        const& r) { result = l *                      r ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator* is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& MultiplyAssign(VALUE& left, VALUE const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                     },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                     },
      [&](bool           & l, bool           const& r) { left = l * static_cast<int64_t>(r); },
      [&](bool           & l, double         const& r) { left = l *                      r ; },
      [&](bool           & l, int64_t        const& r) { left = l *                      r ; },
      [&](double         & l, bool           const& r) { left = l *                      r ; },
      [&](double         & l, double         const& r) { left = l *                      r ; },
      [&](double         & l, int64_t        const& r) { left = l *                      r ; },
      [&](int64_t        & l, bool           const& r) { left = l *                      r ; },
      [&](int64_t        & l, double         const& r) { left = l *                      r ; },
      [&](int64_t        & l, int64_t        const& r) { left = l *                      r ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator*= is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Divide(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;                                                           },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;                                                           },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;                                                           },
      [&](std::wstring   const& l, std::wstring   const& r) { result = std::wstring(std::filesystem::path(l) / std::filesystem::path(r)); },
      [&](bool           const& l, bool           const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](bool           const& l, double         const& r) { result =                                    l  /                       r  ; },
      [&](bool           const& l, int64_t        const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](double         const& l, bool           const& r) { result =                                    l  /                       r  ; },
      [&](double         const& l, double         const& r) { result =                                    l  /                       r  ; },
      [&](double         const& l, int64_t        const& r) { result =                                    l  /                       r  ; },
      [&](int64_t        const& l, bool           const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](int64_t        const& l, double         const& r) { result =                                    l  /                       r  ; },
      [&](int64_t        const& l, int64_t        const& r) { result =                                    l  / static_cast<double>  (r) ; },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator/ is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& DivideAssign(VALUE& left, VALUE const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;                                                           },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;                                                           },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;                                                           },
      [&](std::wstring   & l, std::wstring   const& r) { left = std::wstring(std::filesystem::path(l) / std::filesystem::path(r)); },
      [&](bool           & l, bool           const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](bool           & l, double         const& r) { left =                                    l  /                       r  ; },
      [&](bool           & l, int64_t        const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](double         & l, bool           const& r) { left =                                    l  /                       r  ; },
      [&](double         & l, double         const& r) { left =                                    l  /                       r  ; },
      [&](double         & l, int64_t        const& r) { left =                                    l  /                       r  ; },
      [&](int64_t        & l, bool           const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](int64_t        & l, double         const& r) { left =                                    l  /                       r  ; },
      [&](int64_t        & l, int64_t        const& r) { left =                                    l  / static_cast<double>  (r) ; },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator/= is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Modulo(VALUE const& left, VALUE const& right)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& l, auto           const& r) { result = nullptr;    },
      [&](auto           const& l, std::nullptr_t const& r) { result = nullptr;    },
      [&](std::nullptr_t const& l, std::nullptr_t const& r) { result = nullptr;    },
      [&](bool           const& l, bool           const& r) { result = fmod(l, r); },
      [&](bool           const& l, double         const& r) { result = fmod(l, r); },
      [&](bool           const& l, int64_t        const& r) { result = fmod(l, r); },
      [&](double         const& l, bool           const& r) { result = fmod(l, r); },
      [&](double         const& l, double         const& r) { result = fmod(l, r); },
      [&](double         const& l, int64_t        const& r) { result = fmod(l, r); },
      [&](int64_t        const& l, bool           const& r) { result = fmod(l, r); },
      [&](int64_t        const& l, double         const& r) { result = fmod(l, r); },
      [&](int64_t        const& l, int64_t        const& r) { result = fmod(l, r); },
      [&](auto           const& l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator% is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return result;
  }

  VALUE& ModuloAssign(VALUE& left, VALUE const& right)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & l, auto           const& r) { left = nullptr;    },
      [&](auto           & l, std::nullptr_t const& r) { left = nullptr;    },
      [&](std::nullptr_t & l, std::nullptr_t const& r) { left = nullptr;    },
      [&](bool           & l, bool           const& r) { left = fmod(l, r); },
      [&](bool           & l, double         const& r) { left = fmod(l, r); },
      [&](bool           & l, int64_t        const& r) { left = fmod(l, r); },
      [&](double         & l, bool           const& r) { left = fmod(l, r); },
      [&](double         & l, double         const& r) { left = fmod(l, r); },
      [&](double         & l, int64_t        const& r) { left = fmod(l, r); },
      [&](int64_t        & l, bool           const& r) { left = fmod(l, r); },
      [&](int64_t        & l, double         const& r) { left = fmod(l, r); },
      [&](int64_t        & l, int64_t        const& r) { left = fmod(l, r); },
      [&](auto           & l, auto           const& r)
      {
        auto message = Helper::WString2String(L"Operator%= is not defined for types \""s + Json::Stringify(GetType(left)) + L"\" and \""s + Json::Stringify(GetType(right)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, left, right);
    return left;
  }

  VALUE Negate(VALUE const& value)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = nullptr;                  },
      [&](bool           const& v) { result = -static_cast<int64_t>(v); },
      [&](double         const& v) { result = -v;                       },
      [&](int64_t        const& v) { result = -v;                       },
      [&](auto           const& v)
      {
        auto message = Helper::WString2String(L"Operator- is not defined for type \""s + Json::Stringify(GetType(value)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value);
    return result;
  }

  bool Not(VALUE const& value)
  {
    bool result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t const& v) { result = true; },
      [&](bool           const& v) { result = !v;   },
      [&](double         const& v) { result = !v;   },
      [&](int64_t        const& v) { result = !v;   },
      [&](auto           const& v)
      {
        auto message = Helper::WString2String(L"Operator! is not defined for type \""s + Json::Stringify(GetType(value)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value);
    return result;
  }

  VALUE& PreIncrement(VALUE& value)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { value = nullptr; },
      [&](double         & v) { value = ++v;     },
      [&](int64_t        & v) { value = ++v;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator++ (pre-increment) is not defined for type \""s + Json::Stringify(GetType(value)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value);
    return value;
  }

  VALUE PostIncrement(VALUE& value)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { result = nullptr; },
      [&](double         & v) { result = v++;     },
      [&](int64_t        & v) { result = v++;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator++ (post-increment) is not defined for type \""s + Json::Stringify(GetType(value)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value);
    return result;
  }

  VALUE& PreDecrement(VALUE& value)
  {
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { value = nullptr; },
      [&](double         & v) { value = --v;     },
      [&](int64_t        & v) { value = --v;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator-- (pre-decrement) is not defined for type \""s + Json::Stringify(GetType(value)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value);
    return value;
  }

  VALUE PostDecrement(VALUE& value)
  {
    VALUE result;
    std::visit(Helper::Overload{
      [&](std::nullptr_t & v) { result = nullptr; },
      [&](double         & v) { result = v--;     },
      [&](int64_t        & v) { result = v--;     },
      [&](auto           & v)
      {
        auto message = Helper::WString2String(L"Operator-- (post-decrement) is not defined for type \""s + Json::Stringify(GetType(value)) + L"\"!"s);
        throw std::exception(message.c_str());
      }
    }, value);
    return result;
  }

  bool LogicalAnd(VALUE const& left, VALUE const& right)
  {
    return static_cast<bool>(Json(left)) && static_cast<bool>(Json(right));
  }

  bool LogicalOr(VALUE const& left, VALUE const& right)
  {
    return static_cast<bool>(Json(left)) || static_cast<bool>(Json(right));
  }
#pragma warning(pop)
}