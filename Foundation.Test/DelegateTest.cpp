#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Foundation\Delegate.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace OpenGLUI::Foundation::Test
{
  class DelegateTestClass
  {
  public:
    double Method(int value)
    {
      return value / 2.0;
    }
  };

  double DelegateTestFreeFunction(int value)
  {
    return value / 2.0;
  }

  TEST_CLASS(DelegateTest)
  {
  public:
    TEST_METHOD(TestConstructors)
    {
      auto test = DelegateTestClass();

      auto fromLambda = Delegate([](int value) -> double { return value / 2.0; });
      auto fromMethod = Delegate(&test, &DelegateTestClass::Method);
      auto fromFreeFunction = Delegate(DelegateTestFreeFunction);

      static_assert(is_same_v<decltype(fromLambda), decltype(fromMethod)>, "As the signatures are equals, the types shall be equals too!");
      static_assert(is_same_v<decltype(fromMethod), decltype(fromFreeFunction)>, "As the signatures are equals, the types shall be equals too!");
    }

    TEST_METHOD(TestImplicitBoolConversion)
    {
      auto delegate = Delegate([]() {});

      if (!delegate) Assert::Fail();
      Assert::AreEqual(false, delegate == nullptr);
      Assert::AreEqual(false, nullptr == delegate);
      Assert::AreEqual(true, delegate != nullptr);
      Assert::AreEqual(true, nullptr != delegate);

      delegate = nullptr;

      if (delegate) Assert::Fail();
      Assert::AreEqual(false, delegate != nullptr);
      Assert::AreEqual(false, nullptr != delegate);
      Assert::AreEqual(true, delegate == nullptr);
      Assert::AreEqual(true, nullptr == delegate);
    }

    TEST_METHOD(TestInvoke)
    {
      auto test = DelegateTestClass();

      auto fromNullptr = Delegate<void()>();
      auto fromLambda = Delegate([](int value) -> double { return value / 2.0; });
      auto fromMethod = Delegate(&test, &DelegateTestClass::Method);
      auto fromFreeFunction = Delegate(DelegateTestFreeFunction);

      Assert::ExpectException<runtime_error>([&]() { fromNullptr(); });
      Assert::AreEqual(0.5, fromLambda(1));
      Assert::AreEqual(0.5, fromMethod(1));
      Assert::AreEqual(0.5, fromFreeFunction(1));
    }

    TEST_METHOD(TestForAmbiguity)
    {
      struct Test
      {
        double Method(Delegate<double(int)> delegate) { return delegate(1); }
        double Method(Delegate<double(int, int)> delegate) { return delegate(2, 3); }
        double Method(Delegate<double(float)> delegate) { return delegate(2.5f); }
      };
      auto test = Test();

      Assert::AreEqual(0.5, test.Method([](int a) { return a / 2.0; }));
      Assert::AreEqual(0.5, test.Method([](int a, int b) { return a * b / 12.0; }));
      Assert::AreEqual(0.5, test.Method([](float a) { return a / 5.0; }));
    }
  };
}