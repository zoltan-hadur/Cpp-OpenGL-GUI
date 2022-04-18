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

    TEST_METHOD(TestType)
    {
      auto tuples = vector<tuple<JsonBuilder, JsonBuilderType, JsonBuilderType>>
      {
        { nullptr_t (), JsonBuilderType::Null    , JsonBuilderType::Simple  },
        { wstring   (), JsonBuilderType::String  , JsonBuilderType::Simple  },
        { bool      (), JsonBuilderType::Boolean , JsonBuilderType::Simple  },
        { double    (), JsonBuilderType::Real    , JsonBuilderType::Number  },
        { int64_t   (), JsonBuilderType::Integer , JsonBuilderType::Number  },
        { double    (), JsonBuilderType::Real    , JsonBuilderType::Simple  },
        { int64_t   (), JsonBuilderType::Integer , JsonBuilderType::Simple  },
        { JsonObject(), JsonBuilderType::Object  , JsonBuilderType::Complex },
        { JsonArray (), JsonBuilderType::Array   , JsonBuilderType::Complex },
        { vector<JsonBuilder>()                                                                     , JsonBuilderType::Empty  , JsonBuilderType::Complex },
        { vector<JsonBuilder>{ JsonBuilder(L"Key"), JsonBuilder(L"Value") }                         , JsonBuilderType::Pair   , JsonBuilderType::Complex },
        { vector<JsonBuilder>{ vector<JsonBuilder>{ JsonBuilder(L"Key0"), JsonBuilder(L"Value0") },
                               vector<JsonBuilder>{ JsonBuilder(L"Key1"), JsonBuilder(L"Value1") } }, JsonBuilderType::Object , JsonBuilderType::Complex },
        { vector<JsonBuilder>{ JsonBuilder(1), JsonBuilder(2), JsonBuilder(3) }                     , JsonBuilderType::Array  , JsonBuilderType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::AreEqual(expected1, input.Type());
        Assert::AreEqual(expected2, input.Type());
      }
    }

    TEST_METHOD(TestIs)
    {
      auto tuples = vector<tuple<JsonBuilder, JsonBuilderType, JsonBuilderType>>
      {
        { nullptr_t (), JsonBuilderType::Null    , JsonBuilderType::Simple  },
        { wstring   (), JsonBuilderType::String  , JsonBuilderType::Simple  },
        { bool      (), JsonBuilderType::Boolean , JsonBuilderType::Simple  },
        { double    (), JsonBuilderType::Real    , JsonBuilderType::Number  },
        { int64_t   (), JsonBuilderType::Integer , JsonBuilderType::Number  },
        { double    (), JsonBuilderType::Real    , JsonBuilderType::Simple  },
        { int64_t   (), JsonBuilderType::Integer , JsonBuilderType::Simple  },
        { JsonObject(), JsonBuilderType::Object  , JsonBuilderType::Complex },
        { JsonArray (), JsonBuilderType::Array   , JsonBuilderType::Complex },
        { vector<JsonBuilder>()                                                                     , JsonBuilderType::Empty  , JsonBuilderType::Complex },
        { vector<JsonBuilder>{ JsonBuilder(L"Key"), JsonBuilder(L"Value") }                         , JsonBuilderType::Pair   , JsonBuilderType::Complex },
        { vector<JsonBuilder>{ vector<JsonBuilder>{ JsonBuilder(L"Key0"), JsonBuilder(L"Value0") },
                               vector<JsonBuilder>{ JsonBuilder(L"Key1"), JsonBuilder(L"Value1") } }, JsonBuilderType::Object , JsonBuilderType::Complex },
        { vector<JsonBuilder>{ JsonBuilder(1), JsonBuilder(2), JsonBuilder(3) }                     , JsonBuilderType::Array  , JsonBuilderType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::IsTrue(input.Is(expected1));
        Assert::IsTrue(input.Is(expected2));
      }
    }

    TEST_METHOD(TestOperatorExtraction)
    {
      auto builder = JsonBuilder{
        nullptr,
        true,
        13.37,
        1337,
        JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } },
        JsonArray{ 1, 3, 3, 7 },
        vector<JsonBuilder>{
          nullptr,
          true,
          13.37,
          1337,
          JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } },
          JsonArray{ 1, 3, 3, 7 }
        }
      };
      wostringstream os;
      os << builder;
      auto result = os.str();
      Assert::AreEqual(L"[null,true,13.37,1337,{\"Key1\":1,\"Key2\":2},[1,3,3,7],[null,true,13.37,1337,{\"Key1\":1,\"Key2\":2},[1,3,3,7]]]"s, result);
    }

    TEST_METHOD(TestOperatorEqual)
    {
      JsonBuilder null = nullptr;
      JsonBuilder string1 = L"TestString0"s;
      JsonBuilder string2 = L"TestString1"s;
      JsonBuilder boolean1 = false;
      JsonBuilder boolean2 = true;
      JsonBuilder number1 = 0.0;
      JsonBuilder number2 = 1.0;
      JsonBuilder numberi1 = 0i64;
      JsonBuilder numberi2 = 1i64;
      JsonBuilder object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      JsonBuilder object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      JsonBuilder array1 = JsonArray{ 1, 2, 3 };
      JsonBuilder array2 = JsonArray{ 4, 5, 6 };
      JsonBuilder vec1 = vector<JsonBuilder>();
      JsonBuilder vec2 = vector<JsonBuilder>{ JsonBuilder(1), JsonBuilder(2), JsonBuilder(3) };

      Assert::IsTrue (array1   == array1  );
      Assert::IsFalse(array1   == array2  );
      Assert::IsFalse(array1   == boolean1);
      Assert::IsFalse(array1   == boolean2);
      Assert::IsFalse(array1   == null    );
      Assert::IsFalse(array1   == number1 );
      Assert::IsFalse(array1   == number2 );
      Assert::IsFalse(array1   == numberi1);
      Assert::IsFalse(array1   == numberi2);
      Assert::IsFalse(array1   == object1 );
      Assert::IsFalse(array1   == object2 );
      Assert::IsFalse(array1   == string1 );
      Assert::IsFalse(array1   == string2 );
      Assert::IsFalse(array1   == vec1    );
      Assert::IsFalse(array1   == vec2    );
      Assert::IsFalse(array2   == array1  );
      Assert::IsTrue (array2   == array2  );
      Assert::IsFalse(array2   == boolean1);
      Assert::IsFalse(array2   == boolean2);
      Assert::IsFalse(array2   == null    );
      Assert::IsFalse(array2   == number1 );
      Assert::IsFalse(array2   == number2 );
      Assert::IsFalse(array2   == numberi1);
      Assert::IsFalse(array2   == numberi2);
      Assert::IsFalse(array2   == object1 );
      Assert::IsFalse(array2   == object2 );
      Assert::IsFalse(array2   == string1 );
      Assert::IsFalse(array2   == string2 );
      Assert::IsFalse(array2   == vec1    );
      Assert::IsFalse(array2   == vec2    );
      Assert::IsFalse(boolean1 == array1  );
      Assert::IsFalse(boolean1 == array2  );
      Assert::IsTrue (boolean1 == boolean1);
      Assert::IsFalse(boolean1 == boolean2);
      Assert::IsFalse(boolean1 == null    );
      Assert::IsFalse(boolean1 == number1 );
      Assert::IsFalse(boolean1 == number2 );
      Assert::IsFalse(boolean1 == numberi1);
      Assert::IsFalse(boolean1 == numberi2);
      Assert::IsFalse(boolean1 == object1 );
      Assert::IsFalse(boolean1 == object2 );
      Assert::IsFalse(boolean1 == string1 );
      Assert::IsFalse(boolean1 == string2 );
      Assert::IsFalse(boolean1 == vec1    );
      Assert::IsFalse(boolean1 == vec2    );
      Assert::IsFalse(boolean2 == array1  );
      Assert::IsFalse(boolean2 == array2  );
      Assert::IsFalse(boolean2 == boolean1);
      Assert::IsTrue (boolean2 == boolean2);
      Assert::IsFalse(boolean2 == null    );
      Assert::IsFalse(boolean2 == number1 );
      Assert::IsFalse(boolean2 == number2 );
      Assert::IsFalse(boolean2 == numberi1);
      Assert::IsFalse(boolean2 == numberi2);
      Assert::IsFalse(boolean2 == object1 );
      Assert::IsFalse(boolean2 == object2 );
      Assert::IsFalse(boolean2 == string1 );
      Assert::IsFalse(boolean2 == string2 );
      Assert::IsFalse(boolean2 == vec1    );
      Assert::IsFalse(boolean2 == vec2    );
      Assert::IsFalse(null     == array1  );
      Assert::IsFalse(null     == array2  );
      Assert::IsFalse(null     == boolean1);
      Assert::IsFalse(null     == boolean2);
      Assert::IsTrue (null     == null    );
      Assert::IsFalse(null     == number1 );
      Assert::IsFalse(null     == number2 );
      Assert::IsFalse(null     == numberi1);
      Assert::IsFalse(null     == numberi2);
      Assert::IsFalse(null     == object1 );
      Assert::IsFalse(null     == object2 );
      Assert::IsFalse(null     == string1 );
      Assert::IsFalse(null     == string2 );
      Assert::IsFalse(null     == vec1    );
      Assert::IsFalse(null     == vec2    );
      Assert::IsFalse(number1  == array1  );
      Assert::IsFalse(number1  == array2  );
      Assert::IsFalse(number1  == boolean1);
      Assert::IsFalse(number1  == boolean2);
      Assert::IsFalse(number1  == null    );
      Assert::IsTrue (number1  == number1 );
      Assert::IsFalse(number1  == number2 );
      Assert::IsFalse(number1  == numberi1);
      Assert::IsFalse(number1  == numberi2);
      Assert::IsFalse(number1  == object1 );
      Assert::IsFalse(number1  == object2 );
      Assert::IsFalse(number1  == string1 );
      Assert::IsFalse(number1  == string2 );
      Assert::IsFalse(number1  == vec1    );
      Assert::IsFalse(number1  == vec2    );
      Assert::IsFalse(number2  == array1  );
      Assert::IsFalse(number2  == array2  );
      Assert::IsFalse(number2  == boolean1);
      Assert::IsFalse(number2  == boolean2);
      Assert::IsFalse(number2  == null    );
      Assert::IsFalse(number2  == number1 );
      Assert::IsTrue (number2  == number2 );
      Assert::IsFalse(number2  == numberi1);
      Assert::IsFalse(number2  == numberi2);
      Assert::IsFalse(number2  == object1 );
      Assert::IsFalse(number2  == object2 );
      Assert::IsFalse(number2  == string1 );
      Assert::IsFalse(number2  == string2 );
      Assert::IsFalse(number2  == vec1    );
      Assert::IsFalse(number2  == vec2    );
      Assert::IsFalse(numberi1 == array1  );
      Assert::IsFalse(numberi1 == array2  );
      Assert::IsFalse(numberi1 == boolean1);
      Assert::IsFalse(numberi1 == boolean2);
      Assert::IsFalse(numberi1 == null    );
      Assert::IsFalse(numberi1 == number1 );
      Assert::IsFalse(numberi1 == number2 );
      Assert::IsTrue (numberi1 == numberi1);
      Assert::IsFalse(numberi1 == numberi2);
      Assert::IsFalse(numberi1 == object1 );
      Assert::IsFalse(numberi1 == object2 );
      Assert::IsFalse(numberi1 == string1 );
      Assert::IsFalse(numberi1 == string2 );
      Assert::IsFalse(numberi1 == vec1    );
      Assert::IsFalse(numberi1 == vec2    );
      Assert::IsFalse(numberi2 == array1  );
      Assert::IsFalse(numberi2 == array2  );
      Assert::IsFalse(numberi2 == boolean1);
      Assert::IsFalse(numberi2 == boolean2);
      Assert::IsFalse(numberi2 == null    );
      Assert::IsFalse(numberi2 == number1 );
      Assert::IsFalse(numberi2 == number2 );
      Assert::IsFalse(numberi2 == numberi1);
      Assert::IsTrue (numberi2 == numberi2);
      Assert::IsFalse(numberi2 == object1 );
      Assert::IsFalse(numberi2 == object2 );
      Assert::IsFalse(numberi2 == string1 );
      Assert::IsFalse(numberi2 == string2 );
      Assert::IsFalse(numberi2 == vec1    );
      Assert::IsFalse(numberi2 == vec2    );
      Assert::IsFalse(object1  == array1  );
      Assert::IsFalse(object1  == array2  );
      Assert::IsFalse(object1  == boolean1);
      Assert::IsFalse(object1  == boolean2);
      Assert::IsFalse(object1  == null    );
      Assert::IsFalse(object1  == number1 );
      Assert::IsFalse(object1  == number2 );
      Assert::IsFalse(object1  == numberi1);
      Assert::IsFalse(object1  == numberi2);
      Assert::IsTrue (object1  == object1 );
      Assert::IsFalse(object1  == object2 );
      Assert::IsFalse(object1  == string1 );
      Assert::IsFalse(object1  == string2 );
      Assert::IsFalse(object1  == vec1    );
      Assert::IsFalse(object1  == vec2    );
      Assert::IsFalse(object2  == array1  );
      Assert::IsFalse(object2  == array2  );
      Assert::IsFalse(object2  == boolean1);
      Assert::IsFalse(object2  == boolean2);
      Assert::IsFalse(object2  == null    );
      Assert::IsFalse(object2  == number1 );
      Assert::IsFalse(object2  == number2 );
      Assert::IsFalse(object2  == numberi1);
      Assert::IsFalse(object2  == numberi2);
      Assert::IsFalse(object2  == object1 );
      Assert::IsTrue (object2  == object2 );
      Assert::IsFalse(object2  == string1 );
      Assert::IsFalse(object2  == string2 );
      Assert::IsFalse(object2  == vec1    );
      Assert::IsFalse(object2  == vec2    );
      Assert::IsFalse(string1  == array1  );
      Assert::IsFalse(string1  == array2  );
      Assert::IsFalse(string1  == boolean1);
      Assert::IsFalse(string1  == boolean2);
      Assert::IsFalse(string1  == null    );
      Assert::IsFalse(string1  == number1 );
      Assert::IsFalse(string1  == number2 );
      Assert::IsFalse(string1  == numberi1);
      Assert::IsFalse(string1  == numberi2);
      Assert::IsFalse(string1  == object1 );
      Assert::IsFalse(string1  == object2 );
      Assert::IsTrue (string1  == string1 );
      Assert::IsFalse(string1  == string2 );
      Assert::IsFalse(string1  == vec1    );
      Assert::IsFalse(string1  == vec2    );
      Assert::IsFalse(string2  == array1  );
      Assert::IsFalse(string2  == array2  );
      Assert::IsFalse(string2  == boolean1);
      Assert::IsFalse(string2  == boolean2);
      Assert::IsFalse(string2  == null    );
      Assert::IsFalse(string2  == number1 );
      Assert::IsFalse(string2  == number2 );
      Assert::IsFalse(string2  == numberi1);
      Assert::IsFalse(string2  == numberi2);
      Assert::IsFalse(string2  == object1 );
      Assert::IsFalse(string2  == object2 );
      Assert::IsFalse(string2  == string1 );
      Assert::IsTrue (string2  == string2 );
      Assert::IsFalse(string2  == vec1    );
      Assert::IsFalse(string2  == vec2    );
      Assert::IsFalse(vec1     == array1  );
      Assert::IsFalse(vec1     == array2  );
      Assert::IsFalse(vec1     == boolean1);
      Assert::IsFalse(vec1     == boolean2);
      Assert::IsFalse(vec1     == null    );
      Assert::IsFalse(vec1     == number1 );
      Assert::IsFalse(vec1     == number2 );
      Assert::IsFalse(vec1     == numberi1);
      Assert::IsFalse(vec1     == numberi2);
      Assert::IsFalse(vec1     == object1 );
      Assert::IsFalse(vec1     == object2 );
      Assert::IsFalse(vec1     == string1 );
      Assert::IsFalse(vec1     == string2 );
      Assert::IsTrue (vec1     == vec1    );
      Assert::IsFalse(vec1     == vec2    );
      Assert::IsFalse(vec2     == array1  );
      Assert::IsFalse(vec2     == array2  );
      Assert::IsFalse(vec2     == boolean1);
      Assert::IsFalse(vec2     == boolean2);
      Assert::IsFalse(vec2     == null    );
      Assert::IsFalse(vec2     == number1 );
      Assert::IsFalse(vec2     == number2 );
      Assert::IsFalse(vec2     == numberi1);
      Assert::IsFalse(vec2     == numberi2);
      Assert::IsFalse(vec2     == object1 );
      Assert::IsFalse(vec2     == object2 );
      Assert::IsFalse(vec2     == string1 );
      Assert::IsFalse(vec2     == string2 );
      Assert::IsFalse(vec2     == vec1    );
      Assert::IsTrue (vec2     == vec2    );
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      JsonBuilder null = nullptr;
      JsonBuilder string1 = L"TestString0"s;
      JsonBuilder string2 = L"TestString1"s;
      JsonBuilder boolean1 = false;
      JsonBuilder boolean2 = true;
      JsonBuilder number1 = 0.0;
      JsonBuilder number2 = 1.0;
      JsonBuilder numberi1 = 0i64;
      JsonBuilder numberi2 = 1i64;
      JsonBuilder object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      JsonBuilder object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      JsonBuilder array1 = JsonArray{ 1, 2, 3 };
      JsonBuilder array2 = JsonArray{ 4, 5, 6 };
      JsonBuilder vec1 = vector<JsonBuilder>();
      JsonBuilder vec2 = vector<JsonBuilder>{ JsonBuilder(1), JsonBuilder(2), JsonBuilder(3) };

      Assert::IsFalse(array1   != array1  );
      Assert::IsTrue (array1   != array2  );
      Assert::IsTrue (array1   != boolean1);
      Assert::IsTrue (array1   != boolean2);
      Assert::IsTrue (array1   != null    );
      Assert::IsTrue (array1   != number1 );
      Assert::IsTrue (array1   != number2 );
      Assert::IsTrue (array1   != numberi1);
      Assert::IsTrue (array1   != numberi2);
      Assert::IsTrue (array1   != object1 );
      Assert::IsTrue (array1   != object2 );
      Assert::IsTrue (array1   != string1 );
      Assert::IsTrue (array1   != string2 );
      Assert::IsTrue (array1   != vec1    );
      Assert::IsTrue (array1   != vec2    );
      Assert::IsTrue (array2   != array1  );
      Assert::IsFalse(array2   != array2  );
      Assert::IsTrue (array2   != boolean1);
      Assert::IsTrue (array2   != boolean2);
      Assert::IsTrue (array2   != null    );
      Assert::IsTrue (array2   != number1 );
      Assert::IsTrue (array2   != number2 );
      Assert::IsTrue (array2   != numberi1);
      Assert::IsTrue (array2   != numberi2);
      Assert::IsTrue (array2   != object1 );
      Assert::IsTrue (array2   != object2 );
      Assert::IsTrue (array2   != string1 );
      Assert::IsTrue (array2   != string2 );
      Assert::IsTrue (array2   != vec1    );
      Assert::IsTrue (array2   != vec2    );
      Assert::IsTrue (boolean1 != array1  );
      Assert::IsTrue (boolean1 != array2  );
      Assert::IsFalse(boolean1 != boolean1);
      Assert::IsTrue (boolean1 != boolean2);
      Assert::IsTrue (boolean1 != null    );
      Assert::IsTrue (boolean1 != number1 );
      Assert::IsTrue (boolean1 != number2 );
      Assert::IsTrue (boolean1 != numberi1);
      Assert::IsTrue (boolean1 != numberi2);
      Assert::IsTrue (boolean1 != object1 );
      Assert::IsTrue (boolean1 != object2 );
      Assert::IsTrue (boolean1 != string1 );
      Assert::IsTrue (boolean1 != string2 );
      Assert::IsTrue (boolean1 != vec1    );
      Assert::IsTrue (boolean1 != vec2    );
      Assert::IsTrue (boolean2 != array1  );
      Assert::IsTrue (boolean2 != array2  );
      Assert::IsTrue (boolean2 != boolean1);
      Assert::IsFalse(boolean2 != boolean2);
      Assert::IsTrue (boolean2 != null    );
      Assert::IsTrue (boolean2 != number1 );
      Assert::IsTrue (boolean2 != number2 );
      Assert::IsTrue (boolean2 != numberi1);
      Assert::IsTrue (boolean2 != numberi2);
      Assert::IsTrue (boolean2 != object1 );
      Assert::IsTrue (boolean2 != object2 );
      Assert::IsTrue (boolean2 != string1 );
      Assert::IsTrue (boolean2 != string2 );
      Assert::IsTrue (boolean2 != vec1    );
      Assert::IsTrue (boolean2 != vec2    );
      Assert::IsTrue (null     != array1  );
      Assert::IsTrue (null     != array2  );
      Assert::IsTrue (null     != boolean1);
      Assert::IsTrue (null     != boolean2);
      Assert::IsFalse(null     != null    );
      Assert::IsTrue (null     != number1 );
      Assert::IsTrue (null     != number2 );
      Assert::IsTrue (null     != numberi1);
      Assert::IsTrue (null     != numberi2);
      Assert::IsTrue (null     != object1 );
      Assert::IsTrue (null     != object2 );
      Assert::IsTrue (null     != string1 );
      Assert::IsTrue (null     != string2 );
      Assert::IsTrue (null     != vec1    );
      Assert::IsTrue (null     != vec2    );
      Assert::IsTrue (number1  != array1  );
      Assert::IsTrue (number1  != array2  );
      Assert::IsTrue (number1  != boolean1);
      Assert::IsTrue (number1  != boolean2);
      Assert::IsTrue (number1  != null    );
      Assert::IsFalse(number1  != number1 );
      Assert::IsTrue (number1  != number2 );
      Assert::IsTrue (number1  != numberi1);
      Assert::IsTrue (number1  != numberi2);
      Assert::IsTrue (number1  != object1 );
      Assert::IsTrue (number1  != object2 );
      Assert::IsTrue (number1  != string1 );
      Assert::IsTrue (number1  != string2 );
      Assert::IsTrue (number1  != vec1    );
      Assert::IsTrue (number1  != vec2    );
      Assert::IsTrue (number2  != array1  );
      Assert::IsTrue (number2  != array2  );
      Assert::IsTrue (number2  != boolean1);
      Assert::IsTrue (number2  != boolean2);
      Assert::IsTrue (number2  != null    );
      Assert::IsTrue (number2  != number1 );
      Assert::IsFalse(number2  != number2 );
      Assert::IsTrue (number2  != numberi1);
      Assert::IsTrue (number2  != numberi2);
      Assert::IsTrue (number2  != object1 );
      Assert::IsTrue (number2  != object2 );
      Assert::IsTrue (number2  != string1 );
      Assert::IsTrue (number2  != string2 );
      Assert::IsTrue (number2  != vec1    );
      Assert::IsTrue (number2  != vec2    );
      Assert::IsTrue (numberi1 != array1  );
      Assert::IsTrue (numberi1 != array2  );
      Assert::IsTrue (numberi1 != boolean1);
      Assert::IsTrue (numberi1 != boolean2);
      Assert::IsTrue (numberi1 != null    );
      Assert::IsTrue (numberi1 != number1 );
      Assert::IsTrue (numberi1 != number2 );
      Assert::IsFalse(numberi1 != numberi1);
      Assert::IsTrue (numberi1 != numberi2);
      Assert::IsTrue (numberi1 != object1 );
      Assert::IsTrue (numberi1 != object2 );
      Assert::IsTrue (numberi1 != string1 );
      Assert::IsTrue (numberi1 != string2 );
      Assert::IsTrue (numberi1 != vec1    );
      Assert::IsTrue (numberi1 != vec2    );
      Assert::IsTrue (numberi2 != array1  );
      Assert::IsTrue (numberi2 != array2  );
      Assert::IsTrue (numberi2 != boolean1);
      Assert::IsTrue (numberi2 != boolean2);
      Assert::IsTrue (numberi2 != null    );
      Assert::IsTrue (numberi2 != number1 );
      Assert::IsTrue (numberi2 != number2 );
      Assert::IsTrue (numberi2 != numberi1);
      Assert::IsFalse(numberi2 != numberi2);
      Assert::IsTrue (numberi2 != object1 );
      Assert::IsTrue (numberi2 != object2 );
      Assert::IsTrue (numberi2 != string1 );
      Assert::IsTrue (numberi2 != string2 );
      Assert::IsTrue (numberi2 != vec1    );
      Assert::IsTrue (numberi2 != vec2    );
      Assert::IsTrue (object1  != array1  );
      Assert::IsTrue (object1  != array2  );
      Assert::IsTrue (object1  != boolean1);
      Assert::IsTrue (object1  != boolean2);
      Assert::IsTrue (object1  != null    );
      Assert::IsTrue (object1  != number1 );
      Assert::IsTrue (object1  != number2 );
      Assert::IsTrue (object1  != numberi1);
      Assert::IsTrue (object1  != numberi2);
      Assert::IsFalse(object1  != object1 );
      Assert::IsTrue (object1  != object2 );
      Assert::IsTrue (object1  != string1 );
      Assert::IsTrue (object1  != string2 );
      Assert::IsTrue (object1  != vec1    );
      Assert::IsTrue (object1  != vec2    );
      Assert::IsTrue (object2  != array1  );
      Assert::IsTrue (object2  != array2  );
      Assert::IsTrue (object2  != boolean1);
      Assert::IsTrue (object2  != boolean2);
      Assert::IsTrue (object2  != null    );
      Assert::IsTrue (object2  != number1 );
      Assert::IsTrue (object2  != number2 );
      Assert::IsTrue (object2  != numberi1);
      Assert::IsTrue (object2  != numberi2);
      Assert::IsTrue (object2  != object1 );
      Assert::IsFalse(object2  != object2 );
      Assert::IsTrue (object2  != string1 );
      Assert::IsTrue (object2  != string2 );
      Assert::IsTrue (object2  != vec1    );
      Assert::IsTrue (object2  != vec2    );
      Assert::IsTrue (string1  != array1  );
      Assert::IsTrue (string1  != array2  );
      Assert::IsTrue (string1  != boolean1);
      Assert::IsTrue (string1  != boolean2);
      Assert::IsTrue (string1  != null    );
      Assert::IsTrue (string1  != number1 );
      Assert::IsTrue (string1  != number2 );
      Assert::IsTrue (string1  != numberi1);
      Assert::IsTrue (string1  != numberi2);
      Assert::IsTrue (string1  != object1 );
      Assert::IsTrue (string1  != object2 );
      Assert::IsFalse(string1  != string1 );
      Assert::IsTrue (string1  != string2 );
      Assert::IsTrue (string1  != vec1    );
      Assert::IsTrue (string1  != vec2    );
      Assert::IsTrue (string2  != array1  );
      Assert::IsTrue (string2  != array2  );
      Assert::IsTrue (string2  != boolean1);
      Assert::IsTrue (string2  != boolean2);
      Assert::IsTrue (string2  != null    );
      Assert::IsTrue (string2  != number1 );
      Assert::IsTrue (string2  != number2 );
      Assert::IsTrue (string2  != numberi1);
      Assert::IsTrue (string2  != numberi2);
      Assert::IsTrue (string2  != object1 );
      Assert::IsTrue (string2  != object2 );
      Assert::IsTrue (string2  != string1 );
      Assert::IsFalse(string2  != string2 );
      Assert::IsTrue (string2  != vec1    );
      Assert::IsTrue (string2  != vec2    );
      Assert::IsTrue (vec1     != array1  );
      Assert::IsTrue (vec1     != array2  );
      Assert::IsTrue (vec1     != boolean1);
      Assert::IsTrue (vec1     != boolean2);
      Assert::IsTrue (vec1     != null    );
      Assert::IsTrue (vec1     != number1 );
      Assert::IsTrue (vec1     != number2 );
      Assert::IsTrue (vec1     != numberi1);
      Assert::IsTrue (vec1     != numberi2);
      Assert::IsTrue (vec1     != object1 );
      Assert::IsTrue (vec1     != object2 );
      Assert::IsTrue (vec1     != string1 );
      Assert::IsTrue (vec1     != string2 );
      Assert::IsFalse(vec1     != vec1    );
      Assert::IsTrue (vec1     != vec2    );
      Assert::IsTrue (vec2     != array1  );
      Assert::IsTrue (vec2     != array2  );
      Assert::IsTrue (vec2     != boolean1);
      Assert::IsTrue (vec2     != boolean2);
      Assert::IsTrue (vec2     != null    );
      Assert::IsTrue (vec2     != number1 );
      Assert::IsTrue (vec2     != number2 );
      Assert::IsTrue (vec2     != numberi1);
      Assert::IsTrue (vec2     != numberi2);
      Assert::IsTrue (vec2     != object1 );
      Assert::IsTrue (vec2     != object2 );
      Assert::IsTrue (vec2     != string1 );
      Assert::IsTrue (vec2     != string2 );
      Assert::IsTrue (vec2     != vec1    );
      Assert::IsFalse(vec2     != vec2    );
    }
  };
}