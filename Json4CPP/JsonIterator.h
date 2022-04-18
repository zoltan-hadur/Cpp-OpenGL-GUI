#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <xutility>
#include <string>

namespace Json4CPP
{
  namespace Test {
    class JsonIteratorTest;
  }

  class JSON_API Json;

  class JSON_API JsonIterator
  {
  private:
    friend class ::Json4CPP::Test::JsonIteratorTest;
    friend class Json;
    Json* _json;
    int64_t _position;

    JsonIterator(Json* json, int64_t position);
  public:
    using difference_type = int64_t;
    using value_type = Json;
    using pointer = Json*;
    using reference = Json&;
    using iterator_category = std::random_access_iterator_tag;

    JsonIterator();
    JsonIterator(JsonIterator const& it);
    JsonIterator& operator=(JsonIterator const& it);
    bool operator==(JsonIterator it) const;
    bool operator!=(JsonIterator it) const;
    reference operator*();
    pointer operator->();
    JsonIterator& operator++();
    JsonIterator operator++(int);
    JsonIterator& operator--();
    JsonIterator operator--(int);
    JsonIterator& operator+=(difference_type n);
    friend JSON_API JsonIterator operator+(JsonIterator const& it, difference_type n);
    friend JSON_API JsonIterator operator+(difference_type n, JsonIterator const& it);
    JsonIterator& operator-=(difference_type n);
    friend JSON_API JsonIterator operator-(JsonIterator const& it, difference_type n);
    friend JSON_API difference_type operator-(JsonIterator const& left, JsonIterator const& right);
    reference operator[](difference_type n);
    bool operator< (JsonIterator const& it) const;
    bool operator<=(JsonIterator const& it) const;
    bool operator> (JsonIterator const& it) const;
    bool operator>=(JsonIterator const& it) const;

    difference_type Size() const;
    std::wstring Key() const;
    Json Value() const;
    void Value(Json const& json);
    void Value(Json     && json);
  };
}