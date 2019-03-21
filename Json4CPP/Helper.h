#pragma once

#include <string>

namespace Json4CPP
{
  template<typename T>
  struct TypeDebug;

  template<typename ...Args>
  struct Overload : Args...
  {
    using Args::operator()...;
  };
  template<typename ...Args>
  Overload(Args...)->Overload<Args...>;

  std::wstring EscapeString(std::wstring value);
}