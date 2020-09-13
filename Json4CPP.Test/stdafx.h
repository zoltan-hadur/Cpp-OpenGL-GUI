// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

// TODO: reference additional headers your program requires here
#include <optional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <tuple>
#include <vector>
#include <deque>
#include <map>

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#ifdef _DEBUG
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define new new
#endif
#include "..\Json4CPP\Json.hpp"
#include "..\Foundation\Enumerable.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
  template<> static std::wstring ToString<Json4CPP::        JsonObject     >(Json4CPP::        JsonObject      const& object) { return Json4CPP::Json::Stringify(               object); }
  template<> static std::wstring ToString<Json4CPP::        JsonArray      >(Json4CPP::        JsonArray       const& array ) { return Json4CPP::Json::Stringify(               array ); }
  template<> static std::wstring ToString<Json4CPP::        Json           >(Json4CPP::        Json            const& json  ) { return Json4CPP::Json::Stringify(               json  ); }
  template<> static std::wstring ToString<Json4CPP::        JsonType       >(Json4CPP::        JsonType        const& type  ) { return Json4CPP::Json::Stringify(               type  ); }
  template<> static std::wstring ToString<Json4CPP::Detail::JsonBuilderType>(Json4CPP::Detail::JsonBuilderType const& type  ) { return Json4CPP::Json::Stringify(               type  ); }
  template<> static std::wstring ToString<Json4CPP::Detail::JsonTokenType  >(Json4CPP::Detail::JsonTokenType   const& type  ) { return Json4CPP::Json::Stringify(               type  ); }
  template<> static std::wstring ToString<Json4CPP::Detail::VALUE          >(Json4CPP::Detail::VALUE           const& value ) { return Json4CPP::Json::Stringify(Json4CPP::Json(value)); }
  template<> static std::wstring ToString<Json4CPP::Detail::VALUE_TOKEN    >(Json4CPP::Detail::VALUE_TOKEN     const& value ) { return Json4CPP::Detail::JsonLinter::Dump(value);        }

  template<> static std::wstring ToString<std::nullptr_t>(std::nullptr_t const& value) { return Json4CPP::Json::Stringify(value); }
  template<> static std::wstring ToString<uint16_t>(uint16_t const& value) { return Json4CPP::Json::Stringify(value); }

  template<> static std::wstring ToString<std::u32string>(std::u32string const& value) { return Json4CPP::Detail::U32String2WString(value); }

  template<typename T, typename F>
  static void ExceptException(F func, std::string const& msg)
  {
    auto found = false;
    try
    {
      func();
    }
    catch (T e)
    {
      Assert::AreEqual(msg.c_str(), e.what());
      found = true;
    }
    Assert::AreEqual(found, true);
  }
}