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

    TEST_METHOD(TestParseJsonArray)
    {
      auto pairs = map<wstring, JsonArray>
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

      for (auto [value, expected] : pairs)
      {
        Assert::IsTrue(expected == ParseJsonArray(value));
        Assert::IsTrue(expected == ParseJsonArray(wstringstream(value)));
      }
    }

    TEST_METHOD(TestParseJson)
    {
      auto pairs = map<wstring, Json>
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

      for (auto [value, expected] : pairs)
      {
        Assert::IsTrue(expected == ParseJson(value));
        Assert::IsTrue(expected == ParseJson(wstringstream(value)));
      }
    }

    TEST_METHOD(TestValueWrite)
    {
      JsonDefault::Indentation = 0;
      auto pairs = map<VALUE, wstring>
      {
        { L"Hello \"World\""s                         , L"\"Hello \\\"World\\\"\""s },
        { 13.37                                       , L"13.37"s                   },
        { JsonObject{ { L"key1", 1 }, { L"key2", 2 } }, L"{\"key1\":1,\"key2\":2}"s },
        { JsonArray{ 1, 3, 3, 7 }                     , L"[1,3,3,7]"s               },
        { true                                        , L"true"s                    },
        { false                                       , L"false"s                   },
        { nullptr                                     , L"null"s                    },
      };

      for (auto [value, expected] : pairs)
      {
        auto os = wostringstream();
        ValueWrite(os, value); 
        Assert::IsTrue(expected == os.str());
      }
    }

    TEST_METHOD(TestValueRead)
    {
      auto pairs = map<wstring, VALUE>
      {
        { L"\"Hello \\\"World\\\"\""s, L"Hello \"World\""s                          },
        { L"13.37"s                  , 13.37                                        },
        { L"{\"key1\":1,\"key2\":2}"s, JsonObject{ { L"key1", 1 }, { L"key2", 2 } } },
        { L"[1,3,3,7]"s              , JsonArray{ 1, 3, 3, 7 }                      },
        { L"true"s                   , true                                         },
        { L"false"s                  , false                                        },
        { L"null"s                   , nullptr                                      },
      };

      for (auto [value, expected] : pairs)
      {
        auto is = wistringstream(value);
        VALUE actual;
        ValueRead(is, actual);
        Assert::IsTrue(expected == actual);
      }
    }

    TEST_METHOD(TestValueEqual)
    {
      auto null = nullptr;
      auto string1 = L"TestString0"s;
      auto string2 = L"TestString1"s;
      auto boolean1 = false;
      auto boolean2 = true;
      auto number1 = 0.0;
      auto number2 = 1.0;
      auto object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      auto object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      auto array1 = JsonArray{ 1, 2, 3 };
      auto array2 = JsonArray{ 4, 5, 6 };

      Assert::IsTrue (ValueEqual(array1  , array1  ));
      Assert::IsFalse(ValueEqual(array1  , array2  ));
      Assert::IsFalse(ValueEqual(array1  , boolean1));
      Assert::IsFalse(ValueEqual(array1  , boolean2));
      Assert::IsFalse(ValueEqual(array1  , null    ));
      Assert::IsFalse(ValueEqual(array1  , number1 ));
      Assert::IsFalse(ValueEqual(array1  , number2 ));
      Assert::IsFalse(ValueEqual(array1  , object1 ));
      Assert::IsFalse(ValueEqual(array1  , object2 ));
      Assert::IsFalse(ValueEqual(array1  , string1 ));
      Assert::IsFalse(ValueEqual(array1  , string2 ));
      Assert::IsFalse(ValueEqual(array2  , array1  ));
      Assert::IsTrue (ValueEqual(array2  , array2  ));
      Assert::IsFalse(ValueEqual(array2  , boolean1));
      Assert::IsFalse(ValueEqual(array2  , boolean2));
      Assert::IsFalse(ValueEqual(array2  , null    ));
      Assert::IsFalse(ValueEqual(array2  , number1 ));
      Assert::IsFalse(ValueEqual(array2  , number2 ));
      Assert::IsFalse(ValueEqual(array2  , object1 ));
      Assert::IsFalse(ValueEqual(array2  , object2 ));
      Assert::IsFalse(ValueEqual(array2  , string1 ));
      Assert::IsFalse(ValueEqual(array2  , string2 ));
      Assert::IsFalse(ValueEqual(boolean1, array1  ));
      Assert::IsFalse(ValueEqual(boolean1, array2  ));
      Assert::IsTrue (ValueEqual(boolean1, boolean1));
      Assert::IsFalse(ValueEqual(boolean1, boolean2));
      Assert::IsFalse(ValueEqual(boolean1, null    ));
      Assert::IsTrue (ValueEqual(boolean1, number1 ));
      Assert::IsFalse(ValueEqual(boolean1, number2 ));
      Assert::IsFalse(ValueEqual(boolean1, object1 ));
      Assert::IsFalse(ValueEqual(boolean1, object2 ));
      Assert::IsFalse(ValueEqual(boolean1, string1 ));
      Assert::IsFalse(ValueEqual(boolean1, string2 ));
      Assert::IsFalse(ValueEqual(boolean2, array1  ));
      Assert::IsFalse(ValueEqual(boolean2, array2  ));
      Assert::IsFalse(ValueEqual(boolean2, boolean1));
      Assert::IsTrue (ValueEqual(boolean2, boolean2));
      Assert::IsFalse(ValueEqual(boolean2, null    ));
      Assert::IsFalse(ValueEqual(boolean2, number1 ));
      Assert::IsTrue (ValueEqual(boolean2, number2 ));
      Assert::IsFalse(ValueEqual(boolean2, object1 ));
      Assert::IsFalse(ValueEqual(boolean2, object2 ));
      Assert::IsFalse(ValueEqual(boolean2, string1 ));
      Assert::IsFalse(ValueEqual(boolean2, string2 ));
      Assert::IsFalse(ValueEqual(null    , array1  ));
      Assert::IsFalse(ValueEqual(null    , array2  ));
      Assert::IsFalse(ValueEqual(null    , boolean1));
      Assert::IsFalse(ValueEqual(null    , boolean2));
      Assert::IsTrue (ValueEqual(null    , null    ));
      Assert::IsFalse(ValueEqual(null    , number1 ));
      Assert::IsFalse(ValueEqual(null    , number2 ));
      Assert::IsFalse(ValueEqual(null    , object1 ));
      Assert::IsFalse(ValueEqual(null    , object2 ));
      Assert::IsFalse(ValueEqual(null    , string1 ));
      Assert::IsFalse(ValueEqual(null    , string2 ));
      Assert::IsFalse(ValueEqual(number1 , array1  ));
      Assert::IsFalse(ValueEqual(number1 , array2  ));
      Assert::IsTrue (ValueEqual(number1 , boolean1));
      Assert::IsFalse(ValueEqual(number1 , boolean2));
      Assert::IsFalse(ValueEqual(number1 , null    ));
      Assert::IsTrue (ValueEqual(number1 , number1 ));
      Assert::IsFalse(ValueEqual(number1 , number2 ));
      Assert::IsFalse(ValueEqual(number1 , object1 ));
      Assert::IsFalse(ValueEqual(number1 , object2 ));
      Assert::IsFalse(ValueEqual(number1 , string1 ));
      Assert::IsFalse(ValueEqual(number1 , string2 ));
      Assert::IsFalse(ValueEqual(number2 , array1  ));
      Assert::IsFalse(ValueEqual(number2 , array2  ));
      Assert::IsFalse(ValueEqual(number2 , boolean1));
      Assert::IsTrue (ValueEqual(number2 , boolean2));
      Assert::IsFalse(ValueEqual(number2 , null    ));
      Assert::IsFalse(ValueEqual(number2 , number1 ));
      Assert::IsTrue (ValueEqual(number2 , number2 ));
      Assert::IsFalse(ValueEqual(number2 , object1 ));
      Assert::IsFalse(ValueEqual(number2 , object2 ));
      Assert::IsFalse(ValueEqual(number2 , string1 ));
      Assert::IsFalse(ValueEqual(number2 , string2 ));
      Assert::IsFalse(ValueEqual(object1 , array1  ));
      Assert::IsFalse(ValueEqual(object1 , array2  ));
      Assert::IsFalse(ValueEqual(object1 , boolean1));
      Assert::IsFalse(ValueEqual(object1 , boolean2));
      Assert::IsFalse(ValueEqual(object1 , null    ));
      Assert::IsFalse(ValueEqual(object1 , number1 ));
      Assert::IsFalse(ValueEqual(object1 , number2 ));
      Assert::IsTrue (ValueEqual(object1 , object1 ));
      Assert::IsFalse(ValueEqual(object1 , object2 ));
      Assert::IsFalse(ValueEqual(object1 , string1 ));
      Assert::IsFalse(ValueEqual(object1 , string2 ));
      Assert::IsFalse(ValueEqual(object2 , array1  ));
      Assert::IsFalse(ValueEqual(object2 , array2  ));
      Assert::IsFalse(ValueEqual(object2 , boolean1));
      Assert::IsFalse(ValueEqual(object2 , boolean2));
      Assert::IsFalse(ValueEqual(object2 , null    ));
      Assert::IsFalse(ValueEqual(object2 , number1 ));
      Assert::IsFalse(ValueEqual(object2 , number2 ));
      Assert::IsFalse(ValueEqual(object2 , object1 ));
      Assert::IsTrue (ValueEqual(object2 , object2 ));
      Assert::IsFalse(ValueEqual(object2 , string1 ));
      Assert::IsFalse(ValueEqual(object2 , string2 ));
      Assert::IsFalse(ValueEqual(string1 , array1  ));
      Assert::IsFalse(ValueEqual(string1 , array2  ));
      Assert::IsFalse(ValueEqual(string1 , boolean1));
      Assert::IsFalse(ValueEqual(string1 , boolean2));
      Assert::IsFalse(ValueEqual(string1 , null    ));
      Assert::IsFalse(ValueEqual(string1 , number1 ));
      Assert::IsFalse(ValueEqual(string1 , number2 ));
      Assert::IsFalse(ValueEqual(string1 , object1 ));
      Assert::IsFalse(ValueEqual(string1 , object2 ));
      Assert::IsTrue (ValueEqual(string1 , string1 ));
      Assert::IsFalse(ValueEqual(string1 , string2 ));
      Assert::IsFalse(ValueEqual(string2 , array1  ));
      Assert::IsFalse(ValueEqual(string2 , array2  ));
      Assert::IsFalse(ValueEqual(string2 , boolean1));
      Assert::IsFalse(ValueEqual(string2 , boolean2));
      Assert::IsFalse(ValueEqual(string2 , null    ));
      Assert::IsFalse(ValueEqual(string2 , number1 ));
      Assert::IsFalse(ValueEqual(string2 , number2 ));
      Assert::IsFalse(ValueEqual(string2 , object1 ));
      Assert::IsFalse(ValueEqual(string2 , object2 ));
      Assert::IsFalse(ValueEqual(string2 , string1 ));
      Assert::IsTrue (ValueEqual(string2 , string2 ));
    }

    TEST_METHOD(TestValueNotEqual)
    {
      auto null = nullptr;
      auto string1 = L"TestString0"s;
      auto string2 = L"TestString1"s;
      auto boolean1 = false;
      auto boolean2 = true;
      auto number1 = 0.0;
      auto number2 = 1.0;
      auto object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      auto object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      auto array1 = JsonArray{ 1, 2, 3 };
      auto array2 = JsonArray{ 4, 5, 6 };

      Assert::IsFalse(ValueNotEqual(array1  , array1  ));
      Assert::IsTrue (ValueNotEqual(array1  , array2  ));
      Assert::IsTrue (ValueNotEqual(array1  , boolean1));
      Assert::IsTrue (ValueNotEqual(array1  , boolean2));
      Assert::IsTrue (ValueNotEqual(array1  , null    ));
      Assert::IsTrue (ValueNotEqual(array1  , number1 ));
      Assert::IsTrue (ValueNotEqual(array1  , number2 ));
      Assert::IsTrue (ValueNotEqual(array1  , object1 ));
      Assert::IsTrue (ValueNotEqual(array1  , object2 ));
      Assert::IsTrue (ValueNotEqual(array1  , string1 ));
      Assert::IsTrue (ValueNotEqual(array1  , string2 ));
      Assert::IsTrue (ValueNotEqual(array2  , array1  ));
      Assert::IsFalse(ValueNotEqual(array2  , array2  ));
      Assert::IsTrue (ValueNotEqual(array2  , boolean1));
      Assert::IsTrue (ValueNotEqual(array2  , boolean2));
      Assert::IsTrue (ValueNotEqual(array2  , null    ));
      Assert::IsTrue (ValueNotEqual(array2  , number1 ));
      Assert::IsTrue (ValueNotEqual(array2  , number2 ));
      Assert::IsTrue (ValueNotEqual(array2  , object1 ));
      Assert::IsTrue (ValueNotEqual(array2  , object2 ));
      Assert::IsTrue (ValueNotEqual(array2  , string1 ));
      Assert::IsTrue (ValueNotEqual(array2  , string2 ));
      Assert::IsTrue (ValueNotEqual(boolean1, array1  ));
      Assert::IsTrue (ValueNotEqual(boolean1, array2  ));
      Assert::IsFalse(ValueNotEqual(boolean1, boolean1));
      Assert::IsTrue (ValueNotEqual(boolean1, boolean2));
      Assert::IsTrue (ValueNotEqual(boolean1, null    ));
      Assert::IsFalse(ValueNotEqual(boolean1, number1 ));
      Assert::IsTrue (ValueNotEqual(boolean1, number2 ));
      Assert::IsTrue (ValueNotEqual(boolean1, object1 ));
      Assert::IsTrue (ValueNotEqual(boolean1, object2 ));
      Assert::IsTrue (ValueNotEqual(boolean1, string1 ));
      Assert::IsTrue (ValueNotEqual(boolean1, string2 ));
      Assert::IsTrue (ValueNotEqual(boolean2, array1  ));
      Assert::IsTrue (ValueNotEqual(boolean2, array2  ));
      Assert::IsTrue (ValueNotEqual(boolean2, boolean1));
      Assert::IsFalse(ValueNotEqual(boolean2, boolean2));
      Assert::IsTrue (ValueNotEqual(boolean2, null    ));
      Assert::IsTrue (ValueNotEqual(boolean2, number1 ));
      Assert::IsFalse(ValueNotEqual(boolean2, number2 ));
      Assert::IsTrue (ValueNotEqual(boolean2, object1 ));
      Assert::IsTrue (ValueNotEqual(boolean2, object2 ));
      Assert::IsTrue (ValueNotEqual(boolean2, string1 ));
      Assert::IsTrue (ValueNotEqual(boolean2, string2 ));
      Assert::IsTrue (ValueNotEqual(null    , array1  ));
      Assert::IsTrue (ValueNotEqual(null    , array2  ));
      Assert::IsTrue (ValueNotEqual(null    , boolean1));
      Assert::IsTrue (ValueNotEqual(null    , boolean2));
      Assert::IsFalse(ValueNotEqual(null    , null    ));
      Assert::IsTrue (ValueNotEqual(null    , number1 ));
      Assert::IsTrue (ValueNotEqual(null    , number2 ));
      Assert::IsTrue (ValueNotEqual(null    , object1 ));
      Assert::IsTrue (ValueNotEqual(null    , object2 ));
      Assert::IsTrue (ValueNotEqual(null    , string1 ));
      Assert::IsTrue (ValueNotEqual(null    , string2 ));
      Assert::IsTrue (ValueNotEqual(number1 , array1  ));
      Assert::IsTrue (ValueNotEqual(number1 , array2  ));
      Assert::IsFalse(ValueNotEqual(number1 , boolean1));
      Assert::IsTrue (ValueNotEqual(number1 , boolean2));
      Assert::IsTrue (ValueNotEqual(number1 , null    ));
      Assert::IsFalse(ValueNotEqual(number1 , number1 ));
      Assert::IsTrue (ValueNotEqual(number1 , number2 ));
      Assert::IsTrue (ValueNotEqual(number1 , object1 ));
      Assert::IsTrue (ValueNotEqual(number1 , object2 ));
      Assert::IsTrue (ValueNotEqual(number1 , string1 ));
      Assert::IsTrue (ValueNotEqual(number1 , string2 ));
      Assert::IsTrue (ValueNotEqual(number2 , array1  ));
      Assert::IsTrue (ValueNotEqual(number2 , array2  ));
      Assert::IsTrue (ValueNotEqual(number2 , boolean1));
      Assert::IsFalse(ValueNotEqual(number2 , boolean2));
      Assert::IsTrue (ValueNotEqual(number2 , null    ));
      Assert::IsTrue (ValueNotEqual(number2 , number1 ));
      Assert::IsFalse(ValueNotEqual(number2 , number2 ));
      Assert::IsTrue (ValueNotEqual(number2 , object1 ));
      Assert::IsTrue (ValueNotEqual(number2 , object2 ));
      Assert::IsTrue (ValueNotEqual(number2 , string1 ));
      Assert::IsTrue (ValueNotEqual(number2 , string2 ));
      Assert::IsTrue (ValueNotEqual(object1 , array1  ));
      Assert::IsTrue (ValueNotEqual(object1 , array2  ));
      Assert::IsTrue (ValueNotEqual(object1 , boolean1));
      Assert::IsTrue (ValueNotEqual(object1 , boolean2));
      Assert::IsTrue (ValueNotEqual(object1 , null    ));
      Assert::IsTrue (ValueNotEqual(object1 , number1 ));
      Assert::IsTrue (ValueNotEqual(object1 , number2 ));
      Assert::IsFalse(ValueNotEqual(object1 , object1 ));
      Assert::IsTrue (ValueNotEqual(object1 , object2 ));
      Assert::IsTrue (ValueNotEqual(object1 , string1 ));
      Assert::IsTrue (ValueNotEqual(object1 , string2 ));
      Assert::IsTrue (ValueNotEqual(object2 , array1  ));
      Assert::IsTrue (ValueNotEqual(object2 , array2  ));
      Assert::IsTrue (ValueNotEqual(object2 , boolean1));
      Assert::IsTrue (ValueNotEqual(object2 , boolean2));
      Assert::IsTrue (ValueNotEqual(object2 , null    ));
      Assert::IsTrue (ValueNotEqual(object2 , number1 ));
      Assert::IsTrue (ValueNotEqual(object2 , number2 ));
      Assert::IsTrue (ValueNotEqual(object2 , object1 ));
      Assert::IsFalse(ValueNotEqual(object2 , object2 ));
      Assert::IsTrue (ValueNotEqual(object2 , string1 ));
      Assert::IsTrue (ValueNotEqual(object2 , string2 ));
      Assert::IsTrue (ValueNotEqual(string1 , array1  ));
      Assert::IsTrue (ValueNotEqual(string1 , array2  ));
      Assert::IsTrue (ValueNotEqual(string1 , boolean1));
      Assert::IsTrue (ValueNotEqual(string1 , boolean2));
      Assert::IsTrue (ValueNotEqual(string1 , null    ));
      Assert::IsTrue (ValueNotEqual(string1 , number1 ));
      Assert::IsTrue (ValueNotEqual(string1 , number2 ));
      Assert::IsTrue (ValueNotEqual(string1 , object1 ));
      Assert::IsTrue (ValueNotEqual(string1 , object2 ));
      Assert::IsFalse(ValueNotEqual(string1 , string1 ));
      Assert::IsTrue (ValueNotEqual(string1 , string2 ));
      Assert::IsTrue (ValueNotEqual(string2 , array1  ));
      Assert::IsTrue (ValueNotEqual(string2 , array2  ));
      Assert::IsTrue (ValueNotEqual(string2 , boolean1));
      Assert::IsTrue (ValueNotEqual(string2 , boolean2));
      Assert::IsTrue (ValueNotEqual(string2 , null    ));
      Assert::IsTrue (ValueNotEqual(string2 , number1 ));
      Assert::IsTrue (ValueNotEqual(string2 , number2 ));
      Assert::IsTrue (ValueNotEqual(string2 , object1 ));
      Assert::IsTrue (ValueNotEqual(string2 , object2 ));
      Assert::IsTrue (ValueNotEqual(string2 , string1 ));
      Assert::IsFalse(ValueNotEqual(string2 , string2 ));
    }

    TEST_METHOD(TestValueLessThan)
    {
      auto null = nullptr;
      auto string1 = L"TestString0"s;
      auto string2 = L"TestString1"s;
      auto boolean1 = false;
      auto boolean2 = true;
      auto number1 = 0.0;
      auto number2 = 1.0;
      auto object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      auto object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      auto array1 = JsonArray{ 1, 2, 3 };
      auto array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { ValueLessThan(array1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, boolean2); });
      Assert::IsTrue(ValueLessThan(array1, null));
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, boolean2); });
      Assert::IsTrue(ValueLessThan(array2, null));
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(array2, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean1, array2); });
      Assert::IsFalse(ValueLessThan(boolean1, boolean1));
      Assert::IsTrue (ValueLessThan(boolean1, boolean2));
      Assert::IsTrue (ValueLessThan(boolean1, null    ));
      Assert::IsFalse(ValueLessThan(boolean1, number1 ));
      Assert::IsTrue (ValueLessThan(boolean1, number2 ));
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean2, array2); });
      Assert::IsFalse(ValueLessThan(boolean2, boolean1));
      Assert::IsFalse(ValueLessThan(boolean2, boolean2));
      Assert::IsTrue (ValueLessThan(boolean2, null    ));
      Assert::IsFalse(ValueLessThan(boolean2, number1 ));
      Assert::IsFalse(ValueLessThan(boolean2, number2 ));
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(boolean2, string2); });
      Assert::IsFalse(ValueLessThan(null    , array1  ));
      Assert::IsFalse(ValueLessThan(null    , array2  ));
      Assert::IsFalse(ValueLessThan(null    , boolean1));
      Assert::IsFalse(ValueLessThan(null    , boolean2));
      Assert::IsFalse(ValueLessThan(null    , null    ));
      Assert::IsFalse(ValueLessThan(null    , number1 ));
      Assert::IsFalse(ValueLessThan(null    , number2 ));
      Assert::IsFalse(ValueLessThan(null    , object1 ));
      Assert::IsFalse(ValueLessThan(null    , object2 ));
      Assert::IsFalse(ValueLessThan(null    , string1 ));
      Assert::IsFalse(ValueLessThan(null    , string2 ));
      Assert::ExpectException<exception>([&] { ValueLessThan(number1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number1, array2); });
      Assert::IsFalse(ValueLessThan(number1 , boolean1));
      Assert::IsTrue (ValueLessThan(number1 , boolean2));
      Assert::IsTrue (ValueLessThan(number1 , null    ));
      Assert::IsFalse(ValueLessThan(number1 , number1 ));
      Assert::IsTrue (ValueLessThan(number1 , number2 ));
      Assert::ExpectException<exception>([&] { ValueLessThan(number1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number2, array2); });
      Assert::IsFalse(ValueLessThan(number2 , boolean1));
      Assert::IsFalse(ValueLessThan(number2 , boolean2));
      Assert::IsTrue (ValueLessThan(number2 , null    ));
      Assert::IsFalse(ValueLessThan(number2 , number1 ));
      Assert::IsFalse(ValueLessThan(number2 , number2 ));
      Assert::ExpectException<exception>([&] { ValueLessThan(number2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(number2, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, boolean2); });
      Assert::IsTrue(ValueLessThan(object1, null));
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, boolean2); });
      Assert::IsTrue(ValueLessThan(object2, null));
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(object2, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, boolean2); });
      Assert::IsTrue (ValueLessThan(string1 , null    ));
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string1, object2); });
      Assert::IsFalse(ValueLessThan(string1 , string1 ));
      Assert::IsTrue (ValueLessThan(string1 , string2 ));
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, boolean2); });
      Assert::IsTrue (ValueLessThan(string2 , null    ));
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThan(string2, object2); });
      Assert::IsFalse(ValueLessThan(string2 , string1 ));
      Assert::IsFalse(ValueLessThan(string2 , string2 ));
    }

    TEST_METHOD(TestValueLessThanOrEqual)
    {
      auto null = nullptr;
      auto string1 = L"TestString0"s;
      auto string2 = L"TestString1"s;
      auto boolean1 = false;
      auto boolean2 = true;
      auto number1 = 0.0;
      auto number2 = 1.0;
      auto object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      auto object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      auto array1 = JsonArray{ 1, 2, 3 };
      auto array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, boolean2); });
      Assert::IsTrue (ValueLessThanOrEqual(array1, null));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, boolean2); });
      Assert::IsTrue (ValueLessThanOrEqual(array2, null));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(array2, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean1, array2); });
      Assert::IsTrue (ValueLessThanOrEqual(boolean1, boolean1));
      Assert::IsTrue (ValueLessThanOrEqual(boolean1, boolean2));
      Assert::IsTrue (ValueLessThanOrEqual(boolean1, null));
      Assert::IsTrue (ValueLessThanOrEqual(boolean1, number1));
      Assert::IsTrue (ValueLessThanOrEqual(boolean1, number2));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean2, array2); });
      Assert::IsFalse(ValueLessThanOrEqual(boolean2, boolean1));
      Assert::IsTrue (ValueLessThanOrEqual(boolean2, boolean2));
      Assert::IsTrue (ValueLessThanOrEqual(boolean2, null));
      Assert::IsFalse(ValueLessThanOrEqual(boolean2, number1));
      Assert::IsTrue (ValueLessThanOrEqual(boolean2, number2));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(boolean2, string2); });
      Assert::IsFalse(ValueLessThanOrEqual(null, array1));
      Assert::IsFalse(ValueLessThanOrEqual(null, array2));
      Assert::IsFalse(ValueLessThanOrEqual(null, boolean1));
      Assert::IsFalse(ValueLessThanOrEqual(null, boolean2));
      Assert::IsTrue (ValueLessThanOrEqual(null, null));
      Assert::IsFalse(ValueLessThanOrEqual(null, number1));
      Assert::IsFalse(ValueLessThanOrEqual(null, number2));
      Assert::IsFalse(ValueLessThanOrEqual(null, object1));
      Assert::IsFalse(ValueLessThanOrEqual(null, object2));
      Assert::IsFalse(ValueLessThanOrEqual(null, string1));
      Assert::IsFalse(ValueLessThanOrEqual(null, string2));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number1, array2); });
      Assert::IsTrue (ValueLessThanOrEqual(number1, boolean1));
      Assert::IsTrue (ValueLessThanOrEqual(number1, boolean2));
      Assert::IsTrue (ValueLessThanOrEqual(number1, null));
      Assert::IsTrue (ValueLessThanOrEqual(number1, number1));
      Assert::IsTrue (ValueLessThanOrEqual(number1, number2));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number2, array2); });
      Assert::IsFalse(ValueLessThanOrEqual(number2, boolean1));
      Assert::IsTrue (ValueLessThanOrEqual(number2, boolean2));
      Assert::IsTrue (ValueLessThanOrEqual(number2, null));
      Assert::IsFalse(ValueLessThanOrEqual(number2, number1));
      Assert::IsTrue (ValueLessThanOrEqual(number2, number2));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(number2, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, boolean2); });
      Assert::IsTrue (ValueLessThanOrEqual(object1, null));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object1, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, boolean2); });
      Assert::IsTrue (ValueLessThanOrEqual(object2, null));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, object2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, string1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(object2, string2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, boolean2); });
      Assert::IsTrue (ValueLessThanOrEqual(string1, null));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string1, object2); });
      Assert::IsTrue (ValueLessThanOrEqual(string1, string1));
      Assert::IsTrue (ValueLessThanOrEqual(string1, string2));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, array1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, array2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, boolean2); });
      Assert::IsTrue (ValueLessThanOrEqual(string2, null));
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, number1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, number2); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, object1); });
      Assert::ExpectException<exception>([&] { ValueLessThanOrEqual(string2, object2); });
      Assert::IsFalse(ValueLessThanOrEqual(string2, string1));
      Assert::IsTrue (ValueLessThanOrEqual(string2, string2));
    }

    TEST_METHOD(TestValueGreaterThan)
    {
      auto null = nullptr;
      auto string1 = L"TestString0"s;
      auto string2 = L"TestString1"s;
      auto boolean1 = false;
      auto boolean2 = true;
      auto number1 = 0.0;
      auto number2 = 1.0;
      auto object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      auto object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      auto array1 = JsonArray{ 1, 2, 3 };
      auto array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, array2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, boolean2); });
      Assert::IsFalse(ValueGreaterThan(array1, null));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, number1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, number2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array1, string2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, array2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, boolean2); });
      Assert::IsFalse(ValueGreaterThan(array2, null));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, number1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, number2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(array2, string2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean1, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean1, array2); });
      Assert::IsFalse(ValueGreaterThan(boolean1, boolean1));
      Assert::IsFalse(ValueGreaterThan(boolean1, boolean2));
      Assert::IsFalse(ValueGreaterThan(boolean1, null));
      Assert::IsFalse(ValueGreaterThan(boolean1, number1));
      Assert::IsFalse(ValueGreaterThan(boolean1, number2));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean1, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean1, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean1, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean1, string2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean2, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean2, array2); });
      Assert::IsTrue (ValueGreaterThan(boolean2, boolean1));
      Assert::IsFalse(ValueGreaterThan(boolean2, boolean2));
      Assert::IsFalse(ValueGreaterThan(boolean2, null));
      Assert::IsTrue (ValueGreaterThan(boolean2, number1));
      Assert::IsFalse(ValueGreaterThan(boolean2, number2));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean2, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean2, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean2, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(boolean2, string2); });
      Assert::IsTrue (ValueGreaterThan(null, array1));
      Assert::IsTrue (ValueGreaterThan(null, array2));
      Assert::IsTrue (ValueGreaterThan(null, boolean1));
      Assert::IsTrue (ValueGreaterThan(null, boolean2));
      Assert::IsFalse(ValueGreaterThan(null, null));
      Assert::IsTrue (ValueGreaterThan(null, number1));
      Assert::IsTrue (ValueGreaterThan(null, number2));
      Assert::IsTrue (ValueGreaterThan(null, object1));
      Assert::IsTrue (ValueGreaterThan(null, object2));
      Assert::IsTrue (ValueGreaterThan(null, string1));
      Assert::IsTrue (ValueGreaterThan(null, string2));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number1, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number1, array2); });
      Assert::IsFalse(ValueGreaterThan(number1, boolean1));
      Assert::IsFalse(ValueGreaterThan(number1, boolean2));
      Assert::IsFalse(ValueGreaterThan(number1, null));
      Assert::IsFalse(ValueGreaterThan(number1, number1));
      Assert::IsFalse(ValueGreaterThan(number1, number2));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number1, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number1, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number1, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number1, string2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number2, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number2, array2); });
      Assert::IsTrue (ValueGreaterThan(number2, boolean1));
      Assert::IsFalse(ValueGreaterThan(number2, boolean2));
      Assert::IsFalse(ValueGreaterThan(number2, null));
      Assert::IsTrue (ValueGreaterThan(number2, number1));
      Assert::IsFalse(ValueGreaterThan(number2, number2));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number2, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number2, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number2, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(number2, string2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, array2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, boolean2); });
      Assert::IsFalse(ValueGreaterThan(object1, null));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, number1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, number2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object1, string2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, array2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, boolean2); });
      Assert::IsFalse(ValueGreaterThan(object2, null));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, number1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, number2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, object2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, string1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(object2, string2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, array2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, boolean1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, boolean2); });
      Assert::IsFalse(ValueGreaterThan(string1, null));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, number1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, number2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string1, object2); });
      Assert::IsFalse(ValueGreaterThan(string1, string1));
      Assert::IsFalse(ValueGreaterThan(string1, string2));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, array1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, array2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, boolean1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, boolean2); });
      Assert::IsFalse(ValueGreaterThan(string2, null));
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, number1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, number2); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, object1); });
      Assert::ExpectException<exception>([&] { ValueGreaterThan(string2, object2); });
      Assert::IsTrue (ValueGreaterThan(string2, string1));
      Assert::IsFalse(ValueGreaterThan(string2, string2));
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