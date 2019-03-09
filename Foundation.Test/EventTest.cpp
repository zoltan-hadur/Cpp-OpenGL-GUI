#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Foundation\Event.h"

#include <thread>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace OpenGLUI::Foundation::Test
{
  int EventTestGlobalArgument = 0;
  int EventTestGlobalCalled = 0;

  class EventTestClass
  {
  public:
    void Method(int value)
    {
      EventTestGlobalArgument = value;
      EventTestGlobalCalled++;
    }
  };

  void EventTestFreeFunction(int value)
  {
    EventTestGlobalArgument = value;
    EventTestGlobalCalled++;
  }

  TEST_CLASS(EventTest)
  {
  public:
    TEST_METHOD(TestAdd1)
    {
      auto event = Event<Delegate<void(int)>>();

      auto token1 = event.Add([&](int value) {});
      Assert::AreEqual<size_t>(1, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token1));

      auto token2 = event.Add([&](int value) {});
      Assert::AreEqual<size_t>(2, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token2));

      Assert::AreNotEqual(token1, token2);
    }

    TEST_METHOD(TestAdd2)
    {
      auto event = Event<Delegate<void(int)>>();

      auto token1 = event.Add(EventTestFreeFunction);
      Assert::AreEqual<size_t>(1, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token1));

      auto token2 = event.Add(EventTestFreeFunction);
      Assert::AreEqual<size_t>(2, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token2));

      Assert::AreNotEqual(token1, token2);
    }

    TEST_METHOD(TestAdd3)
    {
      auto test = EventTestClass();
      auto event = Event<Delegate<void(int)>>();

      auto token1 = event.Add({ &test, &EventTestClass::Method });
      Assert::AreEqual<size_t>(1, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token1));

      auto token2 = event.Add({ &test, &EventTestClass::Method });
      Assert::AreEqual<size_t>(2, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token2));

      Assert::AreNotEqual(token1, token2);
    }

    TEST_METHOD(TestAdd4)
    {
      auto test = EventTestClass();
      auto event = Event<Delegate<void(int)>>();

      auto token1 = event.Add([&](int value) {});
      Assert::AreEqual<size_t>(1, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token1));

      auto token2 = event.Add(EventTestFreeFunction);
      Assert::AreEqual<size_t>(2, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token2));

      auto token3 = event.Add({ &test, &EventTestClass::Method });
      Assert::AreEqual<size_t>(3, event._handlers.size());
      Assert::IsTrue(event._handlers.count(token3));

      Assert::AreNotEqual(token1, token2);
      Assert::AreNotEqual(token2, token3);
    }

    TEST_METHOD(TestRemove)
    {
      auto test = EventTestClass();
      auto event = Event<Delegate<void(int)>>();

      auto token1 = event.Add([&](int value) {});
      auto token2 = event.Add(EventTestFreeFunction);
      auto token3 = event.Add({ &test, &EventTestClass::Method });

      Assert::AreEqual<size_t>(3, event._handlers.size());

      event.Remove(token1);
      Assert::AreEqual<size_t>(2, event._handlers.size());
      Assert::IsFalse(event._handlers.count(token1));

      event.Remove(token2);
      Assert::AreEqual<size_t>(1, event._handlers.size());
      Assert::IsFalse(event._handlers.count(token2));

      event.Remove(token3);
      Assert::AreEqual<size_t>(0, event._handlers.size());
      Assert::IsFalse(event._handlers.count(token3));
    }

    TEST_METHOD(TestClear)
    {
      auto test = EventTestClass();
      auto event = Event<Delegate<void(int)>>();

      auto token1 = event.Add([&](int value) {});
      auto token2 = event.Add(EventTestFreeFunction);
      auto token3 = event.Add({ &test, &EventTestClass::Method });

      Assert::AreEqual<size_t>(3, event._handlers.size());
      event.Clear();
      Assert::AreEqual<size_t>(0, event._handlers.size());
    }

    TEST_METHOD(TestInvoke)
    {
      auto test = EventTestClass();
      auto event = Event<Delegate<void(int)>>();

      auto token1 = event.Add([&](int value) { EventTestGlobalArgument = value; EventTestGlobalCalled++; throw exception("Fail on purpose"); });
      auto token2 = event.Add(EventTestFreeFunction);
      auto token3 = event.Add({ &test, &EventTestClass::Method });
      Assert::AreEqual<size_t>(3, event._handlers.size());

      event(1337);
      Assert::AreEqual(1337, EventTestGlobalArgument);
      Assert::AreEqual(3, EventTestGlobalCalled);
      Assert::AreEqual<size_t>(2, event._handlers.size());

      event(1338);
      Assert::AreEqual(1338, EventTestGlobalArgument);
      Assert::AreEqual(5, EventTestGlobalCalled);
      Assert::AreEqual<size_t>(2, event._handlers.size());

      event.Clear();
      event(1338);
      Assert::AreEqual(1338, EventTestGlobalArgument);
      Assert::AreEqual(5, EventTestGlobalCalled);
      Assert::AreEqual<size_t>(0, event._handlers.size());
    }

    TEST_METHOD(TestUniqueEventToken)
    {
      auto event = Event<Delegate<void()>>();
      auto threadCount = thread::hardware_concurrency();
      auto threads = vector<thread>(threadCount);
      auto id = EventCommon::_id.load();
      for (decltype(threadCount) i = 0; i < threadCount; ++i)
      {
        threads[i] = thread([&]()
        {
          for (int j = 0; j < 1'000; ++j)
          {
            event.Add([]() {});
          }
        });
      }
      for (decltype(threadCount) i = 0; i < threadCount; ++i)
      {
        threads[i].join();
      }
      Assert::AreEqual<EventToken>(id + threadCount * 1'000, EventCommon::_id);
    }

    TEST_METHOD(TestCopyConstructor)
    {
      auto called = 0;
      auto event1 = Event<Delegate<void()>>();
      auto token1 = event1.Add([&]() { called++; });
      auto token2 = event1.Add([&]() { called++; });
      Assert::IsTrue(event1._handlers.count(token1));
      Assert::IsTrue(event1._handlers.count(token2));

      auto event2 = event1;
      Assert::IsTrue(event1._handlers.count(token1));
      Assert::IsTrue(event2._handlers.count(token2));

      event1();
      Assert::AreEqual(2, called);
      event2();
      Assert::AreEqual(4, called);
    }

    TEST_METHOD(TestAssignOperator)
    {
      auto called = 0;
      auto event1 = Event<Delegate<void()>>();
      event1 = event1;
      auto token1 = event1.Add([&]() { called++; });
      auto token2 = event1.Add([&]() { called++; });
      Assert::IsTrue(event1._handlers.count(token1));
      Assert::IsTrue(event1._handlers.count(token2));

      Event<Delegate<void()>> event2;
      event2 = event1;
      event2 = event2;
      Assert::IsTrue(event1._handlers.count(token1));
      Assert::IsTrue(event2._handlers.count(token2));

      event1();
      Assert::AreEqual(2, called);
      event2();
      Assert::AreEqual(4, called);
    }
  };
}