#include "stdafx.h"

#include "JsonDefault.h"
#include "Json.h"
#include "JsonBuilder.h"
#include "Value.h"
#include "Helper.h"
#include "JsonIndentSize.h"
#include "JsonIndentChar.h"
#include "JsonConstIterator.h"
#include "JsonObject.h"
#include "JsonArray.h"

using namespace std;

namespace Json4CPP
{
  using difference_type = int64_t;
  using value_type = Json const;
  using pointer = Json const*;
  using reference = Json const&;
  using iterator_category = random_access_iterator_tag;

  JsonConstIterator::JsonConstIterator(Json const* json, int64_t position) : _json(json), _position(position) {}

  JsonConstIterator::JsonConstIterator() : _json(nullptr), _position(0) {}

  JsonConstIterator::JsonConstIterator(JsonConstIterator const& it) : _json(it._json), _position(it._position) {}

  JsonConstIterator& JsonConstIterator::operator=(JsonConstIterator const& it)
  {
    _json = it._json;
    _position = it._position;
    return *this;
  }

  bool JsonConstIterator::operator==(JsonConstIterator it) const
  {
    return _position == it._position;
  }

  bool JsonConstIterator::operator!=(JsonConstIterator it) const
  {
    return !(*this == it);
  }

  reference JsonConstIterator::operator*() const
  {
    return (*_json).At(_position);
  }

  pointer JsonConstIterator::operator->() const
  {
    return &(*_json).At(_position);
  }

  JsonConstIterator& JsonConstIterator::operator++()
  {
    if (++_position > Size()) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonConstIterator JsonConstIterator::operator++(int)
  {
    auto result = *this;
    ++* this;
    return result;
  }

  JsonConstIterator& JsonConstIterator::operator--()
  {
    if (--_position < 0) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonConstIterator JsonConstIterator::operator--(int)
  {
    auto result = *this;
    --* this;
    return result;
  }

  JsonConstIterator& JsonConstIterator::operator+=(difference_type n)
  {
    _position += n;
    if (_position < 0 || _position > Size()) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonConstIterator operator+(JsonConstIterator const& it, difference_type n)
  {
    return JsonConstIterator(it._json, it._position + n);
  }

  JsonConstIterator operator+(difference_type n, JsonConstIterator const& it)
  {
    return JsonConstIterator(it._json, it._position + n);
  }

  JsonConstIterator& JsonConstIterator::operator-=(difference_type n)
  {
    _position -= n;
    if (_position < 0 || _position > Size()) throw out_of_range("Iterator out of range!");
    return *this;
  }

  JsonConstIterator operator-(JsonConstIterator const& it, difference_type n)
  {
    return JsonConstIterator(it._json, it._position - n);
  }

  difference_type operator-(JsonConstIterator const& left, JsonConstIterator const& right)
  {
    return left._position - right._position;
  }

  reference JsonConstIterator::operator[](difference_type n)
  {
    return (*_json).At(_position + n);
  }

  bool JsonConstIterator::operator<(JsonConstIterator const& it) const
  {
    return _position < it._position;
  }

  bool JsonConstIterator::operator<=(JsonConstIterator const& it) const
  {
    return _position <= it._position;
  }

  bool JsonConstIterator::operator>(JsonConstIterator const& it) const
  {
    return _position > it._position;
  }

  bool JsonConstIterator::operator>=(JsonConstIterator const& it) const
  {
    return _position >= it._position;
  }

  difference_type JsonConstIterator::Size() const
  {
    return _json->Size();
  }

  wstring JsonConstIterator::Key() const
  {
    switch (_json->Type())
    {
    case JsonType::Object: return get<JsonObject>(_json->_value)._pairs[_position].first;
    default: throw exception("Key() is only defined for JsonObject!");
    }
  }

  Json JsonConstIterator::Value() const
  {
    switch (_json->Type())
    {
    case JsonType::Object: return get<JsonObject>(_json->_value)._pairs[_position].second;
    default: throw exception("Value() is only defined for JsonObject!");
    }
  }
}