#pragma once

#include <type_traits>
#include <tuple>
#include <map>
#include <memory>

namespace OpenGLUI::Foundation
{
  // Provides information about callable types.
  template<typename Return, typename Class, typename Mutable, typename ...Args>
  struct FunctionDetail
  {
    using Signature = Return(Args...);
    using VoidSignature = void(Args...);

    using ReturnType = Return;
    static const bool IsFree = std::is_same_v<Class, nullptr_t>;
    static const bool IsMutable = std::is_same_v<Mutable, std::true_type>;

    struct Arguments
    {
      typedef typename std::tuple<Args...> Types;
      static const size_t Count = sizeof...(Args);
    };

    template<size_t Index>
    struct Argument
    {
      typedef typename std::tuple_element<Index, std::tuple<Args...>>::type Type;
    };
  };

  template<typename T>
  struct FunctionInfo;

  template<typename T>
  struct FunctionInfo : FunctionInfo<decltype(&T::operator())> {};

  template<typename Return, typename Class, typename ...Args>
  struct FunctionInfo<Return(Class::*)(Args...)> : FunctionDetail<Return, Class, std::true_type, Args...> {};

  template<typename Return, typename Class, typename ...Args>
  struct FunctionInfo<Return(Class::*)(Args...) const> : FunctionDetail<Return, Class, std::false_type, Args...> {};

  template<typename Return, typename ...Args>
  struct FunctionInfo<Return(Args...)> : FunctionDetail<Return, std::nullptr_t, std::true_type, Args...> {};

  template<typename Return, typename ...Args>
  struct FunctionInfo<Return(__cdecl*)(Args...)> : FunctionDetail<Return, std::nullptr_t, std::true_type, Args...> {};

  // For nulled out Delegate
  template<>
  struct FunctionInfo<std::nullptr_t> {};

  // For EventToken
  template<>
  struct FunctionInfo<uint64_t> {};

  template<typename T>
  struct FunctionInfo<std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<T>>>> {};

  template<typename T>
  struct FunctionInfo<std::_Tree_const_iterator<std::_Tree_val<std::_Tree_simple_types<T>>>> {};

  template<typename T>
  struct FunctionInfo<std::_Tree_unchecked_iterator<std::_Tree_val<std::_Tree_simple_types<T>>>> {};

  template<typename T>
  struct FunctionInfo<std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<T>>>> {};

  template<typename _Kty, typename _Ty>
  struct FunctionInfo<std::allocator<std::_Tree_node<std::pair<const _Kty, _Ty>, typename std::_Default_allocator_traits<std::allocator<std::pair<const _Kty, _Ty>>>::void_pointer>>> {};
}