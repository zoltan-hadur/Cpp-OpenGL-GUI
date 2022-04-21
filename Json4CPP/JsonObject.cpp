#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Helper.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"

using namespace std::string_literals;

namespace Json4CPP
{
  JsonObject JsonObject::Read(std::deque<Detail::TOKEN>& tokens)
  {
    if (tokens.empty())
    {
      auto message = Helper::WString2String(L"Expected token: "s + Json::Stringify(Detail::JsonTokenType::StartObject) + L"!"s);
      throw std::exception(message.c_str());
    }

    auto object = JsonObject();
    auto property = L""s;
    auto counter = 0;
    {
      auto& [token, value] = tokens.front();
      if (token == Detail::JsonTokenType::StartObject)
      {
        tokens.pop_front();
      }
      else
      {
        auto message = Helper::WString2String(L"Expected token: "s + Json::Stringify(Detail::JsonTokenType::StartObject) + L"!"s);
        throw std::exception(message.c_str());
      }
    }

    while (tokens.size())
    {
      auto& [token, value] = tokens.front();
      // Property then value then property then value etc...
      if (counter++ % 2 == 0 && token != Detail::JsonTokenType::PropertyName && token != Detail::JsonTokenType::EndObject)
      {
        auto message = Helper::WString2String(L"Expected token: "s + Json::Stringify(Detail::JsonTokenType::PropertyName) + L"!"s);
        throw std::exception(message.c_str());
      }
      switch (token)
      {
      using enum Detail::JsonTokenType;
      case PropertyName:
        property = std::move(std::get<std::wstring>(value));
        tokens.pop_front();
        if (object.Contains(property))
        {
          auto message = Helper::WString2String(L"Duplicated key: \""s + property + L"\"!"s);
          throw std::exception(message.c_str());
        }
        break;
      case Null        : object.Insert({ std::move(property),           std::get<std::nullptr_t>(value)  }); tokens.pop_front(); break;
      case String      : object.Insert({ std::move(property), std::move(std::get<std::wstring  >(value)) }); tokens.pop_front(); break;
      case Boolean     : object.Insert({ std::move(property),           std::get<bool          >(value)  }); tokens.pop_front(); break;
      case Real        : object.Insert({ std::move(property),           std::get<double        >(value)  }); tokens.pop_front(); break;
      case Integer     : object.Insert({ std::move(property),           std::get<int64_t       >(value)  }); tokens.pop_front(); break;
      case StartObject : object.Insert({ std::move(property), JsonObject::Read(tokens) }); break;
      case StartArray  : object.Insert({ std::move(property), JsonArray ::Read(tokens) }); break;
      case EndObject   : tokens.pop_front(); return object;
      default:
      {
        auto message = Helper::WString2String(L"Invalid token: "s + Json::Stringify(token) + L"!"s);
        throw std::exception(message.c_str());
      }
      }
    }
    auto message = Helper::WString2String(L"Expected token: "s + Json::Stringify(Detail::JsonTokenType::EndObject) + L"!"s);
    throw std::exception(message.c_str());
  }

  JsonObject JsonObject::Read(std::deque<Detail::TOKEN> && tokens)
  {
    return Read(tokens);
  }

  std::deque<Detail::TOKEN>& JsonObject::Write(JsonObject const& object, std::deque<Detail::TOKEN>& tokens)
  {
    tokens.push_back({ Detail::JsonTokenType::StartObject, L"{"s });
    for (auto& [key, value] : object)
    {
      tokens.push_back({ Detail::JsonTokenType::PropertyName, key });
      Json::Write(value, tokens);
    }
    tokens.push_back({ Detail::JsonTokenType::EndObject, L"}"s });
    return tokens;
  }

  std::deque<Detail::TOKEN> && JsonObject::Write(JsonObject const& object, std::deque<Detail::TOKEN> && tokens)
  {
    return std::move(Write(object, tokens));
  }

  JsonObject::JsonObject(Json const& json)
  {
    *this = json.operator const Json4CPP::JsonObject & ();
  }

  JsonObject::JsonObject(Json && json)
  {
    *this = std::move(json.operator Json4CPP::JsonObject && ());
  }

  JsonObject::JsonObject(Detail::JsonBuilder const& builder)
  {
    if (auto object = std::get_if<JsonObject>(&builder._value))
    {
      *this = *object;
    }
    else if (auto builders = std::get_if<std::vector<Detail::JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        if (builder.Is(Detail::JsonBuilderType::Pair))
        {
          auto& pair = std::get<std::vector<Detail::JsonBuilder>>(builder._value);
          auto key = std::get<std::wstring>(pair[0]._value);
          auto value = Json(pair[1]);
          if (Contains(key))
          {
            auto message = Helper::WString2String(L"Duplicated key: \""s + key + L"\"!"s);
            throw std::exception(message.c_str());
          }
          Insert({ std::move(key), std::move(value) });
        }
        else
        {
          Detail::VALUE value;
          std::visit(Helper::Overload{
            [&](auto const& arg) { value = arg; },
            [&](std::vector<Detail::JsonBuilder> const& arg) { value = JsonArray(arg); },
          }, builder._value);
          auto message = Helper::WString2String(L"JsonObject(JsonBuilder const& builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s +
                                                L" Error at: "s + Json::Stringify(Json(value)) + L"."s);
          throw std::exception(message.c_str());
        }
      }
    }
    else
    {
      auto message = Helper::WString2String(L"JsonObject(JsonBuilder const& builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw std::exception(message.c_str());
    }
  }

  JsonObject::JsonObject(Detail::JsonBuilder && builder)
  {
    if (auto object = std::get_if<JsonObject>(&builder._value))
    {
      *this = std::move(*object);
    }
    else if (auto builders = std::get_if<std::vector<Detail::JsonBuilder>>(&builder._value))
    {
      for (auto& builder : *builders)
      {
        if (builder.Is(Detail::JsonBuilderType::Pair))
        {
          auto& pair = std::get<std::vector<Detail::JsonBuilder>>(builder._value);
          auto& key = std::get<std::wstring>(pair[0]._value);
          auto value = Json(std::move(pair[1]));
          if (Contains(key))
          {
            auto message = Helper::WString2String(L"Duplicated key: \""s + key + L"\"!"s);
            throw std::exception(message.c_str());
          }
          Insert({ std::move(key), std::move(value) });
        }
        else
        {
          Detail::VALUE value;
          std::visit(Helper::Overload{
            [&](auto && arg) { value = std::move(arg); },
            [&](std::vector<Detail::JsonBuilder> && arg) { value = JsonArray(std::move(arg)); },
          }, move(builder._value));
          auto message = Helper::WString2String(L"JsonObject(JsonBuilder && builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s +
                                                L" Error at: "s + Json::Stringify(Json(value)) + L"."s);
          throw std::exception(message.c_str());
        }
      }
    }
    else
    {
      auto message = Helper::WString2String(L"JsonObject(JsonBuilder && builder) is not defined for type "s + Json::Stringify(builder.Type()) + L"!"s);
      throw std::exception(message.c_str());
    }
  }

  JsonObject::JsonObject(std::initializer_list<Detail::JsonBuilder> builders) : JsonObject(Detail::JsonBuilder(builders))
  {

  }

  std::wstring JsonObject::Dump(uint8_t indentSize, wchar_t indentChar) const
  {
    std::wstringstream os;
    Detail::JsonLinter::Write(os, JsonObject::Write(*this, std::deque<Detail::TOKEN>()), indentSize, indentChar);
    return os.str();
  }

  int64_t JsonObject::Size() const
  {
    return _pairs.size();
  }

  int64_t JsonObject::Count(std::wstring const& key) const
  {
    return _indexes.count(key);
  }

  bool JsonObject::Contains(std::wstring const& key) const
  {
    return _indexes.contains(key);
  }

  void JsonObject::Clear()
  {
    _pairs  .clear();
    _indexes.clear();
  }

  bool JsonObject::Insert(std::pair<std::wstring, Json> const& pair)
  {
    if (_indexes.contains(pair.first)) return false;
    _indexes[pair.first] = _pairs.size();
    _pairs.push_back(pair);
    return true;
  }

  bool JsonObject::Insert(std::pair<std::wstring, Json> && pair)
  {
    if (_indexes.contains(pair.first)) return false;
    _indexes[pair.first] = _pairs.size();
    _pairs.push_back(std::move(pair));
    return true;
  }

  void JsonObject::Erase(std::wstring const& key)
  {
    if (!_indexes.contains(key)) return;
    _pairs.erase(std::remove_if(_pairs.begin(), _pairs.end(), [&](std::pair<std::wstring, Json> const& pair) { return pair.first == key; }), _pairs.end());
    _indexes.clear();
    for (int i = 0; i < _pairs.size(); ++i)
    {
      _indexes[_pairs[i].first] = i;
    }
  }

  std::vector<std::wstring> JsonObject::Keys() const
  {
    std::vector<std::wstring> keys;
    std::transform(_pairs.begin(), _pairs.end(), std::back_inserter(keys), [](std::pair<std::wstring, Json> const& pair) { return pair.first; });
    return keys;
  }

  std::vector<std::reference_wrapper<const std::wstring>> JsonObject::KeysView() const
  {
    std::vector<std::reference_wrapper<const std::wstring>> keys;
    std::transform(_pairs.begin(), _pairs.end(), std::back_inserter(keys), [](std::pair<std::wstring, Json> const& pair) { return std::ref(pair.first); });
    return keys;
  }

  Json& JsonObject::operator[](std::wstring const& key)
  {
    if (!_indexes.contains(key)) Insert({ key, Json{} });
    return _pairs[_indexes[key]].second;
  }

  Json& JsonObject::At(std::wstring const& key)
  {
    return const_cast<Json&>(std::as_const(*this).At(key));
  }

  Json const& JsonObject::At(std::wstring const& key) const
  {
    return _pairs.at(_indexes.at(key)).second;
  }

  std::vector<std::pair<std::wstring, Json>>::iterator JsonObject::begin()
  {
    return _pairs.begin();
  }

  std::vector<std::pair<std::wstring, Json>>::iterator JsonObject::end()
  {
    return _pairs.end();
  }

  std::vector<std::pair<std::wstring, Json>>::const_iterator JsonObject::begin() const
  {
    return _pairs.begin();
  }

  std::vector<std::pair<std::wstring, Json>>::const_iterator JsonObject::end() const
  {
    return _pairs.end();
  }

  std::wostream& operator<<(std::wostream& os, JsonObject const& object)
  {
    auto isIndentSizeActive = JsonIndentSize::IsActive(os);
    auto isIndentCharActive = JsonIndentChar::IsActive(os);
    Detail::JsonLinter::Write(os,
                              JsonObject::Write(object, std::deque<Detail::TOKEN>()),
                              isIndentSizeActive ? JsonIndentSize::GetSize(os) : JsonDefault::IndentSize,
                              isIndentCharActive ? JsonIndentChar::GetChar(os) : JsonDefault::IndentChar);
    if (isIndentSizeActive) JsonIndentSize::ResetState(os);
    if (isIndentCharActive) JsonIndentChar::ResetState(os);
    return os;
  }

  std::wistream& operator>>(std::wistream& is, JsonObject& object)
  {
    object = JsonObject::Read(Detail::JsonLinter::Read(is));
    return is;
  }

  bool operator==(JsonObject const& left, JsonObject const& right)
  {
    if (left._pairs.size() != right._pairs.size())
    {
      return false;
    }
    else
    {
      for (auto& key : left.KeysView())
      {
        if (!right.Contains(key) || left.At(key) != right.At(key))
        {
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(JsonObject const& left, JsonObject const& right)
  {
    return !(left == right);
  }
}