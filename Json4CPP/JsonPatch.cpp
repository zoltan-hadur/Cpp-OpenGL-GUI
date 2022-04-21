#include "stdafx.h"

#include "JsonPatch.h"
#include "Helper.h"
#include "Json.hpp"

#include <iostream>
#include <string>

using namespace std::string_literals;

namespace Json4CPP
{
  JsonPatch::OperationBase::OperationBase(JsonObject const& object)
  {
    if (!object.Contains(L"path"s))
    {
      auto message = Helper::WString2String(L"Key 'path' is missing from object: "s + Json::Stringify(object) + L"!"s);
      throw std::exception(message.c_str());
    }
    auto& path = object.At(L"path"s);
    if (!path.Is(JsonType::String))
    {
      auto message = Helper::WString2String(L"Path: "s + Json::Stringify(path) + L" is not a string!"s);
      throw std::exception(message.c_str());
    }
    _path = JsonPointer(path.Get<std::wstring>());
  }

  JsonPatch::OperationAdd::OperationAdd(JsonObject const& object) : OperationBase(object)
  {
    if (!object.Contains(L"value"s))
    {
      auto message = Helper::WString2String(L"Key 'value' is missing from object: "s + Json::Stringify(object) + L"!"s);
      throw std::exception(message.c_str());
    }
    _value = object.At(L"value"s);
  }

  JsonPatch::OperationAdd::operator JsonObject() const
  {
    return JsonObject{ { L"op"s, L"add"s }, { L"path"s, _path.Path() }, { L"value"s, _value } };
  }

  void JsonPatch::OperationAdd::Execute(Json& json) const
  {
    // The root of the target document - whereupon the specified value
    // becomes the entire content of the target document.
    if (_path.Empty())
    {
      json = _value;
      return;
    }

    auto parent = _path.Parent();
    if (!parent.Exists(json))
    {
      auto message = Helper::WString2String(L"Parent: \""s + parent.Path() + L"\" does not exist!"s);
      throw std::exception(message.c_str());
    }

    auto& parentElement = json.At(parent);
    auto target = _path.Target();
    if (parentElement.Is(JsonType::Array))
    {
      if (JsonPointer::PointsAfterTheLastArrayElement(target))
      {
        parentElement.PushBack(_value);
      }
      else if (JsonPointer::ArrayIndex(target))
      {
        int64_t index;
        std::wstringstream(target) >> index;
        if (parentElement.Size() < index)
        {
          auto message = Helper::WString2String(L"Target location: \""s + _path.Path() + L"\" is out of range!"s);
          throw std::exception(message.c_str());
        }
        parentElement.Insert(index, _value);
      }
      else
      {
        auto message = Helper::WString2String(L"Target location's parent: \""s + parent.Path() + L"\" is an array, thus target : \""s + target + L"\" must be either an index or '-'!"s);
        throw std::exception(message.c_str());
      }
    }
    else
    {
      parentElement[target] = _value; // If it does not exist, it will be added, otherwise it will be overwritten
    }
  }

  JsonPatch::OperationRemove::OperationRemove(JsonObject const& object) : OperationBase(object)
  {
    
  }

  JsonPatch::OperationRemove::operator JsonObject() const
  {
    return JsonObject{ { L"op"s, L"remove"s }, { L"path"s, _path.Path() } };
  }

  void JsonPatch::OperationRemove::Execute(Json& json) const
  {
    if (_path.Empty() || !_path.Exists(json))
    {
      auto message = Helper::WString2String(L"Target location: \""s + _path.Path() + L"\" does not exist!"s);
      throw std::exception(message.c_str());
    }

    auto& parentElement = json.At(_path.Parent());
    auto target = _path.Target();
    if (parentElement.Is(JsonType::Array) && JsonPointer::ArrayIndex(target))
    {
      int64_t index;
      std::wstringstream(target) >> index;
      parentElement.Erase(index);
    }
    else // It is not an array index, thus it must be a key
    {
      parentElement.Erase(target);
    }
  }

  JsonPatch::OperationReplace::OperationReplace(JsonObject const& object) : OperationBase(object)
  {
    if (!object.Contains(L"value"s))
    {
      auto message = Helper::WString2String(L"Key 'value' is missing from object: "s + Json::Stringify(object) + L"!"s);
      throw std::exception(message.c_str());
    }
    _value = object.At(L"value"s);
  }

  JsonPatch::OperationReplace::operator JsonObject() const
  {
    return JsonObject{ { L"op"s, L"replace"s }, { L"path"s, _path.Path() }, { L"value"s, _value } };
  }

  void JsonPatch::OperationReplace::Execute(Json& json) const
  {
    if (_path.Empty() || !_path.Exists(json))
    {
      auto message = Helper::WString2String(L"Target location: \""s + _path.Path() + L"\" does not exist!"s);
      throw std::exception(message.c_str());
    }

    json[_path] = _value;
  }

  JsonPatch::OperationMove::OperationMove(JsonObject const& object) : OperationBase(object)
  {
    if (!object.Contains(L"from"s))
    {
      auto message = Helper::WString2String(L"Key 'from' is missing from object: "s + Json::Stringify(object) + L"!"s);
      throw std::exception(message.c_str());
    }
    auto& from = object.At(L"from"s);
    if (!from.Is(JsonType::String))
    {
      auto message = Helper::WString2String(L"From: "s + Json::Stringify(from) + L" is not a string!"s);
      throw std::exception(message.c_str());
    }
    _from = JsonPointer(from.Get<std::wstring>());
    if (_path.Parent().Path().starts_with(_from.Path()))
    {
      auto message = Helper::WString2String(L"Location 'from': \"" + _from.Path() + L"\" is a prefix of location 'path': \""s + _path.Path() + L"\"! A location cannot be moved into one of its children."s);
      throw std::exception(message.c_str());
    }
  }

  JsonPatch::OperationMove::operator JsonObject() const
  {
    return JsonObject{ { L"op"s, L"move"s }, { L"from"s, _from.Path() }, { L"path"s, _path.Path() } };
  }

  void JsonPatch::OperationMove::Execute(Json& json) const
  {
    if (!_from.Exists(json))
    {
      auto message = Helper::WString2String(L"From location: \""s + _from.Path() + L"\" does not exist!"s);
      throw std::exception(message.c_str());
    }

    auto value = json.At(_from);
    auto remove = OperationRemove(JsonObject{ { L"path"s, _from.Path() }                      });
    auto add =    OperationAdd   (JsonObject{ { L"path"s, _path.Path() }, { L"value", value } });
    remove.Execute(json);
    add   .Execute(json);
  }

  JsonPatch::OperationCopy::OperationCopy(JsonObject const& object) : OperationBase(object)
  {
    if (!object.Contains(L"from"s))
    {
      auto message = Helper::WString2String(L"Key 'from' is missing from object: "s + Json::Stringify(object) + L"!"s);
      throw std::exception(message.c_str());
    }
    auto& from = object.At(L"from"s);
    if (!from.Is(JsonType::String))
    {
      auto message = Helper::WString2String(L"From: "s + Json::Stringify(from) + L" is not a string!"s);
      throw std::exception(message.c_str());
    }
    _from = JsonPointer(from.Get<std::wstring>());
  }

  JsonPatch::OperationCopy::operator JsonObject() const
  {
    return JsonObject{ { L"op"s, L"copy"s }, { L"from"s, _from.Path() }, { L"path"s, _path.Path() } };
  }

  void JsonPatch::OperationCopy::Execute(Json& json) const
  {
    if (!_from.Exists(json))
    {
      auto message = Helper::WString2String(L"From location: \""s + _from.Path() + L"\" does not exist!"s);
      throw std::exception(message.c_str());
    }

    auto value = json.At(_from);
    auto add = OperationAdd(JsonObject{ { L"path"s, _path.Path() }, { L"value", value } });
    add.Execute(json);
  }

  JsonPatch::OperationTest::OperationTest(JsonObject const& object) : OperationBase(object)
  {
    if (!object.Contains(L"value"s))
    {
      auto message = Helper::WString2String(L"Key 'value' is missing from object: "s + Json::Stringify(object) + L"!"s);
      throw std::exception(message.c_str());
    }
    _value = object.At(L"value"s);
  }

  JsonPatch::OperationTest::operator JsonObject() const
  {
    return JsonObject{ { L"op"s, L"test"s }, { L"path"s, _path.Path() }, { L"value"s, _value } };
  }

  void JsonPatch::OperationTest::Execute(Json& json) const
  {
    if (!_path.Exists(json))
    {
      auto message = Helper::WString2String(L"Target location: \""s + _path.Path() + L"\" does not exist!"s);
      throw std::exception(message.c_str());
    }
    else if (json.At(_path) != _value)
    {
      auto message = Helper::WString2String(L"Target location value: "s + Json::Stringify(json.At(_path)) + L" is not equal to " + Json::Stringify(_value) + L"!"s);
      throw std::exception(message.c_str());
    }
  }

  JsonPatch::JsonPatch()
  {

  }

  JsonPatch::JsonPatch(JsonArray const& array)
  {
    for (auto& element : array)
    {
      if (!element.Is(JsonType::Object))
      {
        auto message = Helper::WString2String(L"Value: "s + Json::Stringify(element) + L" is not an object!"s);
        throw std::exception(message.c_str());
      }
      if (!element.Contains(L"op"s))
      {
        auto message = Helper::WString2String(L"Key 'op' is missing from object: "s + Json::Stringify(element) + L"!"s);
        throw std::exception(message.c_str());
      }
      auto op = element.At(L"op"s).Get<std::wstring>();
      static auto factory = std::map<std::wstring, std::function<std::shared_ptr<OperationBase>(JsonObject const&)>>
      {
        { L"add"    , [](JsonObject const& element) { return std::make_shared<OperationAdd    >(element); } },
        { L"remove" , [](JsonObject const& element) { return std::make_shared<OperationRemove >(element); } },
        { L"replace", [](JsonObject const& element) { return std::make_shared<OperationReplace>(element); } },
        { L"move"   , [](JsonObject const& element) { return std::make_shared<OperationMove   >(element); } },
        { L"copy"   , [](JsonObject const& element) { return std::make_shared<OperationCopy   >(element); } },
        { L"test"   , [](JsonObject const& element) { return std::make_shared<OperationTest   >(element); } }
      };
      if (!factory.contains(op))
      {
        auto message = Helper::WString2String(L"Operation \""s + op + L"\" is not a valid operation! It must be one of \"add\", \"remove\", \"replace\", \"move\", \"copy\", or \"test\"."s);
        throw std::exception(message.c_str());
      }
      _operations.push_back(factory[op](element.Get<JsonObject>()));
    }
  }

  Json JsonPatch::Apply(Json const& json) const
  {
    auto result = json;
    for (auto& operation : _operations)
    {
      try
      {
        operation->Execute(result);
      }
      catch (std::exception e)
      {
        auto message = Helper::WString2String(L"Operation: "s + Json::Stringify((JsonObject)*operation) + L" failed! "s) + e.what();
        throw std::exception(message.c_str());
      }
    }
    return result;
  }

  JsonPatch::operator JsonArray() const
  {
    JsonArray array;
    for (auto& operation : _operations)
    {
      array.PushBack((JsonObject)*operation);
    }
    return array;
  }

  std::wostream& operator<<(std::wostream& os, JsonPatch const& patch)
  {
    return os << (JsonArray)patch;
  }

  std::wistream& operator>>(std::wistream& is, JsonPatch& patch)
  {
    JsonArray array;
    is >> array;
    patch = JsonPatch(array);
    return is;
  }

  bool operator==(JsonPatch const& left, JsonPatch const& right)
  {
    return (JsonArray)left == (JsonArray)right;
  }

  bool operator!=(JsonPatch const& left, JsonPatch const& right)
  {
    return (JsonArray)left != (JsonArray)right;
  }

  JsonPatch operator""_JsonPatch(wchar_t const* value, size_t size)
  {
    return JsonPatch(JsonArray(Json::Parse(value)));
  }
}