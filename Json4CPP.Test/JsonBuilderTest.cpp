#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;
using namespace Json4CPP::Detail;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonBuilderTest)
  {
  public:
    TEST_METHOD(TestConstructorDefault)
    {
      Assert::IsTrue(JsonBuilder().Is(JsonBuilderType::Null));
    }

    TEST_METHOD(TestConstructorVALUE)
    {
      auto inputs = vector<pair<VALUE, JsonBuilderType>>
      {
        { nullptr_t (), JsonBuilderType::Null    },
        { wstring   (), JsonBuilderType::String  },
        { bool      (), JsonBuilderType::Boolean },
        { double    (), JsonBuilderType::Number  },
        { JsonObject(), JsonBuilderType::Object  },
        { JsonArray (), JsonBuilderType::Array   },
      };
      for (auto& [input, expected] : inputs)
      {
        Assert::IsTrue(JsonBuilder(input).Is(expected));
      }
    }

    TEST_METHOD(TestConstructorNullptr)
    {
      Assert::IsTrue(JsonBuilder(nullptr_t()).Is(JsonBuilderType::Null));
    }

    TEST_METHOD(TestConstructorConstWcharPointer)
    {
      Assert::IsTrue(JsonBuilder(L"").Is(JsonBuilderType::String));
    }

    TEST_METHOD(TestConstructorWstring)
    {
      Assert::IsTrue(JsonBuilder(wstring()).Is(JsonBuilderType::String));
    }

    TEST_METHOD(TestConstructorBool)
    {
      Assert::IsTrue(JsonBuilder(bool()).Is(JsonBuilderType::Boolean));
    }

    TEST_METHOD(TestConstructorChar)
    {
      Assert::IsTrue(JsonBuilder(char()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorInt8_t)
    {
      Assert::IsTrue(JsonBuilder(int8_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorUInt8_t)
    {
      Assert::IsTrue(JsonBuilder(uint8_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorInt16_t)
    {
      Assert::IsTrue(JsonBuilder(int16_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorUInt16_t)
    {
      Assert::IsTrue(JsonBuilder(uint16_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorInt32_t)
    {
      Assert::IsTrue(JsonBuilder(int32_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorUInt32_t)
    {
      Assert::IsTrue(JsonBuilder(uint32_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorInt64_t)
    {
      Assert::IsTrue(JsonBuilder(int64_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorUInt64_t)
    {
      Assert::IsTrue(JsonBuilder(uint64_t()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorFloat)
    {
      Assert::IsTrue(JsonBuilder(float()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorDouble)
    {
      Assert::IsTrue(JsonBuilder(double()).Is(JsonBuilderType::Number));
    }

    TEST_METHOD(TestConstructorJsonObject)
    {
      Assert::IsTrue(JsonBuilder(JsonObject()).Is(JsonBuilderType::Object));
    }

    TEST_METHOD(TestConstructorJsonArray)
    {
      Assert::IsTrue(JsonBuilder(JsonArray()).Is(JsonBuilderType::Array));
    }

    TEST_METHOD(TestConstructorJson)
    {
      auto inputs = vector<pair<VALUE, JsonBuilderType>>
      {
        { nullptr_t (), JsonBuilderType::Null    },
        { wstring   (), JsonBuilderType::String  },
        { bool      (), JsonBuilderType::Boolean },
        { double    (), JsonBuilderType::Number  },
        { JsonObject(), JsonBuilderType::Object  },
        { JsonArray (), JsonBuilderType::Array   },
      };
      for (auto& [input, expected] : inputs)
      {
        Assert::IsTrue(JsonBuilder(Json(input)).Is(expected));
      }
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      Assert::IsTrue(JsonBuilder({ { nullptr_t()                      } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { wstring()                        } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { bool()                           } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { double()                         } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { L"Key1"s, 1.0 }, { L"Key2"s, 2.0 } }).Is(JsonBuilderType::Object));
      Assert::IsTrue(JsonBuilder({ 1, 2                                 }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ wstring(), 2                         }).Is(JsonBuilderType::Pair  ));
      Assert::IsTrue(JsonBuilder({                                      }).Is(JsonBuilderType::Empty ));
    }

    TEST_METHOD(TestConstructorVector)
    {
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{ nullptr_t()                          }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{ wstring()                            }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{ bool()                               }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{ double()                             }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{ { L"Key1"s, 1.0 }, { L"Key2"s, 2.0 } }).Is(JsonBuilderType::Object));
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{ 1, 2                                 }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{ wstring(), 2                         }).Is(JsonBuilderType::Pair  ));
      Assert::IsTrue(JsonBuilder(vector<JsonBuilder>{                                      }).Is(JsonBuilderType::Empty ));
    }
  };
}