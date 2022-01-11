#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"
#include "JsonIterator.h"
#include "JsonObject.h"
#include "JsonArray.h"

using namespace std;

namespace Json4CPP
{
  using difference_type = int64_t;
  using value_type = Json;
  using pointer = Json*;
  using reference = Json&;
  using iterator_category = random_access_iterator_tag;

  JsonIterator::JsonIterator(Json* json, int64_t position) : _json(json), _position(position) {}

  JsonIterator::JsonIterator() : _json(nullptr), _position(0) {}

  JsonIterator::JsonIterator(JsonIterator const& it) : _json(it._json), _position(it._position) {}

  JsonIterator& JsonIterator::operator=(JsonIterator const& it)
  {
    _json = it._json;
    _position = it._position;
    return *this;
  }

  bool JsonIterator::operator==(JsonIterator it) const
  {
    return _position == it._position;
  }

  bool JsonIterator::operator!=(JsonIterator it) const
  {
    return !(*this == it);
  }

  reference JsonIterator::operator*()
  {
    return (*_json)[_position];
  }

  pointer JsonIterator::operator->()
  {
    return &(*_json)[_position];
  }

  JsonIterator& JsonIterator::operator++()
  {
    if (++_position > Size()) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonIterator JsonIterator::operator++(int)
  {
    auto result = *this;
    ++*this;
    return result;
  }

  JsonIterator& JsonIterator::operator--()
  {
    if (--_position < 0) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonIterator JsonIterator::operator--(int)
  {
    auto result = *this;
    --*this;
    return result;
  }

  JsonIterator& JsonIterator::operator+=(difference_type n)
  {
    _position += n;
    if (_position < 0 || _position > Size()) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonIterator operator+(JsonIterator const& it, difference_type n)
  {
    return JsonIterator(it._json, it._position + n);
  }

  JsonIterator operator+(difference_type n, JsonIterator const& it)
  {
    return JsonIterator(it._json, it._position + n);
  }

  JsonIterator& JsonIterator::operator-=(difference_type n)
  {
    _position -= n;
    if (_position < 0 || _position > Size()) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonIterator operator-(JsonIterator const& it, difference_type n)
  {
    return JsonIterator(it._json, it._position - n);
  }

  difference_type operator-(JsonIterator const& left, JsonIterator const& right)
  {
    return left._position - right._position;
  }

  reference JsonIterator::operator[](difference_type n)
  {
    return (*_json)[_position + n];
  }

  bool JsonIterator::operator<(JsonIterator const& it) const
  {
    return _position < it._position;
  }

  bool JsonIterator::operator<=(JsonIterator const& it) const
  {
    return _position <= it._position;
  }

  bool JsonIterator::operator>(JsonIterator const& it) const
  {
    return _position > it._position;
  }

  bool JsonIterator::operator>=(JsonIterator const& it) const
  {
    return _position >= it._position;
  }

  difference_type JsonIterator::Size() const
  {
    return _json->Size();
  }

  wstring JsonIterator::Key() const
  {
    switch (_json->Type())
    {
    case JsonType::Object: return get<JsonObject>(_json->_value)._pairs[_position].first;
    default: throw exception("Key() is only defined for JsonObject!");
    }
  }

  Json JsonIterator::Value() const
  {
    switch (_json->Type())
    {
    case JsonType::Object: return get<JsonObject>(_json->_value)._pairs[_position].second;
    default: throw exception("Value() is only defined for JsonObject!");
    }
  }

  void JsonIterator::Value(Json const& json)
  {
    switch (_json->Type())
    {
    case JsonType::Object: get<JsonObject>(_json->_value)._pairs[_position].second = json; break;
    default: throw exception("Value(Json const& json) is only defined for JsonObject!");
    }
  }

  void JsonIterator::Value(Json && json)
  {
    switch (_json->Type())
    {
    case JsonType::Object: get<JsonObject>(_json->_value)._pairs[_position].second = move(json); break;
    default: throw exception("Value(Json && json) is only defined for JsonObject!");
    }
  }
}