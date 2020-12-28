#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace std::filesystem;
using namespace Json4CPP;
using namespace Json4CPP::Detail;
using namespace OpenGLUI::Foundation;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonTest)
  {
  public:
    TEST_METHOD(TestLiteral)
    {
      Assert::AreEqual<Json>({ nullptr }, L"[null]"_Json);
      Assert::AreEqual<Json>({ L"TestString" }, L"[\"TestString\"]"_Json);
      Assert::AreEqual<Json>({ true }, L"[true]"_Json);
      Assert::AreEqual<Json>({ 13.37 }, L"[13.37]"_Json);
      Assert::AreEqual<Json>({ 1337 }, L"[1337]"_Json);
      Assert::AreEqual<Json>({ { L"Key1", 1 }, { L"Key2", 2 } }, L"{ \"Key1\": 1, \"Key2\": 2 }"_Json);
      Assert::AreEqual<Json>({ 1, 2, 3 }, L"[ 1, 2, 3 ]"_Json);
    }

    TEST_METHOD(TestConstructorDefault)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::AreEqual<Json>(nullptr, json);
    }

    TEST_METHOD(TestConstructorJsonBuilder)
    {
      Json json;

      json = JsonBuilder();
      Assert::AreEqual<VALUE>(nullptr, json._value);

      // JsonBuilder from VALUE
      json = JsonBuilder(VALUE(nullptr_t()));
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(VALUE(L"TestString"s));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(VALUE(true));
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder(VALUE(13.37));
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(VALUE(1337i64));
      Assert::AreEqual<VALUE>(1337i64, json._value);
      json = JsonBuilder(VALUE(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(VALUE(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);

      // JsonBuilder constructors
      json = JsonBuilder(nullptr_t());
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(L"TestString");
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(L"TestString"s);
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(true);
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder((char)1);
      Assert::AreEqual<VALUE>(1i64, json._value);
      json = JsonBuilder((int8_t)2);
      Assert::AreEqual<VALUE>(2i64, json._value);
      json = JsonBuilder((uint8_t)3);
      Assert::AreEqual<VALUE>(3i64, json._value);
      json = JsonBuilder((int16_t)4);
      Assert::AreEqual<VALUE>(4i64, json._value);
      json = JsonBuilder((uint16_t)5);
      Assert::AreEqual<VALUE>(5i64, json._value);
      json = JsonBuilder((int32_t)6);
      Assert::AreEqual<VALUE>(6i64, json._value);
      json = JsonBuilder((uint32_t)7);
      Assert::AreEqual<VALUE>(7i64, json._value);
      json = JsonBuilder((int64_t)8);
      Assert::AreEqual<VALUE>(8i64, json._value);
      json = JsonBuilder((uint64_t)9);
      Assert::AreEqual<VALUE>(9i64, json._value);
      json = JsonBuilder(13.37f);
      Assert::AreEqual<VALUE>((double)13.37f, json._value);
      json = JsonBuilder(13.37);
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(JsonArray({ 1337, L"1337", true }));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      json = JsonBuilder(Json());
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(Json(nullptr_t()));
      Assert::AreEqual<VALUE>(nullptr, json._value);
      json = JsonBuilder(Json(L"TestString"));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(Json(L"TestString"s));
      Assert::AreEqual<VALUE>(L"TestString"s, json._value);
      json = JsonBuilder(Json(true));
      Assert::AreEqual<VALUE>(true, json._value);
      json = JsonBuilder(Json((char)1));
      Assert::AreEqual<VALUE>(1i64, json._value);
      json = JsonBuilder(Json((int8_t)2));
      Assert::AreEqual<VALUE>(2i64, json._value);
      json = JsonBuilder(Json((uint8_t)3));
      Assert::AreEqual<VALUE>(3i64, json._value);
      json = JsonBuilder(Json((int16_t)4));
      Assert::AreEqual<VALUE>(4i64, json._value);
      json = JsonBuilder(Json((uint16_t)5));
      Assert::AreEqual<VALUE>(5i64, json._value);
      json = JsonBuilder(Json((int32_t)6));
      Assert::AreEqual<VALUE>(6i64, json._value);
      json = JsonBuilder(Json((uint32_t)7));
      Assert::AreEqual<VALUE>(7i64, json._value);
      json = JsonBuilder(Json((int64_t)8));
      Assert::AreEqual<VALUE>(8i64, json._value);
      json = JsonBuilder(Json((uint64_t)9));
      Assert::AreEqual<VALUE>(9i64, json._value);
      json = JsonBuilder(Json(13.37f));
      Assert::AreEqual<VALUE>((double)13.37f, json._value);
      json = JsonBuilder(Json(13.37));
      Assert::AreEqual<VALUE>(13.37, json._value);
      json = JsonBuilder(Json(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual<VALUE>(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } }), json._value);
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
      json = JsonBuilder(Json(JsonObject({ { L"Key1", 1 }, { L"Key2", 2 } })));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual(L"Key1"s, json.Keys()[0]);
      Assert::AreEqual(L"Key2"s, json.Keys()[1]);
      Assert::AreEqual<Json>(1, json[L"Key1"]);
      Assert::AreEqual<Json>(2, json[L"Key2"]);
      json = JsonBuilder(Json(JsonArray({ 1337, L"1337", true })));
      Assert::AreEqual<VALUE>(JsonArray({ 1337, L"1337", true }), json._value);
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Assert::AreEqual<Json>(L"1337", json[1]);
      Assert::AreEqual<Json>(true, json[2]);
    }

    TEST_METHOD(TestConstructorInitializerList)
    {
      Json json = {
        nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
        { { L"Key1", nullptr }, { L"Key2", L"Test1" }, { L"Key3", L"Test2"s }, { L"Key4", false }, { L"Key5", true }, { L"Key6", (char)1 }, { L"Key7", 2i8 }, { L"Key8", 3ui8 },
          { L"Key9", 4i16 }, { L"Key10", 5ui16 }, { L"Key11", 6i32 }, { L"Key12", 7ui32 }, { L"Key13", 8i64 }, { L"Key14", 9ui64 }, { L"Key15", 13.37f }, { L"Key16", 313.37 },
          { L"Key17", { { L"InnerKey1", L"Value1" }, { L"InnerKey2", L"Value2" } } },
          { L"Key18", { 1, 3, 3, 7 } } },
        { nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
          { { L"InnerKey1", L"Value1" }, { L"InnerKey2", L"Value2" } },
          { 1, 3, 3, 7 } }
      };

      Assert::AreEqual<Json>(18, json.Size());
      Assert::AreEqual<Json>(18, json[16].Size());
      Assert::AreEqual<Json>(18, json[17].Size());

      auto values = vector<Json>{ nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37 };
      for (int i = 0; i < values.size(); ++i)
      {
        Assert::AreEqual<Json>(values[i], json[i]);
        Assert::AreEqual<Json>(values[i], json[16][L"Key"s + to_wstring(i + 1)]);
        Assert::AreEqual<Json>(values[i], json[17][i]);
      }

      Assert::AreEqual<Json>(2, json[16][L"Key17"].Size());
      Assert::AreEqual<Json>(L"Value1"s, json[16][L"Key17"][L"InnerKey1"]);
      Assert::AreEqual<Json>(L"Value2"s, json[16][L"Key17"][L"InnerKey2"]);
      Assert::AreEqual<Json>(4, json[16][L"Key18"].Size());
      Assert::AreEqual<Json>(1, json[16][L"Key18"][0]);
      Assert::AreEqual<Json>(3, json[16][L"Key18"][1]);
      Assert::AreEqual<Json>(3, json[16][L"Key18"][2]);
      Assert::AreEqual<Json>(7, json[16][L"Key18"][3]);

      Assert::AreEqual<Json>(2, json[17][16].Size());
      Assert::AreEqual<Json>(L"Value1"s, json[17][16][L"InnerKey1"]);
      Assert::AreEqual<Json>(L"Value2"s, json[17][16][L"InnerKey2"]);
      Assert::AreEqual<Json>(4, json[17][17].Size());
      Assert::AreEqual<Json>(1, json[17][17][0]);
      Assert::AreEqual<Json>(3, json[17][17][1]);
      Assert::AreEqual<Json>(3, json[17][17][2]);
      Assert::AreEqual<Json>(7, json[17][17][3]);
    }

    TEST_METHOD(TestConstructorJson)
    {
      auto json = Json(nullptr);
      Assert::AreEqual<Json>(nullptr, Json(json));
      json = Json(L"Test");
      Assert::AreEqual<Json>(L"Test", Json(json));
      json = Json(L"Test"s);
      Assert::AreEqual<Json>(L"Test"s, Json(json));
      json = Json(false);
      Assert::AreEqual<Json>(false, Json(json));
      json = Json(true);
      Assert::AreEqual<Json>(true, Json(json));
      json = Json((char)1);
      Assert::AreEqual<Json>((char)1, Json(json));
      json = Json(2i8);
      Assert::AreEqual<Json>(2i8, Json(json));
      json = Json(3ui8);
      Assert::AreEqual<Json>(3ui8, Json(json));
      json = Json(4i16);
      Assert::AreEqual<Json>(4i16, Json(json));
      json = Json(5ui16);
      Assert::AreEqual<Json>(5ui16, Json(json));
      json = Json(6i32);
      Assert::AreEqual<Json>(6i32, Json(json));
      json = Json(7ui32);
      Assert::AreEqual<Json>(7ui32, Json(json));
      json = Json(8i64);
      Assert::AreEqual<Json>(8i64, Json(json));
      json = Json(9ui64);
      Assert::AreEqual<Json>(9ui64, Json(json));
      json = Json(13.37f);
      Assert::AreEqual<Json>(13.37f, Json(json));
      json = Json(13.37);
      Assert::AreEqual<Json>(13.37, Json(json));
      json = Json({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } });
      Assert::AreEqual<Json>({ { L"Key1", L"Value1"s }, { L"Key2", L"Value2"s } }, Json(json));
      json = Json({ 1, 3, 3, 7 });
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, Json(json));
    }

    TEST_METHOD(TestConstructorNullptr)
    {
      Assert::IsTrue(Json(nullptr_t()).Is(JsonType::Null));
      Assert::IsTrue(Json(nullptr_t()).Type() == JsonType::Null);
    }

    TEST_METHOD(TestConstructorConstWcharPointer)
    {
      Assert::IsTrue(Json(L"").Is(JsonType::String));
      Assert::IsTrue(Json(L"").Type() == JsonType::String);
    }

    TEST_METHOD(TestConstructorWstring)
    {
      Assert::IsTrue(Json(wstring()).Is(JsonType::String));
      Assert::IsTrue(Json(wstring()).Type() == JsonType::String);
    }

    TEST_METHOD(TestConstructorBool)
    {
      Assert::IsTrue(Json(bool()).Is(JsonType::Boolean));
      Assert::IsTrue(Json(bool()).Type() == JsonType::Boolean);
    }

    TEST_METHOD(TestConstructorChar)
    {
      Assert::IsTrue(Json(char()).Is(JsonType::Integer));
      Assert::IsTrue(Json(char()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorInt8_t)
    {
      Assert::IsTrue(Json(int8_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(int8_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorUInt8_t)
    {
      Assert::IsTrue(Json(uint8_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(uint8_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorInt16_t)
    {
      Assert::IsTrue(Json(int16_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(int16_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorUInt16_t)
    {
      Assert::IsTrue(Json(uint16_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(uint16_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorInt32_t)
    {
      Assert::IsTrue(Json(int32_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(int32_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorUInt32_t)
    {
      Assert::IsTrue(Json(uint32_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(uint32_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorInt64_t)
    {
      Assert::IsTrue(Json(int64_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(int64_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorUInt64_t)
    {
      Assert::IsTrue(Json(uint64_t()).Is(JsonType::Integer));
      Assert::IsTrue(Json(uint64_t()).Type() == JsonType::Integer);
    }

    TEST_METHOD(TestConstructorFloat)
    {
      Assert::IsTrue(Json(float()).Is(JsonType::Real));
      Assert::IsTrue(Json(float()).Type() == JsonType::Real);
    }

    TEST_METHOD(TestConstructorDouble)
    {
      Assert::IsTrue(Json(double()).Is(JsonType::Real));
      Assert::IsTrue(Json(double()).Type() == JsonType::Real);
    }

    TEST_METHOD(TestConstructorJsonObject)
    {
      Assert::IsTrue(Json(JsonObject()).Is(JsonType::Object));
      Assert::IsTrue(Json(JsonObject()).Type() == JsonType::Object);
    }

    TEST_METHOD(TestConstructorJsonArray)
    {
      Assert::IsTrue(Json(JsonArray()).Is(JsonType::Array));
      Assert::IsTrue(Json(JsonArray()).Type() == JsonType::Array);
    }

    TEST_METHOD(TestType)
    {
      auto tuples = vector<tuple<Json, JsonType, JsonType>>
      {
        { nullptr_t (), JsonType::Null   , JsonType::Simple  },
        { wstring   (), JsonType::String , JsonType::Simple  },
        { bool      (), JsonType::Boolean, JsonType::Simple  },
        { double    (), JsonType::Real   , JsonType::Number  },
        { int64_t   (), JsonType::Integer, JsonType::Number  },
        { double    (), JsonType::Real   , JsonType::Simple  },
        { int64_t   (), JsonType::Integer, JsonType::Simple  },
        { JsonObject(), JsonType::Object , JsonType::Complex },
        { JsonArray (), JsonType::Array  , JsonType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::AreEqual(expected1, input.Type());
        Assert::AreEqual(expected2, input.Type());
      }
    }

    TEST_METHOD(TestIs)
    {
      auto tuples = vector<tuple<Json, JsonType, JsonType>>
      {
        { nullptr_t() , JsonType::Null   , JsonType::Simple  },
        { wstring()   , JsonType::String , JsonType::Simple  },
        { bool()      , JsonType::Boolean, JsonType::Simple  },
        { double    (), JsonType::Real   , JsonType::Number  },
        { int64_t   (), JsonType::Integer, JsonType::Number  },
        { double    (), JsonType::Real   , JsonType::Simple  },
        { int64_t   (), JsonType::Integer, JsonType::Simple  },
        { JsonObject(), JsonType::Object , JsonType::Complex },
        { JsonArray() , JsonType::Array  , JsonType::Complex },
      };
      for (auto& [input, expected1, expected2] : tuples)
      {
        Assert::IsTrue(input.Is(expected1));
        Assert::IsTrue(input.Is(expected2));
      }
    }

    TEST_METHOD(TestDump)
    {
      auto pairs = vector<pair<Json, wstring>>
      {
        { nullptr  , L"null"s      },
        { L"Test1" , L"\"Test1\""s },
        { L"Test2"s, L"\"Test2\""s },
        { false    , L"false"s     },
        { true     , L"true"s      },
        { (char)1  , L"1"s         },
        { 2i8      , L"2"s         },
        { 3ui8     , L"3"s         },
        { 4i16     , L"4"s         },
        { 5ui16    , L"5"s         },
        { 6i32     , L"6"s         },
        { 7ui32    , L"7"s         },
        { 8i64     , L"8"s         },
        { 9ui64    , L"9"s         },
        { 13.37f   , L"13.37"s     },
        { 313.37   , L"313.37"s    },
        { 337184288, L"337184288"s },
        { 1.3f     , L"1.3"        },
        { 1.3      , L"1.3"        }
      };
      for (auto& [input, expected] : pairs)
      {
        for (int i = 0; i < 4; ++i)
        {
          for (auto c : { L' ', L'\t' })
          {
            Assert::AreEqual(expected, input.Dump(i, c));
          }
        }
      }

      Json object = {
        { L"Key1", nullptr },
        { L"Key2", L"Test1" },
        { L"Key3", L"Test2"s },
        { L"Key4", false },
        { L"Key5", true },
        { L"Key6", (char)1 },
        { L"Key7", 2i8 },
        { L"Key8", 3ui8 },
        { L"Key9", 4i16 },
        { L"Key10", 5ui16 },
        { L"Key11", 6i32 },
        { L"Key12", 7ui32 },
        { L"Key13", 8i64 },
        { L"Key14", 9ui64 },
        { L"Key15", 13.37f },
        { L"Key16", 313.37 },
        { L"Key17", {
          { L"InnerKey1", L"Value1" },
          { L"InnerKey2", L"Value2" } }
        },
        { L"Key18", { 1, 3, 3, 7 } }
      };

      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump());
      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump(0));
      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump(0, L' '));
      Assert::AreEqual(L"{\"Key1\":null,\"Key2\":\"Test1\",\"Key3\":\"Test2\",\"Key4\":false,\"Key5\":true,\"Key6\":1,\"Key7\":2,\"Key8\":3,\"Key9\":4,\"Key10\":5,\"Key11\":6,\"Key12\":7,\"Key13\":8,\"Key14\":9,"
                         "\"Key15\":13.37,\"Key16\":313.37,\"Key17\":{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},\"Key18\":[1,3,3,7]}"s, object.Dump(0, L'\t'));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         " \"Key1\": null,"                   "\r\n"
         " \"Key2\": \"Test1\","              "\r\n"
         " \"Key3\": \"Test2\","              "\r\n"
         " \"Key4\": false,"                  "\r\n"
         " \"Key5\": true,"                   "\r\n"
         " \"Key6\": 1,"                      "\r\n"
         " \"Key7\": 2,"                      "\r\n"
         " \"Key8\": 3,"                      "\r\n"
         " \"Key9\": 4,"                      "\r\n"
         " \"Key10\": 5,"                     "\r\n"
         " \"Key11\": 6,"                     "\r\n"
         " \"Key12\": 7,"                     "\r\n"
         " \"Key13\": 8,"                     "\r\n"
         " \"Key14\": 9,"                     "\r\n"
         " \"Key15\": 13.37,"                 "\r\n"
         " \"Key16\": 313.37,"                "\r\n"
         " \"Key17\": {"                      "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " \"Key18\": ["                      "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "}"s, object.Dump(1));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         " \"Key1\": null,"                   "\r\n"
         " \"Key2\": \"Test1\","              "\r\n"
         " \"Key3\": \"Test2\","              "\r\n"
         " \"Key4\": false,"                  "\r\n"
         " \"Key5\": true,"                   "\r\n"
         " \"Key6\": 1,"                      "\r\n"
         " \"Key7\": 2,"                      "\r\n"
         " \"Key8\": 3,"                      "\r\n"
         " \"Key9\": 4,"                      "\r\n"
         " \"Key10\": 5,"                     "\r\n"
         " \"Key11\": 6,"                     "\r\n"
         " \"Key12\": 7,"                     "\r\n"
         " \"Key13\": 8,"                     "\r\n"
         " \"Key14\": 9,"                     "\r\n"
         " \"Key15\": 13.37,"                 "\r\n"
         " \"Key16\": 313.37,"                "\r\n"
         " \"Key17\": {"                      "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " \"Key18\": ["                      "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "}"s, object.Dump(1, L' '));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "\t\"Key1\": null,"                  "\r\n"
         "\t\"Key2\": \"Test1\","             "\r\n"
         "\t\"Key3\": \"Test2\","             "\r\n"
         "\t\"Key4\": false,"                 "\r\n"
         "\t\"Key5\": true,"                  "\r\n"
         "\t\"Key6\": 1,"                     "\r\n"
         "\t\"Key7\": 2,"                     "\r\n"
         "\t\"Key8\": 3,"                     "\r\n"
         "\t\"Key9\": 4,"                     "\r\n"
         "\t\"Key10\": 5,"                    "\r\n"
         "\t\"Key11\": 6,"                    "\r\n"
         "\t\"Key12\": 7,"                    "\r\n"
         "\t\"Key13\": 8,"                    "\r\n"
         "\t\"Key14\": 9,"                    "\r\n"
         "\t\"Key15\": 13.37,"                "\r\n"
         "\t\"Key16\": 313.37,"               "\r\n"
         "\t\"Key17\": {"                     "\r\n"
         "\t\t\"InnerKey1\": \"Value1\","     "\r\n"
         "\t\t\"InnerKey2\": \"Value2\""      "\r\n"
         "\t},"                               "\r\n"
         "\t\"Key18\": ["                     "\r\n"
         "\t\t1,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t7"                              "\r\n"
         "\t]"                                "\r\n"
         "}"s, object.Dump(1, L'\t'));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "  \"Key1\": null,"                  "\r\n"
         "  \"Key2\": \"Test1\","             "\r\n"
         "  \"Key3\": \"Test2\","             "\r\n"
         "  \"Key4\": false,"                 "\r\n"
         "  \"Key5\": true,"                  "\r\n"
         "  \"Key6\": 1,"                     "\r\n"
         "  \"Key7\": 2,"                     "\r\n"
         "  \"Key8\": 3,"                     "\r\n"
         "  \"Key9\": 4,"                     "\r\n"
         "  \"Key10\": 5,"                    "\r\n"
         "  \"Key11\": 6,"                    "\r\n"
         "  \"Key12\": 7,"                    "\r\n"
         "  \"Key13\": 8,"                    "\r\n"
         "  \"Key14\": 9,"                    "\r\n"
         "  \"Key15\": 13.37,"                "\r\n"
         "  \"Key16\": 313.37,"               "\r\n"
         "  \"Key17\": {"                     "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  \"Key18\": ["                     "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "}"s, object.Dump(2));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "  \"Key1\": null,"                  "\r\n"
         "  \"Key2\": \"Test1\","             "\r\n"
         "  \"Key3\": \"Test2\","             "\r\n"
         "  \"Key4\": false,"                 "\r\n"
         "  \"Key5\": true,"                  "\r\n"
         "  \"Key6\": 1,"                     "\r\n"
         "  \"Key7\": 2,"                     "\r\n"
         "  \"Key8\": 3,"                     "\r\n"
         "  \"Key9\": 4,"                     "\r\n"
         "  \"Key10\": 5,"                    "\r\n"
         "  \"Key11\": 6,"                    "\r\n"
         "  \"Key12\": 7,"                    "\r\n"
         "  \"Key13\": 8,"                    "\r\n"
         "  \"Key14\": 9,"                    "\r\n"
         "  \"Key15\": 13.37,"                "\r\n"
         "  \"Key16\": 313.37,"               "\r\n"
         "  \"Key17\": {"                     "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  \"Key18\": ["                     "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "}"s, object.Dump(2, L' '));

      Assert::AreEqual(
        L"{"                                  "\r\n"
         "\t\t\"Key1\": null,"                "\r\n"
         "\t\t\"Key2\": \"Test1\","           "\r\n"
         "\t\t\"Key3\": \"Test2\","           "\r\n"
         "\t\t\"Key4\": false,"               "\r\n"
         "\t\t\"Key5\": true,"                "\r\n"
         "\t\t\"Key6\": 1,"                   "\r\n"
         "\t\t\"Key7\": 2,"                   "\r\n"
         "\t\t\"Key8\": 3,"                   "\r\n"
         "\t\t\"Key9\": 4,"                   "\r\n"
         "\t\t\"Key10\": 5,"                  "\r\n"
         "\t\t\"Key11\": 6,"                  "\r\n"
         "\t\t\"Key12\": 7,"                  "\r\n"
         "\t\t\"Key13\": 8,"                  "\r\n"
         "\t\t\"Key14\": 9,"                  "\r\n"
         "\t\t\"Key15\": 13.37,"              "\r\n"
         "\t\t\"Key16\": 313.37,"             "\r\n"
         "\t\t\"Key17\": {"                   "\r\n"
         "\t\t\t\t\"InnerKey1\": \"Value1\"," "\r\n"
         "\t\t\t\t\"InnerKey2\": \"Value2\""  "\r\n"
         "\t\t},"                             "\r\n"
         "\t\t\"Key18\": ["                   "\r\n"
         "\t\t\t\t1,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t7"                          "\r\n"
         "\t\t]"                              "\r\n"
         "}"s, object.Dump(2, L'\t'));

      Json array = {
        nullptr, L"Test1", L"Test2"s, false, true, (char)1, 2i8, 3ui8, 4i16, 5ui16, 6i32, 7ui32, 8i64, 9ui64, 13.37f, 313.37,
        {
          { L"InnerKey1", L"Value1" },
          { L"InnerKey2", L"Value2" }
        },
        { 1, 3, 3, 7 }
      };

      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump());
      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump(0));
      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump(0, L' '));
      Assert::AreEqual(L"[null,\"Test1\",\"Test2\",false,true,1,2,3,4,5,6,7,8,9,13.37,313.37,{\"InnerKey1\":\"Value1\",\"InnerKey2\":\"Value2\"},[1,3,3,7]]"s, array.Dump(0, L'\t'));

      Assert::AreEqual(
        L"["                                  "\r\n"
         " null,"                             "\r\n"
         " \"Test1\","                        "\r\n"
         " \"Test2\","                        "\r\n"
         " false,"                            "\r\n"
         " true,"                             "\r\n"
         " 1,"                                "\r\n"
         " 2,"                                "\r\n"
         " 3,"                                "\r\n"
         " 4,"                                "\r\n"
         " 5,"                                "\r\n"
         " 6,"                                "\r\n"
         " 7,"                                "\r\n"
         " 8,"                                "\r\n"
         " 9,"                                "\r\n"
         " 13.37,"                            "\r\n"
         " 313.37,"                           "\r\n"
         " {"                                 "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " ["                                 "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "]"s, array.Dump(1));

      Assert::AreEqual(
        L"["                                  "\r\n"
         " null,"                             "\r\n"
         " \"Test1\","                        "\r\n"
         " \"Test2\","                        "\r\n"
         " false,"                            "\r\n"
         " true,"                             "\r\n"
         " 1,"                                "\r\n"
         " 2,"                                "\r\n"
         " 3,"                                "\r\n"
         " 4,"                                "\r\n"
         " 5,"                                "\r\n"
         " 6,"                                "\r\n"
         " 7,"                                "\r\n"
         " 8,"                                "\r\n"
         " 9,"                                "\r\n"
         " 13.37,"                            "\r\n"
         " 313.37,"                           "\r\n"
         " {"                                 "\r\n"
         "  \"InnerKey1\": \"Value1\","       "\r\n"
         "  \"InnerKey2\": \"Value2\""        "\r\n"
         " },"                                "\r\n"
         " ["                                 "\r\n"
         "  1,"                               "\r\n"
         "  3,"                               "\r\n"
         "  3,"                               "\r\n"
         "  7"                                "\r\n"
         " ]"                                 "\r\n"
         "]"s, array.Dump(1, L' '));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "\tnull,"                            "\r\n"
         "\t\"Test1\","                       "\r\n"
         "\t\"Test2\","                       "\r\n"
         "\tfalse,"                           "\r\n"
         "\ttrue,"                            "\r\n"
         "\t1,"                               "\r\n"
         "\t2,"                               "\r\n"
         "\t3,"                               "\r\n"
         "\t4,"                               "\r\n"
         "\t5,"                               "\r\n"
         "\t6,"                               "\r\n"
         "\t7,"                               "\r\n"
         "\t8,"                               "\r\n"
         "\t9,"                               "\r\n"
         "\t13.37,"                           "\r\n"
         "\t313.37,"                          "\r\n"
         "\t{"                                "\r\n"
         "\t\t\"InnerKey1\": \"Value1\","     "\r\n"
         "\t\t\"InnerKey2\": \"Value2\""      "\r\n"
         "\t},"                               "\r\n"
         "\t["                                "\r\n"
         "\t\t1,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t7"                              "\r\n"
         "\t]"                                "\r\n"
         "]"s, array.Dump(1, L'\t'));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "  null,"                            "\r\n"
         "  \"Test1\","                       "\r\n"
         "  \"Test2\","                       "\r\n"
         "  false,"                           "\r\n"
         "  true,"                            "\r\n"
         "  1,"                               "\r\n"
         "  2,"                               "\r\n"
         "  3,"                               "\r\n"
         "  4,"                               "\r\n"
         "  5,"                               "\r\n"
         "  6,"                               "\r\n"
         "  7,"                               "\r\n"
         "  8,"                               "\r\n"
         "  9,"                               "\r\n"
         "  13.37,"                           "\r\n"
         "  313.37,"                          "\r\n"
         "  {"                                "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  ["                                "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "]"s, array.Dump(2));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "  null,"                            "\r\n"
         "  \"Test1\","                       "\r\n"
         "  \"Test2\","                       "\r\n"
         "  false,"                           "\r\n"
         "  true,"                            "\r\n"
         "  1,"                               "\r\n"
         "  2,"                               "\r\n"
         "  3,"                               "\r\n"
         "  4,"                               "\r\n"
         "  5,"                               "\r\n"
         "  6,"                               "\r\n"
         "  7,"                               "\r\n"
         "  8,"                               "\r\n"
         "  9,"                               "\r\n"
         "  13.37,"                           "\r\n"
         "  313.37,"                          "\r\n"
         "  {"                                "\r\n"
         "    \"InnerKey1\": \"Value1\","     "\r\n"
         "    \"InnerKey2\": \"Value2\""      "\r\n"
         "  },"                               "\r\n"
         "  ["                                "\r\n"
         "    1,"                             "\r\n"
         "    3,"                             "\r\n"
         "    3,"                             "\r\n"
         "    7"                              "\r\n"
         "  ]"                                "\r\n"
         "]"s, array.Dump(2, L' '));

      Assert::AreEqual(
        L"["                                  "\r\n"
         "\t\tnull,"                          "\r\n"
         "\t\t\"Test1\","                     "\r\n"
         "\t\t\"Test2\","                     "\r\n"
         "\t\tfalse,"                         "\r\n"
         "\t\ttrue,"                          "\r\n"
         "\t\t1,"                             "\r\n"
         "\t\t2,"                             "\r\n"
         "\t\t3,"                             "\r\n"
         "\t\t4,"                             "\r\n"
         "\t\t5,"                             "\r\n"
         "\t\t6,"                             "\r\n"
         "\t\t7,"                             "\r\n"
         "\t\t8,"                             "\r\n"
         "\t\t9,"                             "\r\n"
         "\t\t13.37,"                         "\r\n"
         "\t\t313.37,"                        "\r\n"
         "\t\t{"                              "\r\n"
         "\t\t\t\t\"InnerKey1\": \"Value1\"," "\r\n"
         "\t\t\t\t\"InnerKey2\": \"Value2\""  "\r\n"
         "\t\t},"                             "\r\n"
         "\t\t["                              "\r\n"
         "\t\t\t\t1,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t3,"                         "\r\n"
         "\t\t\t\t7"                          "\r\n"
         "\t\t]"                              "\r\n"
         "]"s, array.Dump(2, L'\t'));
    }

    TEST_METHOD(TestParse)
    {
      auto str = u8"𤭢"s;
      auto wstr = L"𤭢"s;
      auto ustr = U"𤭢"s;
      auto expected = JsonArray{ L"𤭢"s };

      auto i8list = initializer_list<int8_t>{ (int8_t)'[', (int8_t)'"', (int8_t)str[0], (int8_t)str[1], (int8_t)str[2], (int8_t)str[3], (int8_t)'"', (int8_t)']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<int8_t>{ (int8_t)'[', (int8_t)'"', (int8_t)str[0], (int8_t)str[1], (int8_t)str[2], (int8_t)str[3], (int8_t)'"', (int8_t)']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(i8list));
      Assert::AreEqual<Json>(expected, Json::Parse(i8list.begin(), i8list.end()));
      auto i8enumerable = From(i8list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(i8list)));
      Assert::AreEqual<Json>(expected, Json::Parse(i8enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(i8enumerable.begin(), i8enumerable.end()));

      auto ui8list = initializer_list<uint8_t>{ (uint8_t)'[', (uint8_t)'"', (uint8_t)str[0], (uint8_t)str[1], (uint8_t)str[2], (uint8_t)str[3], (uint8_t)'"', (uint8_t)']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<uint8_t>{ (uint8_t)'[', (uint8_t)'"', (uint8_t)str[0], (uint8_t)str[1], (uint8_t)str[2], (uint8_t)str[3], (uint8_t)'"', (uint8_t)']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8list));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8list.begin(), ui8list.end()));
      auto ui8enumerable = From(ui8list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(ui8list)));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(ui8enumerable.begin(), ui8enumerable.end()));

      auto i16list = initializer_list<int16_t>{ (int16_t)L'[', (int16_t)L'"', (int16_t)wstr[0], (int16_t)wstr[1], (int16_t)L'"', (int16_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<int16_t>{ (int16_t)L'[', (int16_t)L'"', (int16_t)wstr[0], (int16_t)wstr[1], (int16_t)L'"', (int16_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(i16list));
      Assert::AreEqual<Json>(expected, Json::Parse(i16list.begin(), i16list.end()));
      auto i16enumerable = From(i16list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(i16list)));
      Assert::AreEqual<Json>(expected, Json::Parse(i16enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(i16enumerable.begin(), i16enumerable.end()));

      auto ui16list = initializer_list<uint16_t>{ (uint16_t)L'[', (uint16_t)L'"', (uint16_t)wstr[0], (uint16_t)wstr[1], (uint16_t)L'"', (uint16_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<uint16_t>{ (uint16_t)L'[', (uint16_t)L'"', (uint16_t)wstr[0], (uint16_t)wstr[1], (uint16_t)L'"', (uint16_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16list));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16list.begin(), ui16list.end()));
      auto ui16enumerable = From(ui16list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(ui16list)));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(ui16enumerable.begin(), ui16enumerable.end()));

      auto i32list = initializer_list<int32_t>{ (int32_t)L'[', (int32_t)L'"', (int32_t)ustr[0], (int32_t)L'"', (int32_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<int32_t>{ (int32_t)L'[', (int32_t)L'"', (int32_t)ustr[0], (int32_t)L'"', (int32_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(i32list));
      Assert::AreEqual<Json>(expected, Json::Parse(i32list.begin(), i32list.end()));
      auto i32enumerable = From(i32list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(i32list)));
      Assert::AreEqual<Json>(expected, Json::Parse(i32enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(i32enumerable.begin(), i32enumerable.end()));

      auto ui32list = initializer_list<uint32_t>{ (uint32_t)L'[', (uint32_t)L'"', (uint32_t)ustr[0], (uint32_t)L'"', (uint32_t)L']' };
      Assert::AreEqual<Json>(expected, Json::Parse(initializer_list<uint32_t>{ (uint32_t)L'[', (uint32_t)L'"', (uint32_t)ustr[0], (uint32_t)L'"', (uint32_t)L']' }));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32list));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32list.begin(), ui32list.end()));
      auto ui32enumerable = From(ui32list);
      Assert::AreEqual<Json>(expected, Json::Parse(From(ui32list)));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32enumerable));
      Assert::AreEqual<Json>(expected, Json::Parse(ui32enumerable.begin(), ui32enumerable.end()));
    }

    TEST_METHOD(TestRead)
    {
      auto input = Json::Read(L"pass01.json");
      
      Assert::IsTrue(input.Is(JsonType::Array));
      Assert::AreEqual(20i64, input.Size());

      Assert::AreEqual<Json>(L"JSON Test Pattern pass1"s, input[0]);

      Assert::IsTrue(input[1].Is(JsonType::Object));
      Assert::AreEqual(1i64, input[1].Size());
      Assert::IsTrue(input[1][L"object with 1 member"].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[1][L"object with 1 member"].Size());
      Assert::AreEqual<Json>(L"array with 1 element", input[1][L"object with 1 member"][0]);

      Assert::IsTrue(input[2].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[2].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[2]);

      Assert::IsTrue(input[3].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[3].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[3]);

      Assert::AreEqual<Json>(-42, input[4]);

      Assert::AreEqual<Json>(true, input[5]);

      Assert::AreEqual<Json>(false, input[6]);

      Assert::AreEqual<Json>(nullptr, input[7]);

      Assert::IsTrue(input[8].Is(JsonType::Object));
      Assert::AreEqual(32i64, input[8].Size());
      Assert::AreEqual<Json>(1234567890, input[8][L"integer"]);
      Assert::AreEqual<Json>(-9876.543210, input[8][L"real"]);
      Assert::AreEqual<Json>(0.123456789e-12, input[8][L"e"]);
      Assert::AreEqual<Json>(1.234567890E+34, input[8][L"E"]);
      Assert::AreEqual<Json>(23456789012E66, input[8][L""]);
      Assert::AreEqual<Json>(0, input[8][L"zero"]);
      Assert::AreEqual<Json>(1, input[8][L"one"]);
      Assert::AreEqual<Json>(L" "s, input[8][L"space"]);
      Assert::AreEqual<Json>(L"\""s, input[8][L"quote"]);
      Assert::AreEqual<Json>(L"\\"s, input[8][L"backslash"]);
      Assert::AreEqual<Json>(L"\b\f\n\r\t"s, input[8][L"controls"]);
      Assert::AreEqual<Json>(L"/ & /"s, input[8][L"slash"]);
      Assert::AreEqual<Json>(L"abcdefghijklmnopqrstuvwyz"s, input[8][L"alpha"]);
      Assert::AreEqual<Json>(L"ABCDEFGHIJKLMNOPQRSTUVWYZ"s, input[8][L"ALPHA"]);
      Assert::AreEqual<Json>(L"0123456789"s, input[8][L"digit"]);
      Assert::AreEqual<Json>(L"digit"s, input[8][L"0123456789"]);
      Assert::AreEqual<Json>(L"`1~!@#$%^&*()_+-={':[,]}|;.</>?"s, input[8][L"special"]);
      Assert::AreEqual<Json>(L"\u0123\u4567\u89AB\uCDEF\uabcd\uef4A"s, input[8][L"hex"]);
      Assert::AreEqual<Json>(true, input[8][L"true"]);
      Assert::AreEqual<Json>(false, input[8][L"false"]);
      Assert::AreEqual<Json>(nullptr, input[8][L"null"]);
      Assert::IsTrue(input[8][L"array"].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[8][L"array"].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[8][L"array"]);
      Assert::IsTrue(input[8][L"object"].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[8][L"object"].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[8][L"object"]);
      Assert::AreEqual<Json>(L"50 St. James Street", input[8][L"address"]);
      Assert::AreEqual<Json>(L"http://www.JSON.org/", input[8][L"url"]);
      Assert::AreEqual<Json>(L"// /* <!-- --", input[8][L"comment"]);
      Assert::AreEqual<Json>(L" ", input[8][L"# -- --> */"]);
      Assert::IsTrue(input[8][L" s p a c e d "].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L" s p a c e d "].Size());
      Assert::AreEqual<Json>(1, input[8][L" s p a c e d "][0]);
      Assert::AreEqual<Json>(2, input[8][L" s p a c e d "][1]);
      Assert::AreEqual<Json>(3, input[8][L" s p a c e d "][2]);
      Assert::AreEqual<Json>(4, input[8][L" s p a c e d "][3]);
      Assert::AreEqual<Json>(5, input[8][L" s p a c e d "][4]);
      Assert::AreEqual<Json>(6, input[8][L" s p a c e d "][5]);
      Assert::AreEqual<Json>(7, input[8][L" s p a c e d "][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L" s p a c e d "]);
      Assert::IsTrue(input[8][L"compact"].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L"compact"].Size());
      Assert::AreEqual<Json>(1, input[8][L"compact"][0]);
      Assert::AreEqual<Json>(2, input[8][L"compact"][1]);
      Assert::AreEqual<Json>(3, input[8][L"compact"][2]);
      Assert::AreEqual<Json>(4, input[8][L"compact"][3]);
      Assert::AreEqual<Json>(5, input[8][L"compact"][4]);
      Assert::AreEqual<Json>(6, input[8][L"compact"][5]);
      Assert::AreEqual<Json>(7, input[8][L"compact"][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L"compact"]);
      Assert::AreEqual<Json>(L"{\"object with 1 member\":[\"array with 1 element\"]}", input[8][L"jsontext"]);
      Assert::AreEqual<Json>(L"&#34; \u0022 %22 0x22 034 &#x22;", input[8][L"quotes"]);
      Assert::AreEqual<Json>(L"A key can be any string", input[8][L"/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"]);

      Assert::AreEqual<Json>(0.5, input[9]);

      Assert::AreEqual<Json>(98.6, input[10]);

      Assert::AreEqual<Json>(99.44, input[11]);

      Assert::AreEqual<Json>(1066, input[12]);

      Assert::AreEqual<Json>(1e1, input[13]);

      Assert::AreEqual<Json>(0.1e1, input[14]);

      Assert::AreEqual<Json>(1e-1, input[15]);

      Assert::AreEqual<Json>(1e00, input[16]);

      Assert::AreEqual<Json>(2e+00, input[17]);

      Assert::AreEqual<Json>(2e-00, input[18]);

      Assert::AreEqual<Json>(L"rosebud", input[19]);
    }

    TEST_METHOD(TestWrite)
    {
      Json::Read(L"pass01.json").Write(L"pass01_copy.json");
      auto input = Json::Read(L"pass01_copy.json");

      Assert::AreEqual<Json>(Json::Read(L"pass01.json"), input);
      Assert::AreEqual<Json>(
        {
          L"JSON Test Pattern pass1"s,
          { { L"object with 1 member"s, { L"array with 1 element"s } } },
          JsonObject{},
          JsonArray{},
          -42,
          true,
          false,
          nullptr,
          {
            { L"integer"s, 1234567890 },
            { L"real"s, -9876.543210 },
            { L"e"s, 0.123456789e-12 },
            { L"E"s, 1.234567890E+34 },
            { L""s, 23456789012E66 },
            { L"zero"s, 0 },
            { L"one"s, 1 },
            { L"space"s, L" "s },
            { L"quote"s, L"\""s },
            { L"backslash"s, L"\\"s },
            { L"controls"s, L"\b\f\n\r\t"s },
            { L"slash"s, L"/ & /"s },
            { L"alpha"s, L"abcdefghijklmnopqrstuvwyz"s },
            { L"ALPHA"s, L"ABCDEFGHIJKLMNOPQRSTUVWYZ"s },
            { L"digit"s, L"0123456789"s },
            { L"0123456789"s, L"digit"s },
            { L"special"s, L"`1~!@#$%^&*()_+-={':[,]}|;.</>?"s },
            { L"hex"s, L"\u0123\u4567\u89AB\uCDEF\uabcd\uef4A"s },
            { L"true"s, true },
            { L"false"s, false },
            { L"null"s, nullptr },
            { L"array"s, JsonArray{} },
            { L"object"s, JsonObject{} },
            { L"address"s, L"50 St. James Street"s },
            { L"url"s, L"http://www.JSON.org/"s },
            { L"comment"s, L"// /* <!-- --"s },
            { L"# -- --> */"s, L" "s },
            { L" s p a c e d "s, { 1, 2, 3, 4, 5, 6, 7 } },
            { L"compact"s, { 1, 2, 3, 4, 5, 6, 7 } },
            { L"jsontext"s, L"{\"object with 1 member\":[\"array with 1 element\"]}"s },
            { L"quotes"s, L"&#34; \u0022 %22 0x22 034 &#x22;"s },
            { L"/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"s, L"A key can be any string"s }
          },
          0.5,
          98.6,
          99.44,
          1066,
          1e1,
          0.1e1,
          1e-1,
          1e00,
          2e+00,
          2e-00,
          L"rosebud"s
        }
        , input);

      auto json = Json{ 1, 3, 3, 7 };
      json.Write(L"test_write_00.json");
      json.Write(L"test_write_01.json", L'\t');
      json.Write(L"test_write_10.json", 1ui8);
      json.Write(L"test_write_11.json", 1ui8, L'\t');

      Assert::AreEqual(ReadAllText(L"test_write_00.json"), L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]"s);
      Assert::AreEqual(ReadAllText(L"test_write_01.json"), L"[\r\n\t\t1,\r\n\t\t3,\r\n\t\t3,\r\n\t\t7\r\n]"s);
      Assert::AreEqual(ReadAllText(L"test_write_10.json"), L"[\r\n 1,\r\n 3,\r\n 3,\r\n 7\r\n]"s);
      Assert::AreEqual(ReadAllText(L"test_write_11.json"), L"[\r\n\t1,\r\n\t3,\r\n\t3,\r\n\t7\r\n]"s);
    }

    TEST_METHOD(TestGet)
    {
      Assert::IsTrue(Json(nullptr).Get<nullptr_t>() == nullptr);
      Assert::IsTrue(Json(L"asd"s).Get<wstring>() == L"asd"s);
      Assert::IsTrue(Json(true).Get<bool>() == true);
      Assert::IsTrue(Json(false).Get<bool>() == false);
      Assert::IsTrue(Json(13.37).Get<double>() == 13.37);
      Assert::IsTrue(Json(1337).Get<int64_t>() == 1337);
      Assert::IsTrue(Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }).Get<JsonObject>() == JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(Json(JsonArray{ 1, 2, 3 }).Get<JsonArray>() == JsonArray{ 1, 2, 3 });
    }

    TEST_METHOD(TestGetIf)
    {
      auto null = Json(nullptr);
      Assert::IsTrue(null.GetIf<wstring>() == nullptr);
      Assert::IsTrue(null.GetIf<bool>() == nullptr);
      Assert::IsTrue(null.GetIf<double>() == nullptr);
      Assert::IsTrue(null.GetIf<int64_t>() == nullptr);
      Assert::IsTrue(null.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(null.GetIf<JsonArray>() == nullptr);
      auto ptrnull = null.GetIf<nullptr_t>();
      Assert::IsTrue(*ptrnull == nullptr);

      auto wstr = Json(L"asd"s);
      Assert::IsTrue(wstr.GetIf<nullptr_t>() == nullptr);
      Assert::IsTrue(wstr.GetIf<bool>() == nullptr);
      Assert::IsTrue(wstr.GetIf<double>() == nullptr);
      Assert::IsTrue(wstr.GetIf<int64_t>() == nullptr);
      Assert::IsTrue(wstr.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(wstr.GetIf<JsonArray>() == nullptr);
      auto ptrwstr = wstr.GetIf<wstring>();
      Assert::IsTrue(*ptrwstr == L"asd"s);
      *ptrwstr = L"test";
      Assert::IsTrue(*ptrwstr == L"test"s);

      auto boolean = Json(true);
      Assert::IsTrue(boolean.GetIf<nullptr_t>() == nullptr);
      Assert::IsTrue(boolean.GetIf<wstring>() == nullptr);
      Assert::IsTrue(boolean.GetIf<double>() == nullptr);
      Assert::IsTrue(boolean.GetIf<int64_t>() == nullptr);
      Assert::IsTrue(boolean.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(boolean.GetIf<JsonArray>() == nullptr);
      auto ptrboolean = boolean.GetIf<bool>();
      Assert::IsTrue(*ptrboolean == true);
      *ptrboolean = false;
      Assert::IsTrue(*ptrboolean == false);

      auto real = Json(13.37);
      Assert::IsTrue(real.GetIf<nullptr_t>() == nullptr);
      Assert::IsTrue(real.GetIf<wstring>() == nullptr);
      Assert::IsTrue(real.GetIf<bool>() == nullptr);
      Assert::IsTrue(real.GetIf<int64_t>() == nullptr);
      Assert::IsTrue(real.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(real.GetIf<JsonArray>() == nullptr);
      auto ptrreal = real.GetIf<double>();
      Assert::IsTrue(*ptrreal == 13.37);
      *ptrreal = 313.37;
      Assert::IsTrue(*ptrreal == 313.37);

      auto integer = Json(1337);
      Assert::IsTrue(integer.GetIf<nullptr_t>() == nullptr);
      Assert::IsTrue(integer.GetIf<wstring>() == nullptr);
      Assert::IsTrue(integer.GetIf<bool>() == nullptr);
      Assert::IsTrue(integer.GetIf<double>() == nullptr);
      Assert::IsTrue(integer.GetIf<JsonObject>() == nullptr);
      Assert::IsTrue(integer.GetIf<JsonArray>() == nullptr);
      auto ptrinteger = integer.GetIf<int64_t>();
      Assert::IsTrue(*ptrinteger == 1337);
      *ptrinteger = 31337;
      Assert::IsTrue(*ptrinteger == 31337);

      auto object = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(object.GetIf<nullptr_t>() == nullptr);
      Assert::IsTrue(object.GetIf<wstring>() == nullptr);
      Assert::IsTrue(object.GetIf<bool>() == nullptr);
      Assert::IsTrue(object.GetIf<double>() == nullptr);
      Assert::IsTrue(object.GetIf<int64_t>() == nullptr);
      Assert::IsTrue(object.GetIf<JsonArray>() == nullptr);
      auto ptrobject = object.GetIf<JsonObject>();
      Assert::IsTrue(*ptrobject == JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      *ptrobject = JsonObject{ { L"Key3"s, 3 }, { L"Key4"s, 4 } };
      Assert::IsTrue(*ptrobject == JsonObject{ { L"Key3"s, 3 }, { L"Key4"s, 4 } });

      auto array = Json(JsonArray{ 1, 2, 3 });
      Assert::IsTrue(array.GetIf<nullptr_t>() == nullptr);
      Assert::IsTrue(array.GetIf<wstring>() == nullptr);
      Assert::IsTrue(array.GetIf<bool>() == nullptr);
      Assert::IsTrue(array.GetIf<double>() == nullptr);
      Assert::IsTrue(array.GetIf<int64_t>() == nullptr);
      Assert::IsTrue(array.GetIf<JsonObject>() == nullptr);
      auto ptrarray = array.GetIf<JsonArray>();
      Assert::IsTrue(*ptrarray == JsonArray{ 1, 2, 3 });
      *ptrarray = JsonArray{ 1, 2, 3, 4 };
      Assert::IsTrue(*ptrarray == JsonArray{ 1, 2, 3, 4 });
    }

    TEST_METHOD(TestStringify)
    {
      Assert::AreEqual(L"null"s, Json::Stringify(nullptr));
      Assert::AreEqual(L"\"asd\""s, Json::Stringify(L"asd"s));
      Assert::AreEqual(L"true"s, Json::Stringify(true));
      Assert::AreEqual(L"false"s, Json::Stringify(false));
      Assert::AreEqual(L"13.37"s, Json::Stringify(13.37));
      Assert::AreEqual(L"313.37"s, Json::Stringify(313.37));
      Assert::AreEqual(L"1337"s, Json::Stringify(1337));
      Assert::AreEqual(L"1e+50"s, Json::Stringify(1e50));
      Assert::AreEqual(L"{\"Key1\":1,\"Key2\":2}"s, Json::Stringify(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }));
      Assert::AreEqual(L"[1,2,3]"s, Json::Stringify(JsonArray{ 1, 2, 3 }));
      Assert::AreEqual(L"Complex"s, Json::Stringify(JsonType::Complex));
      Assert::AreEqual(L"PropertyName"s, Json::Stringify(JsonTokenType::PropertyName));
    }

    TEST_METHOD(TestSize)
    {
      ExceptException<exception>([]() { Json(nullptr_t()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(wstring  ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(bool     ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(double   ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(int64_t  ()).Size(); }, "Size() is only defined for JsonObject and JsonArray!");

      Assert::AreEqual(0i64, Json(JsonObject()).Size());
      Assert::AreEqual(0i64, Json(JsonArray ()).Size());

      Assert::AreEqual(2i64, Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }).Size());
      Assert::AreEqual(4i64, Json(JsonArray{ 1, 3, 3, 7 }).Size());
    }

    TEST_METHOD(TestResize)
    {
      ExceptException<exception>([]() { Json(nullptr_t ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(wstring   ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(bool      ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(double    ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(int64_t   ()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(JsonObject()).Resize(1); }, "Resize(int64_t size) is only defined for JsonArray!");

      ExceptException<exception>([]() { Json(nullptr_t ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(wstring   ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(bool      ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(double    ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(int64_t   ()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(JsonObject()).Resize(1, Json()); }, "Resize(int64_t size, Json const& json) is only defined for JsonArray!");

      auto array = Json(JsonArray());
      Assert::AreEqual(0i64, array.Size());
      array.Resize(1);
      Assert::AreEqual(1i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      array.Resize(2, L"Test"s);
      Assert::AreEqual(2i64, array.Size());
      Assert::AreEqual<Json>(nullptr, array[0]);
      Assert::AreEqual<Json>(L"Test"s, array[1]);
    }

    TEST_METHOD(TestClear)
    {
      ExceptException<exception>([]() { Json(nullptr_t()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(wstring  ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(bool     ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(double   ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");
      ExceptException<exception>([]() { Json(int64_t  ()).Clear(); }, "Clear() is only defined for JsonObject and JsonArray!");

      auto object = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::AreEqual(2i64, object.Size());
      object.Clear();
      Assert::AreEqual(0i64, object.Size());

      auto array = Json(JsonArray{ 1, 3, 3, 7 });
      Assert::AreEqual(4i64, array.Size());
      array.Clear();
      Assert::AreEqual(0i64, array.Size());
    }

    TEST_METHOD(TestPushBack)
    {
      auto defaultJson = Json();
      ExceptException<exception>([&]() { Json(wstring   ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(bool      ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(double    ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(int64_t   ()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(JsonObject()).PushBack(defaultJson); }, "PushBack(Json const& value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(wstring   ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(bool      ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(double    ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(int64_t   ()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");
      ExceptException<exception>([&]() { Json(JsonObject()).PushBack(Json()); }, "PushBack(Json && value) is only defined for JsonArray!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json.PushBack(1337);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      Json integer = 31337;
      json.PushBack(integer);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(31337, json[1]);
    }

    TEST_METHOD(TestInsert1)
    {
      auto defaultPair = pair<wstring, Json>{ wstring(), Json() };
      ExceptException<exception>([&]() { Json(wstring  ()).Insert(defaultPair); }, "Insert(pair<KEY, Json> const& pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(bool     ()).Insert(defaultPair); }, "Insert(pair<KEY, Json> const& pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(double   ()).Insert(defaultPair); }, "Insert(pair<KEY, Json> const& pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(int64_t  ()).Insert(defaultPair); }, "Insert(pair<KEY, Json> const& pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(JsonArray()).Insert(defaultPair); }, "Insert(pair<KEY, Json> const& pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(wstring  ()).Insert({ wstring(), Json() }); }, "Insert(pair<KEY, Json> && pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(bool     ()).Insert({ wstring(), Json() }); }, "Insert(pair<KEY, Json> && pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(double   ()).Insert({ wstring(), Json() }); }, "Insert(pair<KEY, Json> && pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(int64_t  ()).Insert({ wstring(), Json() }); }, "Insert(pair<KEY, Json> && pair) is only defined for JsonObject!");
      ExceptException<exception>([&]() { Json(JsonArray()).Insert({ wstring(), Json() }); }, "Insert(pair<KEY, Json> && pair) is only defined for JsonObject!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json.Insert({ L"Key1"s, 1 });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[L"Key1"s]);
      json.Insert({ L"Key2"s, 2 });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
      auto key3 = pair<wstring, Json>{ L"Key3"s, 3 };
      json.Insert(key3);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(3, json[L"Key3"s]);
      auto key4 = pair<wstring, Json>{ L"Key4"s, 4 };
      json.Insert(key4);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(4i64, json.Size());
      Assert::AreEqual<Json>(4, json[L"Key4"s]);
    }

    TEST_METHOD(TestInsert2)
    {
      ExceptException<exception>([]() { Json(wstring   ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(bool      ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(double    ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(int64_t   ()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(JsonObject()).Insert(int64_t(), Json()); }, "Insert(int64_t index, Json && value) is only defined for JsonArray!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json.Insert(0, 1337);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);
      json.Insert(0, 31337);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(31337, json[0]);
      Assert::AreEqual<Json>(1337, json[1]);
    }

    TEST_METHOD(TestErase1)
    {
      ExceptException<exception>([]() { Json(nullptr_t()).Erase(wstring()); }, "Erase(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(wstring  ()).Erase(wstring()); }, "Erase(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(bool     ()).Erase(wstring()); }, "Erase(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(double   ()).Erase(wstring()); }, "Erase(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(int64_t  ()).Erase(wstring()); }, "Erase(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(JsonArray()).Erase(wstring()); }, "Erase(KEY const& key) is only defined for JsonObject!");

      auto json = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      json.Erase(L"Key1"s);
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
    }

    TEST_METHOD(TestErase2)
    {
      ExceptException<exception>([]() { Json(nullptr_t ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(wstring   ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(bool      ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(double    ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(int64_t   ()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(JsonObject()).Erase(int64_t()); }, "Erase(int64_t index) is only defined for JsonArray!");

      auto json = Json(JsonArray{ 1, 3, 3, 7 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(4i64, json.Size());
      json.Erase(0);
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(3, json[0]);
      Assert::AreEqual<Json>(3, json[1]);
      Assert::AreEqual<Json>(7, json[2]);
    }

    TEST_METHOD(TestKeys)
    {
      ExceptException<exception>([]() { Json(nullptr_t()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(wstring  ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(bool     ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(double   ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(int64_t  ()).Keys(); }, "Keys() is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(JsonArray()).Keys(); }, "Keys() is only defined for JsonObject!");

      auto json = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      auto keys = json.Keys();
      Assert::AreEqual<size_t>(2, keys.size());
      Assert::AreEqual(L"Key1"s, keys[0]);
      Assert::AreEqual(L"Key2"s, keys[1]);
    }

    TEST_METHOD(TestOperatorSubscript1)
    {
      ExceptException<exception>([]() { auto temp = Json(wstring  ())[wstring()]; }, "Operator[](KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto temp = Json(bool     ())[wstring()]; }, "Operator[](KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto temp = Json(double   ())[wstring()]; }, "Operator[](KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto temp = Json(int64_t  ())[wstring()]; }, "Operator[](KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto temp = Json(JsonArray())[wstring()]; }, "Operator[](KEY const& key) is only defined for JsonObject!");

      auto json = Json(nullptr_t());
      Assert::IsTrue(json.Is(JsonType::Null));
      json[L"Key1"s] = 1;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1, json[L"Key1"s]);
      json[L"Key2"s] = 2;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
      json[L"Key2"s] = 1337;
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1337, json[L"Key2"s]);
    }

    TEST_METHOD(TestOperatorSubscript2)
    {
      ExceptException<exception>([]() { auto temp = Json(nullptr_t ())[int64_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto temp = Json(wstring   ())[int64_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto temp = Json(bool      ())[int64_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto temp = Json(double    ())[int64_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto temp = Json(int64_t   ())[int64_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto temp = Json(JsonObject())[int64_t()]; }, "Operator[](int64_t index) is only defined for JsonArray!");

      auto json = Json(JsonArray());
      json.Resize(4);
      json[0] = 1;
      json[1] = 3;
      json[2] = 3;
      json[3] = 7;
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(3, json[1]);
      Assert::AreEqual<Json>(3, json[2]);
      Assert::AreEqual<Json>(7, json[3]);
    }

    TEST_METHOD(TestAt1)
    {
      ExceptException<exception>([]() { Json(nullptr_t()).At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(wstring  ()).At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(bool     ()).At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(double   ()).At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(int64_t  ()).At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { Json(JsonArray()).At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");

      auto json = Json{
        { L"Key1"s, 1 },
        { L"Key2"s, 2 }
      };
      Assert::AreEqual<Json>(1, json.At(L"Key1"s));
      Assert::AreEqual<Json>(2, json.At(L"Key2"s));
      json.At(L"Key1"s) = 1337;
      json.At(L"Key2"s) = 31337;
      Assert::AreEqual<Json>(1337, json.At(L"Key1"s));
      Assert::AreEqual<Json>(31337, json.At(L"Key2"s));
    }

    TEST_METHOD(TestAt2)
    {
      ExceptException<exception>([]() { auto const temp = Json(nullptr_t()); auto tmp = temp.At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto const temp = Json(wstring  ()); auto tmp = temp.At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto const temp = Json(bool     ()); auto tmp = temp.At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto const temp = Json(double   ()); auto tmp = temp.At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto const temp = Json(int64_t  ()); auto tmp = temp.At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");
      ExceptException<exception>([]() { auto const temp = Json(JsonArray()); auto tmp = temp.At(wstring()); }, "At(KEY const& key) is only defined for JsonObject!");

      auto const json = Json{
        { L"Key1"s, 1 },
        { L"Key2"s, 2 }
      };
      Assert::AreEqual<Json>(1, json.At(L"Key1"s));
      Assert::AreEqual<Json>(2, json.At(L"Key2"s));
      static_assert(is_const<remove_reference<decltype(json.At(L"Key1"))>::type>::value, "JsonObject::At(KEY) return type must be Json const&");
    }

    TEST_METHOD(TestAt3)
    {
      ExceptException<exception>([]() { Json(nullptr_t ()).At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(wstring   ()).At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(bool      ()).At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(double    ()).At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(int64_t   ()).At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { Json(JsonObject()).At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");

      auto json = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(1, json.At(0));
      Assert::AreEqual<Json>(3, json.At(1));
      Assert::AreEqual<Json>(3, json.At(2));
      Assert::AreEqual<Json>(7, json.At(3));
      json.At(0) = 1337;
      json.At(1) = 1338;
      json.At(2) = 1339;
      json.At(3) = 1340;
      Assert::AreEqual<Json>(1337, json.At(0));
      Assert::AreEqual<Json>(1338, json.At(1));
      Assert::AreEqual<Json>(1339, json.At(2));
      Assert::AreEqual<Json>(1340, json.At(3));
    }

    TEST_METHOD(TestAt4)
    {
      ExceptException<exception>([]() { auto const temp = Json(nullptr_t ()); auto tmp = temp.At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto const temp = Json(wstring   ()); auto tmp = temp.At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto const temp = Json(bool      ()); auto tmp = temp.At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto const temp = Json(double    ()); auto tmp = temp.At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto const temp = Json(int64_t   ()); auto tmp = temp.At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");
      ExceptException<exception>([]() { auto const temp = Json(JsonObject()); auto tmp = temp.At(int64_t()); }, "At(int64_t index) is only defined for JsonArray!");

      auto const json = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(1, json.At(0));
      Assert::AreEqual<Json>(3, json.At(1));
      Assert::AreEqual<Json>(3, json.At(2));
      Assert::AreEqual<Json>(7, json.At(3));
      static_assert(is_const<remove_reference<decltype(json.At(0))>::type>::value, "JsonArray::At(index) return type must be Json const&");
    }

    TEST_METHOD(TestOperatorConversionNullptr)
    {
      ExceptException<exception>([]() { auto temp = (nullptr_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (nullptr_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (nullptr_t)Json(double    ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (nullptr_t)Json(int64_t   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (nullptr_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (nullptr_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual(nullptr, (nullptr_t)Json(nullptr_t()));
    }

    TEST_METHOD(TestOperatorConversionWstring)
    {
      ExceptException<exception>([]() { auto temp = (wstring)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (wstring)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (wstring)Json(double    ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (wstring)Json(int64_t   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (wstring)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (wstring)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<wstring>(L"Test"s, (wstring)Json(L"Test"s));
    }

    TEST_METHOD(TestOperatorConversionBool)
    {
      Assert::IsFalse((bool)Json(nullptr_t ()));
      Assert::IsTrue ((bool)Json(L"Test"s    ));
      Assert::IsTrue ((bool)Json(true        ));
      Assert::IsFalse((bool)Json(false       ));
      Assert::IsTrue ((bool)Json(1.0         ));
      Assert::IsFalse((bool)Json(0.0         ));
      Assert::IsTrue ((bool)Json(1i64        ));
      Assert::IsFalse((bool)Json(0i64        ));
      Assert::IsTrue ((bool)Json(JsonObject()));
      Assert::IsTrue ((bool)Json(JsonArray ()));
    }

    TEST_METHOD(TestOperatorConversionChar)
    {
      ExceptException<exception>([]() { auto temp = (char)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (char)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (char)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (char)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (char)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<char>('A', (char)Json(65.0));
      Assert::AreEqual<char>('A', (char)Json('A'));
    }

    TEST_METHOD(TestOperatorConversionInt8)
    {
      ExceptException<exception>([]() { auto temp = (int8_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int8_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int8_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int8_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int8_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<int8_t>(-100, (int8_t)Json(-100.0));
      Assert::AreEqual<int8_t>(-100, (int8_t)Json(-100));
    }

    TEST_METHOD(TestOperatorConversionUInt8)
    {
      ExceptException<exception>([]() { auto temp = (uint8_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint8_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint8_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint8_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint8_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<uint8_t>(200, (uint8_t)Json(200.0));
      Assert::AreEqual<uint8_t>(200, (uint8_t)Json(200));
    }

    TEST_METHOD(TestOperatorConversionInt16)
    {
      ExceptException<exception>([]() { auto temp = (int16_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int16_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int16_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int16_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int16_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<int16_t>(-30'000, (int16_t)Json(-30'000.0));
      Assert::AreEqual<int16_t>(-30'000, (int16_t)Json(-30'000));
    }

    TEST_METHOD(TestOperatorConversionUInt16)
    {
      ExceptException<exception>([]() { auto temp = (uint16_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint16_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint16_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint16_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint16_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<uint16_t>(60'000, (uint16_t)Json(60'000.0));
      Assert::AreEqual<uint16_t>(60'000, (uint16_t)Json(60'000));
    }

    TEST_METHOD(TestOperatorConversionInt32)
    {
      ExceptException<exception>([]() { auto temp = (int32_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int32_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int32_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int32_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int32_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<int32_t>(-2'000'000'000, (int32_t)Json(-2'000'000'000.0));
      Assert::AreEqual<int32_t>(-2'000'000'000, (int32_t)Json(-2'000'000'000));
    }

    TEST_METHOD(TestOperatorConversionUInt32)
    {
      ExceptException<exception>([]() { auto temp = (uint32_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint32_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint32_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint32_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint32_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<uint32_t>(4'000'000'000, (uint32_t)Json(4'000'000'000.0));
      Assert::AreEqual<uint32_t>(4'000'000'000, (uint32_t)Json(4'000'000'000ui32));
    }

    TEST_METHOD(TestOperatorConversionInt64)
    {
      ExceptException<exception>([]() { auto temp = (int64_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int64_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int64_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int64_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (int64_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<int64_t>(-16'000'000'000, (int64_t)Json(-16'000'000'000.0));
      Assert::AreEqual<int64_t>(-16'000'000'000, (int64_t)Json(-16'000'000'000i64));
    }

    TEST_METHOD(TestOperatorConversionUInt64)
    {
      ExceptException<exception>([]() { auto temp = (uint64_t)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint64_t)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint64_t)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint64_t)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (uint64_t)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<uint64_t>(32'000'000'000, (uint64_t)Json(32'000'000'000.0));
      Assert::AreEqual<uint64_t>(32'000'000'000, (uint64_t)Json(32'000'000'000ui64));
    }

    TEST_METHOD(TestOperatorConversionFloat)
    {
      ExceptException<exception>([]() { auto temp = (float)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (float)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (float)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (float)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (float)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<float>(13.37f, (float)Json(13.37));
      Assert::AreEqual<float>(13.37f, (float)Json(13.37f));
      Assert::AreEqual<float>(1337.0, (float)Json(1337));
    }

    TEST_METHOD(TestOperatorConversionDouble)
    {
      ExceptException<exception>([]() { auto temp = (double)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (double)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (double)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (double)Json(JsonObject()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (double)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<double>(13.37, (double)Json(13.37));
      Assert::AreEqual<double>(13.37f, (double)Json(13.37f));
      Assert::AreEqual<double>(1337.0, (double)Json(1337));
    }

    TEST_METHOD(TestOperatorConversionJsonObject)
    {
      ExceptException<exception>([]() { auto temp = (JsonObject)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonObject)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonObject)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonObject)Json(double    ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonObject)Json(int64_t   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonObject)Json(JsonArray ()); }, "Invalid conversion!");
      Assert::AreEqual<JsonObject>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, (JsonObject)Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }));
    }

    TEST_METHOD(TestOperatorConversionJsonArray)
    {
      ExceptException<exception>([]() { auto temp = (JsonArray)Json(nullptr_t ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonArray)Json(wstring   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonArray)Json(bool      ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonArray)Json(double    ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonArray)Json(int64_t   ()); }, "Invalid conversion!");
      ExceptException<exception>([]() { auto temp = (JsonArray)Json(JsonObject()); }, "Invalid conversion!");
      Assert::AreEqual<JsonArray>(JsonArray{ 1, 3, 3, 7 }, (JsonArray)Json(JsonArray{ 1, 3, 3, 7 }));
    }

    TEST_METHOD(TestOperatorAssignmentNullptr)
    {
      auto json = Json(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      json = nullptr;
      Assert::AreEqual(nullptr, json.Get<nullptr_t>());
    }

    TEST_METHOD(TestOperatorAssignmentConstWcharPointer)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = L"Test";
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
    }

    TEST_METHOD(TestOperatorAssignmentWstring)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = L"Test"s;
      Assert::AreEqual(L"Test"s, json.Get<wstring>());
    }

    TEST_METHOD(TestOperatorAssignmentBool)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = true;
      Assert::AreEqual(true, json.Get<bool>());
    }

    TEST_METHOD(TestOperatorAssignmentChar)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = 'A';
      Assert::AreEqual(65i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt8)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int8_t)-100;
      Assert::AreEqual(-100i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt8)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint8_t)200;
      Assert::AreEqual(200i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt16)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int16_t)-30'000;
      Assert::AreEqual(-30'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt16)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint16_t)60'000;
      Assert::AreEqual(60'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt32)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int32_t)-2'000'000'000;
      Assert::AreEqual(-2'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt32)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint32_t)4'000'000'000;
      Assert::AreEqual(4'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentInt64)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (int64_t)-16'000'000'000;
      Assert::AreEqual(-16'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentUInt64)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = (uint64_t)32'000'000'000;
      Assert::AreEqual(32'000'000'000i64, json.Get<int64_t>());
    }

    TEST_METHOD(TestOperatorAssignmentFloat)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = 13.37f;
      Assert::AreEqual<double>(13.37f, json.Get<double>());
    }

    TEST_METHOD(TestOperatorAssignmentDouble)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = 13.37;
      Assert::AreEqual(13.37, json.Get<double>());
    }

    TEST_METHOD(TestOperatorAssignmentJson)
    {
      auto json = Json(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      json = Json(nullptr);
      Assert::IsTrue(json.Is(JsonType::Null));
      Assert::AreEqual(nullptr, json.Get<nullptr_t>());

      json = Json(L"Test"s);
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::AreEqual(L"Test"s, json.Get<wstring>());

      json = Json(true);
      Assert::IsTrue(json.Is(JsonType::Boolean));
      Assert::AreEqual(true, json.Get<bool>());

      json = Json(13.37);
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::AreEqual(13.37, json.Get<double>());

      json = Json(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::AreEqual(1337i64, json.Get<int64_t>());

      json = Json(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());

      json = Json(JsonArray{ 1, 3, 3, 7 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
    }

    TEST_METHOD(TestOperatorAssignmentJsonObject)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } };
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());
    }

    TEST_METHOD(TestOperatorAssignmentJsonArray)
    {
      auto json = Json();
      Assert::IsTrue(json.Is(JsonType::Null));
      json = JsonArray{ 1, 3, 3, 7 };
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
    }

    TEST_METHOD(TestOperatorAssignmentJsonBuilder)
    {
      auto json = Json(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      json = JsonBuilder(nullptr);
      Assert::AreEqual(nullptr, json.Get<nullptr_t>());

      json = JsonBuilder(L"Test"s);
      Assert::IsTrue(json.Is(JsonType::String));
      Assert::AreEqual(L"Test"s, json.Get<wstring>());

      json = JsonBuilder(true);
      Assert::IsTrue(json.Is(JsonType::Boolean));
      Assert::AreEqual(true, json.Get<bool>());

      json = JsonBuilder(13.37);
      Assert::IsTrue(json.Is(JsonType::Real));
      Assert::AreEqual(13.37, json.Get<double>());

      json = JsonBuilder(1337);
      Assert::IsTrue(json.Is(JsonType::Integer));
      Assert::AreEqual(1337i64, json.Get<int64_t>());

      json = JsonBuilder(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json.Get<JsonObject>());

      json = JsonBuilder(JsonArray{ 1, 3, 3, 7 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(JsonArray{ 1, 3, 3, 7 }, json.Get<JsonArray>());
    }

    TEST_METHOD(TestOperatorAssignmentInitializerList)
    {
      auto json = Json(initializer_list<JsonBuilder>());
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(0i64, json.Size());

      json = Json(initializer_list<JsonBuilder>{ nullptr });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(nullptr, json[0]);

      json = Json(initializer_list<JsonBuilder>{ L"Test"s });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(L"Test"s, json[0]);

      json = Json(initializer_list<JsonBuilder>{ true });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(true, json[0]);

      json = Json(initializer_list<JsonBuilder>{ 13.37 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(13.37, json[0]);

      json = Json(initializer_list<JsonBuilder>{ 1337 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(1337, json[0]);

      json = Json(initializer_list<JsonBuilder>{ JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } } });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(JsonObject{ { L"Key1"s, 1 }, { L"Key2"s, 2 } }, json[0]);

      json = Json(initializer_list<JsonBuilder>{ JsonArray{ 1, 3, 3, 7 } });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(1i64, json.Size());
      Assert::AreEqual<Json>(JsonArray{ 1, 3, 3, 7 }, json[0]);

      json = Json(initializer_list<JsonBuilder>{ 1, 2 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[0]);
      Assert::AreEqual<Json>(2, json[1]);

      json = Json(initializer_list<JsonBuilder>{ L"Key1"s, 1 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json[0]);
      Assert::AreEqual<Json>(1, json[1]);

      json = Json(initializer_list<JsonBuilder>{ L"Key1"s, 1, L"Key2"s });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(3i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json[0]);
      Assert::AreEqual<Json>(1, json[1]);
      Assert::AreEqual<Json>(L"Key2"s, json[2]);

      json = Json(initializer_list<JsonBuilder>{ L"Key1"s, 1, L"Key2"s, 2 });
      Assert::IsTrue(json.Is(JsonType::Array));
      Assert::AreEqual(4i64, json.Size());
      Assert::AreEqual<Json>(L"Key1"s, json[0]);
      Assert::AreEqual<Json>(1, json[1]);
      Assert::AreEqual<Json>(L"Key2"s, json[2]);
      Assert::AreEqual<Json>(2, json[3]);

      json = Json(initializer_list<JsonBuilder>{ initializer_list<JsonBuilder>{ L"Key1"s, 1 }, initializer_list<JsonBuilder>{ L"Key2"s, 2 } });
      Assert::IsTrue(json.Is(JsonType::Object));
      Assert::AreEqual(2i64, json.Size());
      Assert::AreEqual<Json>(1, json[L"Key1"s]);
      Assert::AreEqual<Json>(2, json[L"Key2"s]);
    }

    TEST_METHOD(TestOperatorInsertion)
    {
      auto pairs = vector<pair<vector<variant<Json, JsonIndentSize, JsonIndentChar>>, wstring>>
      {
        { { Json(nullptr ) }, L"null"s     },
        { { Json(L"Test"s) }, L"\"Test\""s },
        { { Json(true    ) }, L"true"s     },
        { { Json(false   ) }, L"false"s    },
        { { Json(13.37   ) }, L"13.37"s    },
        { { Json(1337    ) }, L"1337"s     },
        { { Json{ { L"Key1"s, 1 }, { L"Key2"s, 2 } } },
          L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}"s },
        { { Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]"s },
        { { Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentSize(1), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n 1,\r\n 3,\r\n 3,\r\n 7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentChar(L'\t'), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t\t1,\r\n\t\t3,\r\n\t\t3,\r\n\t\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentSize(1), JsonIndentChar(L'\t'), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t1,\r\n\t3,\r\n\t3,\r\n\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" },
        { { Json{ 1, 3, 3, 7 }, JsonIndentChar(L'\t'), JsonIndentSize(1), Json{ 1, 3, 3, 7 }, Json{ 1, 3, 3, 7 } },
          L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n][\r\n\t1,\r\n\t3,\r\n\t3,\r\n\t7\r\n][\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]" }
      };
      for (auto& [inputs, expected] : pairs)
      {
        wstringstream is;
        for (auto& input : inputs)
        {
          visit(Overload{
            [&](auto const& v) { is << v; }
          }, input);
        }
        Assert::AreEqual(expected, is.str());
      }
    }

    TEST_METHOD(TestOperatorExtraction)
    {
      auto pairs = vector<pair<wstring, Json>>
      {
        { L"[null]"s    , Json{nullptr } },
        { L"[\"Test\"]"s, Json{L"Test"s} },
        { L"[true]"s    , Json{true    } },
        { L"[false]"s   , Json{false   } },
        { L"[13.37]"s   , Json{13.37   } },
        { L"[1337]"s    , Json{1337    } },
        { L"{\r\n  \"Key1\": 1,\r\n  \"Key2\": 2\r\n}"s, Json{ { L"Key1"s, 1 }, { L"Key2"s, 2 } } },
        { L"[\r\n  1,\r\n  3,\r\n  3,\r\n  7\r\n]"s, Json{ 1, 3, 3, 7 } }
      };
      for (auto& [input, expected] : pairs)
      {
        wstringstream is;
        is.str(input);
        Json output;
        is >> output;
        Assert::AreEqual(expected, output);
      }
    }

    //http://json.org/JSON_checker/
    TEST_METHOD(TestFail)
    {
      auto pairs = vector<pair<string, string>>
      {
        { "fail01.json"s, "Invalid token: String, with invalid data: \"A JSON payload should be an object or array, not a string.\"!"s },
        { "fail02.json"s, "Expected ',' or ']' at position Line: 1 Column: 18!"s },
        { "fail03.json"s, "Expected '\"' at position Line: 1 Column: 2!"s },
        { "fail04.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 16!"s },
        { "fail05.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 23!"s },
        { "fail06.json"s, "Unexpected ',' at position Line: 1 Column: 5!"s },
        { "fail07.json"s, "Unexpected ',' at position Line: 1 Column: 26!"s },
        { "fail08.json"s, "Unexpected ']' at position Line: 1 Column: 16!"s },
        { "fail09.json"s, "Expected '\"' at position Line: 1 Column: 22!"s },
        { "fail10.json"s, "Unexpected '\"' at position Line: 1 Column: 35!"s },
        { "fail11.json"s, "Expected ',' or '}' at position Line: 1 Column: 25!"s },
        { "fail12.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 24!"s },
        { "fail13.json"s, "Unexpected '0' at position Line: 1 Column: 40!"s },
        { "fail14.json"s, "Unexpected '0' at position Line: 1 Column: 27!"s },
        { "fail15.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 30!"s },
        { "fail16.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 2!"s },
        { "fail17.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 30!"s },
        { "fail18.json"s, "Depth is greater or equal to the maximum 20!"s },
        { "fail19.json"s, "Expected ':' at position Line: 1 Column: 18!"s },
        { "fail20.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 17!"s },
        { "fail21.json"s, "Expected ':' at position Line: 1 Column: 26!"s },
        { "fail22.json"s, "Expected ',' or ']' at position Line: 1 Column: 26!"s },
        { "fail23.json"s, "Expected 'true' at position Line: 1 Column: 15!"s },
        { "fail24.json"s, "Expected one of the following characters: 'n', '\"', 't', 'f', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '{' or '[' at position Line: 1 Column: 2!"s },
        { "fail25.json"s, "Invalid character found at position Line: 1 Column: 3!"s },
        { "fail26.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 7!"s },
        { "fail27.json"s, "Invalid character found at position Line: 1 Column: 7!"s },
        { "fail28.json"s, "Expected one of the following characters: '\"', '\\', '/', 'b', 'f', 'n', 'r', 't' or 'u' at position Line: 1 Column: 8!"s },
        { "fail29.json"s, "Expected digit at position Line: 1 Column: 4!"s },
        { "fail30.json"s, "Expected digit at position Line: 1 Column: 5!"s },
        { "fail31.json"s, "Expected digit at position Line: 1 Column: 5!"s },
        { "fail32.json"s, "Expected '\"' at position Line: 1 Column: 41!"s },
        { "fail33.json"s, "Expected ',' or ']' at position Line: 1 Column: 12!"s }
      };
      for (auto& [input, expected] : pairs)
      {
        ExceptException<exception>([&, input = input]() { Json::Read(input); }, expected);
      }
    }

    //http://json.org/JSON_checker/
    TEST_METHOD(TestPass)
    {
      auto input = Json::Read(L"pass01.json");
      
      Assert::IsTrue(input.Is(JsonType::Array));
      Assert::AreEqual(20i64, input.Size());

      Assert::AreEqual<Json>(L"JSON Test Pattern pass1"s, input[0]);

      Assert::IsTrue(input[1].Is(JsonType::Object));
      Assert::AreEqual(1i64, input[1].Size());
      Assert::IsTrue(input[1][L"object with 1 member"].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[1][L"object with 1 member"].Size());
      Assert::AreEqual<Json>(L"array with 1 element", input[1][L"object with 1 member"][0]);

      Assert::IsTrue(input[2].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[2].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[2]);

      Assert::IsTrue(input[3].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[3].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[3]);

      Assert::AreEqual<Json>(-42, input[4]);

      Assert::AreEqual<Json>(true, input[5]);

      Assert::AreEqual<Json>(false, input[6]);

      Assert::AreEqual<Json>(nullptr, input[7]);

      Assert::IsTrue(input[8].Is(JsonType::Object));
      Assert::AreEqual(32i64, input[8].Size());
      Assert::AreEqual<Json>(1234567890, input[8][L"integer"]);
      Assert::AreEqual<Json>(-9876.543210, input[8][L"real"]);
      Assert::AreEqual<Json>(0.123456789e-12, input[8][L"e"]);
      Assert::AreEqual<Json>(1.234567890E+34, input[8][L"E"]);
      Assert::AreEqual<Json>(23456789012E66, input[8][L""]);
      Assert::AreEqual<Json>(0, input[8][L"zero"]);
      Assert::AreEqual<Json>(1, input[8][L"one"]);
      Assert::AreEqual<Json>(L" "s, input[8][L"space"]);
      Assert::AreEqual<Json>(L"\""s, input[8][L"quote"]);
      Assert::AreEqual<Json>(L"\\"s, input[8][L"backslash"]);
      Assert::AreEqual<Json>(L"\b\f\n\r\t"s, input[8][L"controls"]);
      Assert::AreEqual<Json>(L"/ & /"s, input[8][L"slash"]);
      Assert::AreEqual<Json>(L"abcdefghijklmnopqrstuvwyz"s, input[8][L"alpha"]);
      Assert::AreEqual<Json>(L"ABCDEFGHIJKLMNOPQRSTUVWYZ"s, input[8][L"ALPHA"]);
      Assert::AreEqual<Json>(L"0123456789"s, input[8][L"digit"]);
      Assert::AreEqual<Json>(L"digit"s, input[8][L"0123456789"]);
      Assert::AreEqual<Json>(L"`1~!@#$%^&*()_+-={':[,]}|;.</>?"s, input[8][L"special"]);
      Assert::AreEqual<Json>(L"\u0123\u4567\u89AB\uCDEF\uabcd\uef4A"s, input[8][L"hex"]);
      Assert::AreEqual<Json>(true, input[8][L"true"]);
      Assert::AreEqual<Json>(false, input[8][L"false"]);
      Assert::AreEqual<Json>(nullptr, input[8][L"null"]);
      Assert::IsTrue(input[8][L"array"].Is(JsonType::Array));
      Assert::AreEqual(0i64, input[8][L"array"].Size());
      Assert::AreEqual<Json>(JsonArray{}, input[8][L"array"]);
      Assert::IsTrue(input[8][L"object"].Is(JsonType::Object));
      Assert::AreEqual(0i64, input[8][L"object"].Size());
      Assert::AreEqual<Json>(JsonObject{}, input[8][L"object"]);
      Assert::AreEqual<Json>(L"50 St. James Street", input[8][L"address"]);
      Assert::AreEqual<Json>(L"http://www.JSON.org/", input[8][L"url"]);
      Assert::AreEqual<Json>(L"// /* <!-- --", input[8][L"comment"]);
      Assert::AreEqual<Json>(L" ", input[8][L"# -- --> */"]);
      Assert::IsTrue(input[8][L" s p a c e d "].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L" s p a c e d "].Size());
      Assert::AreEqual<Json>(1, input[8][L" s p a c e d "][0]);
      Assert::AreEqual<Json>(2, input[8][L" s p a c e d "][1]);
      Assert::AreEqual<Json>(3, input[8][L" s p a c e d "][2]);
      Assert::AreEqual<Json>(4, input[8][L" s p a c e d "][3]);
      Assert::AreEqual<Json>(5, input[8][L" s p a c e d "][4]);
      Assert::AreEqual<Json>(6, input[8][L" s p a c e d "][5]);
      Assert::AreEqual<Json>(7, input[8][L" s p a c e d "][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L" s p a c e d "]);
      Assert::IsTrue(input[8][L"compact"].Is(JsonType::Array));
      Assert::AreEqual(7i64, input[8][L"compact"].Size());
      Assert::AreEqual<Json>(1, input[8][L"compact"][0]);
      Assert::AreEqual<Json>(2, input[8][L"compact"][1]);
      Assert::AreEqual<Json>(3, input[8][L"compact"][2]);
      Assert::AreEqual<Json>(4, input[8][L"compact"][3]);
      Assert::AreEqual<Json>(5, input[8][L"compact"][4]);
      Assert::AreEqual<Json>(6, input[8][L"compact"][5]);
      Assert::AreEqual<Json>(7, input[8][L"compact"][6]);
      Assert::AreEqual<Json>({ 1, 2, 3, 4, 5, 6, 7 }, input[8][L"compact"]);
      Assert::AreEqual<Json>(L"{\"object with 1 member\":[\"array with 1 element\"]}", input[8][L"jsontext"]);
      Assert::AreEqual<Json>(L"&#34; \u0022 %22 0x22 034 &#x22;", input[8][L"quotes"]);
      Assert::AreEqual<Json>(L"A key can be any string", input[8][L"/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"]);

      Assert::AreEqual<Json>(0.5, input[9]);

      Assert::AreEqual<Json>(98.6, input[10]);

      Assert::AreEqual<Json>(99.44, input[11]);

      Assert::AreEqual<Json>(1066, input[12]);

      Assert::AreEqual<Json>(1e1, input[13]);

      Assert::AreEqual<Json>(0.1e1, input[14]);

      Assert::AreEqual<Json>(1e-1, input[15]);

      Assert::AreEqual<Json>(1e00, input[16]);

      Assert::AreEqual<Json>(2e+00, input[17]);

      Assert::AreEqual<Json>(2e-00, input[18]);

      Assert::AreEqual<Json>(L"rosebud", input[19]);


      input = Json::Read(L"pass02.json");
      Assert::IsTrue(        input.Is(JsonType::Array));
      Assert::AreEqual(1i64, input.Size());
      Assert::IsTrue(        input[0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0].Size());
      Assert::IsTrue(        input[0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0].Size());
      Assert::IsTrue(        input[0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(        input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::Array));
      Assert::AreEqual(1i64, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Size());
      Assert::IsTrue(                          input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0].Is(JsonType::String));
      Assert::AreEqual<Json>(L"Not too deep"s, input[0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]);

      input = Json::Read(L"pass03.json");
      Assert::IsTrue(input.Is(JsonType::Object));
      Assert::AreEqual(1i64, input.Size());
      Assert::AreEqual<size_t>(1i64, input.Keys().size());
      Assert::AreEqual(L"JSON Test Pattern pass3"s, input.Keys()[0]);
      Assert::IsTrue(input[L"JSON Test Pattern pass3"s].Is(JsonType::Object));
      Assert::AreEqual(2i64, input[L"JSON Test Pattern pass3"s].Size());
      Assert::AreEqual<size_t>(2i64, input[L"JSON Test Pattern pass3"s].Keys().size());
      Assert::AreEqual(L"The outermost value"s, input[L"JSON Test Pattern pass3"s].Keys()[0]);
      Assert::AreEqual(L"In this test"s, input[L"JSON Test Pattern pass3"s].Keys()[1]);
      Assert::AreEqual<Json>(L"must be an object or array."s, input[L"JSON Test Pattern pass3"s][L"The outermost value"s]);
      Assert::AreEqual<Json>(L"It is an object."s, input[L"JSON Test Pattern pass3"s][L"In this test"s]);
    }

    //http://json.org/JSON_checker/
    TEST_METHOD(TestRoundtrip)
    {
      for (int i = 1; i <= 27; ++i)
      {
        wstringstream ss;
        ss << setw(2) << setfill(L'0') << i;
        auto const index = ss.str();
        auto origPath = L"roundtrip"s + index + L".json"s;
        auto newPath = L"roundtrip"s + index + L"_new.json"s;

        Json::Read(origPath).Write(newPath);
        Assert::AreEqual(Json::Read(origPath), Json::Read(newPath));
      }
    }

    TEST_METHOD(TestOperatorEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

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
      Assert::IsFalse(boolean1 == array1  );
      Assert::IsFalse(boolean1 == array2  );
      Assert::IsTrue (boolean1 == boolean1);
      Assert::IsFalse(boolean1 == boolean2);
      Assert::IsFalse(boolean1 == null    );
      Assert::IsTrue (boolean1 == number1 );
      Assert::IsFalse(boolean1 == number2 );
      Assert::IsTrue (boolean1 == numberi1);
      Assert::IsFalse(boolean1 == numberi2);
      Assert::IsFalse(boolean1 == object1 );
      Assert::IsFalse(boolean1 == object2 );
      Assert::IsFalse(boolean1 == string1 );
      Assert::IsFalse(boolean1 == string2 );
      Assert::IsFalse(boolean2 == array1  );
      Assert::IsFalse(boolean2 == array2  );
      Assert::IsFalse(boolean2 == boolean1);
      Assert::IsTrue (boolean2 == boolean2);
      Assert::IsFalse(boolean2 == null    );
      Assert::IsFalse(boolean2 == number1 );
      Assert::IsTrue (boolean2 == number2 );
      Assert::IsFalse(boolean2 == numberi1);
      Assert::IsTrue (boolean2 == numberi2);
      Assert::IsFalse(boolean2 == object1 );
      Assert::IsFalse(boolean2 == object2 );
      Assert::IsFalse(boolean2 == string1 );
      Assert::IsFalse(boolean2 == string2 );
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
      Assert::IsFalse(number1  == array1  );
      Assert::IsFalse(number1  == array2  );
      Assert::IsTrue (number1  == boolean1);
      Assert::IsFalse(number1  == boolean2);
      Assert::IsFalse(number1  == null    );
      Assert::IsTrue (number1  == number1 );
      Assert::IsFalse(number1  == number2 );
      Assert::IsTrue (number1  == numberi1);
      Assert::IsFalse(number1  == numberi2);
      Assert::IsFalse(number1  == object1 );
      Assert::IsFalse(number1  == object2 );
      Assert::IsFalse(number1  == string1 );
      Assert::IsFalse(number1  == string2 );
      Assert::IsFalse(number2  == array1  );
      Assert::IsFalse(number2  == array2  );
      Assert::IsFalse(number2  == boolean1);
      Assert::IsTrue (number2  == boolean2);
      Assert::IsFalse(number2  == null    );
      Assert::IsFalse(number2  == number1 );
      Assert::IsTrue (number2  == number2 );
      Assert::IsFalse(number2  == numberi1);
      Assert::IsTrue (number2  == numberi2);
      Assert::IsFalse(number2  == object1 );
      Assert::IsFalse(number2  == object2 );
      Assert::IsFalse(number2  == string1 );
      Assert::IsFalse(number2  == string2 );
      Assert::IsFalse(numberi1 == array1  );
      Assert::IsFalse(numberi1 == array2  );
      Assert::IsTrue (numberi1 == boolean1);
      Assert::IsFalse(numberi1 == boolean2);
      Assert::IsFalse(numberi1 == null    );
      Assert::IsTrue (numberi1 == number1 );
      Assert::IsFalse(numberi1 == number2 );
      Assert::IsTrue (numberi1 == numberi1);
      Assert::IsFalse(numberi1 == numberi2);
      Assert::IsFalse(numberi1 == object1 );
      Assert::IsFalse(numberi1 == object2 );
      Assert::IsFalse(numberi1 == string1 );
      Assert::IsFalse(numberi1 == string2 );
      Assert::IsFalse(numberi2 == array1  );
      Assert::IsFalse(numberi2 == array2  );
      Assert::IsFalse(numberi2 == boolean1);
      Assert::IsTrue (numberi2 == boolean2);
      Assert::IsFalse(numberi2 == null    );
      Assert::IsFalse(numberi2 == number1 );
      Assert::IsTrue (numberi2 == number2 );
      Assert::IsFalse(numberi2 == numberi1);
      Assert::IsTrue (numberi2 == numberi2);
      Assert::IsFalse(numberi2 == object1 );
      Assert::IsFalse(numberi2 == object2 );
      Assert::IsFalse(numberi2 == string1 );
      Assert::IsFalse(numberi2 == string2 );
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
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

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
      Assert::IsTrue (boolean1 != array1  );
      Assert::IsTrue (boolean1 != array2  );
      Assert::IsFalse(boolean1 != boolean1);
      Assert::IsTrue (boolean1 != boolean2);
      Assert::IsTrue (boolean1 != null    );
      Assert::IsFalse(boolean1 != number1 );
      Assert::IsTrue (boolean1 != number2 );
      Assert::IsFalse(boolean1 != numberi1);
      Assert::IsTrue (boolean1 != numberi2);
      Assert::IsTrue (boolean1 != object1 );
      Assert::IsTrue (boolean1 != object2 );
      Assert::IsTrue (boolean1 != string1 );
      Assert::IsTrue (boolean1 != string2 );
      Assert::IsTrue (boolean2 != array1  );
      Assert::IsTrue (boolean2 != array2  );
      Assert::IsTrue (boolean2 != boolean1);
      Assert::IsFalse(boolean2 != boolean2);
      Assert::IsTrue (boolean2 != null    );
      Assert::IsTrue (boolean2 != number1 );
      Assert::IsFalse(boolean2 != number2 );
      Assert::IsTrue (boolean2 != numberi1);
      Assert::IsFalse(boolean2 != numberi2);
      Assert::IsTrue (boolean2 != object1 );
      Assert::IsTrue (boolean2 != object2 );
      Assert::IsTrue (boolean2 != string1 );
      Assert::IsTrue (boolean2 != string2 );
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
      Assert::IsTrue (number1  != array1  );
      Assert::IsTrue (number1  != array2  );
      Assert::IsFalse(number1  != boolean1);
      Assert::IsTrue (number1  != boolean2);
      Assert::IsTrue (number1  != null    );
      Assert::IsFalse(number1  != number1 );
      Assert::IsTrue (number1  != number2 );
      Assert::IsFalse(number1  != numberi1);
      Assert::IsTrue (number1  != numberi2);
      Assert::IsTrue (number1  != object1 );
      Assert::IsTrue (number1  != object2 );
      Assert::IsTrue (number1  != string1 );
      Assert::IsTrue (number1  != string2 );
      Assert::IsTrue (number2  != array1  );
      Assert::IsTrue (number2  != array2  );
      Assert::IsTrue (number2  != boolean1);
      Assert::IsFalse(number2  != boolean2);
      Assert::IsTrue (number2  != null    );
      Assert::IsTrue (number2  != number1 );
      Assert::IsFalse(number2  != number2 );
      Assert::IsTrue (number2  != numberi1);
      Assert::IsFalse(number2  != numberi2);
      Assert::IsTrue (number2  != object1 );
      Assert::IsTrue (number2  != object2 );
      Assert::IsTrue (number2  != string1 );
      Assert::IsTrue (number2  != string2 );
      Assert::IsTrue (numberi1 != array1  );
      Assert::IsTrue (numberi1 != array2  );
      Assert::IsFalse(numberi1 != boolean1);
      Assert::IsTrue (numberi1 != boolean2);
      Assert::IsTrue (numberi1 != null    );
      Assert::IsFalse(numberi1 != number1 );
      Assert::IsTrue (numberi1 != number2 );
      Assert::IsFalse(numberi1 != numberi1);
      Assert::IsTrue (numberi1 != numberi2);
      Assert::IsTrue (numberi1 != object1 );
      Assert::IsTrue (numberi1 != object2 );
      Assert::IsTrue (numberi1 != string1 );
      Assert::IsTrue (numberi1 != string2 );
      Assert::IsTrue (numberi2 != array1  );
      Assert::IsTrue (numberi2 != array2  );
      Assert::IsTrue (numberi2 != boolean1);
      Assert::IsFalse(numberi2 != boolean2);
      Assert::IsTrue (numberi2 != null    );
      Assert::IsTrue (numberi2 != number1 );
      Assert::IsFalse(numberi2 != number2 );
      Assert::IsTrue (numberi2 != numberi1);
      Assert::IsFalse(numberi2 != numberi2);
      Assert::IsTrue (numberi2 != object1 );
      Assert::IsTrue (numberi2 != object2 );
      Assert::IsTrue (numberi2 != string1 );
      Assert::IsTrue (numberi2 != string2 );
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
    }

    TEST_METHOD(TestOperatorLessThan)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   < array1  ; });
      Assert::ExpectException<exception>([&] { array1   < array2  ; });
      Assert::ExpectException<exception>([&] { array1   < boolean1; });
      Assert::ExpectException<exception>([&] { array1   < boolean2; });
      Assert::IsTrue (array1   < null    );
      Assert::ExpectException<exception>([&] { array1   < number1 ; });
      Assert::ExpectException<exception>([&] { array1   < number2 ; });
      Assert::ExpectException<exception>([&] { array1   < numberi1; });
      Assert::ExpectException<exception>([&] { array1   < numberi2; });
      Assert::ExpectException<exception>([&] { array1   < object1 ; });
      Assert::ExpectException<exception>([&] { array1   < object2 ; });
      Assert::ExpectException<exception>([&] { array1   < string1 ; });
      Assert::ExpectException<exception>([&] { array1   < string2 ; });
      Assert::ExpectException<exception>([&] { array2   < array1  ; });
      Assert::ExpectException<exception>([&] { array2   < array2  ; });
      Assert::ExpectException<exception>([&] { array2   < boolean1; });
      Assert::ExpectException<exception>([&] { array2   < boolean2; });
      Assert::IsTrue (array2   < null    );
      Assert::ExpectException<exception>([&] { array2   < number1 ; });
      Assert::ExpectException<exception>([&] { array2   < number2 ; });
      Assert::ExpectException<exception>([&] { array2   < numberi1; });
      Assert::ExpectException<exception>([&] { array2   < numberi2; });
      Assert::ExpectException<exception>([&] { array2   < object1 ; });
      Assert::ExpectException<exception>([&] { array2   < object2 ; });
      Assert::ExpectException<exception>([&] { array2   < string1 ; });
      Assert::ExpectException<exception>([&] { array2   < string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 < array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 < array2  ; });
      Assert::IsFalse(boolean1 < boolean1);
      Assert::IsTrue (boolean1 < boolean2);
      Assert::IsTrue (boolean1 < null    );
      Assert::IsFalse(boolean1 < number1 );
      Assert::IsTrue (boolean1 < number2 );
      Assert::IsFalse(boolean1 < numberi1);
      Assert::IsTrue (boolean1 < numberi2);
      Assert::ExpectException<exception>([&] { boolean1 < object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 < object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 < string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 < string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 < array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 < array2  ; });
      Assert::IsFalse(boolean2 < boolean1);
      Assert::IsFalse(boolean2 < boolean2);
      Assert::IsTrue (boolean2 < null    );
      Assert::IsFalse(boolean2 < number1 );
      Assert::IsFalse(boolean2 < number2 );
      Assert::IsFalse(boolean2 < numberi1);
      Assert::IsFalse(boolean2 < numberi2);
      Assert::ExpectException<exception>([&] { boolean2 < object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 < object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 < string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 < string2 ; });
      Assert::IsFalse(null     < array1  );
      Assert::IsFalse(null     < array2  );
      Assert::IsFalse(null     < boolean1);
      Assert::IsFalse(null     < boolean2);
      Assert::IsFalse(null     < null    );
      Assert::IsFalse(null     < number1 );
      Assert::IsFalse(null     < number2 );
      Assert::IsFalse(null     < numberi1);
      Assert::IsFalse(null     < numberi2);
      Assert::IsFalse(null     < object1 );
      Assert::IsFalse(null     < object2 );
      Assert::IsFalse(null     < string1 );
      Assert::IsFalse(null     < string2 );
      Assert::ExpectException<exception>([&] { number1  < array1  ; });
      Assert::ExpectException<exception>([&] { number1  < array2  ; });
      Assert::IsFalse(number1  < boolean1);
      Assert::IsTrue (number1  < boolean2);
      Assert::IsTrue (number1  < null    );
      Assert::IsFalse(number1  < number1 );
      Assert::IsTrue (number1  < number2 );
      Assert::IsFalse(number1  < numberi1);
      Assert::IsTrue (number1  < numberi2);
      Assert::ExpectException<exception>([&] { number1  < object1 ; });
      Assert::ExpectException<exception>([&] { number1  < object2 ; });
      Assert::ExpectException<exception>([&] { number1  < string1 ; });
      Assert::ExpectException<exception>([&] { number1  < string2 ; });
      Assert::ExpectException<exception>([&] { number2  < array1  ; });
      Assert::ExpectException<exception>([&] { number2  < array2  ; });
      Assert::IsFalse(number2  < boolean1);
      Assert::IsFalse(number2  < boolean2);
      Assert::IsTrue (number2  < null    );
      Assert::IsFalse(number2  < number1 );
      Assert::IsFalse(number2  < number2 );
      Assert::IsFalse(number2  < numberi1);
      Assert::IsFalse(number2  < numberi2);
      Assert::ExpectException<exception>([&] { number2  < object1 ; });
      Assert::ExpectException<exception>([&] { number2  < object2 ; });
      Assert::ExpectException<exception>([&] { number2  < string1 ; });
      Assert::ExpectException<exception>([&] { number2  < string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 < array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 < array2  ; });
      Assert::IsFalse(numberi1 < boolean1);
      Assert::IsTrue (numberi1 < boolean2);
      Assert::IsTrue (numberi1 < null    );
      Assert::IsFalse(numberi1 < number1 );
      Assert::IsTrue (numberi1 < number2 );
      Assert::IsFalse(numberi1 < numberi1);
      Assert::IsTrue (numberi1 < numberi2);
      Assert::ExpectException<exception>([&] { numberi1 < object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 < object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 < string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 < string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 < array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 < array2  ; });
      Assert::IsFalse(numberi2 < boolean1);
      Assert::IsFalse(numberi2 < boolean2);
      Assert::IsTrue (numberi2 < null    );
      Assert::IsFalse(numberi2 < number1 );
      Assert::IsFalse(numberi2 < number2 );
      Assert::IsFalse(numberi2 < numberi1);
      Assert::IsFalse(numberi2 < numberi2);
      Assert::ExpectException<exception>([&] { numberi2 < object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 < object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 < string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 < string2 ; });
      Assert::ExpectException<exception>([&] { object1  < array1  ; });
      Assert::ExpectException<exception>([&] { object1  < array2  ; });
      Assert::ExpectException<exception>([&] { object1  < boolean1; });
      Assert::ExpectException<exception>([&] { object1  < boolean2; });
      Assert::IsTrue (object1  < null    );
      Assert::ExpectException<exception>([&] { object1  < number1 ; });
      Assert::ExpectException<exception>([&] { object1  < number2 ; });
      Assert::ExpectException<exception>([&] { object1  < numberi1; });
      Assert::ExpectException<exception>([&] { object1  < numberi2; });
      Assert::ExpectException<exception>([&] { object1  < object1 ; });
      Assert::ExpectException<exception>([&] { object1  < object2 ; });
      Assert::ExpectException<exception>([&] { object1  < string1 ; });
      Assert::ExpectException<exception>([&] { object1  < string2 ; });
      Assert::ExpectException<exception>([&] { object2  < array1  ; });
      Assert::ExpectException<exception>([&] { object2  < array2  ; });
      Assert::ExpectException<exception>([&] { object2  < boolean1; });
      Assert::ExpectException<exception>([&] { object2  < boolean2; });
      Assert::IsTrue (object2  < null    );
      Assert::ExpectException<exception>([&] { object2  < number1 ; });
      Assert::ExpectException<exception>([&] { object2  < number2 ; });
      Assert::ExpectException<exception>([&] { object2  < numberi1; });
      Assert::ExpectException<exception>([&] { object2  < numberi2; });
      Assert::ExpectException<exception>([&] { object2  < object1 ; });
      Assert::ExpectException<exception>([&] { object2  < object2 ; });
      Assert::ExpectException<exception>([&] { object2  < string1 ; });
      Assert::ExpectException<exception>([&] { object2  < string2 ; });
      Assert::ExpectException<exception>([&] { string1  < array1  ; });
      Assert::ExpectException<exception>([&] { string1  < array2  ; });
      Assert::ExpectException<exception>([&] { string1  < boolean1; });
      Assert::ExpectException<exception>([&] { string1  < boolean2; });
      Assert::IsTrue (string1  < null    );
      Assert::ExpectException<exception>([&] { string1  < number1 ; });
      Assert::ExpectException<exception>([&] { string1  < number2 ; });
      Assert::ExpectException<exception>([&] { string1  < numberi1; });
      Assert::ExpectException<exception>([&] { string1  < numberi2; });
      Assert::ExpectException<exception>([&] { string1  < object1 ; });
      Assert::ExpectException<exception>([&] { string1  < object2 ; });
      Assert::IsFalse(string1  < string1 );
      Assert::IsTrue (string1  < string2 );
      Assert::ExpectException<exception>([&] { string2  < array1  ; });
      Assert::ExpectException<exception>([&] { string2  < array2  ; });
      Assert::ExpectException<exception>([&] { string2  < boolean1; });
      Assert::ExpectException<exception>([&] { string2  < boolean2; });
      Assert::IsTrue (string2  < null    );
      Assert::ExpectException<exception>([&] { string2  < number1 ; });
      Assert::ExpectException<exception>([&] { string2  < number2 ; });
      Assert::ExpectException<exception>([&] { string2  < numberi1; });
      Assert::ExpectException<exception>([&] { string2  < numberi2; });
      Assert::ExpectException<exception>([&] { string2  < object1 ; });
      Assert::ExpectException<exception>([&] { string2  < object2 ; });
      Assert::IsFalse(string2  < string1 );
      Assert::IsFalse(string2  < string2 );
    }

    TEST_METHOD(TestOperatorLessThanOrEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   <= array1  ; });
      Assert::ExpectException<exception>([&] { array1   <= array2  ; });
      Assert::ExpectException<exception>([&] { array1   <= boolean1; });
      Assert::ExpectException<exception>([&] { array1   <= boolean2; });
      Assert::IsTrue (array1   <= null    );
      Assert::ExpectException<exception>([&] { array1   <= number1 ; });
      Assert::ExpectException<exception>([&] { array1   <= number2 ; });
      Assert::ExpectException<exception>([&] { array1   <= numberi1; });
      Assert::ExpectException<exception>([&] { array1   <= numberi2; });
      Assert::ExpectException<exception>([&] { array1   <= object1 ; });
      Assert::ExpectException<exception>([&] { array1   <= object2 ; });
      Assert::ExpectException<exception>([&] { array1   <= string1 ; });
      Assert::ExpectException<exception>([&] { array1   <= string2 ; });
      Assert::ExpectException<exception>([&] { array2   <= array1  ; });
      Assert::ExpectException<exception>([&] { array2   <= array2  ; });
      Assert::ExpectException<exception>([&] { array2   <= boolean1; });
      Assert::ExpectException<exception>([&] { array2   <= boolean2; });
      Assert::IsTrue (array2   <= null    );
      Assert::ExpectException<exception>([&] { array2   <= number1 ; });
      Assert::ExpectException<exception>([&] { array2   <= number2 ; });
      Assert::ExpectException<exception>([&] { array2   <= numberi1; });
      Assert::ExpectException<exception>([&] { array2   <= numberi2; });
      Assert::ExpectException<exception>([&] { array2   <= object1 ; });
      Assert::ExpectException<exception>([&] { array2   <= object2 ; });
      Assert::ExpectException<exception>([&] { array2   <= string1 ; });
      Assert::ExpectException<exception>([&] { array2   <= string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 <= array2  ; });
      Assert::IsTrue (boolean1 <= boolean1);
      Assert::IsTrue (boolean1 <= boolean2);
      Assert::IsTrue (boolean1 <= null    );
      Assert::IsTrue (boolean1 <= number1 );
      Assert::IsTrue (boolean1 <= number2 );
      Assert::IsTrue (boolean1 <= numberi1);
      Assert::IsTrue (boolean1 <= numberi2);
      Assert::ExpectException<exception>([&] { boolean1 <= object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 <= string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 <= array2  ; });
      Assert::IsFalse(boolean2 <= boolean1);
      Assert::IsTrue (boolean2 <= boolean2);
      Assert::IsTrue (boolean2 <= null    );
      Assert::IsFalse(boolean2 <= number1 );
      Assert::IsTrue (boolean2 <= number2 );
      Assert::IsFalse(boolean2 <= numberi1);
      Assert::IsTrue (boolean2 <= numberi2);
      Assert::ExpectException<exception>([&] { boolean2 <= object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 <= string2 ; });
      Assert::IsFalse(null     <= array1  );
      Assert::IsFalse(null     <= array2  );
      Assert::IsFalse(null     <= boolean1);
      Assert::IsFalse(null     <= boolean2);
      Assert::IsTrue (null     <= null    );
      Assert::IsFalse(null     <= number1 );
      Assert::IsFalse(null     <= number2 );
      Assert::IsFalse(null     <= numberi1);
      Assert::IsFalse(null     <= numberi2);
      Assert::IsFalse(null     <= object1 );
      Assert::IsFalse(null     <= object2 );
      Assert::IsFalse(null     <= string1 );
      Assert::IsFalse(null     <= string2 );
      Assert::ExpectException<exception>([&] { number1  <= array1  ; });
      Assert::ExpectException<exception>([&] { number1  <= array2  ; });
      Assert::IsTrue (number1  <= boolean1);
      Assert::IsTrue (number1  <= boolean2);
      Assert::IsTrue (number1  <= null    );
      Assert::IsTrue (number1  <= number1 );
      Assert::IsTrue (number1  <= number2 );
      Assert::IsTrue (number1  <= numberi1);
      Assert::IsTrue (number1  <= numberi2);
      Assert::ExpectException<exception>([&] { number1  <= object1 ; });
      Assert::ExpectException<exception>([&] { number1  <= object2 ; });
      Assert::ExpectException<exception>([&] { number1  <= string1 ; });
      Assert::ExpectException<exception>([&] { number1  <= string2 ; });
      Assert::ExpectException<exception>([&] { number2  <= array1  ; });
      Assert::ExpectException<exception>([&] { number2  <= array2  ; });
      Assert::IsFalse(number2  <= boolean1);
      Assert::IsTrue (number2  <= boolean2);
      Assert::IsTrue (number2  <= null    );
      Assert::IsFalse(number2  <= number1 );
      Assert::IsTrue (number2  <= number2 );
      Assert::IsFalse(number2  <= numberi1);
      Assert::IsTrue (number2  <= numberi2);
      Assert::ExpectException<exception>([&] { number2  <= object1 ; });
      Assert::ExpectException<exception>([&] { number2  <= object2 ; });
      Assert::ExpectException<exception>([&] { number2  <= string1 ; });
      Assert::ExpectException<exception>([&] { number2  <= string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 <= array2  ; });
      Assert::IsTrue (numberi1 <= boolean1);
      Assert::IsTrue (numberi1 <= boolean2);
      Assert::IsTrue (numberi1 <= null    );
      Assert::IsTrue (numberi1 <= number1 );
      Assert::IsTrue (numberi1 <= number2 );
      Assert::IsTrue (numberi1 <= numberi1);
      Assert::IsTrue (numberi1 <= numberi2);
      Assert::ExpectException<exception>([&] { numberi1 <= object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 <= string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 <= array2  ; });
      Assert::IsFalse(numberi2 <= boolean1);
      Assert::IsTrue (numberi2 <= boolean2);
      Assert::IsTrue (numberi2 <= null    );
      Assert::IsFalse(numberi2 <= number1 );
      Assert::IsTrue (numberi2 <= number2 );
      Assert::IsFalse(numberi2 <= numberi1);
      Assert::IsTrue (numberi2 <= numberi2);
      Assert::ExpectException<exception>([&] { numberi2 <= object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 <= string2 ; });
      Assert::ExpectException<exception>([&] { object1  <= array1  ; });
      Assert::ExpectException<exception>([&] { object1  <= array2  ; });
      Assert::ExpectException<exception>([&] { object1  <= boolean1; });
      Assert::ExpectException<exception>([&] { object1  <= boolean2; });
      Assert::IsTrue (object1  <= null    );
      Assert::ExpectException<exception>([&] { object1  <= number1 ; });
      Assert::ExpectException<exception>([&] { object1  <= number2 ; });
      Assert::ExpectException<exception>([&] { object1  <= numberi1; });
      Assert::ExpectException<exception>([&] { object1  <= numberi2; });
      Assert::ExpectException<exception>([&] { object1  <= object1 ; });
      Assert::ExpectException<exception>([&] { object1  <= object2 ; });
      Assert::ExpectException<exception>([&] { object1  <= string1 ; });
      Assert::ExpectException<exception>([&] { object1  <= string2 ; });
      Assert::ExpectException<exception>([&] { object2  <= array1  ; });
      Assert::ExpectException<exception>([&] { object2  <= array2  ; });
      Assert::ExpectException<exception>([&] { object2  <= boolean1; });
      Assert::ExpectException<exception>([&] { object2  <= boolean2; });
      Assert::IsTrue (object2  <= null    );
      Assert::ExpectException<exception>([&] { object2  <= number1 ; });
      Assert::ExpectException<exception>([&] { object2  <= number2 ; });
      Assert::ExpectException<exception>([&] { object2  <= numberi1; });
      Assert::ExpectException<exception>([&] { object2  <= numberi2; });
      Assert::ExpectException<exception>([&] { object2  <= object1 ; });
      Assert::ExpectException<exception>([&] { object2  <= object2 ; });
      Assert::ExpectException<exception>([&] { object2  <= string1 ; });
      Assert::ExpectException<exception>([&] { object2  <= string2 ; });
      Assert::ExpectException<exception>([&] { string1  <= array1  ; });
      Assert::ExpectException<exception>([&] { string1  <= array2  ; });
      Assert::ExpectException<exception>([&] { string1  <= boolean1; });
      Assert::ExpectException<exception>([&] { string1  <= boolean2; });
      Assert::IsTrue (string1  <= null    );
      Assert::ExpectException<exception>([&] { string1  <= number1 ; });
      Assert::ExpectException<exception>([&] { string1  <= number2 ; });
      Assert::ExpectException<exception>([&] { string1  <= numberi1; });
      Assert::ExpectException<exception>([&] { string1  <= numberi2; });
      Assert::ExpectException<exception>([&] { string1  <= object1 ; });
      Assert::ExpectException<exception>([&] { string1  <= object2 ; });
      Assert::IsTrue (string1  <= string1 );
      Assert::IsTrue (string1  <= string2 );
      Assert::ExpectException<exception>([&] { string2  <= array1  ; });
      Assert::ExpectException<exception>([&] { string2  <= array2  ; });
      Assert::ExpectException<exception>([&] { string2  <= boolean1; });
      Assert::ExpectException<exception>([&] { string2  <= boolean2; });
      Assert::IsTrue (string2  <= null    );
      Assert::ExpectException<exception>([&] { string2  <= number1 ; });
      Assert::ExpectException<exception>([&] { string2  <= number2 ; });
      Assert::ExpectException<exception>([&] { string2  <= numberi1; });
      Assert::ExpectException<exception>([&] { string2  <= numberi2; });
      Assert::ExpectException<exception>([&] { string2  <= object1 ; });
      Assert::ExpectException<exception>([&] { string2  <= object2 ; });
      Assert::IsFalse(string2  <= string1 );
      Assert::IsTrue (string2  <= string2 );
    }

    TEST_METHOD(TestOperatorGreaterThan)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   > array1  ; });
      Assert::ExpectException<exception>([&] { array1   > array2  ; });
      Assert::ExpectException<exception>([&] { array1   > boolean1; });
      Assert::ExpectException<exception>([&] { array1   > boolean2; });
      Assert::IsFalse(array1   > null    );
      Assert::ExpectException<exception>([&] { array1   > number1 ; });
      Assert::ExpectException<exception>([&] { array1   > number2 ; });
      Assert::ExpectException<exception>([&] { array1   > numberi1; });
      Assert::ExpectException<exception>([&] { array1   > numberi2; });
      Assert::ExpectException<exception>([&] { array1   > object1 ; });
      Assert::ExpectException<exception>([&] { array1   > object2 ; });
      Assert::ExpectException<exception>([&] { array1   > string1 ; });
      Assert::ExpectException<exception>([&] { array1   > string2 ; });
      Assert::ExpectException<exception>([&] { array2   > array1  ; });
      Assert::ExpectException<exception>([&] { array2   > array2  ; });
      Assert::ExpectException<exception>([&] { array2   > boolean1; });
      Assert::ExpectException<exception>([&] { array2   > boolean2; });
      Assert::IsFalse(array2   > null    );
      Assert::ExpectException<exception>([&] { array2   > number1 ; });
      Assert::ExpectException<exception>([&] { array2   > number2 ; });
      Assert::ExpectException<exception>([&] { array2   > numberi1; });
      Assert::ExpectException<exception>([&] { array2   > numberi2; });
      Assert::ExpectException<exception>([&] { array2   > object1 ; });
      Assert::ExpectException<exception>([&] { array2   > object2 ; });
      Assert::ExpectException<exception>([&] { array2   > string1 ; });
      Assert::ExpectException<exception>([&] { array2   > string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 > array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 > array2  ; });
      Assert::IsFalse(boolean1 > boolean1);
      Assert::IsFalse(boolean1 > boolean2);
      Assert::IsFalse(boolean1 > null    );
      Assert::IsFalse(boolean1 > number1 );
      Assert::IsFalse(boolean1 > number2 );
      Assert::IsFalse(boolean1 > numberi1);
      Assert::IsFalse(boolean1 > numberi2);
      Assert::ExpectException<exception>([&] { boolean1 > object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 > object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 > string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 > string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 > array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 > array2  ; });
      Assert::IsTrue (boolean2 > boolean1);
      Assert::IsFalse(boolean2 > boolean2);
      Assert::IsFalse(boolean2 > null    );
      Assert::IsTrue (boolean2 > number1 );
      Assert::IsFalse(boolean2 > number2 );
      Assert::IsTrue (boolean2 > numberi1);
      Assert::IsFalse(boolean2 > numberi2);
      Assert::ExpectException<exception>([&] { boolean2 > object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 > object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 > string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 > string2 ; });
      Assert::IsTrue (null     > array1  );
      Assert::IsTrue (null     > array2  );
      Assert::IsTrue (null     > boolean1);
      Assert::IsTrue (null     > boolean2);
      Assert::IsFalse(null     > null    );
      Assert::IsTrue (null     > number1 );
      Assert::IsTrue (null     > number2 );
      Assert::IsTrue (null     > numberi1);
      Assert::IsTrue (null     > numberi2);
      Assert::IsTrue (null     > object1 );
      Assert::IsTrue (null     > object2 );
      Assert::IsTrue (null     > string1 );
      Assert::IsTrue (null     > string2 );
      Assert::ExpectException<exception>([&] { number1  > array1  ; });
      Assert::ExpectException<exception>([&] { number1  > array2  ; });
      Assert::IsFalse(number1  > boolean1);
      Assert::IsFalse(number1  > boolean2);
      Assert::IsFalse(number1  > null    );
      Assert::IsFalse(number1  > number1 );
      Assert::IsFalse(number1  > number2 );
      Assert::IsFalse(number1  > numberi1);
      Assert::IsFalse(number1  > numberi2);
      Assert::ExpectException<exception>([&] { number1  > object1 ; });
      Assert::ExpectException<exception>([&] { number1  > object2 ; });
      Assert::ExpectException<exception>([&] { number1  > string1 ; });
      Assert::ExpectException<exception>([&] { number1  > string2 ; });
      Assert::ExpectException<exception>([&] { number2  > array1  ; });
      Assert::ExpectException<exception>([&] { number2  > array2  ; });
      Assert::IsTrue (number2  > boolean1);
      Assert::IsFalse(number2  > boolean2);
      Assert::IsFalse(number2  > null    );
      Assert::IsTrue (number2  > number1 );
      Assert::IsFalse(number2  > number2 );
      Assert::IsTrue (number2  > numberi1);
      Assert::IsFalse(number2  > numberi2);
      Assert::ExpectException<exception>([&] { number2  > object1 ; });
      Assert::ExpectException<exception>([&] { number2  > object2 ; });
      Assert::ExpectException<exception>([&] { number2  > string1 ; });
      Assert::ExpectException<exception>([&] { number2  > string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 > array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 > array2  ; });
      Assert::IsFalse(numberi1 > boolean1);
      Assert::IsFalse(numberi1 > boolean2);
      Assert::IsFalse(numberi1 > null    );
      Assert::IsFalse(numberi1 > number1 );
      Assert::IsFalse(numberi1 > number2 );
      Assert::IsFalse(numberi1 > numberi1);
      Assert::IsFalse(numberi1 > numberi2);
      Assert::ExpectException<exception>([&] { numberi1 > object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 > object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 > string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 > string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 > array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 > array2  ; });
      Assert::IsTrue (numberi2 > boolean1);
      Assert::IsFalse(numberi2 > boolean2);
      Assert::IsFalse(numberi2 > null    );
      Assert::IsTrue (numberi2 > number1 );
      Assert::IsFalse(numberi2 > number2 );
      Assert::IsTrue (numberi2 > numberi1);
      Assert::IsFalse(numberi2 > numberi2);
      Assert::ExpectException<exception>([&] { numberi2 > object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 > object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 > string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 > string2 ; });
      Assert::ExpectException<exception>([&] { object1  > array1  ; });
      Assert::ExpectException<exception>([&] { object1  > array2  ; });
      Assert::ExpectException<exception>([&] { object1  > boolean1; });
      Assert::ExpectException<exception>([&] { object1  > boolean2; });
      Assert::IsFalse(object1  > null    );
      Assert::ExpectException<exception>([&] { object1  > number1 ; });
      Assert::ExpectException<exception>([&] { object1  > number2 ; });
      Assert::ExpectException<exception>([&] { object1  > numberi1; });
      Assert::ExpectException<exception>([&] { object1  > numberi2; });
      Assert::ExpectException<exception>([&] { object1  > object1 ; });
      Assert::ExpectException<exception>([&] { object1  > object2 ; });
      Assert::ExpectException<exception>([&] { object1  > string1 ; });
      Assert::ExpectException<exception>([&] { object1  > string2 ; });
      Assert::ExpectException<exception>([&] { object2  > array1  ; });
      Assert::ExpectException<exception>([&] { object2  > array2  ; });
      Assert::ExpectException<exception>([&] { object2  > boolean1; });
      Assert::ExpectException<exception>([&] { object2  > boolean2; });
      Assert::IsFalse(object2  > null    );
      Assert::ExpectException<exception>([&] { object2  > number1 ; });
      Assert::ExpectException<exception>([&] { object2  > number2 ; });
      Assert::ExpectException<exception>([&] { object2  > numberi1; });
      Assert::ExpectException<exception>([&] { object2  > numberi2; });
      Assert::ExpectException<exception>([&] { object2  > object1 ; });
      Assert::ExpectException<exception>([&] { object2  > object2 ; });
      Assert::ExpectException<exception>([&] { object2  > string1 ; });
      Assert::ExpectException<exception>([&] { object2  > string2 ; });
      Assert::ExpectException<exception>([&] { string1  > array1  ; });
      Assert::ExpectException<exception>([&] { string1  > array2  ; });
      Assert::ExpectException<exception>([&] { string1  > boolean1; });
      Assert::ExpectException<exception>([&] { string1  > boolean2; });
      Assert::IsFalse(string1  > null    );
      Assert::ExpectException<exception>([&] { string1  > number1 ; });
      Assert::ExpectException<exception>([&] { string1  > number2 ; });
      Assert::ExpectException<exception>([&] { string1  > numberi1; });
      Assert::ExpectException<exception>([&] { string1  > numberi2; });
      Assert::ExpectException<exception>([&] { string1  > object1 ; });
      Assert::ExpectException<exception>([&] { string1  > object2 ; });
      Assert::IsFalse(string1  > string1 );
      Assert::IsFalse(string1  > string2 );
      Assert::ExpectException<exception>([&] { string2  > array1  ; });
      Assert::ExpectException<exception>([&] { string2  > array2  ; });
      Assert::ExpectException<exception>([&] { string2  > boolean1; });
      Assert::ExpectException<exception>([&] { string2  > boolean2; });
      Assert::IsFalse(string2  > null    );
      Assert::ExpectException<exception>([&] { string2  > number1 ; });
      Assert::ExpectException<exception>([&] { string2  > number2 ; });
      Assert::ExpectException<exception>([&] { string2  > numberi1; });
      Assert::ExpectException<exception>([&] { string2  > numberi2; });
      Assert::ExpectException<exception>([&] { string2  > object1 ; });
      Assert::ExpectException<exception>([&] { string2  > object2 ; });
      Assert::IsTrue (string2  > string1 );
      Assert::IsFalse(string2  > string2 );
    }

    TEST_METHOD(TestOperatorGreaterThanOrEqual)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   >= array1  ; });
      Assert::ExpectException<exception>([&] { array1   >= array2  ; });
      Assert::ExpectException<exception>([&] { array1   >= boolean1; });
      Assert::ExpectException<exception>([&] { array1   >= boolean2; });
      Assert::IsFalse(array1   >= null    );
      Assert::ExpectException<exception>([&] { array1   >= number1 ; });
      Assert::ExpectException<exception>([&] { array1   >= number2 ; });
      Assert::ExpectException<exception>([&] { array1   >= numberi1; });
      Assert::ExpectException<exception>([&] { array1   >= numberi2; });
      Assert::ExpectException<exception>([&] { array1   >= object1 ; });
      Assert::ExpectException<exception>([&] { array1   >= object2 ; });
      Assert::ExpectException<exception>([&] { array1   >= string1 ; });
      Assert::ExpectException<exception>([&] { array1   >= string2 ; });
      Assert::ExpectException<exception>([&] { array2   >= array1  ; });
      Assert::ExpectException<exception>([&] { array2   >= array2  ; });
      Assert::ExpectException<exception>([&] { array2   >= boolean1; });
      Assert::ExpectException<exception>([&] { array2   >= boolean2; });
      Assert::IsFalse(array2   >= null    );
      Assert::ExpectException<exception>([&] { array2   >= number1 ; });
      Assert::ExpectException<exception>([&] { array2   >= number2 ; });
      Assert::ExpectException<exception>([&] { array2   >= numberi1; });
      Assert::ExpectException<exception>([&] { array2   >= numberi2; });
      Assert::ExpectException<exception>([&] { array2   >= object1 ; });
      Assert::ExpectException<exception>([&] { array2   >= object2 ; });
      Assert::ExpectException<exception>([&] { array2   >= string1 ; });
      Assert::ExpectException<exception>([&] { array2   >= string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 >= array2  ; });
      Assert::IsTrue (boolean1 >= boolean1);
      Assert::IsFalse(boolean1 >= boolean2);
      Assert::IsFalse(boolean1 >= null    );
      Assert::IsTrue (boolean1 >= number1 );
      Assert::IsFalse(boolean1 >= number2 );
      Assert::IsTrue (boolean1 >= numberi1);
      Assert::IsFalse(boolean1 >= numberi2);
      Assert::ExpectException<exception>([&] { boolean1 >= object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 >= string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 >= array2  ; });
      Assert::IsTrue (boolean2 >= boolean1);
      Assert::IsTrue (boolean2 >= boolean2);
      Assert::IsFalse(boolean2 >= null    );
      Assert::IsTrue (boolean2 >= number1 );
      Assert::IsTrue(boolean2 >= number2 );
      Assert::IsTrue (boolean2 >= numberi1 );
      Assert::IsTrue(boolean2 >= numberi2);
      Assert::ExpectException<exception>([&] { boolean2 >= object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 >= string2 ; });
      Assert::IsTrue (null     >= array1  );
      Assert::IsTrue (null     >= array2  );
      Assert::IsTrue (null     >= boolean1);
      Assert::IsTrue (null     >= boolean2);
      Assert::IsTrue (null     >= null    );
      Assert::IsTrue (null     >= number1 );
      Assert::IsTrue (null     >= number2 );
      Assert::IsTrue (null     >= numberi1);
      Assert::IsTrue (null     >= numberi2);
      Assert::IsTrue (null     >= object1 );
      Assert::IsTrue (null     >= object2 );
      Assert::IsTrue (null     >= string1 );
      Assert::IsTrue (null     >= string2 );
      Assert::ExpectException<exception>([&] { number1  >= array1  ; });
      Assert::ExpectException<exception>([&] { number1  >= array2  ; });
      Assert::IsTrue (number1  >= boolean1);
      Assert::IsFalse(number1  >= boolean2);
      Assert::IsFalse(number1  >= null    );
      Assert::IsTrue (number1  >= number1 );
      Assert::IsFalse(number1  >= number2 );
      Assert::IsTrue (number1  >= numberi1);
      Assert::IsFalse(number1  >= numberi2);
      Assert::ExpectException<exception>([&] { number1  >= object1 ; });
      Assert::ExpectException<exception>([&] { number1  >= object2 ; });
      Assert::ExpectException<exception>([&] { number1  >= string1 ; });
      Assert::ExpectException<exception>([&] { number1  >= string2 ; });
      Assert::ExpectException<exception>([&] { number2  >= array1  ; });
      Assert::ExpectException<exception>([&] { number2  >= array2  ; });
      Assert::IsTrue (number2  >= boolean1);
      Assert::IsTrue (number2  >= boolean2);
      Assert::IsFalse(number2  >= null    );
      Assert::IsTrue (number2  >= number1 );
      Assert::IsTrue (number2  >= number2 );
      Assert::IsTrue (number2  >= numberi1);
      Assert::IsTrue (number2  >= numberi2);
      Assert::ExpectException<exception>([&] { number2  >= object1 ; });
      Assert::ExpectException<exception>([&] { number2  >= object2 ; });
      Assert::ExpectException<exception>([&] { number2  >= string1 ; });
      Assert::ExpectException<exception>([&] { number2  >= string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 >= array2  ; });
      Assert::IsTrue (numberi1 >= boolean1);
      Assert::IsFalse(numberi1 >= boolean2);
      Assert::IsFalse(numberi1 >= null    );
      Assert::IsTrue (numberi1 >= number1 );
      Assert::IsFalse(numberi1 >= number2 );
      Assert::IsTrue (numberi1 >= numberi1);
      Assert::IsFalse(numberi1 >= numberi2);
      Assert::ExpectException<exception>([&] { numberi1 >= object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 >= string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 >= array2  ; });
      Assert::IsTrue (numberi2 >= boolean1);
      Assert::IsTrue (numberi2 >= boolean2);
      Assert::IsFalse(numberi2 >= null    );
      Assert::IsTrue (numberi2 >= number1 );
      Assert::IsTrue (numberi2 >= number2 );
      Assert::IsTrue (numberi2 >= numberi1);
      Assert::IsTrue (numberi2 >= numberi2);
      Assert::ExpectException<exception>([&] { numberi2 >= object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 >= string2 ; });
      Assert::ExpectException<exception>([&] { object1  >= array1  ; });
      Assert::ExpectException<exception>([&] { object1  >= array2  ; });
      Assert::ExpectException<exception>([&] { object1  >= boolean1; });
      Assert::ExpectException<exception>([&] { object1  >= boolean2; });
      Assert::IsFalse(object1  >= null    );
      Assert::ExpectException<exception>([&] { object1  >= number1 ; });
      Assert::ExpectException<exception>([&] { object1  >= number2 ; });
      Assert::ExpectException<exception>([&] { object1  >= numberi1; });
      Assert::ExpectException<exception>([&] { object1  >= numberi2; });
      Assert::ExpectException<exception>([&] { object1  >= object1 ; });
      Assert::ExpectException<exception>([&] { object1  >= object2 ; });
      Assert::ExpectException<exception>([&] { object1  >= string1 ; });
      Assert::ExpectException<exception>([&] { object1  >= string2 ; });
      Assert::ExpectException<exception>([&] { object2  >= array1  ; });
      Assert::ExpectException<exception>([&] { object2  >= array2  ; });
      Assert::ExpectException<exception>([&] { object2  >= boolean1; });
      Assert::ExpectException<exception>([&] { object2  >= boolean2; });
      Assert::IsFalse(object2  >= null    );
      Assert::ExpectException<exception>([&] { object2  >= number1 ; });
      Assert::ExpectException<exception>([&] { object2  >= number2 ; });
      Assert::ExpectException<exception>([&] { object2  >= numberi1; });
      Assert::ExpectException<exception>([&] { object2  >= numberi2; });
      Assert::ExpectException<exception>([&] { object2  >= object1 ; });
      Assert::ExpectException<exception>([&] { object2  >= object2 ; });
      Assert::ExpectException<exception>([&] { object2  >= string1 ; });
      Assert::ExpectException<exception>([&] { object2  >= string2 ; });
      Assert::ExpectException<exception>([&] { string1  >= array1  ; });
      Assert::ExpectException<exception>([&] { string1  >= array2  ; });
      Assert::ExpectException<exception>([&] { string1  >= boolean1; });
      Assert::ExpectException<exception>([&] { string1  >= boolean2; });
      Assert::IsFalse(string1  >= null    );
      Assert::ExpectException<exception>([&] { string1  >= number1 ; });
      Assert::ExpectException<exception>([&] { string1  >= number2 ; });
      Assert::ExpectException<exception>([&] { string1  >= numberi1; });
      Assert::ExpectException<exception>([&] { string1  >= numberi2; });
      Assert::ExpectException<exception>([&] { string1  >= object1 ; });
      Assert::ExpectException<exception>([&] { string1  >= object2 ; });
      Assert::IsTrue (string1  >= string1 );
      Assert::IsFalse(string1  >= string2 );
      Assert::ExpectException<exception>([&] { string2  >= array1  ; });
      Assert::ExpectException<exception>([&] { string2  >= array2  ; });
      Assert::ExpectException<exception>([&] { string2  >= boolean1; });
      Assert::ExpectException<exception>([&] { string2  >= boolean2; });
      Assert::IsFalse(string2  >= null    );
      Assert::ExpectException<exception>([&] { string2  >= number1 ; });
      Assert::ExpectException<exception>([&] { string2  >= number2 ; });
      Assert::ExpectException<exception>([&] { string2  >= numberi1; });
      Assert::ExpectException<exception>([&] { string2  >= numberi2; });
      Assert::ExpectException<exception>([&] { string2  >= object1 ; });
      Assert::ExpectException<exception>([&] { string2  >= object2 ; });
      Assert::IsTrue (string2  >= string1 );
      Assert::IsTrue (string2  >= string2 );
    }

    TEST_METHOD(TestOperatorAdd)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   + array1  ; });
      Assert::ExpectException<exception>([&] { array1   + array2  ; });
      Assert::ExpectException<exception>([&] { array1   + boolean1; });
      Assert::ExpectException<exception>([&] { array1   + boolean2; });
      Assert::AreEqual<Json>(null    , array1   + null    );
      Assert::ExpectException<exception>([&] { array1   + number1 ; });
      Assert::ExpectException<exception>([&] { array1   + number2 ; });
      Assert::ExpectException<exception>([&] { array1   + numberi1; });
      Assert::ExpectException<exception>([&] { array1   + numberi2; });
      Assert::ExpectException<exception>([&] { array1   + object1 ; });
      Assert::ExpectException<exception>([&] { array1   + object2 ; });
      Assert::ExpectException<exception>([&] { array1   + string1 ; });
      Assert::ExpectException<exception>([&] { array1   + string2 ; });
      Assert::ExpectException<exception>([&] { array2   + array1  ; });
      Assert::ExpectException<exception>([&] { array2   + array2  ; });
      Assert::ExpectException<exception>([&] { array2   + boolean1; });
      Assert::ExpectException<exception>([&] { array2   + boolean2; });
      Assert::AreEqual<Json>(null    , array2   + null    );
      Assert::ExpectException<exception>([&] { array2   + number1 ; });
      Assert::ExpectException<exception>([&] { array2   + number2 ; });
      Assert::ExpectException<exception>([&] { array2   + numberi1; });
      Assert::ExpectException<exception>([&] { array2   + numberi2; });
      Assert::ExpectException<exception>([&] { array2   + object1 ; });
      Assert::ExpectException<exception>([&] { array2   + object2 ; });
      Assert::ExpectException<exception>([&] { array2   + string1 ; });
      Assert::ExpectException<exception>([&] { array2   + string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 + array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 + array2  ; });
      Assert::AreEqual<Json>(0i64, boolean1 + boolean1);
      Assert::AreEqual<Json>(1i64, boolean1 + boolean2);
      Assert::AreEqual<Json>(null    , boolean1 + null    );
      Assert::AreEqual<Json>(0.0, boolean1 + number1 );
      Assert::AreEqual<Json>(1.0, boolean1 + number2 );
      Assert::AreEqual<Json>(0i64, boolean1 + numberi1);
      Assert::AreEqual<Json>(1i64, boolean1 + numberi2);
      Assert::ExpectException<exception>([&] { boolean1 + object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 + object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 + string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 + string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 + array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 + array2  ; });
      Assert::AreEqual<Json>(1i64, boolean2 + boolean1);
      Assert::AreEqual<Json>(2i64, boolean2 + boolean2);
      Assert::AreEqual<Json>(null    , boolean2 + null    );
      Assert::AreEqual<Json>(1.0, boolean2 + number1 );
      Assert::AreEqual<Json>(2.0, boolean2 + number2 );
      Assert::AreEqual<Json>(1i64, boolean2 + numberi1);
      Assert::AreEqual<Json>(2i64, boolean2 + numberi2);
      Assert::ExpectException<exception>([&] { boolean2 + object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 + object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 + string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 + string2 ; });
      Assert::AreEqual<Json>(null    , null     + array1  );
      Assert::AreEqual<Json>(null    , null     + array2  );
      Assert::AreEqual<Json>(null    , null     + boolean1);
      Assert::AreEqual<Json>(null    , null     + boolean2);
      Assert::AreEqual<Json>(null    , null     + null    );
      Assert::AreEqual<Json>(null    , null     + number1 );
      Assert::AreEqual<Json>(null    , null     + number2 );
      Assert::AreEqual<Json>(null    , null     + numberi1);
      Assert::AreEqual<Json>(null    , null     + numberi2);
      Assert::AreEqual<Json>(null    , null     + object1 );
      Assert::AreEqual<Json>(null    , null     + object2 );
      Assert::AreEqual<Json>(null    , null     + string1 );
      Assert::AreEqual<Json>(null    , null     + string2 );
      Assert::ExpectException<exception>([&] { number1  + array1  ; });
      Assert::ExpectException<exception>([&] { number1  + array2  ; });
      Assert::AreEqual<Json>(0.0, number1  + boolean1);
      Assert::AreEqual<Json>(1.0, number1  + boolean2);
      Assert::AreEqual<Json>(null    , number1  + null    );
      Assert::AreEqual<Json>(0.0, number1  + number1 );
      Assert::AreEqual<Json>(1.0, number1  + number2 );
      Assert::AreEqual<Json>(0.0, number1  + numberi1);
      Assert::AreEqual<Json>(1.0, number1  + numberi2);
      Assert::ExpectException<exception>([&] { number1  + object1 ; });
      Assert::ExpectException<exception>([&] { number1  + object2 ; });
      Assert::ExpectException<exception>([&] { number1  + string1 ; });
      Assert::ExpectException<exception>([&] { number1  + string2 ; });
      Assert::ExpectException<exception>([&] { number2  + array1  ; });
      Assert::ExpectException<exception>([&] { number2  + array2  ; });
      Assert::AreEqual<Json>(1.0, number2  + boolean1);
      Assert::AreEqual<Json>(2.0, number2  + boolean2);
      Assert::AreEqual<Json>(null    , number2  + null    );
      Assert::AreEqual<Json>(1.0, number2  + number1 );
      Assert::AreEqual<Json>(2.0, number2  + number2 );
      Assert::AreEqual<Json>(1.0, number2  + numberi1);
      Assert::AreEqual<Json>(2.0, number2  + numberi2);
      Assert::ExpectException<exception>([&] { number2  + object1 ; });
      Assert::ExpectException<exception>([&] { number2  + object2 ; });
      Assert::ExpectException<exception>([&] { number2  + string1 ; });
      Assert::ExpectException<exception>([&] { number2  + string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 + array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 + array2  ; });
      Assert::AreEqual<Json>(0i64, numberi1 + boolean1);
      Assert::AreEqual<Json>(1i64, numberi1 + boolean2);
      Assert::AreEqual<Json>(null    , numberi1 + null    );
      Assert::AreEqual<Json>(0.0, numberi1 + number1 );
      Assert::AreEqual<Json>(1.0, numberi1 + number2 );
      Assert::AreEqual<Json>(0i64, numberi1 + numberi1);
      Assert::AreEqual<Json>(1i64, numberi1 + numberi2);
      Assert::ExpectException<exception>([&] { numberi1 + object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 + object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 + string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 + string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 + array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 + array2  ; });
      Assert::AreEqual<Json>(1i64, numberi2 + boolean1);
      Assert::AreEqual<Json>(2i64, numberi2 + boolean2);
      Assert::AreEqual<Json>(null    , numberi2 + null    );
      Assert::AreEqual<Json>(1.0, numberi2 + number1 );
      Assert::AreEqual<Json>(2.0, numberi2 + number2 );
      Assert::AreEqual<Json>(1i64, numberi2 + numberi1);
      Assert::AreEqual<Json>(2i64, numberi2 + numberi2);
      Assert::ExpectException<exception>([&] { numberi2 + object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 + object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 + string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 + string2 ; });
      Assert::ExpectException<exception>([&] { object1  + array1  ; });
      Assert::ExpectException<exception>([&] { object1  + array2  ; });
      Assert::ExpectException<exception>([&] { object1  + boolean1; });
      Assert::ExpectException<exception>([&] { object1  + boolean2; });
      Assert::AreEqual<Json>(null    , object1  + null    );
      Assert::ExpectException<exception>([&] { object1  + number1 ; });
      Assert::ExpectException<exception>([&] { object1  + number2 ; });
      Assert::ExpectException<exception>([&] { object1  + numberi1; });
      Assert::ExpectException<exception>([&] { object1  + numberi2; });
      Assert::ExpectException<exception>([&] { object1  + object1 ; });
      Assert::ExpectException<exception>([&] { object1  + object2 ; });
      Assert::ExpectException<exception>([&] { object1  + string1 ; });
      Assert::ExpectException<exception>([&] { object1  + string2 ; });
      Assert::ExpectException<exception>([&] { object2  + array1  ; });
      Assert::ExpectException<exception>([&] { object2  + array2  ; });
      Assert::ExpectException<exception>([&] { object2  + boolean1; });
      Assert::ExpectException<exception>([&] { object2  + boolean2; });
      Assert::AreEqual<Json>(null    , object2  + null    );
      Assert::ExpectException<exception>([&] { object2  + number1 ; });
      Assert::ExpectException<exception>([&] { object2  + number2 ; });
      Assert::ExpectException<exception>([&] { object2  + numberi1; });
      Assert::ExpectException<exception>([&] { object2  + numberi2; });
      Assert::ExpectException<exception>([&] { object2  + object1 ; });
      Assert::ExpectException<exception>([&] { object2  + object2 ; });
      Assert::ExpectException<exception>([&] { object2  + string1 ; });
      Assert::ExpectException<exception>([&] { object2  + string2 ; });
      Assert::ExpectException<exception>([&] { string1  + array1  ; });
      Assert::ExpectException<exception>([&] { string1  + array2  ; });
      Assert::ExpectException<exception>([&] { string1  + boolean1; });
      Assert::ExpectException<exception>([&] { string1  + boolean2; });
      Assert::AreEqual<Json>(null    , string1  + null    );
      Assert::ExpectException<exception>([&] { string1  + number1 ; });
      Assert::ExpectException<exception>([&] { string1  + number2 ; });
      Assert::ExpectException<exception>([&] { string1  + numberi1; });
      Assert::ExpectException<exception>([&] { string1  + numberi2; });
      Assert::ExpectException<exception>([&] { string1  + object1 ; });
      Assert::ExpectException<exception>([&] { string1  + object2 ; });
      Assert::AreEqual<Json>(L"TestString0TestString0"s, string1  + string1 );
      Assert::AreEqual<Json>(L"TestString0TestString1"s, string1  + string2 );
      Assert::ExpectException<exception>([&] { string2  + array1  ; });
      Assert::ExpectException<exception>([&] { string2  + array2  ; });
      Assert::ExpectException<exception>([&] { string2  + boolean1; });
      Assert::ExpectException<exception>([&] { string2  + boolean2; });
      Assert::AreEqual<Json>(null    , string2  + null    );
      Assert::ExpectException<exception>([&] { string2  + number1 ; });
      Assert::ExpectException<exception>([&] { string2  + number2 ; });
      Assert::ExpectException<exception>([&] { string2  + numberi1; });
      Assert::ExpectException<exception>([&] { string2  + numberi2; });
      Assert::ExpectException<exception>([&] { string2  + object1 ; });
      Assert::ExpectException<exception>([&] { string2  + object2 ; });
      Assert::AreEqual<Json>(L"TestString1TestString0"s, string2  + string1 );
      Assert::AreEqual<Json>(L"TestString1TestString1"s, string2  + string2 );
    }

    TEST_METHOD(TestOperatorAddAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value += boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(1i64, value += boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value += number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(1.0, value += number2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value += numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(1i64, value += numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value += boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(2i64, value += boolean2);
      Assert::AreEqual<Json>(2i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value += number1);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(2.0, value += number2);
      Assert::AreEqual<Json>(2.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value += numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(2i64, value += numberi2);
      Assert::AreEqual<Json>(2i64, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value += array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value += string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value += boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(1.0, value += boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value += number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(1.0, value += number2);
      Assert::AreEqual<Json>(1.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value += numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(1.0, value += numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value += boolean1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(2.0, value += boolean2);
      Assert::AreEqual<Json>(2.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value += number1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(2.0, value += number2);
      Assert::AreEqual<Json>(2.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value += numberi1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(2.0, value += numberi2);
      Assert::AreEqual<Json>(2.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value += boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(1i64, value += boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value += number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(1.0, value += number2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value += numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(1i64, value += numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value += boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(2i64, value += boolean2);
      Assert::AreEqual<Json>(2i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value += number1);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(2.0, value += number2);
      Assert::AreEqual<Json>(2.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value += numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(2i64, value += numberi2);
      Assert::AreEqual<Json>(2i64, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value += string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(L"TestString0TestString0"s, value += string1);
      Assert::AreEqual<Json>(L"TestString0TestString0"s, value);
      value = string1;
      Assert::AreEqual<Json>(L"TestString0TestString1"s, value += string2);
      Assert::AreEqual<Json>(L"TestString0TestString1"s, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value += null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value += object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(L"TestString1TestString0"s, value += string1);
      Assert::AreEqual<Json>(L"TestString1TestString0"s, value);
      value = string2;
      Assert::AreEqual<Json>(L"TestString1TestString1"s, value += string2);
      Assert::AreEqual<Json>(L"TestString1TestString1"s, value);
    }

    TEST_METHOD(TestOperatorSubtract)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   - array1  ; });
      Assert::ExpectException<exception>([&] { array1   - array2  ; });
      Assert::ExpectException<exception>([&] { array1   - boolean1; });
      Assert::ExpectException<exception>([&] { array1   - boolean2; });
      Assert::AreEqual<Json>(null , array1   - null    );
      Assert::ExpectException<exception>([&] { array1   - number1 ; });
      Assert::ExpectException<exception>([&] { array1   - number2 ; });
      Assert::ExpectException<exception>([&] { array1   - numberi1; });
      Assert::ExpectException<exception>([&] { array1   - numberi2; });
      Assert::ExpectException<exception>([&] { array1   - object1 ; });
      Assert::ExpectException<exception>([&] { array1   - object2 ; });
      Assert::ExpectException<exception>([&] { array1   - string1 ; });
      Assert::ExpectException<exception>([&] { array1   - string2 ; });
      Assert::ExpectException<exception>([&] { array2   - array1  ; });
      Assert::ExpectException<exception>([&] { array2   - array2  ; });
      Assert::ExpectException<exception>([&] { array2   - boolean1; });
      Assert::ExpectException<exception>([&] { array2   - boolean2; });
      Assert::AreEqual<Json>(null , array2   - null    );
      Assert::ExpectException<exception>([&] { array2   - number1 ; });
      Assert::ExpectException<exception>([&] { array2   - number2 ; });
      Assert::ExpectException<exception>([&] { array2   - numberi1; });
      Assert::ExpectException<exception>([&] { array2   - numberi2; });
      Assert::ExpectException<exception>([&] { array2   - object1 ; });
      Assert::ExpectException<exception>([&] { array2   - object2 ; });
      Assert::ExpectException<exception>([&] { array2   - string1 ; });
      Assert::ExpectException<exception>([&] { array2   - string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 - array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 - array2  ; });
      Assert::AreEqual<Json>( 0i64, boolean1 - boolean1);
      Assert::AreEqual<Json>(-1i64, boolean1 - boolean2);
      Assert::AreEqual<Json>(null , boolean1 - null    );
      Assert::AreEqual<Json>( 0.0 , boolean1 - number1 );
      Assert::AreEqual<Json>(-1.0 , boolean1 - number2 );
      Assert::AreEqual<Json>( 0i64, boolean1 - numberi1);
      Assert::AreEqual<Json>(-1i64, boolean1 - numberi2);
      Assert::ExpectException<exception>([&] { boolean1 - object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 - object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 - string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 - string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 - array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 - array2  ; });
      Assert::AreEqual<Json>( 1i64, boolean2 - boolean1);
      Assert::AreEqual<Json>( 0i64, boolean2 - boolean2);
      Assert::AreEqual<Json>(null , boolean2 - null    );
      Assert::AreEqual<Json>( 1.0 , boolean2 - number1 );
      Assert::AreEqual<Json>( 0.0 , boolean2 - number2 );
      Assert::AreEqual<Json>( 1i64, boolean2 - numberi1);
      Assert::AreEqual<Json>( 0i64, boolean2 - numberi2);
      Assert::ExpectException<exception>([&] { boolean2 - object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 - object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 - string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 - string2 ; });
      Assert::AreEqual<Json>(null , null     - array1  );
      Assert::AreEqual<Json>(null , null     - array2  );
      Assert::AreEqual<Json>(null , null     - boolean1);
      Assert::AreEqual<Json>(null , null     - boolean2);
      Assert::AreEqual<Json>(null , null     - null    );
      Assert::AreEqual<Json>(null , null     - number1 );
      Assert::AreEqual<Json>(null , null     - number2 );
      Assert::AreEqual<Json>(null , null     - numberi1);
      Assert::AreEqual<Json>(null , null     - numberi2);
      Assert::AreEqual<Json>(null , null     - object1 );
      Assert::AreEqual<Json>(null , null     - object2 );
      Assert::AreEqual<Json>(null , null     - string1 );
      Assert::AreEqual<Json>(null , null     - string2 );
      Assert::ExpectException<exception>([&] { number1  - array1  ; });
      Assert::ExpectException<exception>([&] { number1  - array2  ; });
      Assert::AreEqual<Json>( 0.0 , number1  - boolean1);
      Assert::AreEqual<Json>(-1.0 , number1  - boolean2);
      Assert::AreEqual<Json>(null , number1  - null    );
      Assert::AreEqual<Json>( 0.0 , number1  - number1 );
      Assert::AreEqual<Json>(-1.0 , number1  - number2 );
      Assert::AreEqual<Json>( 0.0 , number1  - numberi1);
      Assert::AreEqual<Json>(-1.0 , number1  - numberi2);
      Assert::ExpectException<exception>([&] { number1  - object1 ; });
      Assert::ExpectException<exception>([&] { number1  - object2 ; });
      Assert::ExpectException<exception>([&] { number1  - string1 ; });
      Assert::ExpectException<exception>([&] { number1  - string2 ; });
      Assert::ExpectException<exception>([&] { number2  - array1  ; });
      Assert::ExpectException<exception>([&] { number2  - array2  ; });
      Assert::AreEqual<Json>( 1.0 , number2  - boolean1);
      Assert::AreEqual<Json>( 0.0 , number2  - boolean2);
      Assert::AreEqual<Json>(null , number2  - null    );
      Assert::AreEqual<Json>( 1.0 , number2  - number1 );
      Assert::AreEqual<Json>( 0.0 , number2  - number2 );
      Assert::AreEqual<Json>( 1.0 , number2  - numberi1);
      Assert::AreEqual<Json>( 0.0 , number2  - numberi2);
      Assert::ExpectException<exception>([&] { number2  - object1 ; });
      Assert::ExpectException<exception>([&] { number2  - object2 ; });
      Assert::ExpectException<exception>([&] { number2  - string1 ; });
      Assert::ExpectException<exception>([&] { number2  - string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 - array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 - array2  ; });
      Assert::AreEqual<Json>( 0i64, numberi1 - boolean1);
      Assert::AreEqual<Json>(-1i64, numberi1 - boolean2);
      Assert::AreEqual<Json>(null , numberi1 - null    );
      Assert::AreEqual<Json>( 0.0 , numberi1 - number1 );
      Assert::AreEqual<Json>(-1.0 , numberi1 - number2 );
      Assert::AreEqual<Json>( 0i64, numberi1 - numberi1);
      Assert::AreEqual<Json>(-1i64, numberi1 - numberi2);
      Assert::ExpectException<exception>([&] { numberi1 - object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 - object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 - string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 - string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 - array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 - array2  ; });
      Assert::AreEqual<Json>( 1i64, numberi2 - boolean1);
      Assert::AreEqual<Json>( 0i64, numberi2 - boolean2);
      Assert::AreEqual<Json>(null , numberi2 - null    );
      Assert::AreEqual<Json>( 1.0 , numberi2 - number1 );
      Assert::AreEqual<Json>( 0.0 , numberi2 - number2 );
      Assert::AreEqual<Json>( 1i64, numberi2 - numberi1);
      Assert::AreEqual<Json>( 0i64, numberi2 - numberi2);
      Assert::ExpectException<exception>([&] { numberi2 - object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 - object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 - string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 - string2 ; });
      Assert::ExpectException<exception>([&] { object1  - array1  ; });
      Assert::ExpectException<exception>([&] { object1  - array2  ; });
      Assert::ExpectException<exception>([&] { object1  - boolean1; });
      Assert::ExpectException<exception>([&] { object1  - boolean2; });
      Assert::AreEqual<Json>(null , object1  - null    );
      Assert::ExpectException<exception>([&] { object1  - number1 ; });
      Assert::ExpectException<exception>([&] { object1  - number2 ; });
      Assert::ExpectException<exception>([&] { object1  - numberi1; });
      Assert::ExpectException<exception>([&] { object1  - numberi2; });
      Assert::ExpectException<exception>([&] { object1  - object1 ; });
      Assert::ExpectException<exception>([&] { object1  - object2 ; });
      Assert::ExpectException<exception>([&] { object1  - string1 ; });
      Assert::ExpectException<exception>([&] { object1  - string2 ; });
      Assert::ExpectException<exception>([&] { object2  - array1  ; });
      Assert::ExpectException<exception>([&] { object2  - array2  ; });
      Assert::ExpectException<exception>([&] { object2  - boolean1; });
      Assert::ExpectException<exception>([&] { object2  - boolean2; });
      Assert::AreEqual<Json>(null , object2  - null    );
      Assert::ExpectException<exception>([&] { object2  - number1 ; });
      Assert::ExpectException<exception>([&] { object2  - number2 ; });
      Assert::ExpectException<exception>([&] { object2  - numberi1; });
      Assert::ExpectException<exception>([&] { object2  - numberi2; });
      Assert::ExpectException<exception>([&] { object2  - object1 ; });
      Assert::ExpectException<exception>([&] { object2  - object2 ; });
      Assert::ExpectException<exception>([&] { object2  - string1 ; });
      Assert::ExpectException<exception>([&] { object2  - string2 ; });
      Assert::ExpectException<exception>([&] { string1  - array1  ; });
      Assert::ExpectException<exception>([&] { string1  - array2  ; });
      Assert::ExpectException<exception>([&] { string1  - boolean1; });
      Assert::ExpectException<exception>([&] { string1  - boolean2; });
      Assert::AreEqual<Json>(null , string1  - null    );
      Assert::ExpectException<exception>([&] { string1  - number1 ; });
      Assert::ExpectException<exception>([&] { string1  - number2 ; });
      Assert::ExpectException<exception>([&] { string1  - numberi1; });
      Assert::ExpectException<exception>([&] { string1  - numberi2; });
      Assert::ExpectException<exception>([&] { string1  - object1 ; });
      Assert::ExpectException<exception>([&] { string1  - object2 ; });
      Assert::ExpectException<exception>([&] { string1  - string1 ; });
      Assert::ExpectException<exception>([&] { string1  - string2 ; });
      Assert::ExpectException<exception>([&] { string2  - array1  ; });
      Assert::ExpectException<exception>([&] { string2  - array2  ; });
      Assert::ExpectException<exception>([&] { string2  - boolean1; });
      Assert::ExpectException<exception>([&] { string2  - boolean2; });
      Assert::AreEqual<Json>(null , string2  - null    );
      Assert::ExpectException<exception>([&] { string2  - number1 ; });
      Assert::ExpectException<exception>([&] { string2  - number2 ; });
      Assert::ExpectException<exception>([&] { string2  - numberi1; });
      Assert::ExpectException<exception>([&] { string2  - numberi2; });
      Assert::ExpectException<exception>([&] { string2  - object1 ; });
      Assert::ExpectException<exception>([&] { string2  - object2 ; });
      Assert::ExpectException<exception>([&] { string2  - string1 ; });
      Assert::ExpectException<exception>([&] { string2  - string2 ; });
    }

    TEST_METHOD(TestOperatorSubtractAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value -= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(-1i64, value -= boolean2);
      Assert::AreEqual<Json>(-1i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value -= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(-1.0, value -= number2);
      Assert::AreEqual<Json>(-1.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value -= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(-1i64, value -= numberi2);
      Assert::AreEqual<Json>(-1i64, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value -= boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value -= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value -= number1);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value -= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value -= numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value -= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value -= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value -= boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(-1.0, value -= boolean2);
      Assert::AreEqual<Json>(-1.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value -= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(-1.0, value -= number2);
      Assert::AreEqual<Json>(-1.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value -= numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(-1.0, value -= numberi2);
      Assert::AreEqual<Json>(-1.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value -= boolean1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value -= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value -= number1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value -= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value -= numberi1);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value -= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value -= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(-1i64, value -= boolean2);
      Assert::AreEqual<Json>(-1i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value -= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(-1.0, value -= number2);
      Assert::AreEqual<Json>(-1.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value -= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(-1i64, value -= numberi2);
      Assert::AreEqual<Json>(-1i64, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value -= boolean1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value -= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value -= number1);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value -= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value -= numberi1);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value -= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value -= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= string1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value -= string2; });
      Assert::AreEqual<Json>(string2, value);
    }

    TEST_METHOD(TestOperatorMultiply)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   * array1  ; });
      Assert::ExpectException<exception>([&] { array1   * array2  ; });
      Assert::ExpectException<exception>([&] { array1   * boolean1; });
      Assert::ExpectException<exception>([&] { array1   * boolean2; });
      Assert::AreEqual<Json>(null , array1   * null    );
      Assert::ExpectException<exception>([&] { array1   * number1 ; });
      Assert::ExpectException<exception>([&] { array1   * number2 ; });
      Assert::ExpectException<exception>([&] { array1   * numberi1; });
      Assert::ExpectException<exception>([&] { array1   * numberi2; });
      Assert::ExpectException<exception>([&] { array1   * object1 ; });
      Assert::ExpectException<exception>([&] { array1   * object2 ; });
      Assert::ExpectException<exception>([&] { array1   * string1 ; });
      Assert::ExpectException<exception>([&] { array1   * string2 ; });
      Assert::ExpectException<exception>([&] { array2   * array1  ; });
      Assert::ExpectException<exception>([&] { array2   * array2  ; });
      Assert::ExpectException<exception>([&] { array2   * boolean1; });
      Assert::ExpectException<exception>([&] { array2   * boolean2; });
      Assert::AreEqual<Json>(null , array2   * null    );
      Assert::ExpectException<exception>([&] { array2   * number1 ; });
      Assert::ExpectException<exception>([&] { array2   * number2 ; });
      Assert::ExpectException<exception>([&] { array2   * numberi1; });
      Assert::ExpectException<exception>([&] { array2   * numberi2; });
      Assert::ExpectException<exception>([&] { array2   * object1 ; });
      Assert::ExpectException<exception>([&] { array2   * object2 ; });
      Assert::ExpectException<exception>([&] { array2   * string1 ; });
      Assert::ExpectException<exception>([&] { array2   * string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 * array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 * array2  ; });
      Assert::AreEqual<Json>( 0i64, boolean1 * boolean1);
      Assert::AreEqual<Json>( 0i64, boolean1 * boolean2);
      Assert::AreEqual<Json>(null , boolean1 * null    );
      Assert::AreEqual<Json>( 0.0 , boolean1 * number1 );
      Assert::AreEqual<Json>( 0.0 , boolean1 * number2 );
      Assert::AreEqual<Json>( 0i64, boolean1 * numberi1);
      Assert::AreEqual<Json>( 0i64, boolean1 * numberi2);
      Assert::ExpectException<exception>([&] { boolean1 * object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 * object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 * string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 * string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 * array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 * array2  ; });
      Assert::AreEqual<Json>( 0i64, boolean2 * boolean1);
      Assert::AreEqual<Json>( 1i64, boolean2 * boolean2);
      Assert::AreEqual<Json>(null , boolean2 * null    );
      Assert::AreEqual<Json>( 0.0 , boolean2 * number1 );
      Assert::AreEqual<Json>( 1.0 , boolean2 * number2 );
      Assert::AreEqual<Json>( 0i64, boolean2 * numberi1);
      Assert::AreEqual<Json>( 1i64, boolean2 * numberi2);
      Assert::ExpectException<exception>([&] { boolean2 * object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 * object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 * string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 * string2 ; });
      Assert::AreEqual<Json>(null , null     * array1  );
      Assert::AreEqual<Json>(null , null     * array2  );
      Assert::AreEqual<Json>(null , null     * boolean1);
      Assert::AreEqual<Json>(null , null     * boolean2);
      Assert::AreEqual<Json>(null , null     * null    );
      Assert::AreEqual<Json>(null , null     * number1 );
      Assert::AreEqual<Json>(null , null     * number2 );
      Assert::AreEqual<Json>(null , null     * numberi1);
      Assert::AreEqual<Json>(null , null     * numberi2);
      Assert::AreEqual<Json>(null , null     * object1 );
      Assert::AreEqual<Json>(null , null     * object2 );
      Assert::AreEqual<Json>(null , null     * string1 );
      Assert::AreEqual<Json>(null , null     * string2 );
      Assert::ExpectException<exception>([&] { number1  * array1  ; });
      Assert::ExpectException<exception>([&] { number1  * array2  ; });
      Assert::AreEqual<Json>( 0.0 , number1  * boolean1);
      Assert::AreEqual<Json>( 0.0 , number1  * boolean2);
      Assert::AreEqual<Json>(null , number1  * null    );
      Assert::AreEqual<Json>( 0.0 , number1  * number1 );
      Assert::AreEqual<Json>( 0.0 , number1  * number2 );
      Assert::AreEqual<Json>( 0.0 , number1  * numberi1);
      Assert::AreEqual<Json>( 0.0 , number1  * numberi2);
      Assert::ExpectException<exception>([&] { number1  * object1 ; });
      Assert::ExpectException<exception>([&] { number1  * object2 ; });
      Assert::ExpectException<exception>([&] { number1  * string1 ; });
      Assert::ExpectException<exception>([&] { number1  * string2 ; });
      Assert::ExpectException<exception>([&] { number2  * array1  ; });
      Assert::ExpectException<exception>([&] { number2  * array2  ; });
      Assert::AreEqual<Json>( 0.0 , number2  * boolean1);
      Assert::AreEqual<Json>( 1.0 , number2  * boolean2);
      Assert::AreEqual<Json>(null , number2  * null    );
      Assert::AreEqual<Json>( 0.0 , number2  * number1 );
      Assert::AreEqual<Json>( 1.0 , number2  * number2 );
      Assert::AreEqual<Json>( 0.0 , number2  * numberi1);
      Assert::AreEqual<Json>( 1.0 , number2  * numberi2);
      Assert::ExpectException<exception>([&] { number2  * object1 ; });
      Assert::ExpectException<exception>([&] { number2  * object2 ; });
      Assert::ExpectException<exception>([&] { number2  * string1 ; });
      Assert::ExpectException<exception>([&] { number2  * string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 * array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 * array2  ; });
      Assert::AreEqual<Json>( 0i64, numberi1 * boolean1);
      Assert::AreEqual<Json>( 0i64, numberi1 * boolean2);
      Assert::AreEqual<Json>(null , numberi1 * null    );
      Assert::AreEqual<Json>( 0.0 , numberi1 * number1 );
      Assert::AreEqual<Json>( 0.0 , numberi1 * number2 );
      Assert::AreEqual<Json>( 0i64, numberi1 * numberi1);
      Assert::AreEqual<Json>( 0i64, numberi1 * numberi2);
      Assert::ExpectException<exception>([&] { numberi1 * object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 * object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 * string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 * string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 * array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 * array2  ; });
      Assert::AreEqual<Json>( 0i64, numberi2 * boolean1);
      Assert::AreEqual<Json>( 1i64, numberi2 * boolean2);
      Assert::AreEqual<Json>(null , numberi2 * null    );
      Assert::AreEqual<Json>( 0.0 , numberi2 * number1 );
      Assert::AreEqual<Json>( 1.0 , numberi2 * number2 );
      Assert::AreEqual<Json>( 0i64, numberi2 * numberi1);
      Assert::AreEqual<Json>( 1i64, numberi2 * numberi2);
      Assert::ExpectException<exception>([&] { numberi2 * object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 * object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 * string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 * string2 ; });
      Assert::ExpectException<exception>([&] { object1  * array1  ; });
      Assert::ExpectException<exception>([&] { object1  * array2  ; });
      Assert::ExpectException<exception>([&] { object1  * boolean1; });
      Assert::ExpectException<exception>([&] { object1  * boolean2; });
      Assert::AreEqual<Json>(null , object1  * null    );
      Assert::ExpectException<exception>([&] { object1  * number1 ; });
      Assert::ExpectException<exception>([&] { object1  * number2 ; });
      Assert::ExpectException<exception>([&] { object1  * numberi1; });
      Assert::ExpectException<exception>([&] { object1  * numberi2; });
      Assert::ExpectException<exception>([&] { object1  * object1 ; });
      Assert::ExpectException<exception>([&] { object1  * object2 ; });
      Assert::ExpectException<exception>([&] { object1  * string1 ; });
      Assert::ExpectException<exception>([&] { object1  * string2 ; });
      Assert::ExpectException<exception>([&] { object2  * array1  ; });
      Assert::ExpectException<exception>([&] { object2  * array2  ; });
      Assert::ExpectException<exception>([&] { object2  * boolean1; });
      Assert::ExpectException<exception>([&] { object2  * boolean2; });
      Assert::AreEqual<Json>(null , object2  * null    );
      Assert::ExpectException<exception>([&] { object2  * number1 ; });
      Assert::ExpectException<exception>([&] { object2  * number2 ; });
      Assert::ExpectException<exception>([&] { object2  * numberi1; });
      Assert::ExpectException<exception>([&] { object2  * numberi2; });
      Assert::ExpectException<exception>([&] { object2  * object1 ; });
      Assert::ExpectException<exception>([&] { object2  * object2 ; });
      Assert::ExpectException<exception>([&] { object2  * string1 ; });
      Assert::ExpectException<exception>([&] { object2  * string2 ; });
      Assert::ExpectException<exception>([&] { string1  * array1  ; });
      Assert::ExpectException<exception>([&] { string1  * array2  ; });
      Assert::ExpectException<exception>([&] { string1  * boolean1; });
      Assert::ExpectException<exception>([&] { string1  * boolean2; });
      Assert::AreEqual<Json>(null , string1  * null    );
      Assert::ExpectException<exception>([&] { string1  * number1 ; });
      Assert::ExpectException<exception>([&] { string1  * number2 ; });
      Assert::ExpectException<exception>([&] { string1  * numberi1; });
      Assert::ExpectException<exception>([&] { string1  * numberi2; });
      Assert::ExpectException<exception>([&] { string1  * object1 ; });
      Assert::ExpectException<exception>([&] { string1  * object2 ; });
      Assert::ExpectException<exception>([&] { string1  * string1 ; });
      Assert::ExpectException<exception>([&] { string1  * string2 ; });
      Assert::ExpectException<exception>([&] { string2  * array1  ; });
      Assert::ExpectException<exception>([&] { string2  * array2  ; });
      Assert::ExpectException<exception>([&] { string2  * boolean1; });
      Assert::ExpectException<exception>([&] { string2  * boolean2; });
      Assert::AreEqual<Json>(null , string2  * null    );
      Assert::ExpectException<exception>([&] { string2  * number1 ; });
      Assert::ExpectException<exception>([&] { string2  * number2 ; });
      Assert::ExpectException<exception>([&] { string2  * numberi1; });
      Assert::ExpectException<exception>([&] { string2  * numberi2; });
      Assert::ExpectException<exception>([&] { string2  * object1 ; });
      Assert::ExpectException<exception>([&] { string2  * object2 ; });
      Assert::ExpectException<exception>([&] { string2  * string1 ; });
      Assert::ExpectException<exception>([&] { string2  * string2 ; });
    }

    TEST_METHOD(TestOperatorMultiplyAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value *= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::AreEqual<Json>(0i64, value *= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value *= boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value *= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = boolean2;
      Assert::AreEqual<Json>(1i64, value *= numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value *= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(0.0, value *= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value *= boolean1);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value *= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value *= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(0.0, value *= numberi1);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(1.0, value *= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= boolean2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value *= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::AreEqual<Json>(0i64, value *= numberi2);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value *= boolean1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value *= boolean2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value *= number1);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value *= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(0i64, value *= numberi1);
      Assert::AreEqual<Json>(0i64, value);
      value = numberi2;
      Assert::AreEqual<Json>(1i64, value *= numberi2);
      Assert::AreEqual<Json>(1i64, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value *= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= string1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value *= string2; });
      Assert::AreEqual<Json>(string2, value);
    }

    TEST_METHOD(TestOperatorDivide)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   / array1  ; });
      Assert::ExpectException<exception>([&] { array1   / array2  ; });
      Assert::ExpectException<exception>([&] { array1   / boolean1; });
      Assert::ExpectException<exception>([&] { array1   / boolean2; });
      Assert::AreEqual<Json>(null    , array1   / null    );
      Assert::ExpectException<exception>([&] { array1   / number1 ; });
      Assert::ExpectException<exception>([&] { array1   / number2 ; });
      Assert::ExpectException<exception>([&] { array1   / numberi1; });
      Assert::ExpectException<exception>([&] { array1   / numberi2; });
      Assert::ExpectException<exception>([&] { array1   / object1 ; });
      Assert::ExpectException<exception>([&] { array1   / object2 ; });
      Assert::ExpectException<exception>([&] { array1   / string1 ; });
      Assert::ExpectException<exception>([&] { array1   / string2 ; });
      Assert::ExpectException<exception>([&] { array2   / array1  ; });
      Assert::ExpectException<exception>([&] { array2   / array2  ; });
      Assert::ExpectException<exception>([&] { array2   / boolean1; });
      Assert::ExpectException<exception>([&] { array2   / boolean2; });
      Assert::AreEqual<Json>(null    , array2   / null    );
      Assert::ExpectException<exception>([&] { array2   / number1 ; });
      Assert::ExpectException<exception>([&] { array2   / number2 ; });
      Assert::ExpectException<exception>([&] { array2   / numberi1; });
      Assert::ExpectException<exception>([&] { array2   / numberi2; });
      Assert::ExpectException<exception>([&] { array2   / object1 ; });
      Assert::ExpectException<exception>([&] { array2   / object2 ; });
      Assert::ExpectException<exception>([&] { array2   / string1 ; });
      Assert::ExpectException<exception>([&] { array2   / string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 / array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 / array2  ; });
      Assert::IsTrue(isnan((boolean1 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 / boolean2);
      Assert::AreEqual<Json>(null    , boolean1 / null    );
      Assert::IsTrue(isnan((boolean1 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 / number2 );
      Assert::IsTrue(isnan((boolean1 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 / numberi2);
      Assert::ExpectException<exception>([&] { boolean1 / object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 / object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 / string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 / string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 / array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 / array2  ; });
      Assert::IsTrue(isinf((boolean2 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(1.0, boolean2 / boolean2);
      Assert::AreEqual<Json>(null    , boolean2 / null    );
      Assert::IsTrue(isinf((boolean2 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, boolean2 / number2 );
      Assert::IsTrue(isinf((boolean2 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(1.0, boolean2 / numberi2);
      Assert::ExpectException<exception>([&] { boolean2 / object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 / object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 / string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 / string2 ; });
      Assert::AreEqual<Json>(null    , null     / array1  );
      Assert::AreEqual<Json>(null    , null     / array2  );
      Assert::AreEqual<Json>(null    , null     / boolean1);
      Assert::AreEqual<Json>(null    , null     / boolean2);
      Assert::AreEqual<Json>(null    , null     / null    );
      Assert::AreEqual<Json>(null    , null     / number1 );
      Assert::AreEqual<Json>(null    , null     / number2 );
      Assert::AreEqual<Json>(null    , null     / numberi1);
      Assert::AreEqual<Json>(null    , null     / numberi2);
      Assert::AreEqual<Json>(null    , null     / object1 );
      Assert::AreEqual<Json>(null    , null     / object2 );
      Assert::AreEqual<Json>(null    , null     / string1 );
      Assert::AreEqual<Json>(null    , null     / string2 );
      Assert::ExpectException<exception>([&] { number1  / array1  ; });
      Assert::ExpectException<exception>([&] { number1  / array2  ; });
      Assert::IsTrue(isnan((number1  / boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1  / boolean2);
      Assert::AreEqual<Json>(null    , number1  / null    );
      Assert::IsTrue(isnan((number1  / number1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1  / number2 );
      Assert::IsTrue(isnan((number1 / numberi1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1  / numberi2);
      Assert::ExpectException<exception>([&] { number1  / object1 ; });
      Assert::ExpectException<exception>([&] { number1  / object2 ; });
      Assert::ExpectException<exception>([&] { number1  / string1 ; });
      Assert::ExpectException<exception>([&] { number1  / string2 ; });
      Assert::ExpectException<exception>([&] { number2  / array1  ; });
      Assert::ExpectException<exception>([&] { number2  / array2  ; });
      Assert::IsTrue(isinf((number2  / boolean1).Get<double>()));
      Assert::AreEqual<Json>(1.0, number2  / boolean2);
      Assert::AreEqual<Json>(null    , number2  / null    );
      Assert::IsTrue(isinf((number2  / number1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, number2  / number2 );
      Assert::IsTrue(isinf((number2 / numberi1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, number2  / numberi2);
      Assert::ExpectException<exception>([&] { number2  / object1 ; });
      Assert::ExpectException<exception>([&] { number2  / object2 ; });
      Assert::ExpectException<exception>([&] { number2  / string1 ; });
      Assert::ExpectException<exception>([&] { number2  / string2 ; });
      Assert::ExpectException<exception>([&] { numberi1 / array1  ; });
      Assert::ExpectException<exception>([&] { numberi1 / array2  ; });
      Assert::IsTrue(isnan((numberi1 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 / boolean2);
      Assert::AreEqual<Json>(null    , numberi1 / null    );
      Assert::IsTrue(isnan((numberi1 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 / number2 );
      Assert::IsTrue(isnan((numberi1 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 / numberi2);
      Assert::ExpectException<exception>([&] { numberi1 / object1 ; });
      Assert::ExpectException<exception>([&] { numberi1 / object2 ; });
      Assert::ExpectException<exception>([&] { numberi1 / string1 ; });
      Assert::ExpectException<exception>([&] { numberi1 / string2 ; });
      Assert::ExpectException<exception>([&] { numberi2 / array1  ; });
      Assert::ExpectException<exception>([&] { numberi2 / array2  ; });
      Assert::IsTrue(isinf((numberi2 / boolean1).Get<double>()));
      Assert::AreEqual<Json>(1.0, numberi2 / boolean2);
      Assert::AreEqual<Json>(null    , numberi2 / null    );
      Assert::IsTrue(isinf((numberi2 / number1 ).Get<double>()));
      Assert::AreEqual<Json>(1.0, numberi2 / number2 );
      Assert::IsTrue(isinf((numberi2 / numberi1).Get<double>()));
      Assert::AreEqual<Json>(1.0, numberi2 / numberi2);
      Assert::ExpectException<exception>([&] { numberi2 / object1 ; });
      Assert::ExpectException<exception>([&] { numberi2 / object2 ; });
      Assert::ExpectException<exception>([&] { numberi2 / string1 ; });
      Assert::ExpectException<exception>([&] { numberi2 / string2 ; });
      Assert::ExpectException<exception>([&] { object1  / array1  ; });
      Assert::ExpectException<exception>([&] { object1  / array2  ; });
      Assert::ExpectException<exception>([&] { object1  / boolean1; });
      Assert::ExpectException<exception>([&] { object1  / boolean2; });
      Assert::AreEqual<Json>(null    , object1  / null    );
      Assert::ExpectException<exception>([&] { object1  / number1 ; });
      Assert::ExpectException<exception>([&] { object1  / number2 ; });
      Assert::ExpectException<exception>([&] { object1  / numberi1; });
      Assert::ExpectException<exception>([&] { object1  / numberi2; });
      Assert::ExpectException<exception>([&] { object1  / object1 ; });
      Assert::ExpectException<exception>([&] { object1  / object2 ; });
      Assert::ExpectException<exception>([&] { object1  / string1 ; });
      Assert::ExpectException<exception>([&] { object1  / string2 ; });
      Assert::ExpectException<exception>([&] { object2  / array1  ; });
      Assert::ExpectException<exception>([&] { object2  / array2  ; });
      Assert::ExpectException<exception>([&] { object2  / boolean1; });
      Assert::ExpectException<exception>([&] { object2  / boolean2; });
      Assert::AreEqual<Json>(null    , object2  / null    );
      Assert::ExpectException<exception>([&] { object2  / number1 ; });
      Assert::ExpectException<exception>([&] { object2  / number2 ; });
      Assert::ExpectException<exception>([&] { object2  / numberi1; });
      Assert::ExpectException<exception>([&] { object2  / numberi2; });
      Assert::ExpectException<exception>([&] { object2  / object1 ; });
      Assert::ExpectException<exception>([&] { object2  / object2 ; });
      Assert::ExpectException<exception>([&] { object2  / string1 ; });
      Assert::ExpectException<exception>([&] { object2  / string2 ; });
      Assert::ExpectException<exception>([&] { string1  / array1  ; });
      Assert::ExpectException<exception>([&] { string1  / array2  ; });
      Assert::ExpectException<exception>([&] { string1  / boolean1; });
      Assert::ExpectException<exception>([&] { string1  / boolean2; });
      Assert::AreEqual<Json>(null    , string1  / null    );
      Assert::ExpectException<exception>([&] { string1  / number1 ; });
      Assert::ExpectException<exception>([&] { string1  / number2 ; });
      Assert::ExpectException<exception>([&] { string1  / numberi1; });
      Assert::ExpectException<exception>([&] { string1  / numberi2; });
      Assert::ExpectException<exception>([&] { string1  / object1 ; });
      Assert::ExpectException<exception>([&] { string1  / object2 ; });
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString0"s)), string1  / string1 );
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString1"s)), string1  / string2 );
      Assert::ExpectException<exception>([&] { string2  / array1  ; });
      Assert::ExpectException<exception>([&] { string2  / array2  ; });
      Assert::ExpectException<exception>([&] { string2  / boolean1; });
      Assert::ExpectException<exception>([&] { string2  / boolean2; });
      Assert::AreEqual<Json>(null    , string2  / null    );
      Assert::ExpectException<exception>([&] { string2  / number1 ; });
      Assert::ExpectException<exception>([&] { string2  / number2 ; });
      Assert::ExpectException<exception>([&] { string2  / numberi1; });
      Assert::ExpectException<exception>([&] { string2  / numberi2; });
      Assert::ExpectException<exception>([&] { string2  / object1 ; });
      Assert::ExpectException<exception>([&] { string2  / object2 ; });
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString0"s)), string2  / string1 );
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString1"s)), string2  / string2 );
    }

    TEST_METHOD(TestOperatorDivideAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::IsTrue(isnan((value /= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value /= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::IsTrue(isnan((value /= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value /= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::IsTrue(isnan((value /= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value /= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::IsTrue(isinf((value /= boolean1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value /= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::IsTrue(isinf((value /= number1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value /= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::IsTrue(isinf((value /= numberi1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(1.0, value /= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value /= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::IsTrue(isnan((value /= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value /= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::IsTrue(isnan((value /= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value /= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::IsTrue(isnan((value /= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value /= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::IsTrue(isinf((value /= boolean1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(1.0, value /= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::IsTrue(isinf((value /= number1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(1.0, value /= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::IsTrue(isinf((value /= numberi1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(1.0, value /= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::IsTrue(isnan((value /= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value /= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::IsTrue(isnan((value /= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value /= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::IsTrue(isnan((value /= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value /= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::IsTrue(isinf((value /= boolean1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value /= boolean2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::IsTrue(isinf((value /= number1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value /= number2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::IsTrue(isinf((value /= numberi1).Get<double>()));
      Assert::IsTrue(isinf((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(1.0, value /= numberi2);
      Assert::AreEqual<Json>(1.0, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value /= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString0"s)), value /= string1);
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString0"s)), value);
      value = string1;
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString1"s)), value /= string2);
      Assert::AreEqual<Json>(wstring(path(L"TestString0"s) / path(L"TestString1"s)), value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value /= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value /= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString0"s)), value /= string1);
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString0"s)), value);
      value = string2;
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString1"s)), value /= string2);
      Assert::AreEqual<Json>(wstring(path(L"TestString1"s) / path(L"TestString1"s)), value);
    }

    TEST_METHOD(TestOperatorModulo)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::ExpectException<exception>([&] { array1   % array1  ; });
      Assert::ExpectException<exception>([&] { array1   % array2  ; });
      Assert::ExpectException<exception>([&] { array1   % boolean1; });
      Assert::ExpectException<exception>([&] { array1   % boolean2; });
      Assert::AreEqual<Json>(null, array1 % null);
      Assert::ExpectException<exception>([&] { array1   % number1 ; });
      Assert::ExpectException<exception>([&] { array1   % number2 ; });
      Assert::ExpectException<exception>([&] { array1   % numberi1; });
      Assert::ExpectException<exception>([&] { array1   % numberi2; });
      Assert::ExpectException<exception>([&] { array1   % object1 ; });
      Assert::ExpectException<exception>([&] { array1   % object2 ; });
      Assert::ExpectException<exception>([&] { array1   % string1 ; });
      Assert::ExpectException<exception>([&] { array1   % string2 ; });
      Assert::ExpectException<exception>([&] { array2   % array1  ; });
      Assert::ExpectException<exception>([&] { array2   % array2  ; });
      Assert::ExpectException<exception>([&] { array2   % boolean1; });
      Assert::ExpectException<exception>([&] { array2   % boolean2; });
      Assert::AreEqual<Json>(null, array2 % null);
      Assert::ExpectException<exception>([&] { array2   % number1 ; });
      Assert::ExpectException<exception>([&] { array2   % number2 ; });
      Assert::ExpectException<exception>([&] { array2   % numberi1; });
      Assert::ExpectException<exception>([&] { array2   % numberi2; });
      Assert::ExpectException<exception>([&] { array2   % object1 ; });
      Assert::ExpectException<exception>([&] { array2   % object2 ; });
      Assert::ExpectException<exception>([&] { array2   % string1 ; });
      Assert::ExpectException<exception>([&] { array2   % string2 ; });
      Assert::ExpectException<exception>([&] { boolean1 % array1  ; });
      Assert::ExpectException<exception>([&] { boolean1 % array2  ; });
      Assert::IsTrue(isnan((boolean1 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 % boolean2);
      Assert::AreEqual<Json>(null, boolean1 % null);
      Assert::IsTrue(isnan((boolean1 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 % number2);
      Assert::IsTrue(isnan((boolean1 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean1 % numberi2);
      Assert::ExpectException<exception>([&] { boolean1 % object1 ; });
      Assert::ExpectException<exception>([&] { boolean1 % object2 ; });
      Assert::ExpectException<exception>([&] { boolean1 % string1 ; });
      Assert::ExpectException<exception>([&] { boolean1 % string2 ; });
      Assert::ExpectException<exception>([&] { boolean2 % array1  ; });
      Assert::ExpectException<exception>([&] { boolean2 % array2  ; });
      Assert::IsTrue(isnan((boolean2 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean2 % boolean2);
      Assert::AreEqual<Json>(null, boolean2 % null);
      Assert::IsTrue(isnan((boolean2 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean2 % number2);
      Assert::IsTrue(isnan((boolean2 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, boolean2 % numberi2);
      Assert::ExpectException<exception>([&] { boolean2 % object1 ; });
      Assert::ExpectException<exception>([&] { boolean2 % object2 ; });
      Assert::ExpectException<exception>([&] { boolean2 % string1 ; });
      Assert::ExpectException<exception>([&] { boolean2 % string2 ; });
      Assert::AreEqual<Json>(null, null % array1);
      Assert::AreEqual<Json>(null, null % array2);
      Assert::AreEqual<Json>(null, null % boolean1);
      Assert::AreEqual<Json>(null, null % boolean2);
      Assert::AreEqual<Json>(null, null % null);
      Assert::AreEqual<Json>(null, null % number1);
      Assert::AreEqual<Json>(null, null % number2);
      Assert::AreEqual<Json>(null, null % numberi1);
      Assert::AreEqual<Json>(null, null % numberi2);
      Assert::AreEqual<Json>(null, null % object1);
      Assert::AreEqual<Json>(null, null % object2);
      Assert::AreEqual<Json>(null, null % string1);
      Assert::AreEqual<Json>(null, null % string2);
      Assert::ExpectException<exception>([&] { number1  % array1  ; });
      Assert::ExpectException<exception>([&] { number1  % array2  ; });
      Assert::IsTrue(isnan((number1 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1 % boolean2);
      Assert::AreEqual<Json>(null, number1 % null);
      Assert::IsTrue(isnan((number1 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1 % number2);
      Assert::IsTrue(isnan((number1 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number1 % numberi2);
      Assert::ExpectException<exception>([&] { number1  % object1 ; });
      Assert::ExpectException<exception>([&] { number1  % object2 ; });
      Assert::ExpectException<exception>([&] { number1  % string1 ; });
      Assert::ExpectException<exception>([&] { number1  % string2 ; });
      Assert::ExpectException<exception>([&] { number2  % array1  ; });
      Assert::ExpectException<exception>([&] { number2  % array2  ; });
      Assert::IsTrue(isnan((number2 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number2 % boolean2);
      Assert::AreEqual<Json>(null, number2 % null);
      Assert::IsTrue(isnan((number2 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number2 % number2);
      Assert::IsTrue(isnan((number2 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, number2 % numberi2);
      Assert::ExpectException<exception>([&] { number2  % object1 ; });
      Assert::ExpectException<exception>([&] { number2  % object2 ; });
      Assert::ExpectException<exception>([&] { number2  % string1 ; });
      Assert::ExpectException<exception>([&] { number2  % string2 ; });
      Assert::ExpectException<exception>([&] { numberi1  % array1  ; });
      Assert::ExpectException<exception>([&] { numberi1  % array2  ; });
      Assert::IsTrue(isnan((numberi1 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 % boolean2);
      Assert::AreEqual<Json>(null, numberi1 % null);
      Assert::IsTrue(isnan((numberi1 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 % number2);
      Assert::IsTrue(isnan((numberi1 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi1 % numberi2);
      Assert::ExpectException<exception>([&] { numberi1  % object1 ; });
      Assert::ExpectException<exception>([&] { numberi1  % object2 ; });
      Assert::ExpectException<exception>([&] { numberi1  % string1 ; });
      Assert::ExpectException<exception>([&] { numberi1  % string2 ; });
      Assert::ExpectException<exception>([&] { numberi2  % array1  ; });
      Assert::ExpectException<exception>([&] { numberi2  % array2  ; });
      Assert::IsTrue(isnan((numberi2 % boolean1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi2 % boolean2);
      Assert::AreEqual<Json>(null, numberi2 % null);
      Assert::IsTrue(isnan((numberi2 % number1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi2 % number2);
      Assert::IsTrue(isnan((numberi2 % numberi1).Get<double>()));
      Assert::AreEqual<Json>(0.0, numberi2 % numberi2);
      Assert::ExpectException<exception>([&] { numberi2  % object1 ; });
      Assert::ExpectException<exception>([&] { numberi2  % object2 ; });
      Assert::ExpectException<exception>([&] { numberi2  % string1 ; });
      Assert::ExpectException<exception>([&] { numberi2  % string2 ; });
      Assert::ExpectException<exception>([&] { object1  % array1  ; });
      Assert::ExpectException<exception>([&] { object1  % array2  ; });
      Assert::ExpectException<exception>([&] { object1  % boolean1; });
      Assert::ExpectException<exception>([&] { object1  % boolean2; });
      Assert::AreEqual<Json>(null, object1 % null);
      Assert::ExpectException<exception>([&] { object1  % number1 ; });
      Assert::ExpectException<exception>([&] { object1  % number2 ; });
      Assert::ExpectException<exception>([&] { object1  % numberi1; });
      Assert::ExpectException<exception>([&] { object1  % numberi2; });
      Assert::ExpectException<exception>([&] { object1  % object1 ; });
      Assert::ExpectException<exception>([&] { object1  % object2 ; });
      Assert::ExpectException<exception>([&] { object1  % string1 ; });
      Assert::ExpectException<exception>([&] { object1  % string2 ; });
      Assert::ExpectException<exception>([&] { object2  % array1  ; });
      Assert::ExpectException<exception>([&] { object2  % array2  ; });
      Assert::ExpectException<exception>([&] { object2  % boolean1; });
      Assert::ExpectException<exception>([&] { object2  % boolean2; });
      Assert::AreEqual<Json>(null, object2 % null);
      Assert::ExpectException<exception>([&] { object2  % number1 ; });
      Assert::ExpectException<exception>([&] { object2  % number2 ; });
      Assert::ExpectException<exception>([&] { object2  % numberi1; });
      Assert::ExpectException<exception>([&] { object2  % numberi2; });
      Assert::ExpectException<exception>([&] { object2  % object1 ; });
      Assert::ExpectException<exception>([&] { object2  % object2 ; });
      Assert::ExpectException<exception>([&] { object2  % string1 ; });
      Assert::ExpectException<exception>([&] { object2  % string2 ; });
      Assert::ExpectException<exception>([&] { string1  % array1  ; });
      Assert::ExpectException<exception>([&] { string1  % array2  ; });
      Assert::ExpectException<exception>([&] { string1  % boolean1; });
      Assert::ExpectException<exception>([&] { string1  % boolean2; });
      Assert::AreEqual<Json>(null, string1 % null);
      Assert::ExpectException<exception>([&] { string1  % number1 ; });
      Assert::ExpectException<exception>([&] { string1  % number2 ; });
      Assert::ExpectException<exception>([&] { string1  % numberi1; });
      Assert::ExpectException<exception>([&] { string1  % numberi2; });
      Assert::ExpectException<exception>([&] { string1  % object1 ; });
      Assert::ExpectException<exception>([&] { string1  % object2 ; });
      Assert::ExpectException<exception>([&] { string1  % string1 ; });
      Assert::ExpectException<exception>([&] { string1  % string2 ; });
      Assert::ExpectException<exception>([&] { string2  % array1  ; });
      Assert::ExpectException<exception>([&] { string2  % array2  ; });
      Assert::ExpectException<exception>([&] { string2  % boolean1; });
      Assert::ExpectException<exception>([&] { string2  % boolean2; });
      Assert::AreEqual<Json>(null, string2 % null);
      Assert::ExpectException<exception>([&] { string2  % number1 ; });
      Assert::ExpectException<exception>([&] { string2  % number2 ; });
      Assert::ExpectException<exception>([&] { string2  % numberi1; });
      Assert::ExpectException<exception>([&] { string2  % numberi2; });
      Assert::ExpectException<exception>([&] { string2  % object1 ; });
      Assert::ExpectException<exception>([&] { string2  % object2 ; });
      Assert::ExpectException<exception>([&] { string2  % string1 ; });
      Assert::ExpectException<exception>([&] { string2  % string2 ; });
    }

    TEST_METHOD(TestOperatorModuloAssign)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Json value = array1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(array1, value);
      value = array1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(array1, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(array2, value);
      value = array2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(array2, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean1;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean1;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(boolean1, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = boolean2;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = boolean2;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(boolean2, value);
      value = boolean2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(boolean2, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= array1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= array2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= boolean1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= boolean2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= number1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= number2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= numberi1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= numberi2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= object1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= object2);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= string1);
      Assert::AreEqual<Json>(null, value);
      value = null;
      Assert::AreEqual<Json>(null, value %= string2);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = number1;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number1;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(number1, value);
      value = number1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(number1, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = number2;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = number2;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(number2, value);
      value = number2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(number2, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi1;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi1;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(numberi1, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::IsTrue(isnan((value %= boolean1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value %= boolean2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = numberi2;
      Assert::IsTrue(isnan((value %= number1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value %= number2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::IsTrue(isnan((value %= numberi1).Get<double>()));
      Assert::IsTrue(isnan((value).Get<double>()));
      value = numberi2;
      Assert::AreEqual<Json>(0.0, value %= numberi2);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(numberi2, value);
      value = numberi2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(numberi2, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(object1, value);
      value = object1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(object1, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(object2, value);
      value = object2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(object2, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(string1, value);
      value = string1;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(string1, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= array1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= array2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= boolean1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= boolean2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::AreEqual<Json>(null, value %= null);
      Assert::AreEqual<Json>(null, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= number1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= number2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= numberi1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= numberi2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= object1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= object2; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= string1; });
      Assert::AreEqual<Json>(string2, value);
      value = string2;
      Assert::ExpectException<exception>([&] { value %= string2; });
      Assert::AreEqual<Json>(string2, value);
    }

    TEST_METHOD(TestOperatorNegate)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Assert::ExpectException<exception>([&] { -array; });
      Assert::AreEqual<Json>(-1i64, -boolean);
      Assert::AreEqual<Json>(null, -null);
      Assert::AreEqual<Json>(-1.0, -number);
      Assert::AreEqual<Json>(-1i64, -numberi);
      Assert::ExpectException<exception>([&] { -object; });
      Assert::ExpectException<exception>([&] { -string; });
    }

    TEST_METHOD(TestOperatorNot)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Assert::ExpectException<exception>([&] { !array; });
      Assert::AreEqual<Json>(!true, !boolean);
      Assert::AreEqual<Json>(!nullptr, !null);
      Assert::AreEqual<Json>(!1.0, !number);
      Assert::AreEqual<Json>(!1i64, !numberi);
      Assert::ExpectException<exception>([&] { !object; });
      Assert::ExpectException<exception>([&] { !string; });
    }

    TEST_METHOD(TestOperatorPreIncrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, ++value);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(2.0, ++value);
      Assert::AreEqual<Json>(2.0, value);
      value = numberi;
      Assert::AreEqual<Json>(2i64, ++value);
      Assert::AreEqual<Json>(2i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { ++value; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorPostIncrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, value++);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(1.0, value++);
      Assert::AreEqual<Json>(2.0, value);
      value = numberi;
      Assert::AreEqual<Json>(1i64, value++);
      Assert::AreEqual<Json>(2i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { value++; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorPreDecrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, --value);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(0.0, --value);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi;
      Assert::AreEqual<Json>(0i64, --value);
      Assert::AreEqual<Json>(0i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { --value; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorPostDecrement)
    {
      Json null = nullptr;
      Json string = L"TestString"s;
      Json boolean = true;
      Json number = 1.0;
      Json numberi = 1i64;
      Json object = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json array = JsonArray{ 1, 2, 3 };

      Json value = array;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(array, value);
      value = boolean;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(boolean, value);
      value = null;
      Assert::AreEqual<Json>(null, value--);
      Assert::AreEqual<Json>(null, value);
      value = number;
      Assert::AreEqual<Json>(1.0, value--);
      Assert::AreEqual<Json>(0.0, value);
      value = numberi;
      Assert::AreEqual<Json>(1i64, value--);
      Assert::AreEqual<Json>(0i64, value);
      value = object;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(object, value);
      value = string;
      Assert::ExpectException<exception>([&] { value--; });
      Assert::AreEqual<Json>(string, value);
    }

    TEST_METHOD(TestOperatorLogicalAnd)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::IsTrue (array1   && array1  );
      Assert::IsTrue (array1   && array2  );
      Assert::IsFalse(array1   && boolean1);
      Assert::IsTrue (array1   && boolean2);
      Assert::IsFalse(array1   && null    );
      Assert::IsFalse(array1   && number1 );
      Assert::IsTrue (array1   && number2 );
      Assert::IsFalse(array1   && numberi1);
      Assert::IsTrue (array1   && numberi2);
      Assert::IsTrue (array1   && object1 );
      Assert::IsTrue (array1   && object2 );
      Assert::IsTrue (array1   && string1 );
      Assert::IsTrue (array1   && string2 );
      Assert::IsTrue (array2   && array1  );
      Assert::IsTrue (array2   && array2  );
      Assert::IsFalse(array2   && boolean1);
      Assert::IsTrue (array2   && boolean2);
      Assert::IsFalse(array2   && null    );
      Assert::IsFalse(array2   && number1 );
      Assert::IsTrue (array2   && number2 );
      Assert::IsFalse(array2   && numberi1);
      Assert::IsTrue (array2   && numberi2);
      Assert::IsTrue (array2   && object1 );
      Assert::IsTrue (array2   && object2 );
      Assert::IsTrue (array2   && string1 );
      Assert::IsTrue (array2   && string2 );
      Assert::IsFalse(boolean1 && array1  );
      Assert::IsFalse(boolean1 && array2  );
      Assert::IsFalse(boolean1 && boolean1);
      Assert::IsFalse(boolean1 && boolean2);
      Assert::IsFalse(boolean1 && null    );
      Assert::IsFalse(boolean1 && number1 );
      Assert::IsFalse(boolean1 && number2 );
      Assert::IsFalse(boolean1 && numberi1);
      Assert::IsFalse(boolean1 && numberi2);
      Assert::IsFalse(boolean1 && object1 );
      Assert::IsFalse(boolean1 && object2 );
      Assert::IsFalse(boolean1 && string1 );
      Assert::IsFalse(boolean1 && string2 );
      Assert::IsTrue (boolean2 && array1  );
      Assert::IsTrue (boolean2 && array2  );
      Assert::IsFalse(boolean2 && boolean1);
      Assert::IsTrue (boolean2 && boolean2);
      Assert::IsFalse(boolean2 && null    );
      Assert::IsFalse(boolean2 && number1 );
      Assert::IsTrue (boolean2 && number2 );
      Assert::IsFalse(boolean2 && numberi1);
      Assert::IsTrue (boolean2 && numberi2);
      Assert::IsTrue (boolean2 && object1 );
      Assert::IsTrue (boolean2 && object2 );
      Assert::IsTrue (boolean2 && string1 );
      Assert::IsTrue (boolean2 && string2 );
      Assert::IsFalse(null     && array1  );
      Assert::IsFalse(null     && array2  );
      Assert::IsFalse(null     && boolean1);
      Assert::IsFalse(null     && boolean2);
      Assert::IsFalse(null     && null    );
      Assert::IsFalse(null     && number1 );
      Assert::IsFalse(null     && number2 );
      Assert::IsFalse(null     && numberi1);
      Assert::IsFalse(null     && numberi2);
      Assert::IsFalse(null     && object1 );
      Assert::IsFalse(null     && object2 );
      Assert::IsFalse(null     && string1 );
      Assert::IsFalse(null     && string2 );
      Assert::IsFalse(number1  && array1  );
      Assert::IsFalse(number1  && array2  );
      Assert::IsFalse(number1  && boolean1);
      Assert::IsFalse(number1  && boolean2);
      Assert::IsFalse(number1  && null    );
      Assert::IsFalse(number1  && number1 );
      Assert::IsFalse(number1  && number2 );
      Assert::IsFalse(number1  && numberi1);
      Assert::IsFalse(number1  && numberi2);
      Assert::IsFalse(number1  && object1 );
      Assert::IsFalse(number1  && object2 );
      Assert::IsFalse(number1  && string1 );
      Assert::IsFalse(number1  && string2 );
      Assert::IsTrue (number2  && array1  );
      Assert::IsTrue (number2  && array2  );
      Assert::IsFalse(number2  && boolean1);
      Assert::IsTrue (number2  && boolean2);
      Assert::IsFalse(number2  && null    );
      Assert::IsFalse(number2  && number1 );
      Assert::IsTrue (number2  && number2 );
      Assert::IsFalse(number2  && numberi1);
      Assert::IsTrue (number2  && numberi2);
      Assert::IsTrue (number2  && object1 );
      Assert::IsTrue (number2  && object2 );
      Assert::IsTrue (number2  && string1 );
      Assert::IsTrue (number2  && string2 );
      Assert::IsFalse(numberi1 && array1  );
      Assert::IsFalse(numberi1 && array2  );
      Assert::IsFalse(numberi1 && boolean1);
      Assert::IsFalse(numberi1 && boolean2);
      Assert::IsFalse(numberi1 && null    );
      Assert::IsFalse(numberi1 && number1 );
      Assert::IsFalse(numberi1 && number2 );
      Assert::IsFalse(numberi1 && numberi1);
      Assert::IsFalse(numberi1 && numberi2);
      Assert::IsFalse(numberi1 && object1 );
      Assert::IsFalse(numberi1 && object2 );
      Assert::IsFalse(numberi1 && string1 );
      Assert::IsFalse(numberi1 && string2 );
      Assert::IsTrue (numberi2 && array1  );
      Assert::IsTrue (numberi2 && array2  );
      Assert::IsFalse(numberi2 && boolean1);
      Assert::IsTrue (numberi2 && boolean2);
      Assert::IsFalse(numberi2 && null    );
      Assert::IsFalse(numberi2 && number1 );
      Assert::IsTrue (numberi2 && number2 );
      Assert::IsFalse(numberi2 && numberi1);
      Assert::IsTrue (numberi2 && numberi2);
      Assert::IsTrue (numberi2 && object1 );
      Assert::IsTrue (numberi2 && object2 );
      Assert::IsTrue (numberi2 && string1 );
      Assert::IsTrue (numberi2 && string2 );
      Assert::IsTrue (object1  && array1  );
      Assert::IsTrue (object1  && array2  );
      Assert::IsFalse(object1  && boolean1);
      Assert::IsTrue (object1  && boolean2);
      Assert::IsFalse(object1  && null    );
      Assert::IsFalse(object1  && number1 );
      Assert::IsTrue (object1  && number2 );
      Assert::IsFalse(object1  && numberi1);
      Assert::IsTrue (object1  && numberi2);
      Assert::IsTrue (object1  && object1 );
      Assert::IsTrue (object1  && object2 );
      Assert::IsTrue (object1  && string1 );
      Assert::IsTrue (object1  && string2 );
      Assert::IsTrue (object2  && array1  );
      Assert::IsTrue (object2  && array2  );
      Assert::IsFalse(object2  && boolean1);
      Assert::IsTrue (object2  && boolean2);
      Assert::IsFalse(object2  && null    );
      Assert::IsFalse(object2  && number1 );
      Assert::IsTrue (object2  && number2 );
      Assert::IsFalse(object2  && numberi1);
      Assert::IsTrue (object2  && numberi2);
      Assert::IsTrue (object2  && object1 );
      Assert::IsTrue (object2  && object2 );
      Assert::IsTrue (object2  && string1 );
      Assert::IsTrue (object2  && string2 );
      Assert::IsTrue (string1  && array1  );
      Assert::IsTrue (string1  && array2  );
      Assert::IsFalse(string1  && boolean1);
      Assert::IsTrue (string1  && boolean2);
      Assert::IsFalse(string1  && null    );
      Assert::IsFalse(string1  && number1 );
      Assert::IsTrue (string1  && number2 );
      Assert::IsFalse(string1  && numberi1);
      Assert::IsTrue (string1  && numberi2);
      Assert::IsTrue (string1  && object1 );
      Assert::IsTrue (string1  && object2 );
      Assert::IsTrue (string1  && string1 );
      Assert::IsTrue (string1  && string2 );
      Assert::IsTrue (string2  && array1  );
      Assert::IsTrue (string2  && array2  );
      Assert::IsFalse(string2  && boolean1);
      Assert::IsTrue (string2  && boolean2);
      Assert::IsFalse(string2  && null    );
      Assert::IsFalse(string2  && number1 );
      Assert::IsTrue (string2  && number2 );
      Assert::IsFalse(string2  && numberi1);
      Assert::IsTrue (string2  && numberi2);
      Assert::IsTrue (string2  && object1 );
      Assert::IsTrue (string2  && object2 );
      Assert::IsTrue (string2  && string1 );
      Assert::IsTrue (string2  && string2 );
    }

    TEST_METHOD(TestOperatorLogicalOr)
    {
      Json null = nullptr;
      Json string1 = L"TestString0"s;
      Json string2 = L"TestString1"s;
      Json boolean1 = false;
      Json boolean2 = true;
      Json number1 = 0.0;
      Json number2 = 1.0;
      Json numberi1 = 0i64;
      Json numberi2 = 1i64;
      Json object1 = JsonObject{
        { L"key1", 1337 },
        { L"key2", 1338 }
      };
      Json object2 = JsonObject{
        { L"key3", 1339 },
        { L"key4", 1340 }
      };
      Json array1 = JsonArray{ 1, 2, 3 };
      Json array2 = JsonArray{ 4, 5, 6 };

      Assert::IsTrue (array1   || array1  );
      Assert::IsTrue (array1   || array2  );
      Assert::IsTrue (array1   || boolean1);
      Assert::IsTrue (array1   || boolean2);
      Assert::IsTrue (array1   || null    );
      Assert::IsTrue (array1   || number1 );
      Assert::IsTrue (array1   || number2 );
      Assert::IsTrue (array1   || numberi1);
      Assert::IsTrue (array1   || numberi2);
      Assert::IsTrue (array1   || object1 );
      Assert::IsTrue (array1   || object2 );
      Assert::IsTrue (array1   || string1 );
      Assert::IsTrue (array1   || string2 );
      Assert::IsTrue (array2   || array1  );
      Assert::IsTrue (array2   || array2  );
      Assert::IsTrue (array2   || boolean1);
      Assert::IsTrue (array2   || boolean2);
      Assert::IsTrue (array2   || null    );
      Assert::IsTrue (array2   || number1 );
      Assert::IsTrue (array2   || number2 );
      Assert::IsTrue (array2   || numberi1);
      Assert::IsTrue (array2   || numberi2);
      Assert::IsTrue (array2   || object1 );
      Assert::IsTrue (array2   || object2 );
      Assert::IsTrue (array2   || string1 );
      Assert::IsTrue (array2   || string2 );
      Assert::IsTrue (boolean1 || array1  );
      Assert::IsTrue (boolean1 || array2  );
      Assert::IsFalse(boolean1 || boolean1);
      Assert::IsTrue (boolean1 || boolean2);
      Assert::IsFalse(boolean1 || null    );
      Assert::IsFalse(boolean1 || number1 );
      Assert::IsTrue (boolean1 || number2 );
      Assert::IsFalse(boolean1 || numberi1);
      Assert::IsTrue (boolean1 || numberi2);
      Assert::IsTrue (boolean1 || object1 );
      Assert::IsTrue (boolean1 || object2 );
      Assert::IsTrue (boolean1 || string1 );
      Assert::IsTrue (boolean1 || string2 );
      Assert::IsTrue (boolean2 || array1  );
      Assert::IsTrue (boolean2 || array2  );
      Assert::IsTrue (boolean2 || boolean1);
      Assert::IsTrue (boolean2 || boolean2);
      Assert::IsTrue (boolean2 || null    );
      Assert::IsTrue (boolean2 || number1 );
      Assert::IsTrue (boolean2 || number2 );
      Assert::IsTrue (boolean2 || numberi1);
      Assert::IsTrue (boolean2 || numberi2);
      Assert::IsTrue (boolean2 || object1 );
      Assert::IsTrue (boolean2 || object2 );
      Assert::IsTrue (boolean2 || string1 );
      Assert::IsTrue (boolean2 || string2 );
      Assert::IsTrue (null     || array1  );
      Assert::IsTrue (null     || array2  );
      Assert::IsFalse(null     || boolean1);
      Assert::IsTrue (null     || boolean2);
      Assert::IsFalse(null     || null    );
      Assert::IsFalse(null     || number1 );
      Assert::IsTrue (null     || number2 );
      Assert::IsFalse(null     || numberi1);
      Assert::IsTrue (null     || numberi2);
      Assert::IsTrue (null     || object1 );
      Assert::IsTrue (null     || object2 );
      Assert::IsTrue (null     || string1 );
      Assert::IsTrue (null     || string2 );
      Assert::IsTrue (number1  || array1  );
      Assert::IsTrue (number1  || array2  );
      Assert::IsFalse(number1  || boolean1);
      Assert::IsTrue (number1  || boolean2);
      Assert::IsFalse(number1  || null    );
      Assert::IsFalse(number1  || number1 );
      Assert::IsTrue (number1  || number2 );
      Assert::IsFalse(number1  || numberi1);
      Assert::IsTrue (number1  || numberi2);
      Assert::IsTrue (number1  || object1 );
      Assert::IsTrue (number1  || object2 );
      Assert::IsTrue (number1  || string1 );
      Assert::IsTrue (number1  || string2 );
      Assert::IsTrue (number2  || array1  );
      Assert::IsTrue (number2  || array2  );
      Assert::IsTrue (number2  || boolean1);
      Assert::IsTrue (number2  || boolean2);
      Assert::IsTrue (number2  || null    );
      Assert::IsTrue (number2  || number1 );
      Assert::IsTrue (number2  || number2 );
      Assert::IsTrue (number2  || numberi1);
      Assert::IsTrue (number2  || numberi2);
      Assert::IsTrue (number2  || object1 );
      Assert::IsTrue (number2  || object2 );
      Assert::IsTrue (number2  || string1 );
      Assert::IsTrue (number2  || string2 );
      Assert::IsTrue (numberi1 || array1  );
      Assert::IsTrue (numberi1 || array2  );
      Assert::IsFalse(numberi1 || boolean1);
      Assert::IsTrue (numberi1 || boolean2);
      Assert::IsFalse(numberi1 || null    );
      Assert::IsFalse(numberi1 || number1 );
      Assert::IsTrue (numberi1 || number2 );
      Assert::IsFalse(numberi1 || numberi1);
      Assert::IsTrue (numberi1 || numberi2);
      Assert::IsTrue (numberi1 || object1 );
      Assert::IsTrue (numberi1 || object2 );
      Assert::IsTrue (numberi1 || string1 );
      Assert::IsTrue (numberi1 || string2 );
      Assert::IsTrue (numberi2 || array1  );
      Assert::IsTrue (numberi2 || array2  );
      Assert::IsTrue (numberi2 || boolean1);
      Assert::IsTrue (numberi2 || boolean2);
      Assert::IsTrue (numberi2 || null    );
      Assert::IsTrue (numberi2 || number1 );
      Assert::IsTrue (numberi2 || number2 );
      Assert::IsTrue (numberi2 || numberi1);
      Assert::IsTrue (numberi2 || numberi2);
      Assert::IsTrue (numberi2 || object1 );
      Assert::IsTrue (numberi2 || object2 );
      Assert::IsTrue (numberi2 || string1 );
      Assert::IsTrue (numberi2 || string2 );
      Assert::IsTrue (object1  || array1  );
      Assert::IsTrue (object1  || array2  );
      Assert::IsTrue (object1  || boolean1);
      Assert::IsTrue (object1  || boolean2);
      Assert::IsTrue (object1  || null    );
      Assert::IsTrue (object1  || number1 );
      Assert::IsTrue (object1  || number2 );
      Assert::IsTrue (object1  || numberi1);
      Assert::IsTrue (object1  || numberi2);
      Assert::IsTrue (object1  || object1 );
      Assert::IsTrue (object1  || object2 );
      Assert::IsTrue (object1  || string1 );
      Assert::IsTrue (object1  || string2 );
      Assert::IsTrue (object2  || array1  );
      Assert::IsTrue (object2  || array2  );
      Assert::IsTrue (object2  || boolean1);
      Assert::IsTrue (object2  || boolean2);
      Assert::IsTrue (object2  || null    );
      Assert::IsTrue (object2  || number1 );
      Assert::IsTrue (object2  || number2 );
      Assert::IsTrue (object2  || numberi1);
      Assert::IsTrue (object2  || numberi2);
      Assert::IsTrue (object2  || object1 );
      Assert::IsTrue (object2  || object2 );
      Assert::IsTrue (object2  || string1 );
      Assert::IsTrue (object2  || string2 );
      Assert::IsTrue (string1  || array1  );
      Assert::IsTrue (string1  || array2  );
      Assert::IsTrue (string1  || boolean1);
      Assert::IsTrue (string1  || boolean2);
      Assert::IsTrue (string1  || null    );
      Assert::IsTrue (string1  || number1 );
      Assert::IsTrue (string1  || number2 );
      Assert::IsTrue (string1  || numberi2);
      Assert::IsTrue (string1  || numberi2);
      Assert::IsTrue (string1  || object1 );
      Assert::IsTrue (string1  || object2 );
      Assert::IsTrue (string1  || string1 );
      Assert::IsTrue (string1  || string2 );
      Assert::IsTrue (string2  || array1  );
      Assert::IsTrue (string2  || array2  );
      Assert::IsTrue (string2  || boolean1);
      Assert::IsTrue (string2  || boolean2);
      Assert::IsTrue (string2  || null    );
      Assert::IsTrue (string2  || number1 );
      Assert::IsTrue (string2  || number2 );
      Assert::IsTrue (string2  || numberi1);
      Assert::IsTrue (string2  || numberi2);
      Assert::IsTrue (string2  || object1 );
      Assert::IsTrue (string2  || object2 );
      Assert::IsTrue (string2  || string1 );
      Assert::IsTrue (string2  || string2 );
    }
  };
}