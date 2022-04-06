#include "stdafx.h"

#include "JsonPointer.h"
#include "Helper.h"
#include "Json.h"
#include "JsonObject.h"
#include "JsonArray.h"

#include <vector>
#include <string>
#include <utility>
#include <regex>

using namespace std;
using namespace Json4CPP::Detail;

namespace Json4CPP
{
  vector<wstring> JsonPointer::ExtractEncodedTokens(wstring const& path)
  {
    vector<wstring> result;
    if (path.empty())
    {
      return result;
    }
    if (path[0] != L'/')
    {
      auto message = WString2String(L"Path must be empty or start with '/'! It started with \""s + path.substr(0, path.find(L'/')) + L"\"!"s);
      throw exception(message.c_str());
    }
    auto start = path.find(L'/') + 1;
    auto end   = path.find(L'/', start);
    do
    {
      auto encodedToken = path.substr(start, end - start);
      for (auto pos = encodedToken.find(L'~'); pos != wstring::npos; pos = encodedToken.find(L'~', pos + 1))
      {
        // If '~' is the last char           OR there is no '0' or '1' after the '~'
        if (pos == encodedToken.length() - 1 || encodedToken[pos + 1] != L'0' && encodedToken[pos + 1] != L'1')
        {
          auto message = WString2String(L"Invalid reference token \""s + encodedToken + L"\"! After a '~', there must be either '0' or '1'!");
          throw exception(message.c_str());
        }
      }
      result.push_back(move(encodedToken));
      start = end + 1;
      end = path.find(L'/', start);
    } while (start != 0);
    return result;
  }

  wstring JsonPointer::DecodeToken(wstring encodedToken)
  {
    return regex_replace(regex_replace(encodedToken, wregex(L"~1"s), L"/"s), wregex(L"~0"s), L"~"s);
  }

  bool JsonPointer::PointsAfterTheLastArrayElement(wstring decodedToken)
  {
    return decodedToken.length() == 1 && decodedToken[0] == L'-';
  }

  bool JsonPointer::ArrayIndex(wstring decodedToken)
  {
    return decodedToken.length() == 1 && decodedToken[0] == L'0' || // It is either a single '0' or digits without a leading '0'
           decodedToken.length() >= 1 && decodedToken[0] != L'0' && all_of(decodedToken.begin(), decodedToken.end(), [](wchar_t c) { return iswdigit(c); });
  }

  JsonPointer::JsonPointer()
  {
    Path(L""s);
  }

  JsonPointer::JsonPointer(wchar_t const* path)
  {
    Path(path);
  }

  JsonPointer::JsonPointer(wstring const& path)
  {
    Path(path);
  }

  wstring JsonPointer::Path() const
  {
    return _path;
  }

  void JsonPointer::Path(wstring const& path)
  {
    _path = path;
    _encodedTokens = ExtractEncodedTokens(path);
  }

  JsonPointer JsonPointer::Parent() const
  {
    if (Empty())
    {
      return *this;
    }
    return JsonPointer(_path.substr(0, _path.find_last_of(L'/')));
  }

  wstring JsonPointer::Target() const
  {
    if (Empty())
    {
      return L""s;
    }
    return DecodeToken(_encodedTokens.back());
  }

  bool JsonPointer::Empty() const
  {
    return _path.empty();
  }

  Json& JsonPointer::Navigate(Json& json) const
  {
    return const_cast<Json&>(Navigate(as_const(json)));
  }

  Json const& JsonPointer::Navigate(Json const& json) const
  {
    if (!json.Is(JsonType::Complex))
    {
      throw exception("Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
    }
    if (Empty())
    {
      return json;
    }
    auto* result = &json;
    wstring path;
    for (auto const& encodedToken : _encodedTokens)
    {
      path = path + L"/" + encodedToken;
      auto decodedToken = DecodeToken(encodedToken);
      switch (result->Type())
      {
      case JsonType::Object:
        if (result->Count(decodedToken))
        {
          result = &result->At(decodedToken);
        }
        else
        {
          auto message = WString2String(L"Reference token \""s + encodedToken + L"\" at path \""s + path + L"\" not found!"s);
          throw exception(message.c_str());
        }
        break;
      case JsonType::Array:
        if (PointsAfterTheLastArrayElement(decodedToken))
        {
          auto message = WString2String(L"Reference token \""s + encodedToken + L"\" at path \""s + path + L"\" points to the member after the last array element which does not exist!"s);
          throw exception(message.c_str());
        }
        else if (ArrayIndex(decodedToken))
        {
          int64_t index;
          wstringstream(decodedToken) >> index;
          if (result->Size() <= index)
          {
            auto message = WString2String(L"Reference token \""s + encodedToken + L"\" at path \""s + path + L"\" is out of range!"s);
            throw exception(message.c_str());
          }
          result = &result->At(index);
        }
        else
        {
          auto message = WString2String(L"Reference token \""s + encodedToken + L"\" at path \""s + path + L"\" is not an array index!"s);
          throw exception(message.c_str());
        }
        break;
      default:
        throw exception("Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      }
    }
    return *result;
  }

  bool JsonPointer::Exists(Json const& json) const
  {
    try
    {
      Navigate(json);
      return true;
    }
    catch (...)
    {
      return false;
    }
  }

  wostream& operator<<(wostream& os, JsonPointer const& ptr)
  {
    return os << ptr._path;
  }

  wistream& operator>>(wistream& is, JsonPointer& ptr)
  {
    ptr = JsonPointer(wstring(istreambuf_iterator<wchar_t>(is), {}));
    return is;
  }

  bool operator==(JsonPointer const& left, JsonPointer const& right)
  {
    return left._path == right._path;
  }

  bool operator!=(JsonPointer const& left, JsonPointer const& right)
  {
    return !(left == right);
  }

  JsonPointer operator/ (JsonPointer const& left, JsonPointer const& right)
  {
    return JsonPointer(left._path + right._path);
  }

  JsonPointer operator/ (JsonPointer const& left, wstring && encodedToken)
  {
    return left / JsonPointer(L"/"s + encodedToken);
  }

  JsonPointer operator/ (JsonPointer const& left, int64_t arrayIndex)
  {
    return left / JsonPointer(L"/"s + to_wstring(arrayIndex));
  }

  JsonPointer& operator/=(JsonPointer& left, JsonPointer const& right)
  {
    left.Path(left._path + right._path);
    return left;
  }

  JsonPointer& operator/= (JsonPointer& left, wstring&& encodedToken)
  {
    return left /= JsonPointer(L"/"s + encodedToken);
  }

  JsonPointer& operator/= (JsonPointer& left, int64_t arrayIndex)
  {
    return left /= JsonPointer(L"/"s + to_wstring(arrayIndex));
  }

  JsonPointer operator""_JsonPointer(wchar_t const* value, size_t size)
  {
    return JsonPointer(value);
  }
}