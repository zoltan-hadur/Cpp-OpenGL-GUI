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
    friend class OpenGLUI::Foundation::Test::EnumerableTest;

    std::vector<Source*> _values;
    bool const _isReference;

#pragma region Iterator
    friend class Iterator;
    friend class ConstIterator;
    class Iterator
    {
    private:
      friend class Enumerable;
      Enumerable* _enumerable;
      int64_t _position;

      Iterator(Enumerable& enumerable, int64_t position) : _enumerable(&enumerable), _position(position) {}
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
    class ConstIterator
    {
    private:
      friend class Enumerable;
      Enumerable const* _enumerable;
      int64_t _position;

      ConstIterator(Enumerable const& enumerable, int64_t position) : _enumerable(&enumerable), _position(position) {}
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
      return Iterator(*this, 0);
    }

    Iterator end()
    {
      return Iterator(*this, Size());
    }

    ConstIterator begin() const
    {
      return ConstIterator(*this, 0);
    }

    ConstIterator end() const
    {
      return ConstIterator(*this, Size());
    }

    void Add(Source const& value)
    {
      _values.push_back(_isReference ? (Source*)&value : new Source(value));
    }

    // Projects each element of a sequence into a new form.
    template<typename Result>
    Enumerable<Result> Select(Delegate<Result(Source const&)> selector) const
    {
      Enumerable<Result> result(false);
      for (auto& value : _values)
      {
        result.Add(selector(*value));
      }
      return result;
    }

    // Projects each element of a sequence into a new form by incorporating the element's index.
    template<typename Result>
    Enumerable<Result> Select(Delegate<Result(Source const&, int64_t)> selector) const
    {
      Enumerable<Result> result(false);
      auto size = Size();
      for (int i = 0; i < size; ++i)
      {
        result.Add(selector(*_values[i], i));
      }
      return result;
    }

    // TODO SelectMany

    // Filters a sequence of values based on a predicate.
    Enumerable<Source> Where(Delegate<bool(Source const&)> predicate) const
    {
      Enumerable<Source> result(_isReference);
      for (auto& value : _values)
      {
        if (predicate(*value))
        {
          result.Add(*value);
        }
      }
      return result;
    }

    // Filters a sequence of values based on a predicate. Each element's index is used in the logic of the predicate function.
    Enumerable<Source> Where(Delegate<bool(Source const&, int64_t)> predicate) const
    {
      Enumerable<Source> result(_isReference);
      auto size = Size();
      for (int i = 0; i < size; ++i)
      {
        if (predicate(*_values[i], i))
        {
          result.Add(*_values[i]);
        }
      }
      return result;
    }

    // TODO Group join

    // TODO Join

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
    Enumerable<Source> Distinct()
    {
      return Distinct(DefaultEqualityComparator(Source));
    }

    // Returns distinct elements from a sequence by using a specified equality comparer to compare values.
    Enumerable<Source> Distinct(Delegate<bool(Source const&, Source const&)> comparer)
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
    Enumerable<Source> Except(Enumerable<Source> const& enumerable)
    {
      return Except(enumerable, DefaultEqualityComparator(Source));
    }

    // Produces the set difference of two sequences by using the specified equality comparer to compare values.
    Enumerable<Source> Except(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer)
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
    Enumerable<Source> Intersect(Enumerable<Source> const& enumerable)
    {
      return Intersect(enumerable, DefaultEqualityComparator(Source));
    }

    // Produces the set intersection of two sequences by using the specified equality comparer to compare values.
    Enumerable<Source> Intersect(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer)
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
    Enumerable<Source> Union(Enumerable<Source> const& enumerable)
    {
      return Union(enumerable, DefaultEqualityComparator(Source));
    }

    // Produces the set union of two sequences by using a specified equality comparer.
    Enumerable<Source> Union(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer)
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
    OrderedEnumerable<Source> OrderBy(Delegate<Key(Source const&)> keySelector)
    {
      return OrderBy<Key>(keySelector, DefaultLessThanComparator(Key));
    }

    // Sorts the elements of a sequence in ascending order by using a specified comparer.
    template<typename Key>
    OrderedEnumerable<Source> OrderBy(Delegate<Key(Source const&)> keySelector, Delegate<bool(Key const&, Key const&)> comparer)
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
    OrderedEnumerable<Source> OrderByDescending(Delegate<Key(Source const&)> keySelector)
    {
      return OrderBy(keySelector).Reverse();
    }

    // Sorts the elements of a sequence in descending order by using a specified comparer.
    template<typename Key>
    OrderedEnumerable<Source> OrderByDescending(Delegate<Key(Source const&)> keySelector, Delegate<bool(Key const&, Key const&)> comparer)
    {
      return OrderBy(keySelector, comparer).Reverse();
    }

    // Inverts the order of the elements in a sequence.
    Enumerable<Source> Reverse()
    {
      Enumerable<Source> result = *this;
      std::reverse(result._values.begin(), result._values.end());
      return result;
    }

    // TODO GroupBy

    // Applies an accumulator function over a sequence.
    Source Aggregate(Delegate<Source(Source const&, Source const&)> accumulator)
    {
      if (Size() == 0) throw std::runtime_error("Aggregate can't be called on empty collections!");
      auto accumulation = *_values[0];
      auto size = Size();
      for (int i = 1; i < size; ++i)
      {
        accumulation = accumulator(accumulation, *_values[i]);
      }
      return accumulation;
    }

    // Applies an accumulator function over a sequence. The specified seed value is used as the initial accumulator value.
    template<typename Accumulate>
    Accumulate Aggregate(Accumulate seed, Delegate<Accumulate(Accumulate const&, Source const&)> accumulator)
    {
      auto accumulation = seed;
      auto size = Size();
      for (int i = 0; i < size; ++i)
      {
        accumulation = accumulator(accumulation, *_values[i]);
      }
      return accumulation;
    }

    // Applies an accumulator function over a sequence. The specified seed value is used as the initial accumulator value, and the specified function is used to select the result value.
    template<typename Accumulate, typename Result>
    Result Aggregate(Accumulate seed, Delegate<Accumulate(Accumulate const&, Source const&)> accumulator, Delegate<Result(Accumulate const&)> selector)
    {
      auto accumulation = seed;
      auto size = Size();
      for (int i = 0; i < size; ++i)
      {
        accumulation = accumulator(accumulation, *_values[i]);
      }
      return selector(accumulation);
    }

    // Computes the average of a sequence.
    double Average()
    {
      return (double)Sum() / Size();
    }

    // Computes the average of a sequence of values that is obtained by invoking a projection function on each element of the input sequence.
    template<typename Result>
    double Average(Delegate<Result(Source const&)> selector)
    {
      return (double)Sum(selector) / Size();
    }

    // Returns the number of elements in a sequence.
    int64_t Size() const
    {
      return _values.size();
    }

    // Returns the number of elements in a sequence.
    int64_t Count()
    {
      return Size();
    }

    // Returns the number of elements in the specified sequence that satisfies a condition.
    int64_t Count(Delegate<bool(Source const&)> predicate)
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
    Source Max()
    {
      return Max<Source>(DefaultSelector(Source));
    }

    // Invokes a projection function on each element in a sequence and returns the maximum resulting value.
    template<typename Key>
    Source Max(Delegate<Key(Source const&)> selector)
    {
      auto max = std::numeric_limits<Key>::lowest();
      auto maxi = 0i64;
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        auto projected = selector(*_values[i]);
        if (projected > max)
        {
          max = projected;
          maxi = i;
        }
      }
      return *_values[maxi];
    }

    // Returns the minimum value in a sequence.
    Source Min()
    {
      return Min<Source>(DefaultSelector(Source));
    }

    // Invokes a projection function on each element in a sequence and returns the minimum resulting value.
    template<typename Key>
    Source Min(Delegate<Key(Source const&)> selector)
    {
      auto min = std::numeric_limits<Key>::max();
      auto mini = 0i64;
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        auto projected = selector(*_values[i]);
        if (projected < min)
        {
          min = projected;
          mini = i;
        }
      }
      return *_values[mini];
    }

    // Computes the sum of a sequence.
    Source Sum()
    {
      return Sum<Source>(DefaultSelector(Source));
    }

    // Computes the sum of a sequence of values that is obtained by invoking a projection function on each element of the input sequence.
    template<typename Key>
    Key Sum(Delegate<Key(Source const&)> selector)
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
    Enumerable<Result> StaticCast()
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
    Enumerable<Result> DynamicCast()
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
    Enumerable<Result> OfType()
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
    Source First()
    {
      return *_values[0];
    }

    // Returns the first element of a sequence that satisfies a specified condition.
    Source First(Delegate<bool(Source const&)> predicate)
    {
      return Where(predicate).First();
    }

    // Returns the first element of a sequence, or a default value if the sequence contains no elements.
    Source FirstOrDefault()
    {
      return Any() ? First() : Source();
    }

    // Returns the first element of a sequence that satisfies a specified condition or a default value if no such element is found.
    Source FirstOrDefault(Delegate<bool(Source const&)> predicate)
    {
      return Where(predicate).FirstOrDefault();
    }

    // Returns the last element of a sequence.
    Source Last()
    {
      return *_values[Size() - 1];
    }

    // Returns the last element of a sequence that satisfies a specified condition.
    Source Last(Delegate<bool(Source const&)> predicate)
    {
      return Where(predicate).Last();
    }

    // Returns the last element of a sequence, or a default value if the sequence contains no elements.
    Source LastOrDefault()
    {
      return Any() ? Last() : Source();
    }

    // Returns the last element of a sequence that satisfies a specified condition or a default value if no such element is found.
    Source LastOrDefault(Delegate<bool(Source const&)> predicate)
    {
      return Where(predicate).LastOrDefault();
    }

    // Returns the only element of a sequence, and throws an exception if there is not exactly one element in the sequence.
    Source Single()
    {
      if (Size() != 1) throw std::runtime_error("There is not exactly one element in the sequence!");
      return *_values[0];
    }

    // Returns the only element of a sequence that satisfies a specified condition, and throws an exception if more than one such element exists.
    Source Single(Delegate<bool(Source const&)> predicate)
    {
      return Where(predicate).Single();
    }

    // Returns the only element of a sequence, or a default value if the sequence is empty; this method throws an exception if there is more than one element in the sequence.
    Source SingleOrDefault()
    {
      if (Size() > 1) throw std::runtime_error("There is more than one element in the sequence!");
      return Any() ? *_values[0] : Source();
    }

    // Returns the only element of a sequence that satisfies a specified condition or a default value if no such element exists; this method throws an exception if more than one element satisfies the condition.
    Source SingleOrDefault(Delegate<bool(Source const&)> predicate)
    {
      return Where(predicate).SingleOrDefault();
    }

    // Bypasses a specified number of elements in a sequence and then returns the remaining elements.
    Enumerable<Source> Skip(int64_t count)
    {
      Enumerable<Source> result(_isReference);
      auto size = Size();
      for (int i = count; i < size; ++i)
      {
        result.Add(*_values[i]);
      }
      return result;
    }

    // Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements.
    Enumerable<Source> SkipWhile(Delegate<bool(Source const&)> predicate)
    {
      int64_t count = 0;
      auto size = Size();
      while (count < size && predicate(*_values[count])) count++;
      return Skip(count);
    }

    // Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements. The element's index is used in the logic of the predicate function.
    Enumerable<Source> SkipWhile(Delegate<bool(Source const&, int64_t)> predicate)
    {
      int64_t count = 0;
      auto size = Size();
      while (count < size && predicate(*_values[count], count)) count++;
      return Skip(count);
    }

    // Returns a specified number of contiguous elements from the start of a sequence.
    Enumerable<Source> Take(int64_t count)
    {
      Enumerable<Source> result(_isReference);
      auto size = Size();
      for (int i = 0; i < count && i < size; ++i)
      {
        result.Add(*_values[i]);
      }
      return result;
    }

    // Returns elements from a sequence as long as a specified condition is true. The element's index is used in the logic of the predicate function.
    Enumerable<Source> TakeWhile(Delegate<bool(Source const&)> predicate)
    {
      int64_t count = 0;
      auto size = Size();
      while (count < size && predicate(*_values[count])) count++;
      return Take(count);
    }

    // Returns elements from a sequence as long as a specified condition is true.
    Enumerable<Source> TakeWhile(Delegate<bool(Source const&, int64_t)> predicate)
    {
      int64_t count = 0;
      auto size = Size();
      while (count < size && predicate(*_values[count], count)) count++;
      return Take(count);
    }

    // Merges two sequences by using the specified predicate function.
    template<typename Result, typename Source2>
    Enumerable<Result> Zip(Enumerable<Source2> const& enumerable, Delegate<Result(Source const&, typename std::iterator_traits<decltype(enumerable.begin())>::value_type const&)> resultSelector)
    {
      Enumerable<Result> result(false);
      auto size = std::min(Size(), enumerable.Size());
      for (int i = 0; i < size; ++i)
      {
        result.Add(resultSelector(*_values[i], *enumerable._values[i]));
      }
      return result;
    }

    // Appends a value to the end of the sequence.
    Enumerable<Source> Append(Source const& value)
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
    Enumerable<Source> Prepend(Source const& value)
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
    bool SequenceEqual(Enumerable<Source> const& enumerable)
    {
      if (Size() != enumerable.Size()) return false;
      auto size = Size();
      for (int64_t i = 0; i < size; ++i)
      {
        if (*_values[i] != *enumerable._values[i])
          return false;
      }
      return true;
    }

    // Determines whether two sequences are equal by comparing their elements by using a specified equality comparer.
    bool SequenceEqual(Enumerable<Source> const& enumerable, Delegate<bool(Source const&, Source const&)> comparer)
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
    std::vector<Source> ToVector()
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
    std::map<Key, Value> ToMap(Delegate<Key(Source const&)> keySelector, Delegate<Value(Source const&)> valueSelector)
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

  template<typename Source>
  auto From(Enumerable<Source> const& values)
  {
    return values;
  }

  template<typename Iterable>
  auto From(Iterable const& values)
  {
    return Enumerable(values);
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
}