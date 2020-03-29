#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonTokenTest)
  {
  public:
    TEST_METHOD(TestOperatorInsertion)
    {
      auto pairs = vector<pair<JsonTokenType, wstring>>
      {
        { JsonTokenType::Null        , L"Null"s         },
        { JsonTokenType::String      , L"String"s       },
        { JsonTokenType::Boolean     , L"Boolean"s      },
        { JsonTokenType::Number      , L"Number"s       },
        { JsonTokenType::PropertyName, L"PropertyName"s },
        { JsonTokenType::StartObject , L"StartObject"s  },
        { JsonTokenType::EndObject   , L"EndObject"s    },
        { JsonTokenType::StartArray  , L"StartArray"s   },
        { JsonTokenType::EndArray    , L"EndArray"s     },
      };
      for (auto& [input, expected] : pairs)
      {
        wostringstream os;
        os << input;
        Assert::AreEqual(expected, os.str());
      }
    }

    TEST_METHOD(TestDump)
    {
      auto pairs = vector<pair<JsonTokenType, wstring>>
      {
        { JsonTokenType::Null        , L"Null"s         },
        { JsonTokenType::String      , L"String"s       },
        { JsonTokenType::Boolean     , L"Boolean"s      },
        { JsonTokenType::Number      , L"Number"s       },
        { JsonTokenType::PropertyName, L"PropertyName"s },
        { JsonTokenType::StartObject , L"StartObject"s  },
        { JsonTokenType::EndObject   , L"EndObject"s    },
        { JsonTokenType::StartArray  , L"StartArray"s   },
        { JsonTokenType::EndArray    , L"EndArray"s     },
      };
      for (auto& [input, expected] : pairs)
      {
        Assert::AreEqual(expected, Dump(input));
      }
    }
  };
}