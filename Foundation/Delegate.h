#pragma once

#include "FunctionInfo.h"

#include <functional>
#include <tuple>

namespace OpenGLUI::Foundation
{
  // Stores either a lambda expression, a free function or an object's pointer with it's method.
  template<typename Signature>
  class Delegate;

  // Stores either a lambda expression, a free function or an object's pointer with it's method.
  template<typename Result, typename ...Args>
  class Delegate<Result(Args...)> final
  {
  private:
    using Signature = Result(Args...);
    using VoidSignature = void(Args...);
    std::function<Signature> _func;

  public:
    // The delegate will be null.
    Delegate(std::nullptr_t ptr = nullptr) noexcept : _func(ptr) {}

    // Constructs the delegate with a lambda expression.
    template<typename Lambda,
      std::enable_if_t<std::is_same_v<VoidSignature, typename FunctionInfo<Lambda>::VoidSignature>>* = nullptr>
      Delegate(Lambda lambda)
    {
      _func = lambda;
    }

    // Constructs the delegate with a free function.
    template<typename Function,
      std::enable_if_t<std::is_same_v<VoidSignature, typename FunctionInfo<Function>::VoidSignature>>* = nullptr>
      Delegate(Function* function)
    {
      _func = function;
    }

    // Constructs the delegate with an object's pointer and it's method.
    template<typename Object, typename Method,
      std::enable_if_t<std::is_same_v<VoidSignature, typename FunctionInfo<Method>::VoidSignature>>* = nullptr>
      Delegate(Object* object, Method method)
    {
      _func = [=](Args... args) -> Result
      {
        return std::invoke(method, object, args...);
      };
    }

    // Invokes the delegate with the given arguments.
    Result operator()(Args... args) const
    {
      if (_func)
      {
        return _func(args...);
      }
      else
      {
        throw std::runtime_error("The delegate is null!");
      }
    }

    // Returns true when the delegate is not null.
    operator bool() const
    {
      return _func != nullptr;
    }

    friend bool operator ==(Delegate delegate, std::nullptr_t null)
    {
      return delegate._func == null;
    }

    friend bool operator ==(std::nullptr_t null, Delegate delegate)
    {
      return delegate._func == null;
    }

    friend bool operator !=(Delegate delegate, std::nullptr_t null)
    {
      return delegate._func != null;
    }

    friend bool operator !=(std::nullptr_t null, Delegate delegate)
    {
      return delegate._func != null;
    }
  };

  // Deduction guides.
  template<typename Lambda>
  Delegate(Lambda lambda) -> Delegate<typename FunctionInfo<Lambda>::Signature>;

  template<typename Function>
  Delegate(Function* function) -> Delegate<typename FunctionInfo<Function>::Signature>;

  template<typename Object, typename Method>
  Delegate(Object* object, Method method) -> Delegate<typename FunctionInfo<Method>::Signature>;
}