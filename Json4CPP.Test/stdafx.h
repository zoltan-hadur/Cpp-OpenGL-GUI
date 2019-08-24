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
  template<> static std::wstring ToString<std::nullptr_t>(std::nullptr_t const&) { return L"null"; }
  template<> static std::wstring ToString<Json4CPP::JsonObject>(Json4CPP::JsonObject const& object) { std::wostringstream os; os << object; return os.str(); }
  template<> static std::wstring ToString<Json4CPP::JsonArray>(Json4CPP::JsonArray const& array) { std::wostringstream os; os << array; return os.str(); }
  template<> static std::wstring ToString<Json4CPP::Json>(Json4CPP::Json const& json) { std::wostringstream os; os << json; return os.str(); }
  template<> static std::wstring ToString<Json4CPP::Detail::VALUE>(Json4CPP::Detail::VALUE const& value) { std::wostringstream os; Json4CPP::Detail::Value::Write(os, value); return os.str(); }
}