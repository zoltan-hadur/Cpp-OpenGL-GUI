#pragma once

#include <type_traits>
#include <tuple>
#include <map>
#include <memory>
#include <functional>

namespace OpenGLUI::Foundation
{
  // Provides information about callable types.
  template<typename Return, bool IsFree, bool IsMutable, typename ...Args>
  struct FunctionDetail
  {
    using ReturnType = Return;
    using Signature = ReturnType(Args...);
    using VoidSignature = void(Args...);

    static constexpr bool IsFree = IsFree;
    static constexpr bool IsMutable = IsMutable;

    struct Arguments
    {
      typedef typename std::tuple<Args...> Types;
      static constexpr size_t Count = sizeof...(Args);
    };

    template<size_t Index>
    struct Argument
    {
      typedef typename std::tuple_element<Index, std::tuple<Args...>>::type Type;
    };
  };

  template<typename T, typename = void>
  struct FunctionInfo;

  template<typename T>
  struct FunctionInfo<T, typename std::conditional_t<false, decltype(&T::operator()), void>> : FunctionInfo<decltype(&T::operator())> {};

  template<typename Return, typename Class, typename ...Args>
  struct FunctionInfo<Return(Class::*)(Args...)      > : FunctionDetail<Return, false, true , Args...> {};

  template<typename Return, typename Class, typename ...Args>
  struct FunctionInfo<Return(Class::*)(Args...) const> : FunctionDetail<Return, false, false, Args...> {};

  template<typename Return,                 typename ...Args>
  struct FunctionInfo<Return          (Args...)      > : FunctionDetail<Return, true , true , Args...> {};

  template<typename Return,                 typename ...Args>
  struct FunctionInfo<Return(__cdecl*)(Args...)      > : FunctionDetail<Return, true , true , Args...> {};
}