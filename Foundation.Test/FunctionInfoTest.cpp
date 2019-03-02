#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Foundation\FunctionInfo.h"

#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace OpenGLUI::Foundation::Test
{
  class FunctionInfoTestClass
  {
  public:
    double Method(int, int*, int&, int const*, int const&, int&&)
    {
      return 0.0;
    }
  };

  double FunctionInfoTestFreeFunction(int, int*, int&, int const*, int const&, int&&)
  {
    return 0.0;
  }

  TEST_CLASS(FunctionInfoTest)
  {
  public:
    TEST_METHOD(TestLambda)
    {
      auto lambda = [](int, int*, int&, int const*, int const&, int&&) -> double { return 0.0; };
      using Lambda = decltype(lambda);

      static_assert(is_same_v<FunctionInfo<Lambda>::Signature, double(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Lambda>::VoidSignature, void(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Lambda>::ReturnType, double>);
      static_assert(FunctionInfo<Lambda>::IsFree == false);
      static_assert(FunctionInfo<Lambda>::IsMutable == false);
      static_assert(is_same_v<FunctionInfo<Lambda>::Arguments::Types, tuple<int, int*, int&, int const*, int const&, int&&>>);
      static_assert(FunctionInfo<Lambda>::Arguments::Count == 6);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<0>::Type, int>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<1>::Type, int*>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<2>::Type, int&>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<3>::Type, int const*>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<4>::Type, int const&>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<5>::Type, int&&>);
    }

    TEST_METHOD(TestMutableLambda)
    {
      auto lambda = [](int, int*, int&, int const*, int const&, int&&) mutable -> double { return 0.0; };
      using Lambda = decltype(lambda);

      static_assert(is_same_v<FunctionInfo<Lambda>::Signature, double(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Lambda>::VoidSignature, void(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Lambda>::ReturnType, double>);
      static_assert(FunctionInfo<Lambda>::IsFree == false);
      static_assert(FunctionInfo<Lambda>::IsMutable == true);
      static_assert(is_same_v<FunctionInfo<Lambda>::Arguments::Types, tuple<int, int*, int&, int const*, int const&, int&&>>);
      static_assert(FunctionInfo<Lambda>::Arguments::Count == 6);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<0>::Type, int>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<1>::Type, int*>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<2>::Type, int&>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<3>::Type, int const*>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<4>::Type, int const&>);
      static_assert(is_same_v<FunctionInfo<Lambda>::Argument<5>::Type, int&&>);
    }

    TEST_METHOD(TestFunction)
    {
      function<double(int, int*, int&, int const*, int const&, int&&)> function;
      using Function = decltype(function);

      static_assert(is_same_v<FunctionInfo<Function>::Signature, double(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Function>::VoidSignature, void(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Function>::ReturnType, double>);
      static_assert(FunctionInfo<Function>::IsFree == false);
      static_assert(FunctionInfo<Function>::IsMutable == false);
      static_assert(is_same_v<FunctionInfo<Function>::Arguments::Types, tuple<int, int*, int&, int const*, int const&, int&&>>);
      static_assert(FunctionInfo<Function>::Arguments::Count == 6);
      static_assert(is_same_v<FunctionInfo<Function>::Argument<0>::Type, int>);
      static_assert(is_same_v<FunctionInfo<Function>::Argument<1>::Type, int*>);
      static_assert(is_same_v<FunctionInfo<Function>::Argument<2>::Type, int&>);
      static_assert(is_same_v<FunctionInfo<Function>::Argument<3>::Type, int const*>);
      static_assert(is_same_v<FunctionInfo<Function>::Argument<4>::Type, int const&>);
      static_assert(is_same_v<FunctionInfo<Function>::Argument<5>::Type, int&&>);
    }

    TEST_METHOD(TestMethod)
    {
      using Method = decltype(&FunctionInfoTestClass::Method);

      static_assert(is_same_v<FunctionInfo<Method>::Signature, double(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Method>::VoidSignature, void(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<Method>::ReturnType, double>);
      static_assert(FunctionInfo<Method>::IsFree == false);
      static_assert(FunctionInfo<Method>::IsMutable == true);
      static_assert(is_same_v<FunctionInfo<Method>::Arguments::Types, tuple<int, int*, int&, int const*, int const&, int&&>>);
      static_assert(FunctionInfo<Method>::Arguments::Count == 6);
      static_assert(is_same_v<FunctionInfo<Method>::Argument<0>::Type, int>);
      static_assert(is_same_v<FunctionInfo<Method>::Argument<1>::Type, int*>);
      static_assert(is_same_v<FunctionInfo<Method>::Argument<2>::Type, int&>);
      static_assert(is_same_v<FunctionInfo<Method>::Argument<3>::Type, int const*>);
      static_assert(is_same_v<FunctionInfo<Method>::Argument<4>::Type, int const&>);
      static_assert(is_same_v<FunctionInfo<Method>::Argument<5>::Type, int&&>);
    }

    TEST_METHOD(TestFreeFunction)
    {
      using FreeFunction = decltype(FunctionInfoTestFreeFunction);

      static_assert(is_same_v<FunctionInfo<FreeFunction>::Signature, double(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::VoidSignature, void(int, int*, int&, int const*, int const&, int&&)>);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::ReturnType, double>);
      static_assert(FunctionInfo<FreeFunction>::IsFree == true);
      static_assert(FunctionInfo<FreeFunction>::IsMutable == true);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::Arguments::Types, tuple<int, int*, int&, int const*, int const&, int&&>>);
      static_assert(FunctionInfo<FreeFunction>::Arguments::Count == 6);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::Argument<0>::Type, int>);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::Argument<1>::Type, int*>);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::Argument<2>::Type, int&>);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::Argument<3>::Type, int const*>);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::Argument<4>::Type, int const&>);
      static_assert(is_same_v<FunctionInfo<FreeFunction>::Argument<5>::Type, int&&>);
    }
  };
}