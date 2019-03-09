#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Foundation\Enumerable.h"

#include <array>
#include <vector>
#include <map>
#include <utility>
#include <type_traits>
#include <initializer_list>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace OpenGLUI::Foundation::Test
{
  TEST_CLASS(EnumerableTest)
  {
  private:
    static _CrtMemState _init;
    static _CrtMemState _dest;
    static _CrtMemState _result;

    template<typename T1, typename T2>
    bool Equal(T1 const& left, T2 const& right)
    {
      auto leftIt = left.begin();
      auto rightIt = right.begin();
      while (leftIt != left.end() || rightIt != right.end())
      {
        if (*leftIt != *rightIt)
          return false;
        leftIt++;
        rightIt++;
      }
      return true;
    }

    template<typename T1, typename T2>
    bool Equal(T1 const& left, T2 const& right, Delegate<bool(typename decltype(left.begin())::value_type const&, typename decltype(right.begin())::value_type const&)> comparer)
    {
      auto leftIt = left.begin();
      auto rightIt = right.begin();
      while (leftIt != left.end() || rightIt != right.end())
      {
        if (!comparer(*leftIt, *rightIt))
          return false;
        leftIt++;
        rightIt++;
      }
      return true;
    }
  public:
    TEST_METHOD(TestConstructor1)
    {
      auto values = Enumerable<int>();
      for (int i = 1; i <= 5; ++i) values.Add(i);
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4, 5 }, values));
    }

    TEST_METHOD(TestConstructor2)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      auto output1 = Enumerable(input);
      auto output2 = Enumerable(input, true);
      auto output3 = Enumerable(input, false);

      auto it0 = input.begin();
      auto it1 = output1.begin();
      auto it2 = output2.begin();
      auto it3 = output3.begin();
      while (it0 != input.end() && it1 != output1.end() && it2 != output2.end() && it3 != output3.end())
      {
        Assert::AreSame(*it0, *it1);
        Assert::AreEqual(*it0, *it1);
        Assert::AreSame(*it0, *it2);
        Assert::AreEqual(*it0, *it2);
        Assert::AreNotSame(*it0, *it3);
        Assert::AreEqual(*it0, *it3);
        it0++;
        it1++;
        it2++;
        it3++;
      }
    }

    TEST_METHOD(TestConstructor3)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      auto output1 = Enumerable(Enumerable(input, true));
      auto output2 = Enumerable(Enumerable(input, false));

      auto it0 = input.begin();
      auto it1 = output1.begin();
      auto it2 = output2.begin();
      while (it0 != input.end() && it1 != output1.end() && it2 != output2.end())
      {
        Assert::AreSame(*it0, *it1);
        Assert::AreEqual(*it0, *it1);
        Assert::AreNotSame(*it0, *it2);
        Assert::AreEqual(*it0, *it2);
        it0++;
        it1++;
        it2++;
      }
    }

    TEST_METHOD(TestConstructor4)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      auto output = Enumerable<int>({ 1, 2, 3, 4, 5 });

      auto it0 = input.begin();
      auto it1 = output.begin();
      while (it0 != input.end() && it1 != output.end())
      {
        Assert::AreNotSame(*it0, *it1);
        Assert::AreEqual(*it0, *it1);
        it0++;
        it1++;
      }
    }

    TEST_METHOD(TestSelect1)
    {
      auto input = array{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      auto expected = array{ 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 };

      auto output1 = From(input).Select<int>([](int const& value) { return value * value; });
      auto output2 = From(input).Select(Delegate{ [](int const& value) { return (double)value * value; } });
      static_assert(is_same_v<Enumerable<int>, decltype(output1)>, "The type of output1 shall be Enumerable<int>!");
      static_assert(is_same_v<Enumerable<double>, decltype(output2)>, "The type of output2 shall be Enumerable<double>!");
      Assert::IsTrue(Equal(expected, output1));
      Assert::IsTrue(Equal(expected, output2));
    }

    TEST_METHOD(TestSelect2)
    {
      auto input = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s, L"passionfruit"s, L"grape"s };
      auto expected = array<pair<int64_t, wstring>, 6>{ { { 0, L"apple"s }, { 1, L"banana"s }, { 2, L"mango"s }, { 3, L"orange"s }, { 4, L"passionfruit"s }, { 5, L"grape"s } }};

      auto output1 = From(input).Select<pair<int64_t, wstring>>([](wstring const& value, int64_t index) { return pair(index, value); });
      auto output2 = From(input).Select(Delegate{ [](wstring const& value, int64_t index) { return pair(index, value); } });
      static_assert(is_same_v<Enumerable<pair<int64_t, wstring>>, decltype(output1)>, "The type of output1 shall be Enumerable<pair<int64_t, wstring>>!");
      static_assert(is_same_v<Enumerable<pair<int64_t, wstring>>, decltype(output2)>, "The type of output2 shall be Enumerable<pair<int64_t, wstring>>!");
      Assert::IsTrue(Equal(expected, output1));
      Assert::IsTrue(Equal(expected, output2));
    }

    TEST_METHOD(TestSelectMany1)
    {
      struct PetOwner { wstring Name; vector<wstring> Pets; };
      auto input = array<PetOwner, 3>{{ { L"Higa, Sidney", { L"Scruffy", L"Sam" } },
                                        { L"Ashkenazi, Ronen", { L"Walker", L"Sugar" } },
                                        { L"Price, Vernette", { L"Scratches", L"Diesel" } } }};
      auto expected = array{ L"Scruffy", L"Sam", L"Walker", L"Sugar", L"Scratches", L"Diesel" };

      auto output = From(input).SelectMany<wstring>([](PetOwner const& owner) { return From(owner.Pets); });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestSelectMany2)
    {
      struct PetOwner { wstring Name; vector<wstring> Pets; };
      auto input = array<PetOwner, 4>{{ { L"Higa, Sidney", { L"Scruffy", L"Sam" } },
                                        { L"Ashkenazi, Ronen", { L"Walker", L"Sugar" } },
                                        { L"Price, Vernette", { L"Scratches", L"Diesel" } },
                                        { L"Hines, Patrick", { L"Dusty"} } }};
      auto expected = array{ L"0Scruffy", L"0Sam", L"1Walker", L"1Sugar", L"2Scratches", L"2Diesel", L"3Dusty" };

      auto output = From(input).SelectMany<wstring>([](PetOwner const& owner, int64_t index)
      {
        return From(owner.Pets).Select<wstring>([&](wstring const& name)
        {
          return to_wstring(index) + name;
        });
      });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestSelectMany3)
    {
      struct PetOwner { wstring Name; vector<wstring> Pets; };
      auto input = array<PetOwner, 3>{{ { L"Higa, Sidney", { L"Scruffy", L"Sam" } },
                                        { L"Ashkenazi, Ronen", { L"Walker", L"Sugar" } },
                                        { L"Price, Vernette", { L"Scratches", L"Diesel" } } }};
      auto expected = array{ 7, 3, 6, 5, 9, 6 };

      auto output = From(input).SelectMany<wstring, size_t>([](PetOwner const& owner) { return From(owner.Pets); }, [](wstring const& pet) { return pet.length(); });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestSelectMany4)
    {
      struct PetOwner { wstring Name; vector<wstring> Pets; };
      auto input = array<PetOwner, 4>{{ { L"Higa, Sidney", { L"Scruffy", L"Sam" } },
                                        { L"Ashkenazi, Ronen", { L"Walker", L"Sugar" } },
                                        { L"Price, Vernette", { L"Scratches", L"Diesel" } },
                                        { L"Hines, Patrick", { L"Dusty"} } }};
      auto expected = array{ 8, 4, 7, 6, 10, 7, 6 };

      auto output = From(input).SelectMany<wstring, size_t>([](PetOwner const& owner, int64_t index)
      {
        return From(owner.Pets).Select<wstring>([&](wstring const& name)
        {
          return to_wstring(index) + name;
        });
      }, [](wstring const& pet)
      {
        return pet.length();
      });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestWhere1)
    {
      auto input = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s, L"passionfruit"s, L"grape"s };
      auto expected = array{ L"apple"s, L"mango"s, L"grape"s };

      auto output = From(input).Where([](wstring const& value) { return value.length() < 6; });
      Assert::IsTrue(Equal(expected, output));

      auto ptrs = From(input)
        .Where([](wstring const& value) { return value.length() < 6; })               // It will be L"apple"s, L"mango"s and L"grape"s
        .Select<const wchar_t*>([](wstring const& value) { return value.c_str(); })   // It will be L"apple", L"mango" and L"grape"
        .Where([](const wchar_t *const& str) { return wstring(str) == L"grape"; });   // It will be L"grape"
      Assert::AreEqual(input[5].c_str(), ptrs.ElementAt(0));                          // The pointers shall be equal
    }

    TEST_METHOD(TestWhere2)
    {
      auto input = array{ 0, 30, 20, 15, 90, 85, 40, 75 };
      auto expected = array{ 0, 20, 15, 40 };

      auto output = From(input).Where([](int const& value, int64_t index) { return value <= index * 10; });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestJoin1)
    {
      struct Customer { int Id; wstring Name; };
      struct Order { int Id; double Amount; int CustomerId; };

      auto customers = vector<Customer>
      {
        { 1, L"George Washington" },
        { 2, L"John Adams" },
        { 3, L"Thomas Jefferson" },
        { 4, L"James Madison" },
        { 5, L"James Monroe" }
      };
      auto orders = vector<Order>
      {
        { 1, 234.56, 1 },
        { 2, 78.50, 3 },
        { 3, 124.00, 2 },
        { 4, 65.50, 3 },
        { 5, 25.50, 10 },
        { 6, 14.40, 9 }
      };
      auto expected = vector<pair<wstring, double>>
      {
        { L"George Washington", 234.56 },
        { L"John Adams", 124.00 },
        { L"Thomas Jefferson", 78.50 },
        { L"Thomas Jefferson", 65.50 }
      };

      auto output = From(customers).Join<Order, int, pair<wstring, double>>(
        From(orders),
        [](Customer const& customer) { return customer.Id; }, 
        [](Order const& order) { return order.CustomerId; },
        [](Customer const& customer, Order const& order) { return pair(customer.Name, order.Amount); }
      );
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestJoin2)
    {
      struct Customer { int Id; wstring Name; };
      struct Order { int Id; double Amount; int CustomerId; };

      auto customers = vector<Customer>
      {
        { 1, L"George Washington" },
        { 2, L"John Adams" },
        { 3, L"Thomas Jefferson" },
        { 4, L"James Madison" },
        { 5, L"James Monroe" }
      };
      auto orders = vector<Order>
      {
        { 1, 234.56, 1 },
        { 2, 78.50, 3 },
        { 3, 124.00, 2 },
        { 4, 65.50, 3 },
        { 5, 25.50, 10 },
        { 6, 14.40, 9 }
      };
      auto expected = vector<pair<wstring, double>>
      {
        { L"George Washington", 234.56 },
        { L"John Adams", 124.00 },
        { L"Thomas Jefferson", 78.50 },
        { L"Thomas Jefferson", 65.50 }
      };

      auto output = From(customers).Join<Order, int, pair<wstring, double>>(
        From(orders),
        [](Customer const& customer) { return customer.Id; },
        [](Order const& order) { return order.CustomerId; },
        [](Customer const& customer, Order const& order) { return pair(customer.Name, order.Amount); },
        [](int const& leftId, int const& rightId) { return leftId == rightId; }
      );
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestGroupJoin1)
    {
      struct Customer { int Id; wstring Name; };
      struct Order { int Id; double Amount; int CustomerId; };

      auto customers = vector<Customer>
      {
        { 1, L"George Washington" },
        { 2, L"John Adams" },
        { 3, L"Thomas Jefferson" },
        { 4, L"James Madison" },
        { 5, L"James Monroe" }
      };
      auto orders = vector<Order>
      {
        { 1, 234.56, 1 },
        { 2, 78.50, 3 },
        { 3, 124.00, 2 },
        { 4, 65.50, 3 },
        { 5, 25.50, 10 },
        { 6, 14.40, 9 }
      };
      auto expected = vector<pair<wstring, vector<double>>>
      {
        { L"George Washington", { 234.56 } },
        { L"John Adams", { 124.00 } },
        { L"Thomas Jefferson", { 78.50, 65.50 } }
      };

      auto output = From(customers).GroupJoin<Order, int, pair<wstring, vector<double>>>(
        From(orders),
        [](Customer const& customer) { return customer.Id; },
        [](Order const& order) { return order.CustomerId; },
        [](Customer const& customer, Enumerable<Order> const& orders) { return pair(customer.Name, orders.Select<double>([](Order const& order) { return order.Amount; }).ToVector()); }
      );
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestGroupJoin2)
    {
      struct Customer { int Id; wstring Name; };
      struct Order { int Id; double Amount; int CustomerId; };

      auto customers = vector<Customer>
      {
        { 1, L"George Washington" },
        { 2, L"John Adams" },
        { 3, L"Thomas Jefferson" },
        { 4, L"James Madison" },
        { 5, L"James Monroe" }
      };
      auto orders = vector<Order>
      {
        { 1, 234.56, 1 },
        { 2, 78.50, 3 },
        { 3, 124.00, 2 },
        { 4, 65.50, 3 },
        { 5, 25.50, 10 },
        { 6, 14.40, 9 }
      };
      auto expected = vector<pair<wstring, vector<double>>>
      {
        { L"George Washington", { 234.56 } },
        { L"John Adams", { 124.00 } },
        { L"Thomas Jefferson", { 78.50, 65.50 } }
      };

      auto output = From(customers).GroupJoin<Order, int, pair<wstring, vector<double>>>(
        From(orders),
        [](Customer const& customer) { return customer.Id; },
        [](Order const& order) { return order.CustomerId; },
        [](Customer const& customer, Enumerable<Order> const& orders) { return pair(customer.Name, orders.Select<double>([](Order const& order) { return order.Amount; }).ToVector()); },
        [](int const& left, int const& right) { return left == right; }
      );
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestAll)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Barley", 10 }, { L"Boots", 4 }, { L"Whiskers", 6 } }};

      Assert::IsFalse(From(input).All([](Pet const& pet) { return pet.Name[0] == L'B'; }));
      Assert::IsTrue(From(input).All([](Pet const& pet) { return pet.Age >= 4; }));
      Assert::IsTrue(From(input).All([](Pet const& pet) { return pet.Age <= 10; }));
      Assert::IsFalse(From(input).All([](Pet const& pet) { return pet.Age <= 4; }));
      Assert::IsFalse(From(input).All([](Pet const& pet) { return pet.Age >= 10; }));
    }

    TEST_METHOD(TestAny1)
    {
      Assert::IsFalse(From(array<int, 0>{ }).Any());
      Assert::IsTrue(From(array<int, 1>{ 1 }).Any());
      Assert::IsTrue(From(array<int, 2>{ 1, 2 }).Any());
    }

    TEST_METHOD(TestAny2)
    {
      struct Pet { wstring Name; int Age; bool Vaccinated; };
      auto input = array<Pet, 3>{ { { L"Barley", 8, true }, { L"Boots", 4, false }, { L"Whiskers", 1, false } }};

      Assert::IsFalse(From(input).Any([](Pet const& pet) { return pet.Age > 5 && !pet.Vaccinated; }));
      Assert::IsTrue(From(input).Any([](Pet const& pet) { return pet.Age < 5 && !pet.Vaccinated; }));
    }

    TEST_METHOD(TestContains1)
    {
      auto input = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s, L"passionfruit"s, L"grape"s };

      Assert::IsFalse(From(input).Contains(L"non existent"));
      Assert::IsTrue(From(input).Contains(L"mango"));
    }

    TEST_METHOD(TestContains2)
    {
      auto input = array{ L"apple", L"banana", L"mango", L"orange", L"passionfruit", L"grape" };
      auto mango = L"mango"s;

      Assert::IsFalse(From(input).Contains(mango.c_str())); // Compares pointers, which differs
      Assert::IsTrue(From(input).Contains(mango.c_str(),    // Compares data pointed by the pointer, which does not differ
        [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) == wstring(right); }));
    }

    TEST_METHOD(TestConcat)
    {
      struct Pet { wstring Name; int Age; };
      auto cats = array<Pet, 3>{ { { L"Barley", 8 }, { L"Boots", 4 }, { L"Whiskers", 1 } }};
      auto dogs = array<Pet, 3>{ { { L"Bounder", 3 }, { L"Snoopy", 14 }, { L"Fido", 9 } }};
      auto expected = array{ L"Barley"s, L"Boots"s, L"Whiskers"s, L"Bounder"s, L"Snoopy"s, L"Fido"s };

      auto output = From(cats).Select<wstring>([](Pet const& pet) { return pet.Name; })
        .Concat(From(dogs).Select<wstring>([](Pet const& pet) { return pet.Name; }));
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestDefaultIfEmpty1)
    {
      auto input1 = array{ 1, 3, 3, 7 };
      auto input2 = array<int, 0>{};
      auto expected1 = array{ 1, 3, 3, 7 };
      auto expected2 = array{ 0 };

      Assert::IsTrue(Equal(expected1, From(input1).DefaultIfEmpty()));
      Assert::IsTrue(Equal(expected2, From(input2).DefaultIfEmpty()));
    }

    TEST_METHOD(TestDefaultIfEmpty2)
    {
      auto input1 = array{ 1, 3, 3, 7 };
      auto input2 = array<int, 0>{};
      auto expected1 = array{ 1, 3, 3, 7 };
      auto expected2 = array{ 1337 };

      Assert::IsTrue(Equal(expected1, From(input1).DefaultIfEmpty(1337)));
      Assert::IsTrue(Equal(expected2, From(input2).DefaultIfEmpty(1337)));
    }

    TEST_METHOD(TestDistinct1)
    {
      auto input = array{ 21, 46, 46, 55, 17, 21, 55, 55 };
      auto expected = array{ 21, 46, 55, 17 };

      auto output = From(input).Distinct();
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestDistinct2)
    {
      auto fruits = array{ L"apple"s, L"apple"s, L"mango"s, L"orange"s, L"apple"s, L"mango"s };
      auto input = From(fruits).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });
      auto expected1 = array{ fruits[0].c_str(), fruits[1].c_str(), fruits[2].c_str(), fruits[3].c_str(), fruits[4].c_str(), fruits[5].c_str() };
      auto expected2 = array{ fruits[0].c_str(),fruits[2].c_str(), fruits[3].c_str() };

      auto output1 = From(input).Distinct();    // Compares pointers, which differs
      auto output2 = From(input).Distinct(      // Compares data pointed by the pointer, which does not differ
        [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) == wstring(right); });
      Assert::IsTrue(Equal(expected1, output1));
      Assert::IsTrue(Equal(expected2, output2));
    }

    TEST_METHOD(TestExcept1)
    {
      auto input = array{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      auto expected = array{ 1, 3, 5, 7, 9 };

      auto output = From(input).Except({ 2, 4, 6, 8, 10 });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestExcept2)
    {
      auto fruits = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s, L"passionfruit"s, L"grape"s };
      auto input = From(fruits).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });
      auto expected1 = array{ fruits[0].c_str(), fruits[1].c_str(), fruits[2].c_str(), fruits[3].c_str(), fruits[4].c_str(), fruits[5].c_str() };
      auto expected2 = array{ fruits[0].c_str(),fruits[2].c_str(), fruits[3].c_str() };

      auto output1 = From(input).Except({ L"banana", L"passionfruit", L"grape" });
      auto output2 = From(input).Except({ L"banana", L"passionfruit", L"grape" }, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) == wstring(right); });
      Assert::IsTrue(Equal(expected1, output1));
      Assert::IsTrue(Equal(expected2, output2));
    }

    TEST_METHOD(TestIntersect1)
    {
      auto input1 = array{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
      auto input2 = array{ 3, 4, 4, 5, 6, 6, 7 };
      auto expected = array{ 3, 4, 5 };

      auto output = From(input1).Intersect(input2);
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestIntersect2)
    {
      auto fruits1 = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s };
      auto fruits2 = array{ L"mango"s, L"orange"s, L"passionfruit"s, L"grape"s };
      auto input1 = From(fruits1).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });
      auto input2 = From(fruits2).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });
      auto expected1 = array<const wchar_t*, 0>{ };
      auto expected2 = array{ fruits1[2].c_str(), fruits1[3].c_str() };

      auto output1 = From(input1).Intersect(input2);
      auto output2 = From(input1).Intersect(input2, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) == wstring(right); });
      Assert::IsTrue(Equal(expected1, output1));
      Assert::IsTrue(Equal(expected2, output2));
    }

    TEST_METHOD(TestUnion1)
    {
      auto input1 = array{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
      auto input2 = array{ 3, 4, 4, 5, 6, 6, 7 };
      auto expected = array{ 1, 2, 3, 4, 5, 6, 7 };

      auto output = From(input1).Union(input2);
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestUnion2)
    {
      auto fruits1 = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s };
      auto fruits2 = array{ L"mango"s, L"orange"s, L"passionfruit"s, L"grape"s };
      auto input1 = From(fruits1).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });
      auto input2 = From(fruits2).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });
      auto expected1 = array{ fruits1[0].c_str(), fruits1[1].c_str(), fruits1[2].c_str(), fruits1[3].c_str(), fruits2[0].c_str(), fruits2[1].c_str(), fruits2[2].c_str(), fruits2[3].c_str() };
      auto expected2 = array{ fruits1[0].c_str(), fruits1[1].c_str(), fruits1[2].c_str(), fruits1[3].c_str(), fruits2[2].c_str(), fruits2[3].c_str() };

      auto output1 = From(input1).Union(input2);
      auto output2 = From(input1).Union(input2, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) == wstring(right); });
      Assert::IsTrue(Equal(expected1, output1));
      Assert::IsTrue(Equal(expected2, output2));
    }

    TEST_METHOD(TestOrderBy1)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Barley", 8 }, { L"Boots", 4 }, { L"Whiskers", 1 } }};
      auto expected1 = array<Pet, 3>{ { { L"Whiskers", 1 }, { L"Boots", 4 }, { L"Barley", 8 } }};
      auto expected2 = array<Pet, 3>{ { { L"Barley", 8 }, { L"Boots", 4 }, { L"Whiskers", 1 } }};

      auto output1 = From(input).OrderBy<int>([](Pet const& pet) { return pet.Age; });
      auto output2 = From(input).OrderBy<wstring>([](Pet const& pet) { return wstring(pet.Name); });
      Assert::IsTrue(Equal(expected1, output1, [](Pet const& left, Pet const& right) { return left.Name == right.Name && left.Age == right.Age; }));
      Assert::IsTrue(Equal(expected2, output2, [](Pet const& left, Pet const& right) { return left.Name == right.Name && left.Age == right.Age; }));
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 0 }, { 1, 1 }, { 2, 2 } }, output1._ranges));
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 0 }, { 1, 1 }, { 2, 2 } }, output2._ranges));
    }

    TEST_METHOD(TestOrderBy2)
    {
      //                                        Boots                                       Whiskers                                        Barley
      const wchar_t letters[] = { L'B', L'o', L'o', L't', L's', L'\0', L'W', L'h', L'i', L's', L'k', L'e', L'r', L's', L'\0', L'B', L'a', L'r', L'l', L'e', L'y', L'\0' };
      const wchar_t* boots = &letters[0];
      const wchar_t* whiskers = &letters[6];
      const wchar_t* barley = &letters[15];

      Assert::IsTrue(boots < whiskers);
      Assert::IsTrue(whiskers < barley); // It compares the pointer address

      struct Pet { const wchar_t* Name; int Age; };
      auto input = array<Pet, 4>{ { { boots, 4 }, { barley, 1 }, { whiskers, 8 }, { barley, 2 } }};
      auto expected1 = array<Pet, 4>{ { { boots, 4 }, { whiskers, 8 }, { barley, 1 }, { barley, 2 } }};  // Order when comparing memory addresses, it shall have the same order as the order in letters[]
      auto expected2 = array<Pet, 4>{ { { barley, 1 }, { barley, 2 }, { boots, 4 }, { whiskers, 8 } }};  // Order when comparing content

      auto output1 = From(input).OrderBy<const wchar_t*>([](Pet const& pet) { return pet.Name; });
      auto output2 = From(input).OrderBy<const wchar_t*>([](Pet const& pet) { return pet.Name; }, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) < wstring(right); });
      Assert::IsTrue(Equal(expected1, output1, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      Assert::IsTrue(Equal(expected2, output2, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 0 }, { 1, 1 }, { 2, 3 } }, output1._ranges));
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 1 }, { 2, 2 }, { 3, 3 } }, output2._ranges));
    }

    TEST_METHOD(TestOrderByDescending1)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Whiskers", 1 }, { L"Boots", 4 }, { L"Barley", 8 } }};
      auto expected = array<Pet, 3>{ { { L"Barley", 8 }, { L"Boots", 4 }, { L"Whiskers", 1 } }};

      auto output = From(input).OrderByDescending<int>([](Pet const& pet) { return pet.Age; });
      Assert::IsTrue(Equal(expected, output, [](Pet const& left, Pet const& right) { return left.Name == right.Name && left.Age == right.Age; }));
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 0 }, { 1, 1 }, { 2, 2 } }, output._ranges));
    }

    TEST_METHOD(TestOrderByDescending2)
    {
      //                                        Boots                                       Whiskers                                        Barley
      const wchar_t letters[] = { L'B', L'o', L'o', L't', L's', L'\0', L'W', L'h', L'i', L's', L'k', L'e', L'r', L's', L'\0', L'B', L'a', L'r', L'l', L'e', L'y', L'\0' };
      const wchar_t* boots = &letters[0];
      const wchar_t* whiskers = &letters[6];
      const wchar_t* barley = &letters[15];

      Assert::IsTrue(boots < whiskers);
      Assert::IsTrue(whiskers < barley); // It compares the pointer address

      struct Pet { const wchar_t* Name; int Age; };
      auto input = array<Pet, 4>{ { { boots, 4 }, { barley, 1 }, { whiskers, 8 }, { barley, 2 } }};
      auto expected1 = array<Pet, 4>{ { { barley, 2 }, { barley, 1 }, { whiskers, 8 }, { boots, 4 } }};  // Order when comparing memory addresses, it shall have the same order as the order in letters[]
      auto expected2 = array<Pet, 4>{ { { whiskers, 8 }, { boots, 4 }, { barley, 2 }, { barley, 1 } }};  // Order when comparing content

      auto output1 = From(input).OrderByDescending<const wchar_t*>([](Pet const& pet) { return pet.Name; });
      auto output2 = From(input).OrderByDescending<const wchar_t*>([](Pet const& pet) { return pet.Name; }, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) < wstring(right); });
      Assert::IsTrue(Equal(expected1, output1, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      Assert::IsTrue(Equal(expected2, output2, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 0 }, { 1, 1 }, { 2, 3 } }, output1._ranges));
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 1 }, { 2, 2 }, { 3, 3 } }, output2._ranges));
    }

    TEST_METHOD(TestOrderByThenBy1)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 10>{ { { L"f", 3 }, { L"h", 5 }, { L"d", 2 }, { L"c", 1 }, { L"j", 6 }, { L"b", 1 }, { L"g", 4 }, { L"a", 1 }, { L"e", 3 }, { L"i", 6 } }};
      auto expected = array<Pet, 10>{ { { L"a", 1 }, { L"b", 1 }, { L"c", 1 }, { L"d", 2 }, { L"e", 3 }, { L"f", 3 }, { L"g", 4 }, { L"h", 5 }, { L"i", 6 }, { L"j", 6 } }};

      auto intermediate = From(input).OrderBy<int>([](Pet const& pet) { return pet.Age; });
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 2 }, { 3, 3 }, { 4, 5 }, { 6, 6 }, { 7, 7 }, { 8, 9 } }, intermediate._ranges));
      auto output = intermediate.ThenBy<wstring>([](Pet const& pet) { return wstring(pet.Name); });
      Assert::IsTrue(Equal(expected, output, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      auto ranges = Enumerable<int64_t>::Range(0, 10).Select<pair<int64_t, int64_t>>([](int64_t const& value) { return pair(value, value); });  // { 0, 0 }, { 1, 1 }, { 2, 2} etc...
      Assert::IsTrue(Equal(ranges, output._ranges));
    }

    TEST_METHOD(TestOrderByThenBy2)
    {
      struct Pet { const wchar_t* Name; int Age; };
      auto input = array<Pet, 10>{ { { L"f", 3 }, { L"h", 5 }, { L"d", 2 }, { L"c", 1 }, { L"j", 6 }, { L"b", 1 }, { L"g", 4 }, { L"a", 1 }, { L"e", 3 }, { L"i", 6 } }};
      auto expected = array<Pet, 10>{ { { L"a", 1 }, { L"b", 1 }, { L"c", 1 }, { L"d", 2 }, { L"e", 3 }, { L"f", 3 }, { L"g", 4 }, { L"h", 5 }, { L"i", 6 }, { L"j", 6 } }};

      auto intermediate = From(input).OrderBy<int>([](Pet const& pet) { return pet.Age; });
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 2 }, { 3, 3 }, { 4, 5 }, { 6, 6 }, { 7, 7 }, { 8, 9 } }, intermediate._ranges));
      auto output = intermediate.ThenBy<const wchar_t*>([](Pet const& pet) { return pet.Name; }, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) < wstring(right); });
      Assert::IsTrue(Equal(expected, output, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      auto ranges = Enumerable<int64_t>::Range(0, 10).Select<pair<int64_t, int64_t>>([](int64_t const& value) { return pair(value, value); });  // { 0, 0 }, { 1, 1 }, { 2, 2} etc...
      Assert::IsTrue(Equal(ranges, output._ranges));
    }

    TEST_METHOD(TestOrderByThenByDescending1)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 10>{ { { L"f", 3 }, { L"h", 5 }, { L"d", 2 }, { L"c", 1 }, { L"j", 6 }, { L"b", 1 }, { L"g", 4 }, { L"a", 1 }, { L"e", 3 }, { L"i", 6 } }};
      auto expected = array<Pet, 10>{ { { L"c", 1 }, { L"b", 1 }, { L"a", 1 }, { L"d", 2 }, { L"f", 3 }, { L"e", 3 }, { L"g", 4 }, { L"h", 5 }, { L"j", 6 }, { L"i", 6 } }};

      auto intermediate = From(input).OrderBy<int>([](Pet const& pet) { return pet.Age; });
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 2 }, { 3, 3 }, { 4, 5 }, { 6, 6 }, { 7, 7 }, { 8, 9 } }, intermediate._ranges));
      auto output = intermediate.ThenByDescending<wstring>([](Pet const& pet) { return wstring(pet.Name); });
      Assert::IsTrue(Equal(expected, output, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      auto ranges = Enumerable<int64_t>::Range(0, 10).Select<pair<int64_t, int64_t>>([](int64_t const& value) { return pair(value, value); });  // { 0, 0 }, { 1, 1 }, { 2, 2} etc...
      Assert::IsTrue(Equal(ranges, output._ranges));
    }

    TEST_METHOD(TestOrderByThenByDescending2)
    {
      struct Pet { const wchar_t* Name; int Age; };
      auto input = array<Pet, 10>{ { { L"f", 3 }, { L"h", 5 }, { L"d", 2 }, { L"c", 1 }, { L"j", 6 }, { L"b", 1 }, { L"g", 4 }, { L"a", 1 }, { L"e", 3 }, { L"i", 6 } }};
      auto expected = array<Pet, 10>{ { { L"c", 1 }, { L"b", 1 }, { L"a", 1 }, { L"d", 2 }, { L"f", 3 }, { L"e", 3 }, { L"g", 4 }, { L"h", 5 }, { L"j", 6 }, { L"i", 6 } }};

      auto intermediate = From(input).OrderBy<int>([](Pet const& pet) { return pet.Age; });
      Assert::IsTrue(Equal(vector<pair<int64_t, int64_t>>{ { 0, 2 }, { 3, 3 }, { 4, 5 }, { 6, 6 }, { 7, 7 }, { 8, 9 } }, intermediate._ranges));
      auto output = intermediate.ThenByDescending<const wchar_t*>([](Pet const& pet) { return pet.Name; }, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) < wstring(right); });
      Assert::IsTrue(Equal(expected, output, [](Pet const& left, Pet const& right) { return wstring(left.Name) == wstring(right.Name) && left.Age == right.Age; }));
      auto ranges = Enumerable<int64_t>::Range(0, 10).Select<pair<int64_t, int64_t>>([](int64_t const& value) { return pair(value, value); });  // { 0, 0 }, { 1, 1 }, { 2, 2} etc...
      Assert::IsTrue(Equal(ranges, output._ranges));
    }

    TEST_METHOD(TestReverse)
    {
      Assert::IsTrue(From({ 1, 2, 3, 4, 5 }).Reverse().SequenceEqual({ 5, 4, 3, 2, 1 }));
      Assert::IsTrue(From({ 1.0, 3.0, 2.0 }).Reverse().SequenceEqual({ 2.0, 3.0, 1.0 }));
    }

    TEST_METHOD(TestAggregate1)
    {
      Assert::AreEqual(15, From({ 1, 2, 3, 4, 5 }).Aggregate([](int const& current, int const& next) { return current + next; }));
      Assert::AreEqual(L"a,b,c,d,e"s, From({ L"a"s, L"b"s, L"c"s, L"d"s, L"e"s }).Aggregate([](wstring const& current, wstring const& next) { return current + L"," + next; }));
    }

    TEST_METHOD(TestAggregate2)
    {
      Assert::AreEqual(25, From({ 1, 2, 3, 4, 5 }).Aggregate<int>(10, [](int const& current, int const& next) { return current + next; }));
      Assert::AreEqual(L"z,a,b,c,d,e"s, From({ L"a"s, L"b"s, L"c"s, L"d"s, L"e"s }).Aggregate<wstring>(L"z"s, [](wstring const& current, wstring const& next) { return current + L"," + next; }));
    }

    TEST_METHOD(TestAggregate3)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Whiskers", 1 }, { L"Boots", 4 }, { L"Barley", 8 } }};
      auto expected1 = to_wstring(input[0].Age + input[1].Age + input[2].Age);
      auto expected2 = (input[0].Name + input[1].Name + input[2].Name).length();

      auto output1 = From(input).Aggregate<int, wstring>(0, [](int const& current, Pet const& next) { return current + next.Age; }, [](int const& accumulation) { return to_wstring(accumulation); });
      auto output2 = From(input).Aggregate<wstring, size_t>(L""s, [](wstring const& current, Pet const& next) { return current + next.Name; }, [](wstring const& accumulation) { return accumulation.length(); });
      Assert::AreEqual(expected1, output1);
      Assert::AreEqual(expected2, output2);
    }

    TEST_METHOD(TestAverage)
    {
      Assert::AreEqual((1.5 + 2.5 + 3.5) / 3.0, From({ 1.5, 2.5, 3.5 }).Average());
    }

    TEST_METHOD(TestAverage2)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Whiskers", 1 }, { L"Boots", 4 }, { L"Barley", 8 } }};

      Assert::AreEqual((1.0 + 4.0 + 8.0) / 3.0, From(input).Average<int>([](Pet const& pet) { return pet.Age; }));
    }

    TEST_METHOD(TestSize)
    {
      Assert::AreEqual((int)0, (int)From(array<int, 0>{}).Size());
      Assert::AreEqual((int)1, (int)From({ 1 }).Size());
      Assert::AreEqual((int)2, (int)From({ 1, 2 }).Size());
      Assert::AreEqual((int)3, (int)From({ 1, 2, 3 }).Size());
    }

    TEST_METHOD(TestCount)
    {
      Assert::AreEqual((int)0, (int)From(array<int, 0>{}).Count());
      Assert::AreEqual((int)1, (int)From({ 1 }).Count());
      Assert::AreEqual((int)2, (int)From({ 1, 2 }).Count());
      Assert::AreEqual((int)3, (int)From({ 1, 2, 3 }).Count());
    }

    TEST_METHOD(TestCount2)
    {
      Assert::AreEqual((int)5, (int)From({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }).Count([](int const& value) { return value % 2 == 0; }));
    }

    TEST_METHOD(TestMax1)
    {
      Assert::AreEqual(1337, From({ 1, 2, 3, 1337, 4, 5, 6, 7 }).Max());
      Assert::AreEqual(-4.5, From({ -10.2, -5.6, -4.5, -7.3, -1337.0, -17.0, -6.0, -7.0 }).Max());
    }

    TEST_METHOD(TestMax2)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Whiskers", 1 }, { L"Boots", 4 }, { L"Barley", 8 } }};
      auto expected = Pet{ L"Barley", 8 };

      auto output = From(input).Max<int>([](Pet const& pet) { return pet.Age; });
      Assert::IsTrue(Equal(array{ expected }, array{ output }, [](Pet const& left, Pet const& right) { return left.Name == right.Name && left.Age == right.Age; }));
    }

    TEST_METHOD(TestMin1)
    {
      Assert::AreEqual(1, From({ 1, 2, 3, 1337, 4, 5, 6, 7 }).Min());
      Assert::AreEqual(-1337.0, From({ -10.2, -5.6, -4.5, -7.3, -1337.0, -17.0, -6.0, -7.0 }).Min());
    }

    TEST_METHOD(TestMin2)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Whiskers", 1 }, { L"Boots", 4 }, { L"Barley", 8 } }};
      auto expected = Pet{ L"Whiskers", 1 };

      auto output = From(input).Min<int>([](Pet const& pet) { return pet.Age; });
      Assert::IsTrue(Equal(array{ expected }, array{ output }, [](Pet const& left, Pet const& right) { return left.Name == right.Name && left.Age == right.Age; }));
    }

    TEST_METHOD(TestSum1)
    {
      Assert::AreEqual(15, From({ 1, 2, 3, 4, 5 }).Sum());
      Assert::AreEqual(15, From({ 5, 4, 3, 2, 1 }).Sum());
      Assert::AreEqual(5, From({ 1, 1, 1, 1, 1 }).Sum());
      Assert::AreEqual(3, From({ 1, -2, 3, -4, 5 }).Sum());
      Assert::AreEqual(17.5f, From({ 1.5f, 2.5f, 3.5f, 4.5f, 5.5f }).Sum());
      Assert::AreEqual(17.5, From({ 1.5, 2.5, 3.5, 4.5, 5.5 }).Sum());
      Assert::AreEqual(L"abcd"s, From({ L"a"s, L"b"s, L"c"s, L"d"s }).Sum());
    }

    TEST_METHOD(TestSum2)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Whiskers", 1 }, { L"Boots", 4 }, { L"Barley", 8 } }};

      Assert::AreEqual(13, From(input).Sum<int>([](Pet const& pet) { return pet.Age; }));
      Assert::AreEqual(13, From(input).Sum(Delegate{ [](Pet const& pet) { return pet.Age; } }));
      Assert::AreEqual(L"WhiskersBootsBarley"s, From(input).Sum<wstring>([](Pet const& pet) { return pet.Name; }));
      Assert::AreEqual(L"WhiskersBootsBarley"s, From(input).Sum(Delegate{ [](Pet const& pet) { return pet.Name; } }));
    }

    TEST_METHOD(TestStaticCast)
    {
      auto input = array{ 1.9, 2.9, 3.9 };
      auto expected = array{ 1, 2, 3 };

      auto output = From(input).StaticCast<int>();
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestDynamicCast)
    {
      struct Base { wstring A; int B; Base(wstring a, int b) : A(a), B(b) {} virtual ~Base() {} };      // virtual to make it polymorphic
      struct Derived : Base { wstring C; Derived(wstring a, int b, wstring c) : Base(a, b), C(c) {} };
      auto input = vector<Base*>{ new Derived{ L"a0", 0, L"b0" }, new Derived{ L"a1", 1, L"b1" } };
      auto expected = vector<Derived*>{ dynamic_cast<Derived*>(input[0]), dynamic_cast<Derived*>(input[1]) };

      auto output = From(input).DynamicCast<Derived*>();
      Assert::IsTrue(Equal(expected, output, [](Derived *const& left, Derived *const& right) { return (left->A == right->A) && (left->B == right->B) && (left->C == right->C); }));
      for (auto& ptr : input) { delete ptr; }
    }

    TEST_METHOD(TestOfType)
    {
      struct Base { virtual int Type() = 0; };
      struct Derived1 : Base { int Type() override { return 1; } };
      struct Derived2 : Base { int Type() override { return 2; } };
      auto input = vector<Base*>{ new Derived1(), new Derived1(), new Derived1(), new Derived2(), new Derived2() };

      auto output1 = From(input).OfType<Derived1*>();
      auto output2 = From(input).OfType<Derived2*>();
      Assert::AreEqual(3, (int)output1.Count());
      Assert::AreEqual(2, (int)output2.Count());
      Assert::IsTrue(output1.All([](Derived1 *const& derived) { return derived->Type() == 1; }));
      Assert::IsTrue(output2.All([](Derived2 *const& derived) { return derived->Type() == 2; }));
      for (auto& ptr : input) { delete ptr; }
    }

    TEST_METHOD(TestElementAt)
    {
      auto input = array{ 1, 2, 3 };

      Assert::ExpectException<out_of_range>([&]() { From(input).ElementAt(-1); });
      Assert::AreEqual(1, From(input).ElementAt(0));
      Assert::AreEqual(2, From(input).ElementAt(1));
      Assert::AreEqual(3, From(input).ElementAt(2));
      Assert::ExpectException<out_of_range>([&]() { From(input).ElementAt(3); });
    }

    TEST_METHOD(TestElementAtOrDefault)
    {
      auto input = array{ 1, 2, 3 };

      Assert::AreEqual(decltype(input)::value_type(), From(input).ElementAtOrDefault(-1));
      Assert::AreEqual(1, From(input).ElementAtOrDefault(0));
      Assert::AreEqual(2, From(input).ElementAtOrDefault(1));
      Assert::AreEqual(3, From(input).ElementAtOrDefault(2));
      Assert::AreEqual(decltype(input)::value_type(), From(input).ElementAtOrDefault(3));
    }

    TEST_METHOD(TestFirst1)
    {
      Assert::AreEqual(1, From({ 1, 2, 3 }).First());
      Assert::AreEqual(L"a"s, From({ L"a"s, L"b"s, L"c"s }).First());
    }

    TEST_METHOD(TestFirst2)
    {
      Assert::AreEqual(2, From({ 1, 2, 3 }).First([](int const& value) { return value > 1; }));
      Assert::AreEqual(L"b"s, From({ L"a"s, L"b"s, L"c"s }).First([](wstring const& value) { return value > L"a"; }));
    }

    TEST_METHOD(TestFirstOrDefault1)
    {
      Assert::AreEqual(1, From({ 1, 2, 3 }).FirstOrDefault());
      Assert::AreEqual(L"a"s, From({ L"a"s, L"b"s, L"c"s }).FirstOrDefault());
      Assert::AreEqual(int(), From(array<int, 0>{ }).FirstOrDefault());
      Assert::AreEqual(wstring(), From(array<wstring, 0>{ }).FirstOrDefault());
    }

    TEST_METHOD(TestFirstOrDefault2)
    {
      Assert::AreEqual(2, From({ 1, 2, 3 }).FirstOrDefault([](int const& value) { return value > 1; }));
      Assert::AreEqual(L"b"s, From({ L"a"s, L"b"s, L"c"s }).FirstOrDefault([](wstring const& value) { return value > L"a"; }));
      Assert::AreEqual(int(), From(array<int, 0>{ }).FirstOrDefault([](int const& value) { return value > 1; }));
      Assert::AreEqual(wstring(), From(array<wstring, 0>{ }).FirstOrDefault([](wstring const& value) { return value > L"a"; }));
    }

    TEST_METHOD(TestLast1)
    {
      Assert::AreEqual(3, From({ 1, 2, 3 }).Last());
      Assert::AreEqual(L"c"s, From({ L"a"s, L"b"s, L"c"s }).Last());
    }

    TEST_METHOD(TestLast2)
    {
      Assert::AreEqual(2, From({ 1, 2, 3 }).Last([](int const& value) { return value < 3; }));
      Assert::AreEqual(L"b"s, From({ L"a"s, L"b"s, L"c"s }).Last([](wstring const& value) { return value < L"c"; }));
    }

    TEST_METHOD(TestLastOrDefault1)
    {
      Assert::AreEqual(3, From({ 1, 2, 3 }).LastOrDefault());
      Assert::AreEqual(L"c"s, From({ L"a"s, L"b"s, L"c"s }).LastOrDefault());
      Assert::AreEqual(int(), From(array<int, 0>{ }).LastOrDefault());
      Assert::AreEqual(wstring(), From(array<wstring, 0>{ }).LastOrDefault());
    }

    TEST_METHOD(TestLastOrDefault2)
    {
      Assert::AreEqual(2, From({ 1, 2, 3 }).LastOrDefault([](int const& value) { return value < 3; }));
      Assert::AreEqual(L"b"s, From({ L"a"s, L"b"s, L"c"s }).LastOrDefault([](wstring const& value) { return value < L"c"; }));
      Assert::AreEqual(int(), From(array<int, 0>{ }).LastOrDefault([](int const& value) { return value < 3; }));
      Assert::AreEqual(wstring(), From(array<wstring, 0>{ }).LastOrDefault([](wstring const& value) { return value < L"c"; }));
    }

    TEST_METHOD(TestSingle1)
    {
      Assert::ExpectException<runtime_error>([]() { From({ 1, 2, 3 }).Single(); });
      Assert::ExpectException<runtime_error>([]() { From(array<int, 0>{ }).Single(); });
      Assert::ExpectException<runtime_error>([]() { From({ L"a"s, L"b"s, L"c"s }).Single(); });
      Assert::ExpectException<runtime_error>([]() { From(array<wstring, 0>{ }).Single(); });
      Assert::AreEqual(1, From({ 1 }).Single());
      Assert::AreEqual(L"a"s, From({ L"a"s }).Single());
    }

    TEST_METHOD(TestSingle2)
    {
      Assert::ExpectException<runtime_error>([]() { From({ 1, 2, 3 }).Single([](int const& value) { return value < 3; }); });
      Assert::ExpectException<runtime_error>([]() { From({ L"a"s, L"b"s, L"c"s }).Single([](wstring const& value) { return value < L"c"; }); });
      Assert::ExpectException<runtime_error>([]() { From({ 1, 2, 3 }).Single([](int const& value) { return value < 1; }); });
      Assert::ExpectException<runtime_error>([]() { From({ L"a"s, L"b"s, L"c"s }).Single([](wstring const& value) { return value < L"a"; }); });
      Assert::AreEqual(1, From({ 1, 2, 3 }).Single([](int const& value) { return value < 2; }));
      Assert::AreEqual(L"a"s, From({ L"a"s, L"b"s, L"c"s }).Single([](wstring const& value) { return value < L"b"; }));
    }

    TEST_METHOD(TestSingleOrDefault1)
    {
      Assert::ExpectException<runtime_error>([]() { From({ 1, 2, 3 }).SingleOrDefault(); });
      Assert::ExpectException<runtime_error>([]() { From({ L"a"s, L"b"s, L"c"s }).SingleOrDefault(); });
      Assert::AreEqual(int(), From(array<int, 0>{ }).SingleOrDefault());
      Assert::AreEqual(wstring(), From(array<wstring, 0>{ }).SingleOrDefault());
      Assert::AreEqual(1, From({ 1 }).SingleOrDefault());
      Assert::AreEqual(L"a"s, From({ L"a"s }).SingleOrDefault());
    }

    TEST_METHOD(TestSingleOrDefault2)
    {
      Assert::ExpectException<runtime_error>([]() { From({ 1, 2, 3 }).SingleOrDefault([](int const& value) { return value < 3; }); });
      Assert::ExpectException<runtime_error>([]() { From({ L"a"s, L"b"s, L"c"s }).SingleOrDefault([](wstring const& value) { return value < L"c"; }); });
      Assert::AreEqual(int(), From({ 1, 2, 3 }).SingleOrDefault([](int const& value) { return value < 1; }));
      Assert::AreEqual(wstring(), From({ L"a"s, L"b"s, L"c"s }).SingleOrDefault([](wstring const& value) { return value < L"a"; }));
      Assert::AreEqual(1, From({ 1 }).SingleOrDefault([](int const& value) { return value < 2; }));
      Assert::AreEqual(L"a"s, From({ L"a"s }).SingleOrDefault([](wstring const& value) { return value < L"b"; }));
    }

    TEST_METHOD(TestSkip)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      Assert::IsTrue(Equal(input, From(input).Skip(0)));
      Assert::IsTrue(Equal(array{ 2, 3, 4, 5 }, From(input).Skip(1)));
      Assert::IsTrue(Equal(array{ 3, 4, 5 }, From(input).Skip(2)));
      Assert::IsTrue(Equal(array{ 4, 5 }, From(input).Skip(3)));
      Assert::IsTrue(Equal(array{ 5 }, From(input).Skip(4)));
      Assert::IsTrue(Equal(array<int, 0>{ }, From(input).Skip(5)));
      Assert::IsTrue(Equal(array<int, 0>{ }, From(input).Skip(6)));
    }

    TEST_METHOD(TestSkipWhile1)
    {
      auto input = array{ 1, 2, 3, 4, 5 };

      Assert::IsTrue(Equal(input, From(input).SkipWhile([](int const& value) { return value < 1; })));
      Assert::IsTrue(Equal(array{ 2, 3, 4, 5 }, From(input).SkipWhile([](int const& value) { return value < 2; })));
      Assert::IsTrue(Equal(array{ 3, 4, 5 }, From(input).SkipWhile([](int const& value) { return value < 3; })));
      Assert::IsTrue(Equal(array{ 4, 5 }, From(input).SkipWhile([](int const& value) { return value < 4; })));
      Assert::IsTrue(Equal(array{ 5 }, From(input).SkipWhile([](int const& value) { return value < 5; })));
      Assert::IsTrue(Equal(array<int, 0>{ }, From(input).SkipWhile([](int const& value) { return value < 6; })));
    }

    TEST_METHOD(TestSkipWhile2)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      int64_t idx = -1;

      Assert::IsTrue(Equal(input, From(input).SkipWhile([&](int const& value, int64_t index) { idx = index; return value < 1; })));
      Assert::AreEqual(0, (int)idx);
      Assert::IsTrue(Equal(array{ 2, 3, 4, 5 }, From(input).SkipWhile([&](int const& value, int64_t index) { idx = index; return value < 2; })));
      Assert::AreEqual(1, (int)idx);
      Assert::IsTrue(Equal(array{ 3, 4, 5 }, From(input).SkipWhile([&](int const& value, int64_t index) { idx = index; return value < 3; })));
      Assert::AreEqual(2, (int)idx);
      Assert::IsTrue(Equal(array{ 4, 5 }, From(input).SkipWhile([&](int const& value, int64_t index) { idx = index; return value < 4; })));
      Assert::AreEqual(3, (int)idx);
      Assert::IsTrue(Equal(array{ 5 }, From(input).SkipWhile([&](int const& value, int64_t index) { idx = index; return value < 5; })));
      Assert::AreEqual(4, (int)idx);
      Assert::IsTrue(Equal(array<int, 0>{ }, From(input).SkipWhile([&](int const& value, int64_t index) { idx = index; return value < 6; })));
      Assert::AreEqual(4, (int)idx);
    }

    TEST_METHOD(TestTake)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      Assert::IsTrue(Equal(array<int, 0>{ }, From(input).Take(0)));
      Assert::IsTrue(Equal(array{ 1 }, From(input).Take(1)));
      Assert::IsTrue(Equal(array{ 1, 2 }, From(input).Take(2)));
      Assert::IsTrue(Equal(array{ 1, 2, 3 }, From(input).Take(3)));
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4 }, From(input).Take(4)));
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4, 5 }, From(input).Take(5)));
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4, 5 }, From(input).Take(6)));
    }

    TEST_METHOD(TestTakeWhile1)
    {
      auto input = array{ 1, 2, 3, 4, 5 };

      Assert::IsTrue(Equal(array<int, 0>{ }, From(input).TakeWhile([](int const& value) { return value < 1; })));
      Assert::IsTrue(Equal(array{ 1 }, From(input).TakeWhile([](int const& value) { return value < 2; })));
      Assert::IsTrue(Equal(array{ 1, 2 }, From(input).TakeWhile([](int const& value) { return value < 3; })));
      Assert::IsTrue(Equal(array{ 1, 2, 3 }, From(input).TakeWhile([](int const& value) { return value < 4; })));
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4 }, From(input).TakeWhile([](int const& value) { return value < 5; })));
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4, 5 }, From(input).TakeWhile([](int const& value) { return value < 6; })));
    }

    TEST_METHOD(TestTakeWhile2)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      int64_t idx = -1;

      Assert::IsTrue(Equal(array<int, 0>{ }, From(input).TakeWhile([&](int const& value, int64_t index) { idx = index; return value < 1; })));
      Assert::AreEqual(0, (int)idx);
      Assert::IsTrue(Equal(array{ 1 }, From(input).TakeWhile([&](int const& value, int64_t index) { idx = index; return value < 2; })));
      Assert::AreEqual(1, (int)idx);
      Assert::IsTrue(Equal(array{ 1, 2 }, From(input).TakeWhile([&](int const& value, int64_t index) { idx = index; return value < 3; })));
      Assert::AreEqual(2, (int)idx);
      Assert::IsTrue(Equal(array{ 1, 2, 3 }, From(input).TakeWhile([&](int const& value, int64_t index) { idx = index; return value < 4; })));
      Assert::AreEqual(3, (int)idx);
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4 }, From(input).TakeWhile([&](int const& value, int64_t index) { idx = index; return value < 5; })));
      Assert::AreEqual(4, (int)idx);
      Assert::IsTrue(Equal(array{ 1, 2, 3, 4, 5 }, From(input).TakeWhile([&](int const& value, int64_t index) { idx = index; return value < 6; })));
      Assert::AreEqual(4, (int)idx);
    }

    TEST_METHOD(TestZip)
    {
      auto numbers = array{ 1, 2, 3, 4 };
      auto words = array{ L"one"s, L"two"s, L"three"s };
      auto expected = array{ L"1 one"s, L"2 two"s, L"3 three"s };

      auto output = From(numbers).Zip<wstring>(From(words), [](int const& number, wstring const& word) { return to_wstring(number) + L" " + word; });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestAppend)
    {
      auto input = array{ 1, 2, 3, 4 };
      auto expected = array{ 1, 2, 3, 4, 5 };

      auto output = From(input).Append(5);
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestPrepend)
    {
      auto input = array{ 1, 2, 3, 4 };
      auto expected = array{ 0, 1, 2, 3, 4 };

      auto output = From(input).Prepend(0);
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestSequenceEqual1)
    {
      Assert::IsTrue(From({ 1, 2, 3, 4, 5 }).SequenceEqual({ 1, 2, 3, 4, 5 }));
      Assert::IsFalse(From({ 1, 2, 3, 4, 5 }).SequenceEqual({ 1, 2, 3, 4 }));
    }

    TEST_METHOD(TestSequenceEqual2)
    {
      auto fruits1 = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s };
      auto fruits2 = array{ L"apple"s, L"banana"s, L"mango"s, L"orange"s };
      auto input1 = From(fruits1).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });
      auto input2 = From(fruits2).Select<const wchar_t*>([](wstring const& fruit) { return fruit.c_str(); });

      auto output1 = From(input1).SequenceEqual(input2);
      auto output2 = From(input1).SequenceEqual(input2, [](const wchar_t *const& left, const wchar_t *const& right) { return wstring(left) == wstring(right); });
      Assert::IsFalse(output1);
      Assert::IsTrue(output2);
    }

    TEST_METHOD(TestRange)
    {
      auto expected = array{ 1, 4, 9, 16, 25 };
      auto output = Enumerable<int64_t>::Range(1, 5).Select<int64_t>([](int64_t const& value) { return value * value; });
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestRepeat)
    {
      auto expected = array{ 1337, 1337, 1337 };
      auto output = Enumerable<int64_t>::Repeat(1337, 3);
      Assert::IsTrue(Equal(expected, output));
    }

    TEST_METHOD(TestToVector)
    {
      auto input = array{ 1, 2, 3, 4, 5 };
      auto intermediate = From(input);
      auto output = intermediate.ToVector();

      static_assert(is_same_v<decltype(output), vector<decltype(input)::value_type>>, "output shall be vector!");
      auto it0 = input.begin();
      auto it1 = intermediate.begin();
      auto it2 = output.begin();
      while (it0 != input.end() && it1 != intermediate.end() && it2 != output.end())
      {
        Assert::AreSame(*it0, *it1);
        Assert::AreEqual(*it0, *it1);
        Assert::AreNotSame(*it0, *it2);
        Assert::AreEqual(*it0, *it2);
        it0++;
        it1++;
        it2++;
      }
    }

    TEST_METHOD(TestToMap)
    {
      struct Pet { wstring Name; int Age; };
      auto input = array<Pet, 3>{ { { L"Barley", 2 }, { L"Boots", 1 }, { L"Whiskers", 3 } }};
      auto expected = map<wstring, int>{ {L"Barley", 2}, {L"Boots", 1}, {L"Whiskers", 3} };

      auto output = From(input).ToMap<wstring, int>([](Pet const& pet) { return pet.Name; }, [](Pet const& pet) { return pet.Age; });
      Assert::AreEqual(expected.size(), output.size());
      for (auto&[key, value] : expected)
      {
        Assert::AreEqual(1, (int)output.count(key));
        Assert::AreEqual(expected[key], output[key]);
      }
    }

    TEST_CLASS_INITIALIZE(ClassInitialize)
    {
      _CrtMemCheckpoint(&_init);
    }

    TEST_CLASS_CLEANUP(ClassCleanup)
    {
      _CrtMemCheckpoint(&_dest);
      if (_CrtMemDifference(&_result, &_init, &_dest))
      {
        _CrtMemDumpStatistics(&_result);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
        _CrtDumpMemoryLeaks();
        Assert::Fail();
      }
    }
  };
  _CrtMemState EnumerableTest::_init;
  _CrtMemState EnumerableTest::_dest;
  _CrtMemState EnumerableTest::_result;
}