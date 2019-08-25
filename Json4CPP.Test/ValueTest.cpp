#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;
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
      auto pairs = vector<pair<VALUE, JsonType>>
      {
        { nullptr_t() , JsonType::Null    },
        { wstring()   , JsonType::String  },
        { bool()      , JsonType::Boolean },
        { double()    , JsonType::Number  },
        { JsonObject(), JsonType::Object  },
        { JsonArray() , JsonType::Array   },
      };
      for (auto& [input, expected] : pairs)
      {
        Assert::AreEqual(expected, Value::GetType(input));
      }
    }

    TEST_METHOD(TestGetType2)
    {
      auto pairs = vector<pair<VALUE_BUILDER, JsonBuilderType>>
      {
        { nullptr_t() , JsonBuilderType::Null    },
        { wstring()   , JsonBuilderType::String  },
        { bool()      , JsonBuilderType::Boolean },
        { double()    , JsonBuilderType::Number  },
        { JsonObject(), JsonBuilderType::Object  },
        { JsonArray() , JsonBuilderType::Array   },
        { vector<JsonBuilder>()                                                                     , JsonBuilderType::Empty  },
        { vector<JsonBuilder>{ JsonBuilder(L"Key"), JsonBuilder(L"Value") }                         , JsonBuilderType::Pair   },
        { vector<JsonBuilder>{ vector<JsonBuilder>{ JsonBuilder(L"Key0"), JsonBuilder(L"Value0") },
                               vector<JsonBuilder>{ JsonBuilder(L"Key1"), JsonBuilder(L"Value1") } }, JsonBuilderType::Object },
        { vector<JsonBuilder>{ JsonBuilder(1), JsonBuilder(2), JsonBuilder(3) }                     , JsonBuilderType::Array  },
      };
      for (auto& [input, expected] : pairs)
      {
        Assert::AreEqual(expected, Value::GetType(input));
      }
    }

    TEST_METHOD(TestParseNull)
    {
      auto pairs = vector<pair<wstring, bool>>
      {
        { L""      , true  },
        { L" "     , true  },
        { L" n"    , true  },
        { L" nu"   , true  },
        { L" nul"  , true  },
        { L" null" , false },
        { L" null ", false },
        { L" nu11 ", true  },
        { L" nill ", true  },
        { L" mull ", true  },
      };
      for (auto& [input, expectException] : pairs)
      {
        if (!expectException)
        {
          Assert::AreEqual<VALUE>(nullptr, Value::ParseNull(input));
          Assert::AreEqual<VALUE>(nullptr, Value::ParseNull(wstringstream(input)));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { Value::ParseNull(input); });
          Assert::ExpectException<exception>([&]() { Value::ParseNull(wstringstream(input)); });
        }
      }
    }

    TEST_METHOD(TestParseString)
    {
      auto pairs = vector<pair<wstring, wstring>>
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

      for (auto& [value, expected] : pairs)
      {
        Assert::AreEqual(expected, Value::ParseString(value));
        Assert::AreEqual(expected, Value::ParseString(wstringstream(value)));
      }
    }

    TEST_METHOD(TestParseBoolean1)
    {
      auto pairs = vector<pair<wstring, bool>>
      {
        { L"",       true  },
        { L" ",      true  },
        { L" t",     true  },
        { L" tr",    true  },
        { L" tru",   true  },
        { L" true",  false },
        { L" true ", false },
        { L" trie ", true  },
      };
      for (auto& [input, expectException] : pairs)
      {
        if (!expectException)
        {
          Assert::IsTrue(Value::ParseBoolean(input));
          Assert::IsTrue(Value::ParseBoolean(wstringstream(input)));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { Value::ParseBoolean(input); });
          Assert::ExpectException<exception>([&]() { Value::ParseBoolean(wstringstream(input)); });
        }
      }
    }

    TEST_METHOD(TestParseBoolean2)
    {
      auto pairs = vector<pair<wstring, bool>>
      {
        { L"",        true  },
        { L" ",       true  },
        { L" f",      true  },
        { L" fa",     true  },
        { L" fal",    true  },
        { L" fals",   true  },
        { L" false",  false },
        { L" false ", false },
        { L" fakse ", true  }
      };
      for (auto& [input, expectException] : pairs)
      {
        if (!expectException)
        {
          Assert::IsFalse(Value::ParseBoolean(input));
          Assert::IsFalse(Value::ParseBoolean(wstringstream(input)));
        }
        else
        {
          Assert::ExpectException<exception>([&]() { Value::ParseBoolean(input); });
          Assert::ExpectException<exception>([&]() { Value::ParseBoolean(wstringstream(input)); });
        }
      }
    }

    TEST_METHOD(TestParseNumber)
    {
      auto pairs = vector<pair<wstring, double>>
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

      for (auto& [value, expected] : pairs)
      {
        Assert::AreEqual(expected, Value::ParseNumber(value));
        Assert::AreEqual(expected, Value::ParseNumber(wstringstream(value)));
      }
    }

    TEST_METHOD(TestParseJsonObject)
    {
      auto pairs = vector<pair<wstring, JsonObject>>
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

      for (auto& [value, expected] : pairs)
      {
        Assert::AreEqual(expected, Value::ParseJsonObject(value));
        Assert::AreEqual(expected, Value::ParseJsonObject(wstringstream(value)));
      }
    }

    TEST_METHOD(TestParseJsonArray)
    {
      auto pairs = vector<pair<wstring, JsonArray>>
      {
        // Test whitespace handling
        { L"[]"s, {} },
        { L"[  ]"s, {} },
        { L"[1,2]"s,         { 1, 2 } },
        { L"[  1,2]"s,       { 1, 2 } },
        { L"[1  ,2]"s,       { 1, 2 } },
        { L"[1,  2]"s,       { 1, 2 } },
        { L"[1,2  ]"s,       { 1, 2 } },
        { L"[  1  ,2]"s,     { 1, 2 } },
        { L"[  1,  2]"s,     { 1, 2 } },
        { L"[  1,2  ]"s,     { 1, 2 } },
        { L"[1  ,  2]"s,     { 1, 2 } },
        { L"[1  ,2  ]"s,     { 1, 2 } },
        { L"[1,  2  ]"s,     { 1, 2 } },
        { L"[  1  ,  2]"s,   { 1, 2 } },
        { L"[  1  ,2  ]"s,   { 1, 2 } },
        { L"[  1,  2  ]"s,   { 1, 2 } },
        { L"[1  ,  2  ]"s,   { 1, 2 } },
        { L"[  1  ,  2  ]"s, { 1, 2 } },
        // Complex array with all types of values (string, number, object, array, bool, null)
        { L"[ \"string\",1337, {   \"key1\":\"value1\" ,   \"key2\":\"value2\"  }, [ 1, 3, 3, 7 ], true, false, null    ]"s,
            { L"string" ,1337, { { L"key1", L"value1" }, { L"key2", L"value2" } }, { 1, 3, 3, 7 }, true, false, nullptr } },
      };

      for (auto& [value, expected] : pairs)
      {
        Assert::AreEqual(expected, Value::ParseJsonArray(value));
        Assert::AreEqual(expected, Value::ParseJsonArray(wstringstream(value)));
      }
    }

    TEST_METHOD(TestParseJson)
    {
      auto pairs = vector<pair<wstring, Json>>
      {
        { L"\"string\""s, L"string"s },
        { L"1337"s, 1337 },

        // Test whitespace handling
        { L"{}"s, JsonObject{} },   // Must use explicit JsonObject, becase Json{} evalutes to null
        { L"{  }"s, JsonObject{} }, // Must use explicit JsonObject, becase Json{} evalutes to null
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

        // Test whitespace handling
        { L"[]"s, JsonArray{} },   // Must use explicit JsonArray, becase Json{} evalutes to null
        { L"[  ]"s, JsonArray{} }, // Must use explicit JsonArray, becase Json{} evalutes to null
        { L"[1,2]"s,         { 1, 2 } },
        { L"[  1,2]"s,       { 1, 2 } },
        { L"[1  ,2]"s,       { 1, 2 } },
        { L"[1,  2]"s,       { 1, 2 } },
        { L"[1,2  ]"s,       { 1, 2 } },
        { L"[  1  ,2]"s,     { 1, 2 } },
        { L"[  1,  2]"s,     { 1, 2 } },
        { L"[  1,2  ]"s,     { 1, 2 } },
        { L"[1  ,  2]"s,     { 1, 2 } },
        { L"[1  ,2  ]"s,     { 1, 2 } },
        { L"[1,  2  ]"s,     { 1, 2 } },
        { L"[  1  ,  2]"s,   { 1, 2 } },
        { L"[  1  ,2  ]"s,   { 1, 2 } },
        { L"[  1,  2  ]"s,   { 1, 2 } },
        { L"[1  ,  2  ]"s,   { 1, 2 } },
        { L"[  1  ,  2  ]"s, { 1, 2 } },
        // Complex array with all types of values (string, number, object, array, bool, null)
        { L"[ \"string\",1337, {   \"key1\":\"value1\" ,   \"key2\":\"value2\"  }, [ 1, 3, 3, 7 ], true, false, null    ]"s,
            { L"string" ,1337, { { L"key1", L"value1" }, { L"key2", L"value2" } }, { 1, 3, 3, 7 }, true, false, nullptr } },

        { L"true"s, true },
        { L"false"s, false },
        { L"null"s, nullptr },
      };

      for (auto& [value, expected] : pairs)
      {
        Assert::AreEqual(expected, Value::ParseJson(value));
        Assert::AreEqual(expected, Value::ParseJson(wstringstream(value)));
      }
    }

    TEST_METHOD(TestValueWrite)
    {
      JsonDefault::Indentation = 0;
      auto pairs = vector<pair<VALUE, wstring>>
      {
        { L"Hello \"World\""s                         , L"\"Hello \\\"World\\\"\""s },
        { 13.37                                       , L"13.37"s                   },
        { JsonObject{ { L"key1", 1 }, { L"key2", 2 } }, L"{\"key1\":1,\"key2\":2}"s },
        { JsonArray{ 1, 3, 3, 7 }                     , L"[1,3,3,7]"s               },
        { true                                        , L"true"s                    },
        { false                                       , L"false"s                   },
        { nullptr                                     , L"null"s                    },
      };

      for (auto& [value, expected] : pairs)
      {
        auto os = wostringstream();
        Value::Write(os, value);
        Assert::AreEqual(expected, os.str());
      }
    }

    TEST_METHOD(TestValueRead)
    {
      auto pairs = vector<pair<wstring, VALUE>>
      {
        { L"\"Hello \\\"World\\\"\""s, L"Hello \"World\""s                          },
        { L"13.37"s                  , 13.37                                        },
        { L"{\"key1\":1,\"key2\":2}"s, JsonObject{ { L"key1", 1 }, { L"key2", 2 } } },
        { L"[1,3,3,7]"s              , JsonArray{ 1, 3, 3, 7 }                      },
        { L"true"s                   , true                                         },
        { L"false"s                  , false                                        },
        { L"null"s                   , nullptr                                      },
      };

      for (auto& [value, expected] : pairs)
      {
        auto is = wistringstream(value);
        VALUE actual;
        Value::Read(is, actual);
        Assert::AreEqual(expected, actual);
      }
    }

    TEST_METHOD(TestValueEqual)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      Assert::IsTrue (Value::Equal(array1  , array1  ));
      Assert::IsFalse(Value::Equal(array1  , array2  ));
      Assert::IsFalse(Value::Equal(array1  , boolean1));
      Assert::IsFalse(Value::Equal(array1  , boolean2));
      Assert::IsFalse(Value::Equal(array1  , null    ));
      Assert::IsFalse(Value::Equal(array1  , number1 ));
      Assert::IsFalse(Value::Equal(array1  , number2 ));
      Assert::IsFalse(Value::Equal(array1  , object1 ));
      Assert::IsFalse(Value::Equal(array1  , object2 ));
      Assert::IsFalse(Value::Equal(array1  , string1 ));
      Assert::IsFalse(Value::Equal(array1  , string2 ));
      Assert::IsFalse(Value::Equal(array2  , array1  ));
      Assert::IsTrue (Value::Equal(array2  , array2  ));
      Assert::IsFalse(Value::Equal(array2  , boolean1));
      Assert::IsFalse(Value::Equal(array2  , boolean2));
      Assert::IsFalse(Value::Equal(array2  , null    ));
      Assert::IsFalse(Value::Equal(array2  , number1 ));
      Assert::IsFalse(Value::Equal(array2  , number2 ));
      Assert::IsFalse(Value::Equal(array2  , object1 ));
      Assert::IsFalse(Value::Equal(array2  , object2 ));
      Assert::IsFalse(Value::Equal(array2  , string1 ));
      Assert::IsFalse(Value::Equal(array2  , string2 ));
      Assert::IsFalse(Value::Equal(boolean1, array1  ));
      Assert::IsFalse(Value::Equal(boolean1, array2  ));
      Assert::IsTrue (Value::Equal(boolean1, boolean1));
      Assert::IsFalse(Value::Equal(boolean1, boolean2));
      Assert::IsFalse(Value::Equal(boolean1, null    ));
      Assert::IsTrue (Value::Equal(boolean1, number1 ));
      Assert::IsFalse(Value::Equal(boolean1, number2 ));
      Assert::IsFalse(Value::Equal(boolean1, object1 ));
      Assert::IsFalse(Value::Equal(boolean1, object2 ));
      Assert::IsFalse(Value::Equal(boolean1, string1 ));
      Assert::IsFalse(Value::Equal(boolean1, string2 ));
      Assert::IsFalse(Value::Equal(boolean2, array1  ));
      Assert::IsFalse(Value::Equal(boolean2, array2  ));
      Assert::IsFalse(Value::Equal(boolean2, boolean1));
      Assert::IsTrue (Value::Equal(boolean2, boolean2));
      Assert::IsFalse(Value::Equal(boolean2, null    ));
      Assert::IsFalse(Value::Equal(boolean2, number1 ));
      Assert::IsTrue (Value::Equal(boolean2, number2 ));
      Assert::IsFalse(Value::Equal(boolean2, object1 ));
      Assert::IsFalse(Value::Equal(boolean2, object2 ));
      Assert::IsFalse(Value::Equal(boolean2, string1 ));
      Assert::IsFalse(Value::Equal(boolean2, string2 ));
      Assert::IsFalse(Value::Equal(null    , array1  ));
      Assert::IsFalse(Value::Equal(null    , array2  ));
      Assert::IsFalse(Value::Equal(null    , boolean1));
      Assert::IsFalse(Value::Equal(null    , boolean2));
      Assert::IsTrue (Value::Equal(null    , null    ));
      Assert::IsFalse(Value::Equal(null    , number1 ));
      Assert::IsFalse(Value::Equal(null    , number2 ));
      Assert::IsFalse(Value::Equal(null    , object1 ));
      Assert::IsFalse(Value::Equal(null    , object2 ));
      Assert::IsFalse(Value::Equal(null    , string1 ));
      Assert::IsFalse(Value::Equal(null    , string2 ));
      Assert::IsFalse(Value::Equal(number1 , array1  ));
      Assert::IsFalse(Value::Equal(number1 , array2  ));
      Assert::IsTrue (Value::Equal(number1 , boolean1));
      Assert::IsFalse(Value::Equal(number1 , boolean2));
      Assert::IsFalse(Value::Equal(number1 , null    ));
      Assert::IsTrue (Value::Equal(number1 , number1 ));
      Assert::IsFalse(Value::Equal(number1 , number2 ));
      Assert::IsFalse(Value::Equal(number1 , object1 ));
      Assert::IsFalse(Value::Equal(number1 , object2 ));
      Assert::IsFalse(Value::Equal(number1 , string1 ));
      Assert::IsFalse(Value::Equal(number1 , string2 ));
      Assert::IsFalse(Value::Equal(number2 , array1  ));
      Assert::IsFalse(Value::Equal(number2 , array2  ));
      Assert::IsFalse(Value::Equal(number2 , boolean1));
      Assert::IsTrue (Value::Equal(number2 , boolean2));
      Assert::IsFalse(Value::Equal(number2 , null    ));
      Assert::IsFalse(Value::Equal(number2 , number1 ));
      Assert::IsTrue (Value::Equal(number2 , number2 ));
      Assert::IsFalse(Value::Equal(number2 , object1 ));
      Assert::IsFalse(Value::Equal(number2 , object2 ));
      Assert::IsFalse(Value::Equal(number2 , string1 ));
      Assert::IsFalse(Value::Equal(number2 , string2 ));
      Assert::IsFalse(Value::Equal(object1 , array1  ));
      Assert::IsFalse(Value::Equal(object1 , array2  ));
      Assert::IsFalse(Value::Equal(object1 , boolean1));
      Assert::IsFalse(Value::Equal(object1 , boolean2));
      Assert::IsFalse(Value::Equal(object1 , null    ));
      Assert::IsFalse(Value::Equal(object1 , number1 ));
      Assert::IsFalse(Value::Equal(object1 , number2 ));
      Assert::IsTrue (Value::Equal(object1 , object1 ));
      Assert::IsFalse(Value::Equal(object1 , object2 ));
      Assert::IsFalse(Value::Equal(object1 , string1 ));
      Assert::IsFalse(Value::Equal(object1 , string2 ));
      Assert::IsFalse(Value::Equal(object2 , array1  ));
      Assert::IsFalse(Value::Equal(object2 , array2  ));
      Assert::IsFalse(Value::Equal(object2 , boolean1));
      Assert::IsFalse(Value::Equal(object2 , boolean2));
      Assert::IsFalse(Value::Equal(object2 , null    ));
      Assert::IsFalse(Value::Equal(object2 , number1 ));
      Assert::IsFalse(Value::Equal(object2 , number2 ));
      Assert::IsFalse(Value::Equal(object2 , object1 ));
      Assert::IsTrue (Value::Equal(object2 , object2 ));
      Assert::IsFalse(Value::Equal(object2 , string1 ));
      Assert::IsFalse(Value::Equal(object2 , string2 ));
      Assert::IsFalse(Value::Equal(string1 , array1  ));
      Assert::IsFalse(Value::Equal(string1 , array2  ));
      Assert::IsFalse(Value::Equal(string1 , boolean1));
      Assert::IsFalse(Value::Equal(string1 , boolean2));
      Assert::IsFalse(Value::Equal(string1 , null    ));
      Assert::IsFalse(Value::Equal(string1 , number1 ));
      Assert::IsFalse(Value::Equal(string1 , number2 ));
      Assert::IsFalse(Value::Equal(string1 , object1 ));
      Assert::IsFalse(Value::Equal(string1 , object2 ));
      Assert::IsTrue (Value::Equal(string1 , string1 ));
      Assert::IsFalse(Value::Equal(string1 , string2 ));
      Assert::IsFalse(Value::Equal(string2 , array1  ));
      Assert::IsFalse(Value::Equal(string2 , array2  ));
      Assert::IsFalse(Value::Equal(string2 , boolean1));
      Assert::IsFalse(Value::Equal(string2 , boolean2));
      Assert::IsFalse(Value::Equal(string2 , null    ));
      Assert::IsFalse(Value::Equal(string2 , number1 ));
      Assert::IsFalse(Value::Equal(string2 , number2 ));
      Assert::IsFalse(Value::Equal(string2 , object1 ));
      Assert::IsFalse(Value::Equal(string2 , object2 ));
      Assert::IsFalse(Value::Equal(string2 , string1 ));
      Assert::IsTrue (Value::Equal(string2 , string2 ));
    }

    TEST_METHOD(TestValueNotEqual)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      Assert::IsFalse(Value::NotEqual(array1  , array1  ));
      Assert::IsTrue (Value::NotEqual(array1  , array2  ));
      Assert::IsTrue (Value::NotEqual(array1  , boolean1));
      Assert::IsTrue (Value::NotEqual(array1  , boolean2));
      Assert::IsTrue (Value::NotEqual(array1  , null    ));
      Assert::IsTrue (Value::NotEqual(array1  , number1 ));
      Assert::IsTrue (Value::NotEqual(array1  , number2 ));
      Assert::IsTrue (Value::NotEqual(array1  , object1 ));
      Assert::IsTrue (Value::NotEqual(array1  , object2 ));
      Assert::IsTrue (Value::NotEqual(array1  , string1 ));
      Assert::IsTrue (Value::NotEqual(array1  , string2 ));
      Assert::IsTrue (Value::NotEqual(array2  , array1  ));
      Assert::IsFalse(Value::NotEqual(array2  , array2  ));
      Assert::IsTrue (Value::NotEqual(array2  , boolean1));
      Assert::IsTrue (Value::NotEqual(array2  , boolean2));
      Assert::IsTrue (Value::NotEqual(array2  , null    ));
      Assert::IsTrue (Value::NotEqual(array2  , number1 ));
      Assert::IsTrue (Value::NotEqual(array2  , number2 ));
      Assert::IsTrue (Value::NotEqual(array2  , object1 ));
      Assert::IsTrue (Value::NotEqual(array2  , object2 ));
      Assert::IsTrue (Value::NotEqual(array2  , string1 ));
      Assert::IsTrue (Value::NotEqual(array2  , string2 ));
      Assert::IsTrue (Value::NotEqual(boolean1, array1  ));
      Assert::IsTrue (Value::NotEqual(boolean1, array2  ));
      Assert::IsFalse(Value::NotEqual(boolean1, boolean1));
      Assert::IsTrue (Value::NotEqual(boolean1, boolean2));
      Assert::IsTrue (Value::NotEqual(boolean1, null    ));
      Assert::IsFalse(Value::NotEqual(boolean1, number1 ));
      Assert::IsTrue (Value::NotEqual(boolean1, number2 ));
      Assert::IsTrue (Value::NotEqual(boolean1, object1 ));
      Assert::IsTrue (Value::NotEqual(boolean1, object2 ));
      Assert::IsTrue (Value::NotEqual(boolean1, string1 ));
      Assert::IsTrue (Value::NotEqual(boolean1, string2 ));
      Assert::IsTrue (Value::NotEqual(boolean2, array1  ));
      Assert::IsTrue (Value::NotEqual(boolean2, array2  ));
      Assert::IsTrue (Value::NotEqual(boolean2, boolean1));
      Assert::IsFalse(Value::NotEqual(boolean2, boolean2));
      Assert::IsTrue (Value::NotEqual(boolean2, null    ));
      Assert::IsTrue (Value::NotEqual(boolean2, number1 ));
      Assert::IsFalse(Value::NotEqual(boolean2, number2 ));
      Assert::IsTrue (Value::NotEqual(boolean2, object1 ));
      Assert::IsTrue (Value::NotEqual(boolean2, object2 ));
      Assert::IsTrue (Value::NotEqual(boolean2, string1 ));
      Assert::IsTrue (Value::NotEqual(boolean2, string2 ));
      Assert::IsTrue (Value::NotEqual(null    , array1  ));
      Assert::IsTrue (Value::NotEqual(null    , array2  ));
      Assert::IsTrue (Value::NotEqual(null    , boolean1));
      Assert::IsTrue (Value::NotEqual(null    , boolean2));
      Assert::IsFalse(Value::NotEqual(null    , null    ));
      Assert::IsTrue (Value::NotEqual(null    , number1 ));
      Assert::IsTrue (Value::NotEqual(null    , number2 ));
      Assert::IsTrue (Value::NotEqual(null    , object1 ));
      Assert::IsTrue (Value::NotEqual(null    , object2 ));
      Assert::IsTrue (Value::NotEqual(null    , string1 ));
      Assert::IsTrue (Value::NotEqual(null    , string2 ));
      Assert::IsTrue (Value::NotEqual(number1 , array1  ));
      Assert::IsTrue (Value::NotEqual(number1 , array2  ));
      Assert::IsFalse(Value::NotEqual(number1 , boolean1));
      Assert::IsTrue (Value::NotEqual(number1 , boolean2));
      Assert::IsTrue (Value::NotEqual(number1 , null    ));
      Assert::IsFalse(Value::NotEqual(number1 , number1 ));
      Assert::IsTrue (Value::NotEqual(number1 , number2 ));
      Assert::IsTrue (Value::NotEqual(number1 , object1 ));
      Assert::IsTrue (Value::NotEqual(number1 , object2 ));
      Assert::IsTrue (Value::NotEqual(number1 , string1 ));
      Assert::IsTrue (Value::NotEqual(number1 , string2 ));
      Assert::IsTrue (Value::NotEqual(number2 , array1  ));
      Assert::IsTrue (Value::NotEqual(number2 , array2  ));
      Assert::IsTrue (Value::NotEqual(number2 , boolean1));
      Assert::IsFalse(Value::NotEqual(number2 , boolean2));
      Assert::IsTrue (Value::NotEqual(number2 , null    ));
      Assert::IsTrue (Value::NotEqual(number2 , number1 ));
      Assert::IsFalse(Value::NotEqual(number2 , number2 ));
      Assert::IsTrue (Value::NotEqual(number2 , object1 ));
      Assert::IsTrue (Value::NotEqual(number2 , object2 ));
      Assert::IsTrue (Value::NotEqual(number2 , string1 ));
      Assert::IsTrue (Value::NotEqual(number2 , string2 ));
      Assert::IsTrue (Value::NotEqual(object1 , array1  ));
      Assert::IsTrue (Value::NotEqual(object1 , array2  ));
      Assert::IsTrue (Value::NotEqual(object1 , boolean1));
      Assert::IsTrue (Value::NotEqual(object1 , boolean2));
      Assert::IsTrue (Value::NotEqual(object1 , null    ));
      Assert::IsTrue (Value::NotEqual(object1 , number1 ));
      Assert::IsTrue (Value::NotEqual(object1 , number2 ));
      Assert::IsFalse(Value::NotEqual(object1 , object1 ));
      Assert::IsTrue (Value::NotEqual(object1 , object2 ));
      Assert::IsTrue (Value::NotEqual(object1 , string1 ));
      Assert::IsTrue (Value::NotEqual(object1 , string2 ));
      Assert::IsTrue (Value::NotEqual(object2 , array1  ));
      Assert::IsTrue (Value::NotEqual(object2 , array2  ));
      Assert::IsTrue (Value::NotEqual(object2 , boolean1));
      Assert::IsTrue (Value::NotEqual(object2 , boolean2));
      Assert::IsTrue (Value::NotEqual(object2 , null    ));
      Assert::IsTrue (Value::NotEqual(object2 , number1 ));
      Assert::IsTrue (Value::NotEqual(object2 , number2 ));
      Assert::IsTrue (Value::NotEqual(object2 , object1 ));
      Assert::IsFalse(Value::NotEqual(object2 , object2 ));
      Assert::IsTrue (Value::NotEqual(object2 , string1 ));
      Assert::IsTrue (Value::NotEqual(object2 , string2 ));
      Assert::IsTrue (Value::NotEqual(string1 , array1  ));
      Assert::IsTrue (Value::NotEqual(string1 , array2  ));
      Assert::IsTrue (Value::NotEqual(string1 , boolean1));
      Assert::IsTrue (Value::NotEqual(string1 , boolean2));
      Assert::IsTrue (Value::NotEqual(string1 , null    ));
      Assert::IsTrue (Value::NotEqual(string1 , number1 ));
      Assert::IsTrue (Value::NotEqual(string1 , number2 ));
      Assert::IsTrue (Value::NotEqual(string1 , object1 ));
      Assert::IsTrue (Value::NotEqual(string1 , object2 ));
      Assert::IsFalse(Value::NotEqual(string1 , string1 ));
      Assert::IsTrue (Value::NotEqual(string1 , string2 ));
      Assert::IsTrue (Value::NotEqual(string2 , array1  ));
      Assert::IsTrue (Value::NotEqual(string2 , array2  ));
      Assert::IsTrue (Value::NotEqual(string2 , boolean1));
      Assert::IsTrue (Value::NotEqual(string2 , boolean2));
      Assert::IsTrue (Value::NotEqual(string2 , null    ));
      Assert::IsTrue (Value::NotEqual(string2 , number1 ));
      Assert::IsTrue (Value::NotEqual(string2 , number2 ));
      Assert::IsTrue (Value::NotEqual(string2 , object1 ));
      Assert::IsTrue (Value::NotEqual(string2 , object2 ));
      Assert::IsTrue (Value::NotEqual(string2 , string1 ));
      Assert::IsFalse(Value::NotEqual(string2 , string2 ));
    }

    TEST_METHOD(TestValueLessThan)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { Value::LessThan(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, boolean2); });
      Assert::IsTrue(Value::LessThan(array1, null));
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, boolean2); });
      Assert::IsTrue(Value::LessThan(array2, null));
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean1, array2); });
      Assert::IsFalse(Value::LessThan(boolean1, boolean1));
      Assert::IsTrue (Value::LessThan(boolean1, boolean2));
      Assert::IsTrue (Value::LessThan(boolean1, null    ));
      Assert::IsFalse(Value::LessThan(boolean1, number1 ));
      Assert::IsTrue (Value::LessThan(boolean1, number2 ));
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean2, array2); });
      Assert::IsFalse(Value::LessThan(boolean2, boolean1));
      Assert::IsFalse(Value::LessThan(boolean2, boolean2));
      Assert::IsTrue (Value::LessThan(boolean2, null    ));
      Assert::IsFalse(Value::LessThan(boolean2, number1 ));
      Assert::IsFalse(Value::LessThan(boolean2, number2 ));
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(boolean2, string2); });
      Assert::IsFalse(Value::LessThan(null    , array1  ));
      Assert::IsFalse(Value::LessThan(null    , array2  ));
      Assert::IsFalse(Value::LessThan(null    , boolean1));
      Assert::IsFalse(Value::LessThan(null    , boolean2));
      Assert::IsFalse(Value::LessThan(null    , null    ));
      Assert::IsFalse(Value::LessThan(null    , number1 ));
      Assert::IsFalse(Value::LessThan(null    , number2 ));
      Assert::IsFalse(Value::LessThan(null    , object1 ));
      Assert::IsFalse(Value::LessThan(null    , object2 ));
      Assert::IsFalse(Value::LessThan(null    , string1 ));
      Assert::IsFalse(Value::LessThan(null    , string2 ));
      Assert::ExpectException<exception>([&] { Value::LessThan(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number1, array2); });
      Assert::IsFalse(Value::LessThan(number1 , boolean1));
      Assert::IsTrue (Value::LessThan(number1 , boolean2));
      Assert::IsTrue (Value::LessThan(number1 , null    ));
      Assert::IsFalse(Value::LessThan(number1 , number1 ));
      Assert::IsTrue (Value::LessThan(number1 , number2 ));
      Assert::ExpectException<exception>([&] { Value::LessThan(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number2, array2); });
      Assert::IsFalse(Value::LessThan(number2 , boolean1));
      Assert::IsFalse(Value::LessThan(number2 , boolean2));
      Assert::IsTrue (Value::LessThan(number2 , null    ));
      Assert::IsFalse(Value::LessThan(number2 , number1 ));
      Assert::IsFalse(Value::LessThan(number2 , number2 ));
      Assert::ExpectException<exception>([&] { Value::LessThan(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, boolean2); });
      Assert::IsTrue(Value::LessThan(object1, null));
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, boolean2); });
      Assert::IsTrue(Value::LessThan(object2, null));
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, boolean2); });
      Assert::IsTrue (Value::LessThan(string1 , null    ));
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string1, object2); });
      Assert::IsFalse(Value::LessThan(string1 , string1 ));
      Assert::IsTrue (Value::LessThan(string1 , string2 ));
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, boolean2); });
      Assert::IsTrue (Value::LessThan(string2 , null    ));
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThan(string2, object2); });
      Assert::IsFalse(Value::LessThan(string2 , string1 ));
      Assert::IsFalse(Value::LessThan(string2 , string2 ));
    }

    TEST_METHOD(TestValueLessThanOrEqual)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, boolean2); });
      Assert::IsTrue (Value::LessThanOrEqual(array1, null));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, boolean2); });
      Assert::IsTrue (Value::LessThanOrEqual(array2, null));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean1, array2); });
      Assert::IsTrue (Value::LessThanOrEqual(boolean1, boolean1));
      Assert::IsTrue (Value::LessThanOrEqual(boolean1, boolean2));
      Assert::IsTrue (Value::LessThanOrEqual(boolean1, null));
      Assert::IsTrue (Value::LessThanOrEqual(boolean1, number1));
      Assert::IsTrue (Value::LessThanOrEqual(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean2, array2); });
      Assert::IsFalse(Value::LessThanOrEqual(boolean2, boolean1));
      Assert::IsTrue (Value::LessThanOrEqual(boolean2, boolean2));
      Assert::IsTrue (Value::LessThanOrEqual(boolean2, null));
      Assert::IsFalse(Value::LessThanOrEqual(boolean2, number1));
      Assert::IsTrue (Value::LessThanOrEqual(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(boolean2, string2); });
      Assert::IsFalse(Value::LessThanOrEqual(null, array1));
      Assert::IsFalse(Value::LessThanOrEqual(null, array2));
      Assert::IsFalse(Value::LessThanOrEqual(null, boolean1));
      Assert::IsFalse(Value::LessThanOrEqual(null, boolean2));
      Assert::IsTrue (Value::LessThanOrEqual(null, null));
      Assert::IsFalse(Value::LessThanOrEqual(null, number1));
      Assert::IsFalse(Value::LessThanOrEqual(null, number2));
      Assert::IsFalse(Value::LessThanOrEqual(null, object1));
      Assert::IsFalse(Value::LessThanOrEqual(null, object2));
      Assert::IsFalse(Value::LessThanOrEqual(null, string1));
      Assert::IsFalse(Value::LessThanOrEqual(null, string2));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number1, array2); });
      Assert::IsTrue (Value::LessThanOrEqual(number1, boolean1));
      Assert::IsTrue (Value::LessThanOrEqual(number1, boolean2));
      Assert::IsTrue (Value::LessThanOrEqual(number1, null));
      Assert::IsTrue (Value::LessThanOrEqual(number1, number1));
      Assert::IsTrue (Value::LessThanOrEqual(number1, number2));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number2, array2); });
      Assert::IsFalse(Value::LessThanOrEqual(number2, boolean1));
      Assert::IsTrue (Value::LessThanOrEqual(number2, boolean2));
      Assert::IsTrue (Value::LessThanOrEqual(number2, null));
      Assert::IsFalse(Value::LessThanOrEqual(number2, number1));
      Assert::IsTrue (Value::LessThanOrEqual(number2, number2));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, boolean2); });
      Assert::IsTrue (Value::LessThanOrEqual(object1, null));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, boolean2); });
      Assert::IsTrue (Value::LessThanOrEqual(object2, null));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, boolean2); });
      Assert::IsTrue (Value::LessThanOrEqual(string1, null));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string1, object2); });
      Assert::IsTrue (Value::LessThanOrEqual(string1, string1));
      Assert::IsTrue (Value::LessThanOrEqual(string1, string2));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, boolean2); });
      Assert::IsTrue (Value::LessThanOrEqual(string2, null));
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::LessThanOrEqual(string2, object2); });
      Assert::IsFalse(Value::LessThanOrEqual(string2, string1));
      Assert::IsTrue (Value::LessThanOrEqual(string2, string2));
    }

    TEST_METHOD(TestValueGreaterThan)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, boolean2); });
      Assert::IsFalse(Value::GreaterThan(array1, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, boolean2); });
      Assert::IsFalse(Value::GreaterThan(array2, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean1, array2); });
      Assert::IsFalse(Value::GreaterThan(boolean1, boolean1));
      Assert::IsFalse(Value::GreaterThan(boolean1, boolean2));
      Assert::IsFalse(Value::GreaterThan(boolean1, null));
      Assert::IsFalse(Value::GreaterThan(boolean1, number1));
      Assert::IsFalse(Value::GreaterThan(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean2, array2); });
      Assert::IsTrue (Value::GreaterThan(boolean2, boolean1));
      Assert::IsFalse(Value::GreaterThan(boolean2, boolean2));
      Assert::IsFalse(Value::GreaterThan(boolean2, null));
      Assert::IsTrue (Value::GreaterThan(boolean2, number1));
      Assert::IsFalse(Value::GreaterThan(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(boolean2, string2); });
      Assert::IsTrue (Value::GreaterThan(null, array1));
      Assert::IsTrue (Value::GreaterThan(null, array2));
      Assert::IsTrue (Value::GreaterThan(null, boolean1));
      Assert::IsTrue (Value::GreaterThan(null, boolean2));
      Assert::IsFalse(Value::GreaterThan(null, null));
      Assert::IsTrue (Value::GreaterThan(null, number1));
      Assert::IsTrue (Value::GreaterThan(null, number2));
      Assert::IsTrue (Value::GreaterThan(null, object1));
      Assert::IsTrue (Value::GreaterThan(null, object2));
      Assert::IsTrue (Value::GreaterThan(null, string1));
      Assert::IsTrue (Value::GreaterThan(null, string2));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number1, array2); });
      Assert::IsFalse(Value::GreaterThan(number1, boolean1));
      Assert::IsFalse(Value::GreaterThan(number1, boolean2));
      Assert::IsFalse(Value::GreaterThan(number1, null));
      Assert::IsFalse(Value::GreaterThan(number1, number1));
      Assert::IsFalse(Value::GreaterThan(number1, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number2, array2); });
      Assert::IsTrue (Value::GreaterThan(number2, boolean1));
      Assert::IsFalse(Value::GreaterThan(number2, boolean2));
      Assert::IsFalse(Value::GreaterThan(number2, null));
      Assert::IsTrue (Value::GreaterThan(number2, number1));
      Assert::IsFalse(Value::GreaterThan(number2, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, boolean2); });
      Assert::IsFalse(Value::GreaterThan(object1, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, boolean2); });
      Assert::IsFalse(Value::GreaterThan(object2, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, boolean2); });
      Assert::IsFalse(Value::GreaterThan(string1, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string1, object2); });
      Assert::IsFalse(Value::GreaterThan(string1, string1));
      Assert::IsFalse(Value::GreaterThan(string1, string2));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, boolean2); });
      Assert::IsFalse(Value::GreaterThan(string2, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThan(string2, object2); });
      Assert::IsTrue (Value::GreaterThan(string2, string1));
      Assert::IsFalse(Value::GreaterThan(string2, string2));
    }

    TEST_METHOD(TestValueGreaterThanOrEqual)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, boolean2); });
      Assert::IsFalse(Value::GreaterThanOrEqual(array1, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, boolean2); });
      Assert::IsFalse(Value::GreaterThanOrEqual(array2, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean1, array2); });
      Assert::IsTrue (Value::GreaterThanOrEqual(boolean1, boolean1));
      Assert::IsFalse(Value::GreaterThanOrEqual(boolean1, boolean2));
      Assert::IsFalse(Value::GreaterThanOrEqual(boolean1, null));
      Assert::IsTrue (Value::GreaterThanOrEqual(boolean1, number1));
      Assert::IsFalse(Value::GreaterThanOrEqual(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean2, array2); });
      Assert::IsTrue (Value::GreaterThanOrEqual(boolean2, boolean1));
      Assert::IsTrue (Value::GreaterThanOrEqual(boolean2, boolean2));
      Assert::IsFalse(Value::GreaterThanOrEqual(boolean2, null));
      Assert::IsTrue (Value::GreaterThanOrEqual(boolean2, number1));
      Assert::IsTrue(Value::GreaterThanOrEqual(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(boolean2, string2); });
      Assert::IsTrue (Value::GreaterThanOrEqual(null, array1));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, array2));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, boolean1));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, boolean2));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, null));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, number1));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, number2));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, object1));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, object2));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, string1));
      Assert::IsTrue (Value::GreaterThanOrEqual(null, string2));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number1, array2); });
      Assert::IsTrue (Value::GreaterThanOrEqual(number1, boolean1));
      Assert::IsFalse(Value::GreaterThanOrEqual(number1, boolean2));
      Assert::IsFalse(Value::GreaterThanOrEqual(number1, null));
      Assert::IsTrue (Value::GreaterThanOrEqual(number1, number1));
      Assert::IsFalse(Value::GreaterThanOrEqual(number1, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number2, array2); });
      Assert::IsTrue (Value::GreaterThanOrEqual(number2, boolean1));
      Assert::IsTrue (Value::GreaterThanOrEqual(number2, boolean2));
      Assert::IsFalse(Value::GreaterThanOrEqual(number2, null));
      Assert::IsTrue (Value::GreaterThanOrEqual(number2, number1));
      Assert::IsTrue (Value::GreaterThanOrEqual(number2, number2));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, boolean2); });
      Assert::IsFalse(Value::GreaterThanOrEqual(object1, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, boolean2); });
      Assert::IsFalse(Value::GreaterThanOrEqual(object2, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, boolean2); });
      Assert::IsFalse(Value::GreaterThanOrEqual(string1, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string1, object2); });
      Assert::IsTrue (Value::GreaterThanOrEqual(string1, string1));
      Assert::IsFalse(Value::GreaterThanOrEqual(string1, string2));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, boolean2); });
      Assert::IsFalse(Value::GreaterThanOrEqual(string2, null));
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::GreaterThanOrEqual(string2, object2); });
      Assert::IsTrue (Value::GreaterThanOrEqual(string2, string1));
      Assert::IsTrue (Value::GreaterThanOrEqual(string2, string2));
    }

    TEST_METHOD(TestValueAdd)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

#pragma warning(push)
#pragma warning(disable : 26444)
      Assert::ExpectException<exception>([&] { Value::Add(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Add(array1, null));
      Assert::ExpectException<exception>([&] { Value::Add(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Add(array2, null));
      Assert::ExpectException<exception>([&] { Value::Add(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean1, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Add(boolean1, boolean1));
      Assert::AreEqual<VALUE>(1.0, Value::Add(boolean1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Add(boolean1, null));
      Assert::AreEqual<VALUE>(0.0, Value::Add(boolean1, number1));
      Assert::AreEqual<VALUE>(1.0, Value::Add(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::Add(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean2, array2); });
      Assert::AreEqual<VALUE>(1.0, Value::Add(boolean2, boolean1));
      Assert::AreEqual<VALUE>(2.0, Value::Add(boolean2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Add(boolean2, null));
      Assert::AreEqual<VALUE>(1.0, Value::Add(boolean2, number1));
      Assert::AreEqual<VALUE>(2.0, Value::Add(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::Add(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(boolean2, string2); });
      Assert::AreEqual<VALUE>(null, Value::Add(null, array1));
      Assert::AreEqual<VALUE>(null, Value::Add(null, array2));
      Assert::AreEqual<VALUE>(null, Value::Add(null, boolean1));
      Assert::AreEqual<VALUE>(null, Value::Add(null, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Add(null, null));
      Assert::AreEqual<VALUE>(null, Value::Add(null, number1));
      Assert::AreEqual<VALUE>(null, Value::Add(null, number2));
      Assert::AreEqual<VALUE>(null, Value::Add(null, object1));
      Assert::AreEqual<VALUE>(null, Value::Add(null, object2));
      Assert::AreEqual<VALUE>(null, Value::Add(null, string1));
      Assert::AreEqual<VALUE>(null, Value::Add(null, string2));
      Assert::ExpectException<exception>([&] { Value::Add(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(number1, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Add(number1, boolean1));
      Assert::AreEqual<VALUE>(1.0, Value::Add(number1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Add(number1, null));
      Assert::AreEqual<VALUE>(0.0, Value::Add(number1, number1));
      Assert::AreEqual<VALUE>(1.0, Value::Add(number1, number2));
      Assert::ExpectException<exception>([&] { Value::Add(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::Add(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(number2, array2); });
      Assert::AreEqual<VALUE>(1.0, Value::Add(number2, boolean1));
      Assert::AreEqual<VALUE>(2.0, Value::Add(number2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Add(number2, null));
      Assert::AreEqual<VALUE>(1.0, Value::Add(number2, number1));
      Assert::AreEqual<VALUE>(2.0, Value::Add(number2, number2));
      Assert::ExpectException<exception>([&] { Value::Add(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Add(object1, null));
      Assert::ExpectException<exception>([&] { Value::Add(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Add(object2, null));
      Assert::ExpectException<exception>([&] { Value::Add(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::Add(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::Add(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::Add(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Add(string1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Add(string1, null));
      Assert::ExpectException<exception>([&] { Value::Add(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::Add(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::Add(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(string1, object2); });
      Assert::AreEqual<VALUE>(L"TestString0TestString0"s, Value::Add(string1, string1));
      Assert::AreEqual<VALUE>(L"TestString0TestString1"s, Value::Add(string1, string2));
      Assert::ExpectException<exception>([&] { Value::Add(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::Add(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::Add(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Add(string2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Add(string2, null));
      Assert::ExpectException<exception>([&] { Value::Add(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::Add(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::Add(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::Add(string2, object2); });
      Assert::AreEqual<VALUE>(L"TestString1TestString0"s, Value::Add(string2, string1));
      Assert::AreEqual<VALUE>(L"TestString1TestString1"s, Value::Add(string2, string2));
#pragma warning( pop )
    }

    TEST_METHOD(TestAddAssign)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      VALUE value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::AddAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, boolean2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::AddAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, number2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, boolean1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(2.0, Value::AddAssign(value, boolean2));
      Assert::AreEqual<VALUE>(2.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, number1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(2.0, Value::AddAssign(value, number2));
      Assert::AreEqual<VALUE>(2.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, array1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, array2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, boolean1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, boolean2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, number1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, number2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, object1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, object2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, string1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, string2));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::AddAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, boolean2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::AddAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, number2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, boolean1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(2.0, Value::AddAssign(value, boolean2));
      Assert::AreEqual<VALUE>(2.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::AddAssign(value, number1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(2.0, Value::AddAssign(value, number2));
      Assert::AreEqual<VALUE>(2.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, string2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::AreEqual<VALUE>(L"TestString0TestString0"s, Value::AddAssign(value, string1));
      Assert::AreEqual<VALUE>(L"TestString0TestString0"s, value);
      value = string1;
      Assert::AreEqual<VALUE>(L"TestString0TestString1"s, Value::AddAssign(value, string2));
      Assert::AreEqual<VALUE>(L"TestString0TestString1"s, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, array2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::AreEqual<VALUE>(null, Value::AddAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, number2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::AddAssign(value, object2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::AreEqual<VALUE>(L"TestString1TestString0"s, Value::AddAssign(value, string1));
      Assert::AreEqual<VALUE>(L"TestString1TestString0"s, value);
      value = string2;
      Assert::AreEqual<VALUE>(L"TestString1TestString1"s, Value::AddAssign(value, string2));
      Assert::AreEqual<VALUE>(L"TestString1TestString1"s, value);
    }

    TEST_METHOD(TestValueSubtract)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

#pragma warning(push)
#pragma warning(disable : 26444)
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Subtract(array1, null));
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Subtract(array2, null));
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean1, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(boolean1, boolean1));
      Assert::AreEqual<VALUE>(-1.0, Value::Subtract(boolean1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(boolean1, null));
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(boolean1, number1));
      Assert::AreEqual<VALUE>(-1.0, Value::Subtract(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean2, array2); });
      Assert::AreEqual<VALUE>(1.0, Value::Subtract(boolean2, boolean1));
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(boolean2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(boolean2, null));
      Assert::AreEqual<VALUE>(1.0, Value::Subtract(boolean2, number1));
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(boolean2, string2); });
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, array1));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, array2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, boolean1));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, null));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, number1));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, number2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, object1));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, object2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, string1));
      Assert::AreEqual<VALUE>(null, Value::Subtract(null, string2));
      Assert::ExpectException<exception>([&] { Value::Subtract(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number1, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(number1, boolean1));
      Assert::AreEqual<VALUE>(-1.0, Value::Subtract(number1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(number1, null));
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(number1, number1));
      Assert::AreEqual<VALUE>(-1.0, Value::Subtract(number1, number2));
      Assert::ExpectException<exception>([&] { Value::Subtract(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number2, array2); });
      Assert::AreEqual<VALUE>(1.0, Value::Subtract(number2, boolean1));
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(number2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Subtract(number2, null));
      Assert::AreEqual<VALUE>(1.0, Value::Subtract(number2, number1));
      Assert::AreEqual<VALUE>(0.0, Value::Subtract(number2, number2));
      Assert::ExpectException<exception>([&] { Value::Subtract(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Subtract(object1, null));
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Subtract(object2, null));
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Subtract(string1, null));
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string1, string2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Subtract(string2, null));
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, object2); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, string1); });
      Assert::ExpectException<exception>([&] { Value::Subtract(string2, string2); });
#pragma warning( pop )
    }

    TEST_METHOD(TestValueSubtractAssign)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      VALUE value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(-1.0, Value::SubtractAssign(value, boolean2));
      Assert::AreEqual<VALUE>(-1.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(-1.0, Value::SubtractAssign(value, number2));
      Assert::AreEqual<VALUE>(-1.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::SubtractAssign(value, boolean1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::SubtractAssign(value, number1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, array1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, array2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, boolean1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, boolean2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, number1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, number2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, object1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, object2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, string1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, string2));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(-1.0, Value::SubtractAssign(value, boolean2));
      Assert::AreEqual<VALUE>(-1.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(-1.0, Value::SubtractAssign(value, number2));
      Assert::AreEqual<VALUE>(-1.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::SubtractAssign(value, boolean1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::SubtractAssign(value, number1));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(0.0, Value::SubtractAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, array2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::AreEqual<VALUE>(null, Value::SubtractAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, number2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, object2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::SubtractAssign(value, string2); });
      Assert::AreEqual<VALUE>(string2, value);
    }

    TEST_METHOD(TestValueMultiply)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

#pragma warning(push)
#pragma warning(disable : 26444)
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Multiply(array1, null));
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Multiply(array2, null));
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean1, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(boolean1, boolean1));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(boolean1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(boolean1, null));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(boolean1, number1));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean2, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(boolean2, boolean1));
      Assert::AreEqual<VALUE>(1.0, Value::Multiply(boolean2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(boolean2, null));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(boolean2, number1));
      Assert::AreEqual<VALUE>(1.0, Value::Multiply(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(boolean2, string2); });
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, array1));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, array2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, boolean1));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, null));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, number1));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, number2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, object1));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, object2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, string1));
      Assert::AreEqual<VALUE>(null, Value::Multiply(null, string2));
      Assert::ExpectException<exception>([&] { Value::Multiply(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number1, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(number1, boolean1));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(number1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(number1, null));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(number1, number1));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(number1, number2));
      Assert::ExpectException<exception>([&] { Value::Multiply(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number2, array2); });
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(number2, boolean1));
      Assert::AreEqual<VALUE>(1.0, Value::Multiply(number2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Multiply(number2, null));
      Assert::AreEqual<VALUE>(0.0, Value::Multiply(number2, number1));
      Assert::AreEqual<VALUE>(1.0, Value::Multiply(number2, number2));
      Assert::ExpectException<exception>([&] { Value::Multiply(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Multiply(object1, null));
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Multiply(object2, null));
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Multiply(string1, null));
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string1, string2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Multiply(string2, null));
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, object2); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, string1); });
      Assert::ExpectException<exception>([&] { Value::Multiply(string2, string2); });
#pragma warning( pop )
    }

    TEST_METHOD(TestValueMultiplyAssign)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      VALUE value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::MultiplyAssign(value, boolean2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::MultiplyAssign(value, number2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, array1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, array2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, boolean1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, boolean2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, number1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, number2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, object1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, object2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, string1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, string2));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, boolean1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::MultiplyAssign(value, boolean2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number2;
      Assert::AreEqual<VALUE>(0.0, Value::MultiplyAssign(value, number1));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::MultiplyAssign(value, number2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, array2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::AreEqual<VALUE>(null, Value::MultiplyAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, number2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, object2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::MultiplyAssign(value, string2); });
      Assert::AreEqual<VALUE>(string2, value);
    }

    TEST_METHOD(TestValueDivide)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

#pragma warning(push)
#pragma warning(disable : 26444)
      Assert::ExpectException<exception>([&] { Value::Divide(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Divide(array1, null));
      Assert::ExpectException<exception>([&] { Value::Divide(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Divide(array2, null));
      Assert::ExpectException<exception>([&] { Value::Divide(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean1, array2); });
      Assert::IsTrue(isnan(get<double>(Value::Divide(boolean1, boolean1))));
      Assert::AreEqual<VALUE>(0.0, Value::Divide(boolean1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Divide(boolean1, null));
      Assert::IsTrue(isnan(get<double>(Value::Divide(boolean1, number1))));
      Assert::AreEqual<VALUE>(0.0, Value::Divide(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::Divide(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean2, array2); });
      Assert::IsTrue(isinf(get<double>(Value::Divide(boolean2, boolean1))));
      Assert::AreEqual<VALUE>(1.0, Value::Divide(boolean2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Divide(boolean2, null));
      Assert::IsTrue(isinf(get<double>(Value::Divide(boolean2, number1))));
      Assert::AreEqual<VALUE>(1.0, Value::Divide(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::Divide(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(boolean2, string2); });
      Assert::AreEqual<VALUE>(null, Value::Divide(null, array1));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, array2));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, boolean1));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, null));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, number1));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, number2));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, object1));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, object2));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, string1));
      Assert::AreEqual<VALUE>(null, Value::Divide(null, string2));
      Assert::ExpectException<exception>([&] { Value::Divide(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(number1, array2); });
      Assert::IsTrue(isnan(get<double>(Value::Divide(number1, boolean1))));
      Assert::AreEqual<VALUE>(0.0, Value::Divide(number1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Divide(number1, null));
      Assert::IsTrue(isnan(get<double>(Value::Divide(number1, number1))));
      Assert::AreEqual<VALUE>(0.0, Value::Divide(number1, number2));
      Assert::ExpectException<exception>([&] { Value::Divide(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::Divide(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(number2, array2); });
      Assert::IsTrue(isinf(get<double>(Value::Divide(number2, boolean1))));
      Assert::AreEqual<VALUE>(1.0, Value::Divide(number2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Divide(number2, null));
      Assert::IsTrue(isinf(get<double>(Value::Divide(number2, number1))));
      Assert::AreEqual<VALUE>(1.0, Value::Divide(number2, number2));
      Assert::ExpectException<exception>([&] { Value::Divide(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Divide(object1, null));
      Assert::ExpectException<exception>([&] { Value::Divide(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Divide(object2, null));
      Assert::ExpectException<exception>([&] { Value::Divide(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::Divide(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::Divide(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::Divide(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Divide(string1, null));
      Assert::ExpectException<exception>([&] { Value::Divide(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::Divide(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string1, object2); });
      Assert::AreEqual<VALUE>(wstring(L"TestString0"s / L"TestString0"s), Value::Divide(string1, string1));
      Assert::AreEqual<VALUE>(wstring(L"TestString0"s / L"TestString1"s), Value::Divide(string1, string2));
      Assert::ExpectException<exception>([&] { Value::Divide(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::Divide(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Divide(string2, null));
      Assert::ExpectException<exception>([&] { Value::Divide(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::Divide(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::Divide(string2, object2); });
      Assert::AreEqual<VALUE>(wstring(L"TestString1"s / L"TestString0"s), Value::Divide(string2, string1));
      Assert::AreEqual<VALUE>(wstring(L"TestString1"s / L"TestString1"s), Value::Divide(string2, string2));
#pragma warning( pop )
    }

    TEST_METHOD(TestDivideAssign)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      VALUE value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::IsTrue(isnan(get<double>(Value::DivideAssign(value, boolean1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::DivideAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean1;
      Assert::IsTrue(isnan(get<double>(Value::DivideAssign(value, number1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::DivideAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::IsTrue(isinf(get<double>(Value::DivideAssign(value, boolean1))));
      Assert::IsTrue(isinf(get<double>(value)));
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::DivideAssign(value, boolean2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean2;
      Assert::IsTrue(isinf(get<double>(Value::DivideAssign(value, number1))));
      Assert::IsTrue(isinf(get<double>(value)));
      value = boolean2;
      Assert::AreEqual<VALUE>(1.0, Value::DivideAssign(value, number2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, array1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, array2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, boolean1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, boolean2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, number1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, number2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, object1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, object2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, string1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, string2));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::IsTrue(isnan(get<double>(Value::DivideAssign(value, boolean1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::DivideAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::IsTrue(isnan(get<double>(Value::DivideAssign(value, number1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::DivideAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::IsTrue(isinf(get<double>(Value::DivideAssign(value, boolean1))));
      Assert::IsTrue(isinf(get<double>(value)));
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::DivideAssign(value, boolean2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number2;
      Assert::IsTrue(isinf(get<double>(Value::DivideAssign(value, number1))));
      Assert::IsTrue(isinf(get<double>(value)));
      value = number2;
      Assert::AreEqual<VALUE>(1.0, Value::DivideAssign(value, number2));
      Assert::AreEqual<VALUE>(1.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, string2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::AreEqual<VALUE>(wstring(L"TestString0"s / L"TestString0"s), Value::DivideAssign(value, string1));
      Assert::AreEqual<VALUE>(wstring(L"TestString0"s / L"TestString0"s), value);
      value = string1;
      Assert::AreEqual<VALUE>(wstring(L"TestString0"s / L"TestString1"s), Value::DivideAssign(value, string2));
      Assert::AreEqual<VALUE>(wstring(L"TestString0"s / L"TestString1"s), value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, array2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::AreEqual<VALUE>(null, Value::DivideAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, number2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::DivideAssign(value, object2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::AreEqual<VALUE>(wstring(L"TestString1"s / L"TestString0"s), Value::DivideAssign(value, string1));
      Assert::AreEqual<VALUE>(wstring(L"TestString1"s / L"TestString0"s), value);
      value = string2;
      Assert::AreEqual<VALUE>(wstring(L"TestString1"s / L"TestString1"s), Value::DivideAssign(value, string2));
      Assert::AreEqual<VALUE>(wstring(L"TestString1"s / L"TestString1"s), value);
    }

    TEST_METHOD(TestValueModulo)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

#pragma warning(push)
#pragma warning(disable : 26444)
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, array2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Modulo(array1, null));
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, number1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, number2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array1, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, array2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Modulo(array2, null));
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, number1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, number2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(array2, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean1, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean1, array2); });
      Assert::IsTrue(isnan(get<double>(Value::Modulo(boolean1, boolean1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(boolean1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(boolean1, null));
      Assert::IsTrue(isnan(get<double>(Value::Modulo(boolean1, number1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(boolean1, number2));
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean1, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean1, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean1, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean1, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean2, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean2, array2); });
      Assert::IsTrue(isnan(get<double>(Value::Modulo(boolean2, boolean1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(boolean2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(boolean2, null));
      Assert::IsTrue(isnan(get<double>(Value::Modulo(boolean2, number1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(boolean2, number2));
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean2, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean2, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean2, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(boolean2, string2); });
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, array1));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, array2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, boolean1));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, null));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, number1));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, number2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, object1));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, object2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, string1));
      Assert::AreEqual<VALUE>(null, Value::Modulo(null, string2));
      Assert::ExpectException<exception>([&] { Value::Modulo(number1, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number1, array2); });
      Assert::IsTrue(isnan(get<double>(Value::Modulo(number1, boolean1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(number1, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(number1, null));
      Assert::IsTrue(isnan(get<double>(Value::Modulo(number1, number1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(number1, number2));
      Assert::ExpectException<exception>([&] { Value::Modulo(number1, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number1, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number1, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number1, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number2, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number2, array2); });
      Assert::IsTrue(isnan(get<double>(Value::Modulo(number2, boolean1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(number2, boolean2));
      Assert::AreEqual<VALUE>(null, Value::Modulo(number2, null));
      Assert::IsTrue(isnan(get<double>(Value::Modulo(number2, number1))));
      Assert::AreEqual<VALUE>(0.0, Value::Modulo(number2, number2));
      Assert::ExpectException<exception>([&] { Value::Modulo(number2, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number2, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number2, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(number2, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, array2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Modulo(object1, null));
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, number1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, number2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object1, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, array2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Modulo(object2, null));
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, number1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, number2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(object2, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, array2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Modulo(string1, null));
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, number1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, number2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string1, string2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, array1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, array2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, boolean1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, boolean2); });
      Assert::AreEqual<VALUE>(null, Value::Modulo(string2, null));
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, number1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, number2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, object1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, object2); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, string1); });
      Assert::ExpectException<exception>([&] { Value::Modulo(string2, string2); });
#pragma warning( pop )
    }

    TEST_METHOD(TestModuloAssign)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      VALUE value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, boolean1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean1;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, number1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = boolean1;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, boolean1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = boolean2;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean2;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = boolean2;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, number1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = boolean2;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(boolean2, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, array1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, array2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, boolean1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, boolean2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, number1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, number2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, object1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, object2));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, string1));
      Assert::AreEqual<VALUE>(null, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, string2));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, boolean1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number1;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, number1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = number1;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, boolean1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = number2;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, boolean2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number2;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = number2;
      Assert::IsTrue(isnan(get<double>(Value::ModuloAssign(value, number1))));
      Assert::IsTrue(isnan(get<double>(value)));
      value = number2;
      Assert::AreEqual<VALUE>(0.0, Value::ModuloAssign(value, number2));
      Assert::AreEqual<VALUE>(0.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(number2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, array2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, boolean2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::AreEqual<VALUE>(null, Value::ModuloAssign(value, null));
      Assert::AreEqual<VALUE>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, number2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, object2); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string1); });
      Assert::AreEqual<VALUE>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { Value::ModuloAssign(value, string2); });
      Assert::AreEqual<VALUE>(string2, value);
    }

    TEST_METHOD(TestValueNegate)
    {
      VALUE null = nullptr;
      VALUE string = L"TestString"s;
      VALUE boolean = true;
      VALUE number = 1.0;
      VALUE object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE array = JsonArray{ 1, 2, 3 };

#pragma warning(push)
#pragma warning(disable : 26444)
      Assert::ExpectException<exception>([&] { Value::Negate(array); });
      Assert::AreEqual<VALUE>(-1.0, Value::Negate(boolean));
      Assert::AreEqual<VALUE>(null, Value::Negate(null));
      Assert::AreEqual<VALUE>(-1.0, Value::Negate(number));
      Assert::ExpectException<exception>([&] { Value::Negate(object); });
      Assert::ExpectException<exception>([&] { Value::Negate(string); });
#pragma warning( pop )
    }

    TEST_METHOD(TestValueNot)
    {
      VALUE null = nullptr;
      VALUE string = L"TestString"s;
      VALUE boolean = true;
      VALUE number = 1.0;
      VALUE object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE array = JsonArray{ 1, 2, 3 };

#pragma warning(push)
#pragma warning(disable : 26444)
      Assert::ExpectException<exception>([&] { Value::Not(array); });
      Assert::AreEqual<VALUE>(!true, Value::Not(boolean));
      Assert::AreEqual<VALUE>(!nullptr, Value::Not(null));
      Assert::AreEqual<VALUE>(!1.0, Value::Not(number));
      Assert::ExpectException<exception>([&] { Value::Not(object); });
      Assert::ExpectException<exception>([&] { Value::Not(string); });
#pragma warning( pop )
    }

    TEST_METHOD(TestValuePreIncrement)
    {
      VALUE null = nullptr;
      VALUE string = L"TestString"s;
      VALUE boolean = true;
      VALUE number = 1.0;
      VALUE object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE array = JsonArray{ 1, 2, 3 };

      VALUE value = array;
      Assert::ExpectException<exception>([&] { Value::PreIncrement(value); });
      Assert::AreEqual<VALUE>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { Value::PreIncrement(value); });
      Assert::AreEqual<VALUE>(boolean, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::PreIncrement(value));
      Assert::AreEqual<VALUE>(null, value);
      value = number;
      Assert::AreEqual<VALUE>(2.0, Value::PreIncrement(value));
      Assert::AreEqual<VALUE>(2.0, value);
      value = object;
      Assert::ExpectException<exception>([&] { Value::PreIncrement(value); });
      Assert::AreEqual<VALUE>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { Value::PreIncrement(value); });
      Assert::AreEqual<VALUE>(string, value);
    }

    TEST_METHOD(TestValuePostIncrement)
    {
      VALUE null = nullptr;
      VALUE string = L"TestString"s;
      VALUE boolean = true;
      VALUE number = 1.0;
      VALUE object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE array = JsonArray{ 1, 2, 3 };

#pragma warning(push)
#pragma warning(disable : 26444)
      VALUE value = array;
      Assert::ExpectException<exception>([&] { Value::PostIncrement(value); });
      Assert::AreEqual<VALUE>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { Value::PostIncrement(value); });
      Assert::AreEqual<VALUE>(boolean, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::PostIncrement(value));
      Assert::AreEqual<VALUE>(null, value);
      value = number;
      Assert::AreEqual<VALUE>(1.0, Value::PostIncrement(value));
      Assert::AreEqual<VALUE>(2.0, value);
      value = object;
      Assert::ExpectException<exception>([&] { Value::PostIncrement(value); });
      Assert::AreEqual<VALUE>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { Value::PostIncrement(value); });
      Assert::AreEqual<VALUE>(string, value);
#pragma warning( pop )
    }

    TEST_METHOD(TestValuePreDecrement)
    {
      VALUE null = nullptr;
      VALUE string = L"TestString"s;
      VALUE boolean = true;
      VALUE number = 1.0;
      VALUE object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE array = JsonArray{ 1, 2, 3 };

      VALUE value = array;
      Assert::ExpectException<exception>([&] { Value::PreDecrement(value); });
      Assert::AreEqual<VALUE>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { Value::PreDecrement(value); });
      Assert::AreEqual<VALUE>(boolean, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::PreDecrement(value));
      Assert::AreEqual<VALUE>(null, value);
      value = number;
      Assert::AreEqual<VALUE>(0.0, Value::PreDecrement(value));
      Assert::AreEqual<VALUE>(0.0, value);
      value = object;
      Assert::ExpectException<exception>([&] { Value::PreDecrement(value); });
      Assert::AreEqual<VALUE>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { Value::PreDecrement(value); });
      Assert::AreEqual<VALUE>(string, value);
    }

    TEST_METHOD(TestValuePostDecrement)
    {
      VALUE null = nullptr;
      VALUE string = L"TestString"s;
      VALUE boolean = true;
      VALUE number = 1.0;
      VALUE object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE array = JsonArray{ 1, 2, 3 };

#pragma warning(push)
#pragma warning(disable : 26444)
      VALUE value = array;
      Assert::ExpectException<exception>([&] { Value::PostDecrement(value); });
      Assert::AreEqual<VALUE>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { Value::PostDecrement(value); });
      Assert::AreEqual<VALUE>(boolean, value);
      value = null;
      Assert::AreEqual<VALUE>(null, Value::PostDecrement(value));
      Assert::AreEqual<VALUE>(null, value);
      value = number;
      Assert::AreEqual<VALUE>(1.0, Value::PostDecrement(value));
      Assert::AreEqual<VALUE>(0.0, value);
      value = object;
      Assert::ExpectException<exception>([&] { Value::PostDecrement(value); });
      Assert::AreEqual<VALUE>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { Value::PostDecrement(value); });
      Assert::AreEqual<VALUE>(string, value);
#pragma warning( pop )
    }

    TEST_METHOD(TestValueLogicalAnd)
    {
      VALUE null = nullptr;
      VALUE string1 = L"TestString0"s;
      VALUE string2 = L"TestString1"s;
      VALUE boolean1 = false;
      VALUE boolean2 = true;
      VALUE number1 = 0.0;
      VALUE number2 = 1.0;
      VALUE object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      VALUE object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      VALUE array1 = JsonArray{ 1, 2, 3 };
      VALUE array2 = JsonArray{ 4, 5, 6 };

      Assert::IsTrue (Value::LogicalAnd(array1  , array1  ));
      Assert::IsTrue (Value::LogicalAnd(array1  , array2  ));
      Assert::IsFalse(Value::LogicalAnd(array1  , boolean1));
      Assert::IsTrue (Value::LogicalAnd(array1  , boolean2));
      Assert::IsFalse(Value::LogicalAnd(array1  , null    ));
      Assert::IsFalse(Value::LogicalAnd(array1  , number1 ));
      Assert::IsTrue (Value::LogicalAnd(array1  , number2 ));
      Assert::IsTrue (Value::LogicalAnd(array1  , object1 ));
      Assert::IsTrue (Value::LogicalAnd(array1  , object2 ));
      Assert::IsTrue (Value::LogicalAnd(array1  , string1 ));
      Assert::IsTrue (Value::LogicalAnd(array1  , string2 ));
      Assert::IsTrue (Value::LogicalAnd(array2  , array1  ));
      Assert::IsTrue (Value::LogicalAnd(array2  , array2  ));
      Assert::IsFalse(Value::LogicalAnd(array2  , boolean1));
      Assert::IsTrue (Value::LogicalAnd(array2  , boolean2));
      Assert::IsFalse(Value::LogicalAnd(array2  , null    ));
      Assert::IsFalse(Value::LogicalAnd(array2  , number1 ));
      Assert::IsTrue (Value::LogicalAnd(array2  , number2 ));
      Assert::IsTrue (Value::LogicalAnd(array2  , object1 ));
      Assert::IsTrue (Value::LogicalAnd(array2  , object2 ));
      Assert::IsTrue (Value::LogicalAnd(array2  , string1 ));
      Assert::IsTrue (Value::LogicalAnd(array2  , string2 ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, array1  ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, array2  ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, boolean1));
      Assert::IsFalse(Value::LogicalAnd(boolean1, boolean2));
      Assert::IsFalse(Value::LogicalAnd(boolean1, null    ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, number1 ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, number2 ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, object1 ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, object2 ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, string1 ));
      Assert::IsFalse(Value::LogicalAnd(boolean1, string2 ));
      Assert::IsTrue (Value::LogicalAnd(boolean2, array1  ));
      Assert::IsTrue (Value::LogicalAnd(boolean2, array2  ));
      Assert::IsFalse(Value::LogicalAnd(boolean2, boolean1));
      Assert::IsTrue (Value::LogicalAnd(boolean2, boolean2));
      Assert::IsFalse(Value::LogicalAnd(boolean2, null    ));
      Assert::IsFalse(Value::LogicalAnd(boolean2, number1 ));
      Assert::IsTrue (Value::LogicalAnd(boolean2, number2 ));
      Assert::IsTrue (Value::LogicalAnd(boolean2, object1 ));
      Assert::IsTrue (Value::LogicalAnd(boolean2, object2 ));
      Assert::IsTrue (Value::LogicalAnd(boolean2, string1 ));
      Assert::IsTrue (Value::LogicalAnd(boolean2, string2 ));
      Assert::IsFalse(Value::LogicalAnd(null    , array1  ));
      Assert::IsFalse(Value::LogicalAnd(null    , array2  ));
      Assert::IsFalse(Value::LogicalAnd(null    , boolean1));
      Assert::IsFalse(Value::LogicalAnd(null    , boolean2));
      Assert::IsFalse(Value::LogicalAnd(null    , null    ));
      Assert::IsFalse(Value::LogicalAnd(null    , number1 ));
      Assert::IsFalse(Value::LogicalAnd(null    , number2 ));
      Assert::IsFalse(Value::LogicalAnd(null    , object1 ));
      Assert::IsFalse(Value::LogicalAnd(null    , object2 ));
      Assert::IsFalse(Value::LogicalAnd(null    , string1 ));
      Assert::IsFalse(Value::LogicalAnd(null    , string2 ));
      Assert::IsFalse(Value::LogicalAnd(number1 , array1  ));
      Assert::IsFalse(Value::LogicalAnd(number1 , array2  ));
      Assert::IsFalse(Value::LogicalAnd(number1 , boolean1));
      Assert::IsFalse(Value::LogicalAnd(number1 , boolean2));
      Assert::IsFalse(Value::LogicalAnd(number1 , null    ));
      Assert::IsFalse(Value::LogicalAnd(number1 , number1 ));
      Assert::IsFalse(Value::LogicalAnd(number1 , number2 ));
      Assert::IsFalse(Value::LogicalAnd(number1 , object1 ));
      Assert::IsFalse(Value::LogicalAnd(number1 , object2 ));
      Assert::IsFalse(Value::LogicalAnd(number1 , string1 ));
      Assert::IsFalse(Value::LogicalAnd(number1 , string2 ));
      Assert::IsTrue (Value::LogicalAnd(number2 , array1  ));
      Assert::IsTrue (Value::LogicalAnd(number2 , array2  ));
      Assert::IsFalse(Value::LogicalAnd(number2 , boolean1));
      Assert::IsTrue (Value::LogicalAnd(number2 , boolean2));
      Assert::IsFalse(Value::LogicalAnd(number2 , null    ));
      Assert::IsFalse(Value::LogicalAnd(number2 , number1 ));
      Assert::IsTrue (Value::LogicalAnd(number2 , number2 ));
      Assert::IsTrue (Value::LogicalAnd(number2 , object1 ));
      Assert::IsTrue (Value::LogicalAnd(number2 , object2 ));
      Assert::IsTrue (Value::LogicalAnd(number2 , string1 ));
      Assert::IsTrue (Value::LogicalAnd(number2 , string2 ));
      Assert::IsTrue (Value::LogicalAnd(object1 , array1  ));
      Assert::IsTrue (Value::LogicalAnd(object1 , array2  ));
      Assert::IsFalse(Value::LogicalAnd(object1 , boolean1));
      Assert::IsTrue (Value::LogicalAnd(object1 , boolean2));
      Assert::IsFalse(Value::LogicalAnd(object1 , null    ));
      Assert::IsFalse(Value::LogicalAnd(object1 , number1 ));
      Assert::IsTrue (Value::LogicalAnd(object1 , number2 ));
      Assert::IsTrue (Value::LogicalAnd(object1 , object1 ));
      Assert::IsTrue (Value::LogicalAnd(object1 , object2 ));
      Assert::IsTrue (Value::LogicalAnd(object1 , string1 ));
      Assert::IsTrue (Value::LogicalAnd(object1 , string2 ));
      Assert::IsTrue (Value::LogicalAnd(object2 , array1  ));
      Assert::IsTrue (Value::LogicalAnd(object2 , array2  ));
      Assert::IsFalse(Value::LogicalAnd(object2 , boolean1));
      Assert::IsTrue (Value::LogicalAnd(object2 , boolean2));
      Assert::IsFalse(Value::LogicalAnd(object2 , null    ));
      Assert::IsFalse(Value::LogicalAnd(object2 , number1 ));
      Assert::IsTrue (Value::LogicalAnd(object2 , number2 ));
      Assert::IsTrue (Value::LogicalAnd(object2 , object1 ));
      Assert::IsTrue (Value::LogicalAnd(object2 , object2 ));
      Assert::IsTrue (Value::LogicalAnd(object2 , string1 ));
      Assert::IsTrue (Value::LogicalAnd(object2 , string2 ));
      Assert::IsTrue (Value::LogicalAnd(string1 , array1  ));
      Assert::IsTrue (Value::LogicalAnd(string1 , array2  ));
      Assert::IsFalse(Value::LogicalAnd(string1 , boolean1));
      Assert::IsTrue (Value::LogicalAnd(string1 , boolean2));
      Assert::IsFalse(Value::LogicalAnd(string1 , null    ));
      Assert::IsFalse(Value::LogicalAnd(string1 , number1 ));
      Assert::IsTrue (Value::LogicalAnd(string1 , number2 ));
      Assert::IsTrue (Value::LogicalAnd(string1 , object1 ));
      Assert::IsTrue (Value::LogicalAnd(string1 , object2 ));
      Assert::IsTrue (Value::LogicalAnd(string1 , string1 ));
      Assert::IsTrue (Value::LogicalAnd(string1 , string2 ));
      Assert::IsTrue (Value::LogicalAnd(string2 , array1  ));
      Assert::IsTrue (Value::LogicalAnd(string2 , array2  ));
      Assert::IsFalse(Value::LogicalAnd(string2 , boolean1));
      Assert::IsTrue (Value::LogicalAnd(string2 , boolean2));
      Assert::IsFalse(Value::LogicalAnd(string2 , null    ));
      Assert::IsFalse(Value::LogicalAnd(string2 , number1 ));
      Assert::IsTrue (Value::LogicalAnd(string2 , number2 ));
      Assert::IsTrue (Value::LogicalAnd(string2 , object1 ));
      Assert::IsTrue (Value::LogicalAnd(string2 , object2 ));
      Assert::IsTrue (Value::LogicalAnd(string2 , string1 ));
      Assert::IsTrue (Value::LogicalAnd(string2 , string2 ));
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