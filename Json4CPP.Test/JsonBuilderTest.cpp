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
      auto inputs = vector<tuple<VALUE, VALUE, JsonBuilderType>>
      {
        { nullptr     , nullptr , JsonBuilderType::Null    },
        { wstring   (), L"Test"s, JsonBuilderType::String  },
        { true        , true    , JsonBuilderType::Boolean },
        { 13.37       , 13.37   , JsonBuilderType::Real    },
        { 1337i64     , 1337i64 , JsonBuilderType::Integer },
        { JsonObject(), JsonObject{ { L"Key1"s, L"Value1"s }, { L"Key2"s, L"Value2"s } }, JsonBuilderType::Object },
        { JsonArray (), JsonArray{ 1, 3, 3, 7 }, JsonBuilderType::Array }
      };
      for (auto& [moved, input, expected] : inputs)
      {
        auto copy = input;
        auto output = JsonBuilder(input);
        Assert::IsTrue(output.Is(expected));
        Assert::AreEqual<Json>(Json(copy), output);
        Assert::AreEqual<VALUE>(copy, input);
        output = JsonBuilder(move(input));
        Assert::IsTrue(output.Is(expected));
        Assert::AreEqual<Json>(Json(copy), output);
        Assert::AreEqual<VALUE>(moved, input);
      }
    }

    TEST_METHOD(TestConstructorNullptr)
    {
      auto null = nullptr;
      Assert::IsTrue(JsonBuilder(     null ).Is(JsonBuilderType::Null));
      Assert::IsTrue(JsonBuilder(move(null)).Is(JsonBuilderType::Null));
    }

    TEST_METHOD(TestConstructorConstWcharPointer)
    {
      Assert::IsTrue(JsonBuilder(L"").Is(JsonBuilderType::String));
    }

    TEST_METHOD(TestConstructorWstring)
    {
      auto str = L"Test"s;
      auto output = JsonBuilder(str);
      Assert::IsTrue(output.Is(JsonBuilderType::String));
      Assert::AreEqual<Json>(Json(L"Test"s), output);
      Assert::AreEqual(L"Test"s, str);
      output = JsonBuilder(move(str));
      Assert::IsTrue(output.Is(JsonBuilderType::String));
      Assert::AreEqual<Json>(Json(L"Test"s), output);
      Assert::AreEqual(L""s, str);
    }

    TEST_METHOD(TestConstructorBool)
    {
      auto boolean = true;
      Assert::IsTrue(JsonBuilder(     boolean ).Is(JsonBuilderType::Boolean));
      Assert::IsTrue(boolean);
      Assert::IsTrue(JsonBuilder(move(boolean)).Is(JsonBuilderType::Boolean));
      Assert::IsTrue(boolean);
    }

    TEST_METHOD(TestConstructorChar)
    {
      auto c = 'A';
      Assert::IsTrue(JsonBuilder(     c ).Is(JsonBuilderType::Integer));
      Assert::AreEqual('A', c);
      Assert::IsTrue(JsonBuilder(move(c)).Is(JsonBuilderType::Integer));
      Assert::AreEqual('A', c);
    }

    TEST_METHOD(TestConstructorInt8_t)
    {
      int8_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int8_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int8_t)1, integer);
    }

    TEST_METHOD(TestConstructorUInt8_t)
    {
      uint8_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint8_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint8_t)1, integer);
    }

    TEST_METHOD(TestConstructorInt16_t)
    {
      int16_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int16_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int16_t)1, integer);
    }

    TEST_METHOD(TestConstructorUInt16_t)
    {
      uint16_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint16_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint16_t)1, integer);
    }

    TEST_METHOD(TestConstructorInt32_t)
    {
      int32_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int32_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int32_t)1, integer);
    }

    TEST_METHOD(TestConstructorUInt32_t)
    {
      uint32_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint32_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint32_t)1, integer);
    }

    TEST_METHOD(TestConstructorInt64_t)
    {
      int64_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int64_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((int64_t)1, integer);
    }

    TEST_METHOD(TestConstructorUInt64_t)
    {
      uint64_t integer = 1;
      Assert::IsTrue(JsonBuilder(     integer ).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint64_t)1, integer);
      Assert::IsTrue(JsonBuilder(move(integer)).Is(JsonBuilderType::Integer));
      Assert::AreEqual((uint64_t)1, integer);
    }

    TEST_METHOD(TestConstructorFloat)
    {
      float real = 13.37f;
      Assert::IsTrue(JsonBuilder(     real ).Is(JsonBuilderType::Real));
      Assert::AreEqual((float)13.37, real);
      Assert::IsTrue(JsonBuilder(move(real)).Is(JsonBuilderType::Real));
      Assert::AreEqual((float)13.37, real);
    }

    TEST_METHOD(TestConstructorDouble)
    {
      double real = 13.37;
      Assert::IsTrue(JsonBuilder(     real ).Is(JsonBuilderType::Real));
      Assert::AreEqual((double)13.37, real);
      Assert::IsTrue(JsonBuilder(move(real)).Is(JsonBuilderType::Real));
      Assert::AreEqual((double)13.37, real);
    }

    TEST_METHOD(TestConstructorJsonObject)
    {
      auto object = JsonObject{
        { L"Key1", L"Value1" },
        { L"Key2", L"Value2" }
      };
      auto output = JsonBuilder(object);
      Assert::IsTrue(output.Is(JsonBuilderType::Object));
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, output);
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, object);
      output = JsonBuilder(move(object));
      Assert::IsTrue(output.Is(JsonBuilderType::Object));
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1", L"Value1" }, { L"Key2", L"Value2" } }, output);
      Assert::AreEqual<JsonObject>(JsonObject{ }, object);
    }

    TEST_METHOD(TestConstructorJsonArray)
    {
      auto array = JsonArray{ 1, 3, 3, 7 };
      auto output = JsonBuilder(array);
      Assert::IsTrue(output.Is(JsonBuilderType::Array));
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 3, 3, 7 }, output);
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 3, 3, 7 }, array);
      output = JsonBuilder(move(array));
      Assert::IsTrue(output.Is(JsonBuilderType::Array));
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 3, 3, 7 }, output);
      Assert::AreEqual<JsonArray>(JsonArray{ }, array);
    }

    TEST_METHOD(TestConstructorJson)
    {
      auto inputs = vector<tuple<Json, Json, JsonBuilderType>>
      {
        { Json(nullptr     ), Json(nullptr ), JsonBuilderType::Null    },
        { Json(wstring   ()), Json(L"Test"s), JsonBuilderType::String  },
        { Json(true        ), Json(true    ), JsonBuilderType::Boolean },
        { Json(13.37       ), Json(13.37   ), JsonBuilderType::Real    },
        { Json(1337i64     ), Json(1337i64 ), JsonBuilderType::Integer },
        { Json(JsonObject()), Json(JsonObject{ { L"Key1"s, L"Value1"s }, { L"Key2"s, L"Value2"s } }), JsonBuilderType::Object },
        { Json(JsonArray ()), Json(JsonArray{ 1, 3, 3, 7 }), JsonBuilderType::Array }
      };
      for (auto& [moved, input, expected] : inputs)
      {
        auto copy = input;
        auto output = JsonBuilder(input);
        Assert::IsTrue(output.Is(expected));
        Assert::AreEqual<Json>(copy, output);
        Assert::AreEqual<Json>(copy, input);
        output = JsonBuilder(move(input));
        Assert::IsTrue(output.Is(expected));
        Assert::AreEqual<Json>(copy, output);
        Assert::AreEqual<Json>(moved, input);
      }
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      Assert::IsTrue(JsonBuilder({ { nullptr_t()                      } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { wstring()                        } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { bool()                           } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { double()                         } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { int64_t()                        } }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ { L"Key1"s, 1.0 }, { L"Key2"s, 2.0 } }).Is(JsonBuilderType::Object));
      Assert::IsTrue(JsonBuilder({ 1, 2                                 }).Is(JsonBuilderType::Array ));
      Assert::IsTrue(JsonBuilder({ wstring(), 2                         }).Is(JsonBuilderType::Pair  ));
      Assert::IsTrue(JsonBuilder({                                      }).Is(JsonBuilderType::Empty ));
    }

    TEST_METHOD(TestConstructorVector)
    {
      auto pairs = vector<pair<vector<JsonBuilder>, JsonBuilderType>>
      {
        { vector<JsonBuilder>{ nullptr_t()                          }, JsonBuilderType::Array  },
        { vector<JsonBuilder>{ wstring()                            }, JsonBuilderType::Array  },
        { vector<JsonBuilder>{ bool()                               }, JsonBuilderType::Array  },
        { vector<JsonBuilder>{ double()                             }, JsonBuilderType::Array  },
        { vector<JsonBuilder>{ int64_t()                            }, JsonBuilderType::Array  },
        { vector<JsonBuilder>{ { L"Key1"s, 1.0 }, { L"Key2"s, 2.0 } }, JsonBuilderType::Object },
        { vector<JsonBuilder>{ 1, 2                                 }, JsonBuilderType::Array  },
        { vector<JsonBuilder>{ wstring(), 2                         }, JsonBuilderType::Pair   },
        { vector<JsonBuilder>{                                      }, JsonBuilderType::Empty  }
      };

      for (auto& [input, expected] : pairs)
      {
        auto copy = input;
        auto output = JsonBuilder(input);
        Assert::IsTrue(output.Is(expected));
        Assert::AreEqual<Json>(Json(copy), output);
        Assert::AreEqual(Json(copy), Json(input));
        output = JsonBuilder(move(input));
        Assert::IsTrue(output.Is(expected));
        Assert::AreEqual<Json>(Json(copy), output);
        Assert::AreEqual(Json(vector<JsonBuilder>()), Json(input));
      }
    }
  };
}