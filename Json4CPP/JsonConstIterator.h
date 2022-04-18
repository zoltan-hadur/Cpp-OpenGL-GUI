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
    class JsonConstIteratorTest;
  }

  class JSON_API Json;

  class JSON_API JsonConstIterator
  {
  private:
    friend class ::Json4CPP::Test::JsonConstIteratorTest;
    friend class Json;
    Json const* _json;
    int64_t _position;

    JsonConstIterator(Json const* json, int64_t position);
  public:
    using difference_type = int64_t;
    using value_type = Json const;
    using pointer = Json const*;
    using reference = Json const&;
    using iterator_category = std::random_access_iterator_tag;

    JsonConstIterator();
    JsonConstIterator(JsonConstIterator const& it);
    JsonConstIterator& operator=(JsonConstIterator const& it);
    bool operator==(JsonConstIterator it) const;
    bool operator!=(JsonConstIterator it) const;
    reference operator*() const;
    pointer operator->() const;
    JsonConstIterator& operator++();
    JsonConstIterator operator++(int);
    JsonConstIterator& operator--();
    JsonConstIterator operator--(int);
    JsonConstIterator& operator+=(difference_type n);
    friend JSON_API JsonConstIterator operator+(JsonConstIterator const& it, difference_type n);
    friend JSON_API JsonConstIterator operator+(difference_type n, JsonConstIterator const& it);
    JsonConstIterator& operator-=(difference_type n);
    friend JSON_API JsonConstIterator operator-(JsonConstIterator const& it, difference_type n);
    friend JSON_API difference_type operator-(JsonConstIterator const& left, JsonConstIterator const& right);
    reference operator[](difference_type n);
    bool operator< (JsonConstIterator const& it) const;
    bool operator<=(JsonConstIterator const& it) const;
    bool operator> (JsonConstIterator const& it) const;
    bool operator>=(JsonConstIterator const& it) const;

    difference_type Size() const;
    std::wstring Key() const;
    Json Value() const;
  };
}