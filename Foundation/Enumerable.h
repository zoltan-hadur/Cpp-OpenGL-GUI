#pragma once

#include "Delegate.h"

#include <vector>
#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <utility>
#include <map>

namespace OpenGLUI::Foundation::Test
{
  class EnumerableTest;
}

namespace OpenGLUI::Foundation
{
#define DefaultSelector(Type) [](Type const& value) { return value; }
#define DefaultEqualityComparator(Type) [](Type const& left, Type const& right) { return left == right; }
#define DefaultLessThanComparator(Type) [](Type const& left, Type const& right) { return left < right; }

  template<typename Source>
  class OrderedEnumerable;

  template<typename Source>
  class Enumerable
  {
  private:
    template<typename Source> friend class Enumerable;
    template<typename Source> friend class OrderedEnumerable;
    template<typename Key, typename Source> friend class Grouping;
    friend class OpenGLUI::Foundation::Test::EnumerableTest;

    std::vector<Source*> _values;
    bool const _isReference;

#pragma region Iterator
    friend class Iterator;
    class Iterator
    {
    private:
      friend class Enumerable;
      Enumerable* _enumerable;
      int64_t _position;

      Iterator(Enumerable* enumerable, int64_t position) : _enumerable(enumerable), _position(position) {}
    public:
      using difference_type = int64_t;
      using value_type = Source;
      using pointer = Source * ;
      using reference = Source & ;
      using iterator_category = std::random_access_iterator_tag;

      difference_type Size() const
      {
        return _enumerable->Size();
      }

      Iterator& operator=(Iterator const& it)
      {
        _enumerable = it._enumerable;
        _position = it._position;
        return *this;
      }

      bool operator==(Iterator it) const
      {
        return _position == it._position;
      }

      bool operator!=(Iterator it) const
      {
        return !(*this == it);
      }

      reference operator*()
      {
        return *_enumerable->_values[_position];
      }

      pointer operator->()
      {
        return _enumerable->_values[_position];
      }

      Iterator& operator++()
      {
        if (++_position > Size()) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      Iterator operator++(int)
      {
        auto result = *this;
        ++*this;
        return result;
      }

      Iterator& operator--()
      {
        if (--_position < 0) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      Iterator operator--(int)
      {
        auto result = *this;
        --*this;
        return result;
      }

      Iterator& operator+=(difference_type n)
      {
        _position += n;
        if (_position < 0 || _position > Size()) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      friend Iterator& operator+(Iterator& it, difference_type n)
      {
        return it += n;
      }

      friend Iterator& operator+(difference_type n, Iterator& it)
      {
        return it += n;
      }

      Iterator& operator-=(difference_type n)
      {
        _position -= n;
        if (_position < 0 || _position > Size()) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      friend Iterator& operator-(Iterator& it, difference_type n)
      {
        return it -= n;
      }

      friend difference_type operator-(Iterator const& left, Iterator const& right)
      {
        return left._position - right._position;
      }

      reference operator[](difference_type n)
      {
        return *_enumerable->_values[_position + n];
      }

      bool operator<(Iterator const& it)
      {
        return _position < it._position;
      }

      bool operator<=(Iterator const& it)
      {
        return _position <= it._position;
      }

      bool operator>(Iterator const& it)
      {
        return _position > it._position;
      }

      bool operator>=(Iterator const& it)
      {
        return _position >= it._position;
      }
    };
#pragma endregion

#pragma region ConstIterator
    friend class ConstIterator;
    class ConstIterator
    {
    private:
      friend class Enumerable;
      Enumerable const* _enumerable;
      int64_t _position;

      ConstIterator(Enumerable const* enumerable, int64_t position) : _enumerable(enumerable), _position(position) {}
    public:
      using difference_type = int64_t;
      using value_type = Source const;
      using pointer = Source const*;
      using reference = Source const&;
      using iterator_category = std::random_access_iterator_tag;

      difference_type Size() const
      {
        return _enumerable->Size();
      }

      ConstIterator& operator=(ConstIterator const& it)
      {
        _enumerable = it._enumerable;
        _position = it._position;
        return *this;
      }

      bool operator==(ConstIterator it) const
      {
        return _position == it._position;
      }

      bool operator!=(ConstIterator it) const
      {
        return !(*this == it);
      }

      reference operator*() const
      {
        return *_enumerable->_values[_position];
      }

      pointer operator->()
      {
        return _enumerable->_values[_position];
      }

      ConstIterator& operator++()
      {
        if (++_position > Size()) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      ConstIterator operator++(int)
      {
        auto result = *this;
        ++*this;
        return result;
      }

      ConstIterator& operator--()
      {
        if (--_position < 0) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      ConstIterator operator--(int)
      {
        auto result = *this;
        --*this;
        return result;
      }

      ConstIterator& operator+=(difference_type n)
      {
        _position += n;
        if (_position < 0 || _position > Size()) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      friend ConstIterator& operator+(ConstIterator& it, difference_type n)
      {
        return it += n;
      }

      friend ConstIterator& operator+(difference_type n, ConstIterator& it)
      {
        return it += n;
      }

      ConstIterator& operator-=(difference_type n)
      {
        _position -= n;
        if (_position < 0 || _position > Size()) throw std::out_of_range("Iterator out of range!");
        return *this;
      }

      friend ConstIterator& operator-(ConstIterator& it, difference_type n)
      {
        return it -= n;
      }

      friend difference_type operator-(ConstIterator const& left, ConstIterator const& right)
      {
        return left._position - right._position;
      }

      reference operator[](difference_type n)
      {
        return *_enumerable->_values[_position + n];
      }

      bool operator<(ConstIterator const& it)
      {
        return _position < it._position;
      }

      bool operator<=(ConstIterator const& it)
      {
        return _position <= it._position;
      }

      bool operator>(ConstIterator const& it)
      {
        return _position > it._position;
      }

      bool operator>=(ConstIterator const& it)
      {
        return _position >= it._position;
      }
    };
#pragma endregion

  public:
    Enumerable(bool isReference = false) : _isReference(isReference) {}

    template<typename Iterable>
    Enumerable(Iterable const& values, bool isReference = true) : _isReference(isReference)
    {
      for (auto& value : values)
      {
        Add(value);
      }
    }

    Enumerable(Enumerable const& enumerable) : _isReference(enumerable._isReference)
    {
      for (auto& value : enumerable._values)
      {
        Add(*value);
      }
    }

    Enumerable(std::initializer_list<Source> enumerable) : _isReference(false)
    {
      for (auto& value : enumerable)
      {
        Add(value);
      }
    }

    ~Enumerable()
    {
      if (!_isReference)
      {
        for (auto& value : _values) delete value;
      }
      _values.clear();
    }

    Iterator begin()
    {
      return Iterator(this, 0);
    }

    Iterator end()
    {
      return Iterator(this, Size());
    }

    ConstIterator begin() const
    {
      return ConstIterator(this, 0);
    }

    ConstIterator end() const
    {
      return ConstIterator(this, Size());
    }

    inline void Add(Source const& value)
    {
      _values.push_back(_isReference ? (Source*)&value : new Source(value));
    }

    // Projects each element of a sequence into a new form.
    template<typename Result>
    Enumerable<Result> Select(Delegate<Result(Source const&)> selector) const
    {
      return Select<Result>([&](Source const& source, int64_t index) { return selector(source); });
    }

    // Projects each element of a sequence into a new form by incorporating the element's index.
    template<typename Result>
    Enumerable<Result> Select(Delegate<Result(Source const&, int64_t)> selector) const
    {
      Enumerable<Result> result(false);
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        result.Add(selector(*_values[i], i));
      }
      return result;
    }

    // Projects each element of a sequence to an Enumerable, and flattens the resulting sequences into one sequence.
    template<typename Result>
    Enumerable<Result> SelectMany(Delegate<Enumerable<Result>(Source const&)> selector) const
    {
      return SelectMany<Result>([&](Source const& source, int64_t index) { return selector(source); });
    }

    // Projects each element of a sequence to an Enumerable, and flattens the resulting sequences into one sequence. The index of each source element is used in the projected form of that element.
    template<typename Result>
    Enumerable<Result> SelectMany(Delegate<Enumerable<Result>(Source const&, int64_t)> selector) const
    {
      return SelectMany<Result, Result>(selector, DefaultSelector(Result));
    }

    // Projects each element of a sequence to an Enumerable, flattens the resulting sequences into one sequence, and invokes a result selector function on each element therein.
    template<typename Source2, typename Result>
    Enumerable<Result> SelectMany(Delegate<Enumerable<Source2>(Source const&)> collectionSelector, Delegate<Result(Source2 const&)> resultSelector) const
    {
      return SelectMany<Source2, Result>([&](Source const& source, int64_t index) { return collectionSelector(source); }, resultSelector);
    }

    // Projects each element of a sequence to an Enumerable, flattens the resulting sequences into one sequence, and invokes a result selector function on each element therein. The index of each source element is used in the intermediate projected form of that element.
    template<typename Source2, typename Result>
    Enumerable<Result> SelectMany(Delegate<Enumerable<Source2>(Source const&, int64_t)> collectionSelector, Delegate<Result(Source2 const&)> resultSelector) const
    {
      Enumerable<Result> result(false);
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        auto inner = collectionSelector(*_values[i], i);
        for (auto& innerValue : inner._values)
        {
          result.Add(resultSelector(*innerValue));
        }
      }
      return result;
    }

    // Filters a sequence of values based on a predicate.
    Enumerable<Source> Where(Delegate<bool(Source const&)> predicate) const
    {
      return Where([&](Source const& source, int64_t index) { return predicate(source); });
    }

    // Filters a sequence of values based on a predicate. Each element's index is used in the logic of the predicate function.
    Enumerable<Source> Where(Delegate<bool(Source const&, int64_t)> predicate) const
    {
      Enumerable<Source> result(_isReference);
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        if (predicate(*_values[i], i))
        {
          result.Add(*_values[i]);
        }
      }
      return result;
    }

    // Correlates the elements of two sequences based on equality of keys and groups the results. The default equality comparer is used to compare keys.
    template<typename Inner, typename Key, typename Result>
    Enumerable<Result> GroupJoin(Enumerable<Inner> const& inner,
                                 Delegate<Key(Source const&)> sourceKeySelector,
                                 Delegate<Key(Inner const&)> innerKeySelector,
                                 Delegate<Result(Source const&, Enumerable<Inner> const&)> resultSelector) const
    {
      return GroupJoin<Inner, Key, Result>(inner, sourceKeySelector, innerKeySelector, resultSelector, DefaultEqualityComparator(Key));
    }

    // Correlates the elements of two sequences based on key equality and groups the results. A specified equality comparer is used to compare keys.
    template<typename Inner, typename Key, typename Result>
    Enumerable<Result> GroupJoin(Enumerable<Inner> const& inner,
                                 Delegate<Key(Source const&)> sourceKeySelector,
                                 Delegate<Key(Inner const&)> innerKeySelector,
                                 Delegate<Result(Source const&, Enumerable<Inner> const&)> resultSelector,
                                 Delegate<bool(Key const&, Key const&)> comparer) const
    {
      Enumerable<Result> result(false);
      for (auto& value : _values)
      {
        Enumerable<Inner> innerResult(false);
        for (auto& innerValue : inner._values)
        {
          if (comparer(sourceKeySelector(*value), innerKeySelector(*innerValue)))
          {
            innerResult.Add(*innerValue);
          }
        }
        if (innerResult.Any())
        {
          result.Add(resultSelector(*value, innerResult));
        }
      }
      return result;
    }

    // Correlates the elements of two sequences based on matching keys. The default equality comparer is used to compare keys.
    template<typename Inner, typename Key, typename Result>
    Enumerable<Result> Join(Enumerable<Inner> const& inner,
                            Delegate<Key(Source const&)> sourceKeySelector,
                            Delegate<Key(Inner const&)> innerKeySelector,
                            Delegate<Result(Source const&, Inner const&)> resultSelector) const
    {
      return Join<Inner, Key, Result>(inner, sourceKeySelector, innerKeySelector, resultSelector, DefaultEqualityComparator(Key));
    }

    // Correlates the elements of two sequences based on matching keys. A specified equality comparer is used to compare keys.
    template<typename Inner, typename Key, typename Result>
    Enumerable<Result> Join(Enumerable<Inner> const& inner,
                            Delegate<Key(Source const&)> sourceKeySelector,
                            Delegate<Key(Inner const&)> innerKeySelector,
                            Delegate<Result(Source const&, Inner const&)> resultSelector,
                            Delegate<bool(Key const&, Key const&)> comparer) const
    {
      Enumerable<Result> result(false);
      for (auto& value : _values)
      {
        for (auto& innerValue : inner._values)
        {
          if (comparer(sourceKeySelector(*value), innerKeySelector(*innerValue)))
          {
            result.Add(resultSelector(*value, *innerValue));
          }
        }
      }
      return result;
    }

    // Determines whether all the elements of a sequence satisfy a condition.
    bool All(Delegate<bool(Source const&)> predicate) const
    {
      for (auto& value : _values)
      {
        if (!predicate(*value))
        {
          return false;
        }
      }
      return true;
    }

    // Determines whether a sequence contains any elements.
    bool Any() const
    {
      return Size() > 0;
    }

    // Determines whether any element of a sequence satisfies a condition.
    bool Any(Delegate<bool(Source const&)> predicate) const
    {
      for (auto& value : _values)
      {
        if (predicate(*value))
        {
          return true;
        }
      }
      return false;
    }

    // Determines whether a sequence contains a specified element by using the default equality comparer.
    bool Contains(Source const& element) const
    {
      return Contains(element, DefaultEqualityComparator(Source));
    }

    // Determines whether a sequence contains a specified element by using a specified equality comparer.
    bool Contains(Source const& element, Delegate<bool(Source const&, Source const&)> comparer) const
    {
      for (auto& value : _values)
      {
        if (comparer(*value, element))
        {
          return true;
        }
      }
      return false;
    }

    // Concatenates two sequences.
    Enumerable<Source> Concat(Enumerable<Source> const& enumerable) const
    {
      Enumerable<Source> result(_isReference && enumerable._isReference);
      for (auto& value : _values)
      {
        result.Add(*value);
      }
      for (auto& value : enumerable._values)
      {
        result.Add(*value);
      }
      return result;
    }

    // Returns the elements of the specified sequence or the type parameter's default value in a singleton collection if the sequence is empty.
    Enumerable<Source> DefaultIfEmpty() const
    {
      return DefaultIfEmpty(Source());
    }

    // Returns the elements of the specified sequence or the specified value in a singleton collection if the sequence is empty.
    Enumerable<Source> DefaultIfEmpty(Source const& defaultValue) const
    {
      if (Any()) return *this;
      Enumerable<Source> result(false);
      result.Add(defaultValue);
      return result;
    }

    // Returns distinct elements from a sequence by using the default equality comparer to compare values.
    Enumerable<Source> Distinct() const
    {
      return Distinct(DefaultEqualityComparator(Source));
    }

    // Returns distinct elements from a sequence by using a specified equality comparer to compare values.
    Enumerable<Source> Distinct(Delegate<bool(Source const&, Source const&)> comparer) const
    {
      Enumerable<Source> result(_isReference);
      for (auto& value : _values)
      {
        if (!result.Contains(*value, comparer))
        {
          result.Add(*value);
        }
      }
      return result;
    }

    // Produces the set difference of two sequences by using the default equality comparer to compare values.
    Enumerable<Source> Except(Enumerable<Source> const& enumerable) const
    {
      return Except(enumerable, DefaultEqualityComparator(Source));
    }

    // Produces the set difference of two sequences by using the specified equality comparer to compare values.
    Enumerable<Source> Except(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer) const
    {
      Enumerable<Source> result(_isReference);
      for (auto& value : _values)
      {
        if (!enumerable.Contains(*value, comparer))
        {
          result.Add(*value);
        }
      }
      return result;
    }

    // Produces the set intersection of two sequences by using the default equality comparer to compare values.
    Enumerable<Source> Intersect(Enumerable<Source> const& enumerable) const
    {
      return Intersect(enumerable, DefaultEqualityComparator(Source));
    }

    // Produces the set intersection of two sequences by using the specified equality comparer to compare values.
    Enumerable<Source> Intersect(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer) const
    {
      Enumerable<Source> result(_isReference);
      for (auto& value : _values)
      {
        if (!result.Contains(*value, comparer) && enumerable.Contains(*value, comparer))
        {
          result.Add(*value);
        }
      }
      return result;
    }

    // Produces the set union of two sequences by using the default equality comparer.
    Enumerable<Source> Union(Enumerable<Source> const& enumerable) const
    {
      return Union(enumerable, DefaultEqualityComparator(Source));
    }

    // Produces the set union of two sequences by using a specified equality comparer.
    Enumerable<Source> Union(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer) const
    {
      Enumerable<Source> result(_isReference && enumerable._isReference);
      for (auto& value : _values)
      {
        if (!result.Contains(*value, comparer))
        {
          result.Add(*value);
        }
      }
      for (auto& value : enumerable._values)
      {
        if (!result.Contains(*value, comparer))
        {
          result.Add(*value);
        }
      }
      return result;
    }

    // Sorts the elements of a sequence in ascending order according to a key.
    template<typename Key>
    OrderedEnumerable<Source> OrderBy(Delegate<Key(Source const&)> keySelector) const
    {
      return OrderBy<Key>(keySelector, DefaultLessThanComparator(Key));
    }

    // Sorts the elements of a sequence in ascending order by using a specified comparer.
    template<typename Key>
    OrderedEnumerable<Source> OrderBy(Delegate<Key(Source const&)> keySelector, Delegate<bool(Key const&, Key const&)> comparer) const
    {
      OrderedEnumerable<Source> result = *this;
      std::sort(result._values.begin(), result._values.end(), [&](Source *const& left, Source *const& right) -> bool
      {
        return comparer(keySelector(*left), keySelector(*right));
      });
      result.DetermineRanges(keySelector);
      return result;
    }

    // Sorts the elements of a sequence in descending order according to a key.
    template<typename Key>
    OrderedEnumerable<Source> OrderByDescending(Delegate<Key(Source const&)> keySelector) const
    {
      return OrderBy(keySelector).Reverse();
    }

    // Sorts the elements of a sequence in descending order by using a specified comparer.
    template<typename Key>
    OrderedEnumerable<Source> OrderByDescending(Delegate<Key(Source const&)> keySelector, Delegate<bool(Key const&, Key const&)> comparer) const
    {
      return OrderBy(keySelector, comparer).Reverse();
    }

    // Inverts the order of the elements in a sequence.
    Enumerable<Source> Reverse() const
    {
      Enumerable<Source> result = *this;
      std::reverse(result._values.begin(), result._values.end());
      return result;
    }

    // Groups the elements of a sequence according to a specified key selector function.
    template<typename Key>
    Enumerable<Grouping<Key, Source>> GroupBy(Delegate<Key(Source const&)> keySelector) const
    {
      return GroupBy<Key, Source, Grouping<Key, Source>>(
        keySelector,
        DefaultSelector(Source),
        [](Key const& key, Enumerable<Source> const& enumerable) { return Grouping(key, enumerable); },
        DefaultEqualityComparator(Key)
      );
    }

    // Groups the elements of a sequence according to a specified key selector function and compares the keys by using a specified comparer.
    template<typename Key>
    Enumerable<Grouping<Key, Source>> GroupBy(Delegate<Key(Source const&)> keySelector, Delegate<bool(Key const&, Key const&)> comparer) const
    {
      return GroupBy<Key, Source, Grouping<Key, Source>>(
        keySelector,
        DefaultSelector(Source),
        [](Key const& key, Enumerable<Source> const& enumerable) { return Grouping(key, enumerable); },
        comparer
      );
    }

    // Groups the elements of a sequence according to a specified key selector function and creates a result value from each group and its key.
    template<typename Key, typename Result>
    Enumerable<Result> GroupBy(Delegate<Key(Source const&)> keySelector, Delegate<Result(Key const&, Enumerable<Source> const&)> resultSelector) const
    {
      return GroupBy<Key, Source, Result>(
        keySelector,
        DefaultSelector(Source),
        resultSelector,
        DefaultEqualityComparator(Key)
      );
    }

    // Groups the elements of a sequence according to a specified key selector function and creates a result value from each group and its key. The keys are compared by using a specified comparer.
    template<typename Key, typename Result>
    Enumerable<Result> GroupBy(Delegate<Key(Source const&)> keySelector, Delegate<Result(Key const&, Enumerable<Source> const&)> resultSelector, Delegate<bool(Key const&, Key const&)> comparer) const
    {
      return GroupBy<Key, Source, Result>(
        keySelector,
        DefaultSelector(Source),
        resultSelector,
        comparer
      );
    }

    // Groups the elements of a sequence according to a specified key selector function and projects the elements for each group by using a specified function.
    template<typename Key, typename Element>
    Enumerable<Grouping<Key, Element>> GroupBy(Delegate<Key(Source const&)> keySelector, Delegate<Element(Source const&)> elementSelector) const
    {
      return GroupBy<Key, Element, Grouping<Key, Element>>(
        keySelector,
        elementSelector,
        [](Key const& key, Enumerable<Element> const& enumerable) { return Grouping(key, enumerable); },
        DefaultEqualityComparator(Key)
      );
    }

    // Groups the elements of a sequence according to a key selector function. The keys are compared by using a comparer and each group's elements are projected by using a specified function.
    template<typename Key, typename Element>
    Enumerable<Grouping<Key, Element>> GroupBy(Delegate<Key(Source const&)> keySelector, Delegate<Element(Source const&)> elementSelector, Delegate<bool(Key const&, Key const&)> comparer) const
    {
      return GroupBy<Key, Element, Grouping<Key, Element>>(
        keySelector,
        elementSelector,
        [](Key const& key, Enumerable<Element> const& enumerable) { return Grouping(key, enumerable); },
        comparer
      );
    }

    // Groups the elements of a sequence according to a specified key selector function and creates a result value from each group and its key. The elements of each group are projected by using a specified function.
    template<typename Key, typename Element, typename Result>
    Enumerable<Result> GroupBy(
      Delegate<Key(Source const&)> keySelector,
      Delegate<Element(Source const&)> elementSelector,
      Delegate<Result(Key const&, Enumerable<Element> const&)> resultSelector
    ) const
    {
      return GroupBy<Key, Element, Result>(
        keySelector,
        elementSelector,
        resultSelector,
        DefaultEqualityComparator(Key)
      );
    }

    // Groups the elements of a sequence according to a specified key selector function and creates a result value from each group and its key. Key values are compared by using a specified comparer, and the elements of each group are projected by using a specified function.
    template<typename Key, typename Element, typename Result>
    Enumerable<Result> GroupBy(
      Delegate<Key(Source const&)> keySelector,
      Delegate<Element(Source const&)> elementSelector,
      Delegate<Result(Key const&, Enumerable<Element> const&)> resultSelector,
      Delegate<bool(Key const&, Key const&)> comparer
    ) const
    {
      Enumerable<Grouping<Key, Element>> groups(false);
      for (auto& value : _values)
      {
        auto key = keySelector(*value);
        Grouping<Key, Element>* groupPtr = nullptr;
        for (auto& group : groups)
        {
          if (comparer(group.Key(), key))
          {
            groupPtr = &group;
            break;
          }
        }
        if (!groupPtr)
        {
          groupPtr = groups._values.emplace_back(new Grouping<Key, Element>(key, false));
        }
        groupPtr->Add(elementSelector(*value));
      }
      Enumerable<Result> result(false);
      for (auto& group : groups)
      {
        result.Add(resultSelector(group.Key(), group));
      }
      return result;
    }

    // Applies an accumulator function over a sequence.
    Source Aggregate(Delegate<Source(Source const&, Source const&)> accumulator) const
    {
      if (Size() == 0) throw std::runtime_error("Aggregate can't be called on empty collections!");
      auto accumulation = *_values[0];
      auto size = Size();
      for (int64_t i = 1; i < size; ++i)
      {
        accumulation = accumulator(accumulation, *_values[i]);
      }
      return accumulation;
    }

    // Applies an accumulator function over a sequence. The specified seed value is used as the initial accumulator value.
    template<typename Accumulate>
    Accumulate Aggregate(Accumulate seed, Delegate<Accumulate(Accumulate const&, Source const&)> accumulator) const
    {
      return Aggregate<Accumulate, Accumulate>(seed, accumulator, DefaultSelector(Accumulate));
    }

    // Applies an accumulator function over a sequence. The specified seed value is used as the initial accumulator value, and the specified function is used to select the result value.
    template<typename Accumulate, typename Result>
    Result Aggregate(Accumulate seed, Delegate<Accumulate(Accumulate const&, Source const&)> accumulator, Delegate<Result(Accumulate const&)> selector) const
    {
      auto accumulation = seed;
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        accumulation = accumulator(accumulation, *_values[i]);
      }
      return selector(accumulation);
    }

    // Computes the average of a sequence.
    double Average() const
    {
      return (double)Sum() / Size();
    }

    // Computes the average of a sequence of values that is obtained by invoking a projection function on each element of the input sequence.
    template<typename Result>
    double Average(Delegate<Result(Source const&)> selector) const
    {
      return (double)Sum(selector) / Size();
    }

    // Returns the number of elements in a sequence.
    int64_t Size() const
    {
      return _values.size();
    }

    // Returns the number of elements in a sequence.
    int64_t Count() const
    {
      return Size();
    }

    // Returns the number of elements in the specified sequence that satisfies a condition.
    int64_t Count(Delegate<bool(Source const&)> predicate) const
    {
      int64_t count = 0;
      for (auto& value : _values)
      {
        if (predicate(*value))
        {
          count++;
        }
      }
      return count;
    }

    // Returns the maximum value in a sequence.
    Source Max() const
    {
      return Max<Source>(DefaultSelector(Source));
    }

    // Invokes a projection function on each element in a sequence and returns the maximum resulting value.
    template<typename Key>
    Key Max(Delegate<Key(Source const&)> selector) const
    {
      auto max = std::numeric_limits<Key>::lowest();
      for (auto& value : _values)
      {
        auto projected = selector(*value);
        if (projected > max)
        {
          max = projected;
        }
      }
      return max;
    }

    // Returns the minimum value in a sequence.
    Source Min() const
    {
      return Min<Source>(DefaultSelector(Source));
    }

    // Invokes a projection function on each element in a sequence and returns the minimum resulting value.
    template<typename Key>
    Key Min(Delegate<Key(Source const&)> selector) const
    {
      auto min = std::numeric_limits<Key>::max();
      for (auto& value: _values)
      {
        auto projected = selector(*value);
        if (projected < min)
        {
          min = projected;
        }
      }
      return min;
    }

    // Computes the sum of a sequence.
    Source Sum() const
    {
      return Sum<Source>(DefaultSelector(Source));
    }

    // Computes the sum of a sequence of values that is obtained by invoking a projection function on each element of the input sequence.
    template<typename Key>
    Key Sum(Delegate<Key(Source const&)> selector) const
    {
      auto sum = Key();
      for (auto& value : _values)
      {
        sum += selector(*value);
      }
      return sum;
    }

    // Converts the elements of a sequence to the specified type.
    template<typename Result>
    Enumerable<Result> StaticCast() const
    {
      Enumerable<Result> result(false);
      for (auto& value : _values)
      {
        result.Add(static_cast<Result>(*value));
      }
      return result;
    }

    // Converts the elements of a sequence to the specified type.
    template<typename Result>
    Enumerable<Result> DynamicCast() const
    {
      Enumerable<Result> result(false);
      for (auto& value : _values)
      {
        result.Add(dynamic_cast<Result>(*value));
      }
      return result;
    }

    // Filters the elements of a sequence based on the specified type.
    template<typename Result>
    Enumerable<Result> OfType() const
    {
      Enumerable<Result> result(false);
      for (auto& value : _values)
      {
        if (auto casted = dynamic_cast<Result>(*value))
        {
          result.Add(casted);
        }
      }
      return result;
    }

    // Returns the element at a specified index in a sequence.
    Source ElementAt(int64_t index) const
    {
      if (0 > index || index >= Size()) throw std::out_of_range("Index " + to_string(index) + " is out of range [0, " + to_string(Size()) + ")!");
      return *_values[index];
    }

    // Returns the element at a specified index in a sequence or a default value if the index is out of range.
    Source ElementAtOrDefault(int64_t index) const
    {
      return (0 <= index && index < Size()) ? *_values[index] : Source();
    }

    // Returns the first element of a sequence.
    Source First() const
    {
      return *_values[0];
    }

    // Returns the first element of a sequence that satisfies a specified condition.
    Source First(Delegate<bool(Source const&)> predicate) const
    {
      return Where(predicate).First();
    }

    // Returns the first element of a sequence, or a default value if the sequence contains no elements.
    Source FirstOrDefault() const
    {
      return Any() ? First() : Source();
    }

    // Returns the first element of a sequence that satisfies a specified condition or a default value if no such element is found.
    Source FirstOrDefault(Delegate<bool(Source const&)> predicate) const
    {
      return Where(predicate).FirstOrDefault();
    }

    // Returns the last element of a sequence.
    Source Last() const
    {
      return *_values[Size() - 1];
    }

    // Returns the last element of a sequence that satisfies a specified condition.
    Source Last(Delegate<bool(Source const&)> predicate) const
    {
      return Where(predicate).Last();
    }

    // Returns the last element of a sequence, or a default value if the sequence contains no elements.
    Source LastOrDefault() const
    {
      return Any() ? Last() : Source();
    }

    // Returns the last element of a sequence that satisfies a specified condition or a default value if no such element is found.
    Source LastOrDefault(Delegate<bool(Source const&)> predicate) const
    {
      return Where(predicate).LastOrDefault();
    }

    // Returns the only element of a sequence, and throws an exception if there is not exactly one element in the sequence.
    Source Single() const
    {
      if (Size() != 1) throw std::runtime_error("There is not exactly one element in the sequence!");
      return *_values[0];
    }

    // Returns the only element of a sequence that satisfies a specified condition, and throws an exception if more than one such element exists.
    Source Single(Delegate<bool(Source const&)> predicate) const
    {
      return Where(predicate).Single();
    }

    // Returns the only element of a sequence, or a default value if the sequence is empty; this method throws an exception if there is more than one element in the sequence.
    Source SingleOrDefault() const
    {
      if (Size() > 1) throw std::runtime_error("There is more than one element in the sequence!");
      return Any() ? *_values[0] : Source();
    }

    // Returns the only element of a sequence that satisfies a specified condition or a default value if no such element exists; this method throws an exception if more than one element satisfies the condition.
    Source SingleOrDefault(Delegate<bool(Source const&)> predicate) const
    {
      return Where(predicate).SingleOrDefault();
    }

    // Bypasses a specified number of elements in a sequence and then returns the remaining elements.
    Enumerable<Source> Skip(int64_t count) const
    {
      return SkipWhile([&](Source const& source, int64_t index) { return index < count; });
    }

    // Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements.
    Enumerable<Source> SkipWhile(Delegate<bool(Source const&)> predicate) const
    {
      return SkipWhile([&](Source const& source, int64_t index) { return predicate(source); });
    }

    // Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements. The element's index is used in the logic of the predicate function.
    Enumerable<Source> SkipWhile(Delegate<bool(Source const&, int64_t)> predicate) const
    {
      Enumerable<Source> result(_isReference);
      auto size = Size();
      auto count = 0i64;
      for (int64_t i = 0; i < size && predicate(*_values[i], i); ++i)
      {
        count++;
      }
      for (int64_t i = count; i < size; ++i)
      {
        result.Add(*_values[i]);
      }
      return result;
    }

    // Returns a specified number of contiguous elements from the start of a sequence.
    Enumerable<Source> Take(int64_t count) const
    {
      return TakeWhile([&](Source const& source, int64_t index) { return index < count; });
    }

    // Returns elements from a sequence as long as a specified condition is true. The element's index is used in the logic of the predicate function.
    Enumerable<Source> TakeWhile(Delegate<bool(Source const&)> predicate) const
    {
      return TakeWhile([&](Source const& source, int64_t index) { return predicate(source); });
    }

    // Returns elements from a sequence as long as a specified condition is true.
    Enumerable<Source> TakeWhile(Delegate<bool(Source const&, int64_t)> predicate) const
    {
      Enumerable<Source> result(_isReference);
      auto size = Size();
      for (int64_t i = 0; i < size && predicate(*_values[i], i); ++i)
      {
        result.Add(*_values[i]);
      }
      return result;
    }

    // Merges two sequences by using the specified predicate function.
    template<typename Result, typename Source2>
    Enumerable<Result> Zip(Enumerable<Source2> const& enumerable, Delegate<Result(Source const&, typename std::iterator_traits<decltype(enumerable.begin())>::value_type const&)> resultSelector) const
    {
      Enumerable<Result> result(false);
      auto size = std::min(Size(), enumerable.Size());
      for (int64_t i = 0; i < size; ++i)
      {
        result.Add(resultSelector(*_values[i], *enumerable._values[i]));
      }
      return result;
    }

    // Appends a value to the end of the sequence.
    Enumerable<Source> Append(Source const& value) const
    {
      Enumerable<Source> result(false);
      for (auto& value : _values)
      {
        result.Add(*value);
      }
      result.Add(value);
      return result;
    }

    // Adds a value to the beginning of the sequence.
    Enumerable<Source> Prepend(Source const& value) const
    {
      Enumerable<Source> result(false);
      result.Add(value);
      for (auto& value : _values)
      {
        result.Add(*value);
      }
      return result;
    }

    // Determines whether two sequences are equal by comparing the elements by using the default equality comparer for their type.
    bool SequenceEqual(Enumerable<Source> const& enumerable) const
    {
      return SequenceEqual(enumerable, DefaultEqualityComparator(Source));
    }

    // Determines whether two sequences are equal by comparing their elements by using a specified equality comparer.
    bool SequenceEqual(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer) const
    {
      if (Size() != enumerable.Size()) return false;
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        if (!comparer(*_values[i], *enumerable._values[i]))
          return false;
      }
      return true;
    }

    // Generates a sequence of integral numbers within a specified range.
    static Enumerable<Source> Range(int64_t start, int64_t count)
    {
      Enumerable<Source> result(false);
      for (auto i = start; i < start + count; ++i)
      {
        result.Add(i);
      }
      return result;
    }

    // Generates a sequence that contains one repeated value.
    static Enumerable<Source> Repeat(Source const& value, int64_t count)
    {
      Enumerable<Source> result(false);
      for (auto i = 0; i < count; ++i)
      {
        result.Add(value);
      }
      return result;
    }

    // Creates a vector<Source> from an Enumerable<Source>.
    std::vector<Source> ToVector() const
    {
      std::vector<Source> result;
      for (auto& value : _values)
      {
        result.push_back(*value);
      }
      return result;
    }

    // Creates a map<Key, Value> from an Enumerable<Source> according to the specified key selector and element selector functions.
    template<typename Key, typename Value>
    std::map<Key, Value> ToMap(Delegate<Key(Source const&)> keySelector, Delegate<Value(Source const&)> valueSelector) const
    {
      std::map<Key, Value> result;
      for (auto& value : _values)
      {
        result[keySelector(*value)] = valueSelector(*value);
      }
      return result;
    }
  };

  template<typename Iterable>
  Enumerable(Iterable values, bool isReference = true) -> Enumerable<typename std::iterator_traits<decltype(values.begin())>::value_type>;

  template<typename Iterable>
  auto From(Iterable && values)
  {
    return Enumerable(values, is_lvalue_reference_v<decltype(values)>);
  }

  template<typename Source>
  auto From(std::initializer_list<Source> const& values)
  {
    return Enumerable(values);
  }

  template<typename Source>
  class OrderedEnumerable : public Enumerable<Source>
  {
  private:
    template<typename Source> friend class Enumerable;
    template<typename Source> friend class OrderedEnumerable;
    template<typename Key, typename Source> friend class Grouping;
    friend class OpenGLUI::Foundation::Test::EnumerableTest;

    std::vector<std::pair<int64_t, int64_t>> _ranges;

    OrderedEnumerable(bool isReference = true) : Enumerable(isReference) {}
    OrderedEnumerable(Enumerable const& enumerable) : Enumerable(enumerable) {}

    template<typename Key>
    void DetermineRanges(Delegate<Key(Source const&)> keySelector)
    {
      if (Size() <= 1) return;
      _ranges.clear();
      int64_t from = 0;
      auto size = Size();
      for (int64_t to = 0; to < size - 1; ++to)
      {
        if (keySelector(*_values[to]) != keySelector(*_values[to + 1]))
        {
          _ranges.push_back({ from, to });
          from = to + 1;
        }
      }
      _ranges.push_back({ from, Size() - 1 });
    }
  public:
    // Inverts the order of the elements in a sequence.
    OrderedEnumerable<Source> Reverse()
    {
      OrderedEnumerable<Source> result = *this;
      std::reverse(result._values.begin(), result._values.end());
      auto size = Size();
      for (auto& range : _ranges)
      {
        range.first = size - 1 - range.first;
        range.second = size - 1 - range.second;
      }
      return result;
    }

    // Performs a subsequent ordering of the elements in a sequence in ascending order according to a key.
    template<typename Key>
    OrderedEnumerable<Source> ThenBy(Delegate<Key(Source const&)> keySelector)
    {
      return ThenBy<Key>(keySelector, DefaultLessThanComparator(Key));
    }

    // Performs a subsequent ordering of the elements in a sequence in ascending order by using a specified comparer.
    template<typename Key>
    OrderedEnumerable<Source> ThenBy(Delegate<Key(Source const&)> keySelector, Delegate<bool(Key const&, Key const&)> comparer)
    {
      OrderedEnumerable<Source> result = *this;
      for (auto& range : _ranges)
      {
        auto&[from, to] = range;
        if (from == to) continue;
        std::sort(result._values.begin() + from, result._values.begin() + to + 1, [&](Source *const& left, Source *const& right) -> bool
        {
          return comparer(keySelector(*left), keySelector(*right));
        });
      }
      result.DetermineRanges(keySelector);
      return result;
    }

    // Performs a subsequent ordering of the elements in a sequence in descending order, according to a key.
    template<typename Key>
    OrderedEnumerable<Source> ThenByDescending(Delegate<Key(Source const&)> keySelector)
    {
      return ThenByDescending<Key>(keySelector, DefaultLessThanComparator(Key));
    }

    // Performs a subsequent ordering of the elements in a sequence in descending order by using a specified comparer.
    template<typename Key>
    OrderedEnumerable<Source> ThenByDescending(Delegate<Key(Source const&)> keySelector, Delegate<bool(Key const&, Key const&)> comparer)
    {
      OrderedEnumerable<Source> result = *this;
      for (auto& range : _ranges)
      {
        auto&[from, to] = range;
        if (from == to) continue;
        std::sort(result._values.begin() + from, result._values.begin() + to + 1, [&](Source *const& left, Source *const& right) -> bool
        {
          return comparer(keySelector(*left), keySelector(*right));
        });
        std::reverse(result._values.begin() + from, result._values.begin() + to + 1);
      }
      result.DetermineRanges(keySelector);
      return result;
    }
  };

  template<typename Key, typename Source>
  class Grouping : Enumerable<Source>
  {
  private:
    template<typename Source> friend class Enumerable;
    template<typename Source> friend class OrderedEnumerable;
    template<typename Key, typename Source> friend class Grouping;
    friend class OpenGLUI::Foundation::Test::EnumerableTest;

    Key _key;

    Grouping() : Enumerable(false) { _key = Key(); }
    Grouping(Key key, bool isReference = true) : Enumerable(isReference) { _key = key; }
    Grouping(Key key, Enumerable const& enumerable) : Enumerable(enumerable) { _key = key; }
  public:
    
    Key Key() const
    {
      return _key;
    }
  };
}