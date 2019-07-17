#include "stdafx.h"
#include "CppUnitTest.h"
#include <optional>

#include "..\Json4CPP\Json.hpp"
#include "..\Foundation\Enumerable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace OpenGLUI::Foundation;

namespace Json4CPP::Test
{
  TEST_CLASS(ValueTest)
  {
  private:
    static _CrtMemState _init;
    static _CrtMemState _dest;
    static _CrtMemState _result;
  public:
    TEST_METHOD(TestGetType1)
    {
      auto input    = vector<VALUE>   {    nullptr_t(),        wstring(),            bool(),         double(),     JsonObject(),     JsonArray() };
      auto expected = vector<JsonType>{ JsonType::Null, JsonType::String, JsonType::Boolean, JsonType::Number, JsonType::Object, JsonType::Array };
      auto output = From(input).Select<JsonType>([](VALUE const& value) { return GetType(value); });
      Assert::IsTrue(From(expected).SequenceEqual(output));
    }

    TEST_METHOD(TestGetType2)
    {
      auto input = vector<VALUE_BUILDER>
      {
        nullptr_t(), wstring(), bool(), double(), JsonObject(), JsonArray(),
        vector<JsonBuilder>(),
        vector<JsonBuilder>{ JsonBuilder(L"Key"), JsonBuilder(L"Value") },
        vector<JsonBuilder>{ vector<JsonBuilder>{ JsonBuilder(L"Key0"), JsonBuilder(L"Value0") },
                             vector<JsonBuilder>{ JsonBuilder(L"Key1"), JsonBuilder(L"Value1") } },
        vector<JsonBuilder>{ JsonBuilder(1), JsonBuilder(2), JsonBuilder(3) }
      };
      auto expected = vector<JsonBuilderType>
      {
        JsonBuilderType::Null, JsonBuilderType::String, JsonBuilderType::Boolean, JsonBuilderType::Number, JsonBuilderType::Object, JsonBuilderType::Array,
        JsonBuilderType::Empty,
        JsonBuilderType::Pair,
        JsonBuilderType::Object,
        JsonBuilderType::Array
      };
      auto output = From(input).Select<JsonBuilderType>([](VALUE_BUILDER const& value) { return GetType(value); });
      Assert::IsTrue(From(expected).SequenceEqual(output));
    }

    TEST_METHOD(TestParseNull)
    {
      auto input           = vector<wstring>{  L"", L" ", L" n", L" nu", L" nul", L" null", L" null ", L" nu11 ", L" nill ", L" mull " };
      auto expectException = vector<bool>   { true, true,  true,   true,    true,    false,     false,      true,      true,      true };
      for (int i = 0; i < input.size(); ++i)
      {
        if (!expectException[i])
        {
          Assert::IsTrue(nullptr == ParseNull(input[i]));
          Assert::IsTrue(nullptr == ParseNull(wstringstream(input[i])));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { ParseNull(input[i]); });
          Assert::ExpectException<exception>([&]() { ParseNull(wstringstream(input[i])); });
        }
      }
    }

    TEST_METHOD(TestParseString)
    {
      auto pairs = map<wstring, wstring>
      {
        { L"\"\""s        , L""s        },  // Empty
        { L"\"a\""s       , L"a"s       },  // a
        { L"\"\\\"\""s    , L"\""s      },  // Quote
        { L"\"\\\\\""s    , L"\\"s      },  // Backslash
        { L"\"\\/\""s     , L"/"s       },  // Slash
        { L"\"\\b\""s     , L"\b"s      },  // Backspace
        { L"\"\\f\""s     , L"\f"s      },  // Form feed
        { L"\"\\n\""s     , L"\n"s      },  // New line
        { L"\"\\r\""s     , L"\r"s      },  // Carriage return
        { L"\"\\t\""s     , L"\t"s      },  // Tab
        { L"\"\\u03A9\""s , L"\u03A9"s  },  // Ω
        { L"\"ab\""s      , L"ab"s      },
        { L"\"\\\"b\""s   , L"\"b"s     },
        { L"\"\\\\b\""s   , L"\\b"s     },
        { L"\"\\/b\""s    , L"/b"s      },
        { L"\"\\bb\""s    , L"\bb"s     },
        { L"\"\\fb\""s    , L"\fb"s     },
        { L"\"\\nb\""s    , L"\nb"s     },
        { L"\"\\rb\""s    , L"\rb"s     },
        { L"\"\\tb\""s    , L"\tb"s     },
        { L"\"\\u03A9b\""s, L"\u03A9b"s },
      };

      for (auto [value, expected] : pairs)
      {
        Assert::AreEqual(expected, ParseString(value));
        Assert::AreEqual(expected, ParseString(wstringstream(value)));
      }
    }

    TEST_METHOD(TestParseBoolean1)
    {
      auto input           = vector<wstring>{  L"", L" ", L" t", L" tr", L" tru", L" true", L" true ", L" trie " };
      auto expectException = vector<bool>   { true, true,  true,   true,    true,    false,     false,      true };
      for (int i = 0; i < input.size(); ++i)
      {
        if (!expectException[i])
        {
          Assert::IsTrue(ParseBoolean(input[i]));
          Assert::IsTrue(ParseBoolean(wstringstream(input[i])));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { ParseBoolean(input[i]); });
          Assert::ExpectException<exception>([&]() { ParseBoolean(wstringstream(input[i])); });
        }
      }
    }

    TEST_METHOD(TestParseBoolean2)
    {
      auto input           = vector<wstring>{  L"", L" ", L" f", L" fa", L" fal", L" fals", L" false", L" fakse " };
      auto expectException = vector<bool>   { true, true,  true,   true,    true,     true,     false,       true };
      for (int i = 0; i < input.size(); ++i)
      {
        if (!expectException[i])
        {
          Assert::IsFalse(ParseBoolean(input[i]));
          Assert::IsFalse(ParseBoolean(wstringstream(input[i])));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { ParseBoolean(input[i]); });
          Assert::ExpectException<exception>([&]() { ParseBoolean(wstringstream(input[i])); });
        }
      }
    }

    TEST_METHOD(TestParseNumber)
    {
      auto pairs = map<wstring, double>
      {
        { L"0"s          , 0.0                },
        { L"0E+1"s       , 0.0                },
        { L"0E+12"s      , 0.0                },
        { L"0E1"s        , 0.0                },
        { L"0E12"s       , 0.0                },
        { L"0E-1"s       , 0.0                },
        { L"0E-12"s      , 0.0                },
        { L"0e+1"s       , 0.0                },
        { L"0e+12"s      , 0.0                },
        { L"0e1"s        , 0.0                },
        { L"0e12"s       , 0.0                },
        { L"0e-1"s       , 0.0                },
        { L"0e-12"s      , 0.0                },
        { L"0.1"s        , 0.1                },
        { L"0.1E+1"s     , 1.0                },
        { L"0.1E+12"s    , 100000000000.0     },
        { L"0.1E1"s      , 1.0                },
        { L"0.1E12"s     , 100000000000.0     },
        { L"0.1E-1"s     , 0.01               },
        { L"0.1E-12"s    , 0.0000000000001    },
        { L"0.1e+1"s     , 1.0                },
        { L"0.1e+12"s    , 100000000000.0     },
        { L"0.1e1"s      , 1.0                },
        { L"0.1e12"s     , 100000000000.0     },
        { L"0.1e-1"s     , 0.01               },
        { L"0.1e-12"s    , 0.0000000000001    },
        { L"0.12"s       , 0.12               },
        { L"0.12E+1"s    , 1.2                },
        { L"0.12E+12"s   , 120000000000.0     },
        { L"0.12E1"s     , 1.2                },
        { L"0.12E12"s    , 120000000000.0     },
        { L"0.12E-1"s    , 0.012              },
        { L"0.12E-12"s   , 0.00000000000012   },
        { L"0.12e+1"s    , 1.2                },
        { L"0.12e+12"s   , 120000000000.0     },
        { L"0.12e1"s     , 1.2                },
        { L"0.12e12"s    , 120000000000.0     },
        { L"0.12e-1"s    , 0.012              },
        { L"0.12e-12"s   , 0.00000000000012   },
        { L"1"s          , 1.0                },
        { L"1E+1"s       , 10.0               },
        { L"1E+12"s      , 1000000000000.0    },
        { L"1E1"s        , 10.0               },
        { L"1E12"s       , 1000000000000.0    },
        { L"1E-1"s       , 0.1                },
        { L"1E-12"s      , 0.000000000001     },
        { L"1e+1"s       , 10.0               },
        { L"1e+12"s      , 1000000000000.0    },
        { L"1e1"s        , 10.0               },
        { L"1e12"s       , 1000000000000.0    },
        { L"1e-1"s       , 0.1                },
        { L"1e-12"s      , 0.000000000001     },
        { L"1.1"s        , 1.1                },
        { L"1.1E+1"s     , 11.0               },
        { L"1.1E+12"s    , 1100000000000.0    },
        { L"1.1E1"s      , 11.0               },
        { L"1.1E12"s     , 1100000000000.0    },
        { L"1.1E-1"s     , 0.11               },
        { L"1.1E-12"s    , 0.0000000000011    },
        { L"1.1e+1"s     , 11.0               },
        { L"1.1e+12"s    , 1100000000000.0    },
        { L"1.1e1"s      , 11.0               },
        { L"1.1e12"s     , 1100000000000.0    },
        { L"1.1e-1"s     , 0.11               },
        { L"1.1e-12"s    , 0.0000000000011    },
        { L"1.12"s       , 1.12               },
        { L"1.12E+1"s    , 11.2               },
        { L"1.12E+12"s   , 1120000000000.0    },
        { L"1.12E1"s     , 11.2               },
        { L"1.12E12"s    , 1120000000000.0    },
        { L"1.12E-1"s    , 0.112              },
        { L"1.12E-12"s   , 0.00000000000112   },
        { L"1.12e+1"s    , 11.2               },
        { L"1.12e+12"s   , 1120000000000.0    },
        { L"1.12e1"s     , 11.2               },
        { L"1.12e12"s    , 1120000000000.0    },
        { L"1.12e-1"s    , 0.112              },
        { L"1.12e-12"s   , 0.00000000000112   },
        { L"12"s         , 12.0               },
        { L"12E+1"s      , 120.0              },
        { L"12E+12"s     , 12000000000000.0   },
        { L"12E1"s       , 120.0              },
        { L"12E12"s      , 12000000000000.0   },
        { L"12E-1"s      , 1.2                },
        { L"12E-12"s     , 0.000000000012     },
        { L"12e+1"s      , 120.0              },
        { L"12e+12"s     , 12000000000000.0   },
        { L"12e1"s       , 120.0              },
        { L"12e12"s      , 12000000000000.0   },
        { L"12e-1"s      , 1.2                },
        { L"12e-12"s     , 0.000000000012     },
        { L"12.1"s       , 12.1               },
        { L"12.1E+1"s    , 121.0              },
        { L"12.1E+12"s   , 12100000000000.0   },
        { L"12.1E1"s     , 121.0              },
        { L"12.1E12"s    , 12100000000000.0   },
        { L"12.1E-1"s    , 1.21               },
        { L"12.1E-12"s   , 0.0000000000121    },
        { L"12.1e+1"s    , 121.0              },
        { L"12.1e+12"s   , 12100000000000.0   },
        { L"12.1e1"s     , 121.0              },
        { L"12.1e12"s    , 12100000000000.0   },
        { L"12.1e-1"s    , 1.21               },
        { L"12.1e-12"s   , 0.0000000000121    },
        { L"12.12"s      , 12.12              },
        { L"12.12E+1"s   , 121.2              },
        { L"12.12E+12"s  , 12120000000000.0   },
        { L"12.12E1"s    , 121.2              },
        { L"12.12E12"s   , 12120000000000.0   },
        { L"12.12E-1"s   , 1.212              },
        { L"12.12E-12"s  , 0.00000000001212   },
        { L"12.12e+1"s   , 121.2              },
        { L"12.12e+12"s  , 12120000000000.0   },
        { L"12.12e1"s    , 121.2              },
        { L"12.12e12"s   , 12120000000000.0   },
        { L"12.12e-1"s   , 1.212              },
        { L"12.12e-12"s  , 0.00000000001212   },
        { L"123"s        , 123.0              },
        { L"123E+1"s     , 1230.0             },
        { L"123E+12"s    , 123000000000000.0  },
        { L"123E1"s      , 1230.0             },
        { L"123E12"s     , 123000000000000.0  },
        { L"123E-1"s     , 12.3               },
        { L"123E-12"s    , 0.000000000123     },
        { L"123e+1"s     , 1230.0             },
        { L"123e+12"s    , 123000000000000.0  },
        { L"123e1"s      , 1230.0             },
        { L"123e12"s     , 123000000000000.0  },
        { L"123e-1"s     , 12.3               },
        { L"123e-12"s    , 0.000000000123     },
        { L"123.1"s      , 123.1              },
        { L"123.1E+1"s   , 1231.0             },
        { L"123.1E+12"s  , 123100000000000.0  },
        { L"123.1E1"s    , 1231.0             },
        { L"123.1E12"s   , 123100000000000.0  },
        { L"123.1E-1"s   , 12.31              },
        { L"123.1E-12"s  , 0.0000000001231    },
        { L"123.1e+1"s   , 1231.0             },
        { L"123.1e+12"s  , 123100000000000.0  },
        { L"123.1e1"s    , 1231.0             },
        { L"123.1e12"s   , 123100000000000.0  },
        { L"123.1e-1"s   , 12.31              },
        { L"123.1e-12"s  , 0.0000000001231    },
        { L"123.12"s     , 123.12             },
        { L"123.12E+1"s  , 1231.2             },
        { L"123.12E+12"s , 123120000000000.0  },
        { L"123.12E1"s   , 1231.2             },
        { L"123.12E12"s  , 123120000000000.0  },
        { L"123.12E-1"s  , 12.312             },
        { L"123.12E-12"s , 0.00000000012312   },
        { L"123.12e+1"s  , 1231.2             },
        { L"123.12e+12"s , 123120000000000.0  },
        { L"123.12e1"s   , 1231.2             },
        { L"123.12e12"s  , 123120000000000.0  },
        { L"123.12e-1"s  , 12.312             },
        { L"123.12e-12"s , 0.00000000012312   },
        { L"-0"s         , 0.0                },
        { L"-0E+1"s      , 0.0                },
        { L"-0E+12"s     , 0.0                },
        { L"-0E1"s       , 0.0                },
        { L"-0E12"s      , 0.0                },
        { L"-0E-1"s      , 0.0                },
        { L"-0E-12"s     , 0.0                },
        { L"-0e+1"s      , 0.0                },
        { L"-0e+12"s     , 0.0                },
        { L"-0e1"s       , 0.0                },
        { L"-0e12"s      , 0.0                },
        { L"-0e-1"s      , 0.0                },
        { L"-0e-12"s     , 0.0                },
        { L"-0.1"s       , -0.1               },
        { L"-0.1E+1"s    , -1.0               },
        { L"-0.1E+12"s   , -100000000000.0    },
        { L"-0.1E1"s     , -1.0               },
        { L"-0.1E12"s    , -100000000000.0    },
        { L"-0.1E-1"s    , -0.01              },
        { L"-0.1E-12"s   , -0.0000000000001   },
        { L"-0.1e+1"s    , -1.0               },
        { L"-0.1e+12"s   , -100000000000.0    },
        { L"-0.1e1"s     , -1.0               },
        { L"-0.1e12"s    , -100000000000.0    },
        { L"-0.1e-1"s    , -0.01              },
        { L"-0.1e-12"s   , -0.0000000000001   },
        { L"-0.12"s      , -0.12              },
        { L"-0.12E+1"s   , -1.2               },
        { L"-0.12E+12"s  , -120000000000.0    },
        { L"-0.12E1"s    , -1.2               },
        { L"-0.12E12"s   , -120000000000.0    },
        { L"-0.12E-1"s   , -0.012             },
        { L"-0.12E-12"s  , -0.00000000000012  },
        { L"-0.12e+1"s   , -1.2               },
        { L"-0.12e+12"s  , -120000000000.0    },
        { L"-0.12e1"s    , -1.2               },
        { L"-0.12e12"s   , -120000000000.0    },
        { L"-0.12e-1"s   , -0.012             },
        { L"-0.12e-12"s  , -0.00000000000012  },
        { L"-1"s         , -1.0               },
        { L"-1E+1"s      , -10.0              },
        { L"-1E+12"s     , -1000000000000.0   },
        { L"-1E1"s       , -10.0              },
        { L"-1E12"s      , -1000000000000.0   },
        { L"-1E-1"s      , -0.1               },
        { L"-1E-12"s     , -0.000000000001    },
        { L"-1e+1"s      , -10.0              },
        { L"-1e+12"s     , -1000000000000.0   },
        { L"-1e1"s       , -10.0              },
        { L"-1e12"s      , -1000000000000.0   },
        { L"-1e-1"s      , -0.1               },
        { L"-1e-12"s     , -0.000000000001    },
        { L"-1.1"s       , -1.1               },
        { L"-1.1E+1"s    , -11.0              },
        { L"-1.1E+12"s   , -1100000000000.0   },
        { L"-1.1E1"s     , -11.0              },
        { L"-1.1E12"s    , -1100000000000.0   },
        { L"-1.1E-1"s    , -0.11              },
        { L"-1.1E-12"s   , -0.0000000000011   },
        { L"-1.1e+1"s    , -11.0              },
        { L"-1.1e+12"s   , -1100000000000.0   },
        { L"-1.1e1"s     , -11.0              },
        { L"-1.1e12"s    , -1100000000000.0   },
        { L"-1.1e-1"s    , -0.11              },
        { L"-1.1e-12"s   , -0.0000000000011   },
        { L"-1.12"s      , -1.12              },
        { L"-1.12E+1"s   , -11.2              },
        { L"-1.12E+12"s  , -1120000000000.0   },
        { L"-1.12E1"s    , -11.2              },
        { L"-1.12E12"s   , -1120000000000.0   },
        { L"-1.12E-1"s   , -0.112             },
        { L"-1.12E-12"s  , -0.00000000000112  },
        { L"-1.12e+1"s   , -11.2              },
        { L"-1.12e+12"s  , -1120000000000.0   },
        { L"-1.12e1"s    , -11.2              },
        { L"-1.12e12"s   , -1120000000000.0   },
        { L"-1.12e-1"s   , -0.112             },
        { L"-1.12e-12"s  , -0.00000000000112  },
        { L"-12"s        , -12.0              },
        { L"-12E+1"s     , -120.0             },
        { L"-12E+12"s    , -12000000000000.0  },
        { L"-12E1"s      , -120.0             },
        { L"-12E12"s     , -12000000000000.0  },
        { L"-12E-1"s     , -1.2               },
        { L"-12E-12"s    , -0.000000000012    },
        { L"-12e+1"s     , -120.0             },
        { L"-12e+12"s    , -12000000000000.0  },
        { L"-12e1"s      , -120.0             },
        { L"-12e12"s     , -12000000000000.0  },
        { L"-12e-1"s     , -1.2               },
        { L"-12e-12"s    , -0.000000000012    },
        { L"-12.1"s      , -12.1              },
        { L"-12.1E+1"s   , -121.0             },
        { L"-12.1E+12"s  , -12100000000000.0  },
        { L"-12.1E1"s    , -121.0             },
        { L"-12.1E12"s   , -12100000000000.0  },
        { L"-12.1E-1"s   , -1.21              },
        { L"-12.1E-12"s  , -0.0000000000121   },
        { L"-12.1e+1"s   , -121.0             },
        { L"-12.1e+12"s  , -12100000000000.0  },
        { L"-12.1e1"s    , -121.0             },
        { L"-12.1e12"s   , -12100000000000.0  },
        { L"-12.1e-1"s   , -1.21              },
        { L"-12.1e-12"s  , -0.0000000000121   },
        { L"-12.12"s     , -12.12             },
        { L"-12.12E+1"s  , -121.2             },
        { L"-12.12E+12"s , -12120000000000.0  },
        { L"-12.12E1"s   , -121.2             },
        { L"-12.12E12"s  , -12120000000000.0  },
        { L"-12.12E-1"s  , -1.212             },
        { L"-12.12E-12"s , -0.00000000001212  },
        { L"-12.12e+1"s  , -121.2             },
        { L"-12.12e+12"s , -12120000000000.0  },
        { L"-12.12e1"s   , -121.2             },
        { L"-12.12e12"s  , -12120000000000.0  },
        { L"-12.12e-1"s  , -1.212             },
        { L"-12.12e-12"s , -0.00000000001212  },
        { L"-123"s       , -123.0             },
        { L"-123E+1"s    , -1230.0            },
        { L"-123E+12"s   , -123000000000000.0 },
        { L"-123E1"s     , -1230.0            },
        { L"-123E12"s    , -123000000000000.0 },
        { L"-123E-1"s    , -12.3              },
        { L"-123E-12"s   , -0.000000000123    },
        { L"-123e+1"s    , -1230.0            },
        { L"-123e+12"s   , -123000000000000.0 },
        { L"-123e1"s     , -1230.0            },
        { L"-123e12"s    , -123000000000000.0 },
        { L"-123e-1"s    , -12.3              },
        { L"-123e-12"s   , -0.000000000123    },
        { L"-123.1"s     , -123.1             },
        { L"-123.1E+1"s  , -1231.0            },
        { L"-123.1E+12"s , -123100000000000.0 },
        { L"-123.1E1"s   , -1231.0            },
        { L"-123.1E12"s  , -123100000000000.0 },
        { L"-123.1E-1"s  , -12.31             },
        { L"-123.1E-12"s , -0.0000000001231   },
        { L"-123.1e+1"s  , -1231.0            },
        { L"-123.1e+12"s , -123100000000000.0 },
        { L"-123.1e1"s   , -1231.0            },
        { L"-123.1e12"s  , -123100000000000.0 },
        { L"-123.1e-1"s  , -12.31             },
        { L"-123.1e-12"s , -0.0000000001231   },
        { L"-123.12"s    , -123.12            },
        { L"-123.12E+1"s , -1231.2            },
        { L"-123.12E+12"s, -123120000000000.0 },
        { L"-123.12E1"s  , -1231.2            },
        { L"-123.12E12"s , -123120000000000.0 },
        { L"-123.12E-1"s , -12.312            },
        { L"-123.12E-12"s, -0.00000000012312  },
        { L"-123.12e+1"s , -1231.2            },
        { L"-123.12e+12"s, -123120000000000.0 },
        { L"-123.12e1"s  , -1231.2            },
        { L"-123.12e12"s , -123120000000000.0 },
        { L"-123.12e-1"s , -12.312            },
        { L"-123.12e-12"s, -0.00000000012312  }
      };

      for (auto [value, expected] : pairs)
      {
        Assert::AreEqual(expected, ParseNumber(value));
        Assert::AreEqual(expected, ParseNumber(wstringstream(value)));
      }
    }

    TEST_METHOD(TestParseJsonObject)
    {
      auto pairs = map<wstring, JsonObject>
      {
        // Test whitespace handling
        { L"{}"s, {} },
        { L"{  }"s, {} },
        { L"{\"key1\":1337}"s,         { { L"key1", 1337 } } },
        { L"{  \"key1\":1337}"s,       { { L"key1", 1337 } } },
        { L"{\"key1\"  :1337}"s,       { { L"key1", 1337 } } },
        { L"{  \"key1\"  :1337}"s,     { { L"key1", 1337 } } },
        { L"{\"key1\":  1337}"s,       { { L"key1", 1337 } } },
        { L"{  \"key1\":  1337}"s,     { { L"key1", 1337 } } },
        { L"{\"key1\"  :  1337}"s,     { { L"key1", 1337 } } },
        { L"{  \"key1\"  :  1337}"s,   { { L"key1", 1337 } } },
        { L"{\"key1\":1337  }"s,       { { L"key1", 1337 } } },
        { L"{  \"key1\":1337  }"s,     { { L"key1", 1337 } } },
        { L"{\"key1\"  :1337  }"s,     { { L"key1", 1337 } } },
        { L"{  \"key1\"  :1337  }"s,   { { L"key1", 1337 } } },
        { L"{\"key1\":  1337  }"s,     { { L"key1", 1337 } } },
        { L"{  \"key1\":  1337  }"s,   { { L"key1", 1337 } } },
        { L"{\"key1\"  :  1337  }"s,   { { L"key1", 1337 } } },
        { L"{  \"key1\"  :  1337  }"s, { { L"key1", 1337 } } },
        // Simple object with 2 key value pair
        { L"{\"key1\":1337,\"key2\":\"value2\"}"s, { { L"key1", 1337 }, { L"key2", L"value2" } } },
        // Complex object with all types of values (string, number, object, array, bool, null)
        { L"{   \"string\":\"string\" ,   \"number\":1337  ,   \"object\":{   \"key1\":\"value1\" ,   \"key2\":\"value2\"  }  ,   \"array\":[ 1, 3, 3, 7 ]  ,   \"true\":true  ,   \"false\":false  ,   \"null\":null      }"s,
            { { L"string", L"string" }, { L"number", 1337 }, { L"object", { { L"key1", L"value1" }, { L"key2", L"value2" } } }, { L"array", { 1, 3, 3, 7 } }, { L"true", true }, { L"false", false }, { L"null", nullptr } } },
      };

      for (auto [value, expected] : pairs)
      {
        Assert::IsTrue(expected == ParseJsonObject(value));
        Assert::IsTrue(expected == ParseJsonObject(wstringstream(value)));
      }
    }

    TEST_METHOD(TestValueWrite)
    {
      auto input = vector<VALUE>{ nullptr, L"Hello \"World\""s, false, true, 13.37,
        JsonObject{ { L"a", 1 }, { L"b", 2.5 } },
        JsonArray{ 1, 2, 3 }
      };
      
      auto expected = vector<wstring>
      {
        LR"(null)"s,
        LR"("Hello \"World\"")"s,
        LR"(false)"s,
        LR"(true)"s,
        LR"(13.37)"s,
        LR"({
  "a": 1,
  "b": 2.5
})"s,
        LR"([
  1,
  2,
  3
])"s,
      };

      auto output = From(input).Select<wstring>([](VALUE const& value)
      {
        auto os = wostringstream();
        ValueWrite(os, value);
        return os.str();
      });
      
      Assert::IsTrue(From(expected).SequenceEqual(output));
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
  _CrtMemState ValueTest::_init;
  _CrtMemState ValueTest::_dest;
  _CrtMemState ValueTest::_result;
}