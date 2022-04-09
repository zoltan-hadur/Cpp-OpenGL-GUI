#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"

using namespace std::string_literals;

namespace Json4CPP
{
  JsonArray JsonArray::Read(std::deque<Detail::TOKEN>& tokens)
  {
    if (tokens.empty())
    {
      auto message = Helper::WString2String(L"Expected token: "s + Json::Stringify(Detail::JsonTokenType::StartArray) + L"!"s);
      throw std::exception(message.c_str());
    }

    auto array = JsonArray();
    auto& [token, value] = tokens.front();
    if (token == Detail::JsonTokenType::StartArray)
    {
      tokens.pop_front();
    }
    else
    {
      auto message = Helper::WString2String(L"Expected token: "s + Json::Stringify(Detail::JsonTokenType::StartArray) + L"!"s);
      throw std::exception(message.c_str());
    }

    while (tokens.size())
    {
      tie(token, value) = tokens.front();
      switch (token)
      {
      using enum Detail::JsonTokenType;
      case Null       : array._values.push_back(          std::get<std::nullptr_t>(value)) ; tokens.pop_front(); break;
      case String     : array._values.push_back(std::move(std::get<std::wstring  >(value))); tokens.pop_front(); break;
      case Boolean    : array._values.push_back(          std::get<bool          >(value)) ; tokens.pop_front(); break;
      case Real       : array._values.push_back(          std::get<double        >(value)) ; tokens.pop_front(); break;
      case Integer    : array._values.push_back(          std::get<int64_t       >(value)) ; tokens.pop_front(); break;
      case StartObject: array._values.push_back(JsonObject::Read(tokens)); break;
      case StartArray : array._values.push_back(JsonArray ::Read(tokens)); break;
      case EndArray   : tokens.pop_front(); return array;
      default:
      {
        auto message = Helper::WString2String(L"Invalid token: "s + Json::Stringify(token) + L"!"s);
        throw std::exception(message.c_str());
      }
      }
    }
    auto message = Helper::WString2String(L"Expected token: "s + Json::Stringify(Detail::JsonTokenType::EndArray) + L"!"s);
    throw std::exception(message.c_str());
  }

  JsonArray JsonArray::Read(std::deque<Detail::TOKEN> && tokens)
  {
    return Read(tokens);
  }

  std::deque<Detail::TOKEN>& JsonArray::Write(JsonArray const& array, std::deque<Detail::TOKEN>& tokens)
  {
    tokens.push_back({ Detail::JsonTokenType::StartArray, L"["s });
    for (auto& value : array)
    {
      Json::Write(value, tokens);
    }
    tokens.push_back({ Detail::JsonTokenType::EndArray, L"]"s });
    return tokens;
  }

  std::deque<Detail::TOKEN> && JsonArray::Write(JsonArray const& array, std::deque<Detail::TOKEN> && tokens)
  {
    return std::move(Write(array, tokens));
  }

  JsonArray::JsonArray(Json const& json)
  {
    *this = json.operator const Json4CPP::JsonArray & ();
  }

  JsonArray::JsonArray(Json && json)
  {
    *this = std::move(json.operator Json4CPP::JsonArray && ());
  }

  JsonArray::JsonArray(Detail::JsonBuilder const& builder)
  {
    if (auto array = std::get_if<JsonArray>(&builder._value))
    {
      *this = *array;
    }
    else if (auto builders = std::get_if<std::vector<Detail::JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        _values.push_back(Json(builder));
      }
    }
    else
    {
      auto message = Helper::WString2String(L"JsonArray(JsonBuilder const& builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw std::exception(message.c_str());
    }
  }

  JsonArray::JsonArray(Detail::JsonBuilder && builder)
  {
    if (auto array = std::get_if<JsonArray>(&builder._value))
    {
      *this = std::move(*array);
    }
    else if (auto builders = std::get_if<std::vector<Detail::JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        _values.push_back(Json(std::move(builder)));
      }
    }
    else
    {
      auto message = Helper::WString2String(L"JsonArray(JsonBuilder && builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw std::exception(message.c_str());
    }
  }

  JsonArray::JsonArray(std::initializer_list<Detail::JsonBuilder> builders) : JsonArray(Detail::JsonBuilder(builders))
  {

  }

  std::wstring JsonArray::Dump(uint8_t indentSize, wchar_t indentChar) const
  {
    std::wstringstream os;
    Detail::JsonLinter::Write(os, JsonArray::Write(*this, std::deque<Detail::TOKEN>()), indentSize, indentChar);
    return os.str();
  }

  int64_t JsonArray::Size() const
  {
    return _values.size();
  }

  void JsonArray::Resize(int64_t size)
  {
    _values.resize(size);
  }

  void JsonArray::Resize(int64_t size, Json const& json)
  {
    _values.resize(size, json);
  }

  void JsonArray::Clear()
  {
    _values.clear();
  }

  void JsonArray::PushBack(Json const& value)
  {
    _values.push_back(value);
  }

  void JsonArray::PushBack(Json && value)
  {
    _values.push_back(std::move(value));
  }

  void JsonArray::Insert(int64_t index, Json const& value)
  {
    _values.insert(_values.begin() + index, value);
  }

  void JsonArray::Insert(int64_t index, Json && value)
  {
    _values.insert(_values.begin() + index, std::move(value));
  }
  
  void JsonArray::Erase(int64_t index)
  {
    _values.erase(_values.begin() + index);
  }

  Json& JsonArray::operator[](int64_t index)
  {
    return _values[index];
  }

  Json& JsonArray::At(int64_t index)
  {
    return const_cast<Json&>(std::as_const(*this).At(index));
  }

  Json const& JsonArray::At(int64_t index) const
  {
    return _values.at(index);
  }

  std::vector<Json>::iterator JsonArray::begin()
  {
    return _values.begin();
  }

  std::vector<Json>::iterator JsonArray::end()
  {
    return _values.end();
  }

  std::vector<Json>::const_iterator JsonArray::begin() const
  {
    return _values.begin();
  }

  std::vector<Json>::const_iterator JsonArray::end() const
  {
    return _values.end();
  }

  std::wostream& operator<<(std::wostream& os, JsonArray const& array)
  {
    auto isIndentSizeActive = JsonIndentSize::IsActive(os);
    auto isIndentCharActive = JsonIndentChar::IsActive(os);
    Detail::JsonLinter::Write(os,
                              JsonArray::Write(array, std::deque<Detail::TOKEN>()),
                              isIndentSizeActive ? JsonIndentSize::GetSize(os) : JsonDefault::IndentSize,
                              isIndentCharActive ? JsonIndentChar::GetChar(os) : JsonDefault::IndentChar);
    if (isIndentSizeActive) JsonIndentSize::ResetState(os);
    if (isIndentCharActive) JsonIndentChar::ResetState(os);
    return os;
  }

  std::wistream& operator>>(std::wistream& is, JsonArray& array)
  {
    array = JsonArray::Read(Detail::JsonLinter::Read(is));
    return is;
  }

  bool operator==(JsonArray const& left, JsonArray const& right)
  {
    return left._values == right._values;
  }

  bool operator!=(JsonArray const& left, JsonArray const& right)
  {
    return left._values != right._values;
  }
}