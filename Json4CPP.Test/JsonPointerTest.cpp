#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonPointerTest)
  {
    TEST_METHOD(TestConstructorDefault)
    {
      auto ptr = JsonPointer();
      Assert::AreEqual(L""s, ptr.Path());
      Assert::AreEqual(ptr, ptr.Parent());
      Assert::IsTrue(ptr.Empty());
    }

    TEST_METHOD(TestConstructorConstWcharPointer)
    {
      auto ptr = JsonPointer(L"/foo/0");
      Assert::AreEqual(L"/foo/0"s, ptr.Path());

      ptr = JsonPointer(L"");
      Assert::AreEqual(L""s, ptr.Path());

      ExpectException<exception>([&] { auto tmp = JsonPointer(L"|foo/0"); }, "Path must be empty or start with '/'! It started with \"|foo\"!");
      ExpectException<exception>([&] { auto tmp = JsonPointer(L"/foo/asd~"); }, "Invalid reference token \"asd~\"! After a '~', there must be either '0' or '1'!");
      ExpectException<exception>([&] { auto tmp = JsonPointer(L"/foo/asd~2"); }, "Invalid reference token \"asd~2\"! After a '~', there must be either '0' or '1'!");
    }

    TEST_METHOD(TestConstructorWstring)
    {
      auto ptr = JsonPointer(L"/foo/0"s);
      Assert::AreEqual(L"/foo/0"s, ptr.Path());

      ptr = JsonPointer(L""s);
      Assert::AreEqual(L""s, ptr.Path());

      ExpectException<exception>([&] { auto tmp = JsonPointer(L"|foo/0"s); }, "Path must be empty or start with '/'! It started with \"|foo\"!");
      ExpectException<exception>([&] { auto tmp = JsonPointer(L"/foo/asd~"s); }, "Invalid reference token \"asd~\"! After a '~', there must be either '0' or '1'!");
      ExpectException<exception>([&] { auto tmp = JsonPointer(L"/foo/asd~2"s); }, "Invalid reference token \"asd~2\"! After a '~', there must be either '0' or '1'!");
    }

    TEST_METHOD(TestPath1)
    {
      auto ptr = JsonPointer();
      Assert::AreEqual(L""s, ptr.Path());
      ptr = JsonPointer(L"/foo/0"s);
      Assert::AreEqual(L"/foo/0"s, ptr.Path());
    }

    TEST_METHOD(TestPath2)
    {
      auto ptr = JsonPointer();
      Assert::AreEqual(L""s, ptr.Path());
      ptr.Path(L"/foo/0"s);
      Assert::AreEqual(L"/foo/0"s, ptr.Path());
      ptr.Path(L"/foo/asd~01"s);
      Assert::AreEqual(L"/foo/asd~01"s, ptr.Path());

      ExpectException<exception>([&] { ptr.Path(L"|foo/0"s); }, "Path must be empty or start with '/'! It started with \"|foo\"!");
      ExpectException<exception>([&] { ptr.Path(L"/foo/asd~"s); }, "Invalid reference token \"asd~\"! After a '~', there must be either '0' or '1'!");
      ExpectException<exception>([&] { ptr.Path(L"/foo/asd~2"s); }, "Invalid reference token \"asd~2\"! After a '~', there must be either '0' or '1'!");
    }

    TEST_METHOD(TestParent)
    {
      auto ptr = JsonPointer();
      Assert::AreEqual(ptr, ptr.Parent());
      Assert::AreEqual(L""s, ptr.Path());
      Assert::AreEqual(L""s, ptr.Parent().Path());
      ptr = JsonPointer(L"/foo/0");
      Assert::AreEqual(L"/foo"s, ptr.Parent().Path());
      Assert::AreEqual(L""s, ptr.Parent().Parent().Path());
      Assert::AreEqual(L""s, ptr.Parent().Parent().Parent().Path());
    }

    TEST_METHOD(TestTarget)
    {
      auto ptr = JsonPointer();
      Assert::AreEqual(L""s, ptr.Target());
      ptr = JsonPointer(L"/foo/aa~1bb~1cc~0dd~0ee~01ff~10gg"s);
      Assert::AreEqual(L"aa/bb/cc~dd~ee~1ff/0gg"s, ptr.Target());
    }

    TEST_METHOD(TestEmpty)
    {
      auto ptr = JsonPointer();
      Assert::IsTrue(ptr.Empty());
      ptr = JsonPointer(L"/foo/0");
      Assert::IsFalse(ptr.Empty());
    }

    TEST_METHOD(TestNavigate)
    {
      ExpectException<exception>([]() { JsonPointer().Navigate(Json(nullptr_t())); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { JsonPointer().Navigate(Json(wstring  ())); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { JsonPointer().Navigate(Json(bool     ())); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { JsonPointer().Navigate(Json(double   ())); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([]() { JsonPointer().Navigate(Json(int64_t  ())); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");

      auto json0 = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(json0, JsonPointer(L""s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(0), JsonPointer(L"/0"s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(1), JsonPointer(L"/1"s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(2), JsonPointer(L"/2"s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(3), JsonPointer(L"/3"s).Navigate(json0));
      JsonPointer(L"/2"s).Navigate(json0) = 1337;
      Assert::AreEqual<Json>(1337, JsonPointer(L"/2"s).Navigate(json0));

      auto json1 = Json{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual<Json>(json1                          , JsonPointer(L""s)            .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Null")              , JsonPointer(L"/Null"s)       .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"String")            , JsonPointer(L"/String"s)     .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Boolean")           , JsonPointer(L"/Boolean"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Real")              , JsonPointer(L"/Real"s)       .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Integer")           , JsonPointer(L"/Integer"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Object")            , JsonPointer(L"/Object"s)     .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key1"), JsonPointer(L"/Object/Key1"s).Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key2"), JsonPointer(L"/Object/Key2"s).Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array")             , JsonPointer(L"/Array"s)      .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array").At(0)       , JsonPointer(L"/Array/0"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array").At(1)       , JsonPointer(L"/Array/1"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array").At(2)       , JsonPointer(L"/Array/2"s)    .Navigate(json1));

      auto json2 = Json{
        { L"foo"s   , { L"bar"s, L"baz"s } } ,
        { L""s      , 0 },
        { L"a/b"s   , 1 },
        { L"c%d"s   , 2 },
        { L"e^f"s   , 3 },
        { L"g|h"s   , 4 },
        { L"i\\j"s  , 5},
        { L"k\"l"s  , 6},
        { L" "s     , 7 },
        { L"m~n"s   , 8 }
      };
      Assert::AreEqual<Json>(json2                  , JsonPointer(L""s)      .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"foo"s)      , JsonPointer(L"/foo"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"foo"s).At(0), JsonPointer(L"/foo/0"s).Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L""s)         , JsonPointer(L"/"s)     .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"a/b"s)      , JsonPointer(L"/a~1b"s) .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"c%d"s)      , JsonPointer(L"/c%d"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"e^f"s)      , JsonPointer(L"/e^f"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"g|h"s)      , JsonPointer(L"/g|h"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"i\\j"s)     , JsonPointer(L"/i\\j"s) .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"k\"l"s)     , JsonPointer(L"/k\"l"s) .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L" "s)        , JsonPointer(L"/ "s)    .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"m~n"s)      , JsonPointer(L"/m~0n"s) .Navigate(json2));

      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/Non/Existent"s)      .Navigate(json2); }, "Reference token \"Non\" at path \"/Non\" not found!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/Object/NonExistent"s).Navigate(json1); }, "Reference token \"NonExistent\" at path \"/Object/NonExistent\" not found!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/-"s)             .Navigate(json2); }, "Reference token \"-\" at path \"/foo/-\" points to the member after the last array element which does not exist!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/00"s)            .Navigate(json2); }, "Reference token \"00\" at path \"/foo/00\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/01"s)            .Navigate(json2); }, "Reference token \"01\" at path \"/foo/01\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/-1"s)            .Navigate(json2); }, "Reference token \"-1\" at path \"/foo/-1\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/1a"s)            .Navigate(json2); }, "Reference token \"1a\" at path \"/foo/1a\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/3"s)             .Navigate(json2); }, "Reference token \"3\" at path \"/foo/3\" is out of range!");
    }

    TEST_METHOD(TestNavigateConst)
    {
      auto const null    = Json(nullptr_t());
      auto const text    = Json(wstring  ());
      auto const boolean = Json(bool     ());
      auto const real    = Json(double   ());
      auto const number  = Json(int64_t  ());
      ExpectException<exception>([&]() { JsonPointer().Navigate(null   ); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([&]() { JsonPointer().Navigate(text   ); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([&]() { JsonPointer().Navigate(boolean); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([&]() { JsonPointer().Navigate(real   ); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");
      ExpectException<exception>([&]() { JsonPointer().Navigate(number ); }, "Navigate(Json const& json) is only defined for JsonObject and JsonArray!");

      auto const json0 = Json{ 1, 3, 3, 7 };
      Assert::AreEqual<Json>(json0, JsonPointer(L""s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(0), JsonPointer(L"/0"s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(1), JsonPointer(L"/1"s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(2), JsonPointer(L"/2"s).Navigate(json0));
      Assert::AreEqual<Json>(json0.At(3), JsonPointer(L"/3"s).Navigate(json0));

      auto const json1 = Json{
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Real", 13.37 },
        { L"Integer", 1337 },
        { L"Object", {
          { L"Key1", 1 },
          { L"Key2", 2 } }
        },
        { L"Array", { 1, 2, 3 } },
      };
      Assert::AreEqual<Json>(json1                          , JsonPointer(L""s)            .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Null")              , JsonPointer(L"/Null"s)       .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"String")            , JsonPointer(L"/String"s)     .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Boolean")           , JsonPointer(L"/Boolean"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Real")              , JsonPointer(L"/Real"s)       .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Integer")           , JsonPointer(L"/Integer"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Object")            , JsonPointer(L"/Object"s)     .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key1"), JsonPointer(L"/Object/Key1"s).Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Object").At(L"Key2"), JsonPointer(L"/Object/Key2"s).Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array")             , JsonPointer(L"/Array"s)      .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array").At(0)       , JsonPointer(L"/Array/0"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array").At(1)       , JsonPointer(L"/Array/1"s)    .Navigate(json1));
      Assert::AreEqual<Json>(json1.At(L"Array").At(2)       , JsonPointer(L"/Array/2"s)    .Navigate(json1));

      auto const json2 = Json{
        { L"foo"s   , { L"bar"s, L"baz"s } } ,
        { L""s      , 0 },
        { L"a/b"s   , 1 },
        { L"c%d"s   , 2 },
        { L"e^f"s   , 3 },
        { L"g|h"s   , 4 },
        { L"i\\j"s  , 5},
        { L"k\"l"s  , 6},
        { L" "s     , 7 },
        { L"m~n"s   , 8 }
      };
      Assert::AreEqual<Json>(json2                  , JsonPointer(L""s)      .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"foo"s)      , JsonPointer(L"/foo"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"foo"s).At(0), JsonPointer(L"/foo/0"s).Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L""s)         , JsonPointer(L"/"s)     .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"a/b"s)      , JsonPointer(L"/a~1b"s) .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"c%d"s)      , JsonPointer(L"/c%d"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"e^f"s)      , JsonPointer(L"/e^f"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"g|h"s)      , JsonPointer(L"/g|h"s)  .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"i\\j"s)     , JsonPointer(L"/i\\j"s) .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"k\"l"s)     , JsonPointer(L"/k\"l"s) .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L" "s)        , JsonPointer(L"/ "s)    .Navigate(json2));
      Assert::AreEqual<Json>(json2.At(L"m~n"s)      , JsonPointer(L"/m~0n"s) .Navigate(json2));

      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/Non/Existent"s)      .Navigate(json2); }, "Reference token \"Non\" at path \"/Non\" not found!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/Object/NonExistent"s).Navigate(json1); }, "Reference token \"NonExistent\" at path \"/Object/NonExistent\" not found!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/-"s)             .Navigate(json2); }, "Reference token \"-\" at path \"/foo/-\" points to the member after the last array element which does not exist!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/00"s)            .Navigate(json2); }, "Reference token \"00\" at path \"/foo/00\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/01"s)            .Navigate(json2); }, "Reference token \"01\" at path \"/foo/01\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/-1"s)            .Navigate(json2); }, "Reference token \"-1\" at path \"/foo/-1\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/1a"s)            .Navigate(json2); }, "Reference token \"1a\" at path \"/foo/1a\" is not an array index!");
      ExpectException<exception>([&]() { auto temp = JsonPointer(L"/foo/3"s)             .Navigate(json2); }, "Reference token \"3\" at path \"/foo/3\" is out of range!");

      static_assert(is_const<remove_reference<decltype(JsonPointer(L"/0"s).Navigate(json0))>::type>::value, "JsonObject::At(JsonPointer) return type must be Json const&");
    }

    TEST_METHOD(TestOperatorInsertion)
    {
      auto ptr = JsonPointer();
      wstringstream is;

      is << ptr;
      Assert::AreEqual(L""s, is.str());

      ptr = JsonPointer(L"//foo/0"s);
      is.str(L""s);
      is << ptr;
      Assert::AreEqual(L"//foo/0"s, is.str());

      ptr = JsonPointer(L"////foo/asd\nasd/0"s);
      is.str(L""s);
      is << ptr;
      Assert::AreEqual(L"////foo/asd\nasd/0"s, is.str());
    }

    TEST_METHOD(TestOperatorExtraction)
    {
      auto ptr = JsonPointer();
      wstringstream is(L""s);

      is >> ptr;
      Assert::AreEqual(L""s, is.str());

      is.str(L"//foo/0"s);
      is >> ptr;
      Assert::AreEqual(L"//foo/0"s, is.str());

      is.str(L"////foo/asd\nasd/0"s);
      is >> ptr;
      Assert::AreEqual(L"////foo/asd\nasd/0"s, is.str());

      is.str(L"|foo/0"s);
      ExpectException<exception>([&] { is >> ptr; }, "Path must be empty or start with '/'! It started with \"|foo\"!");
      is.str(L"/foo/asd~"s);
      ExpectException<exception>([&] { is >> ptr; }, "Invalid reference token \"asd~\"! After a '~', there must be either '0' or '1'!");
      is.str(L"/foo/asd~2"s);
      ExpectException<exception>([&] { is >> ptr; }, "Invalid reference token \"asd~2\"! After a '~', there must be either '0' or '1'!");
    }

    TEST_METHOD(TestOperatorEqual)
    {
      Assert::IsTrue (JsonPointer(L"/0"s) == JsonPointer(L"/0"s));
      Assert::IsFalse(JsonPointer(L"/0"s) == JsonPointer(L"/1"s));
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      Assert::IsFalse(JsonPointer(L"/0"s) != JsonPointer(L"/0"s));
      Assert::IsTrue (JsonPointer(L"/0"s) != JsonPointer(L"/1"s));
    }

    TEST_METHOD(TestOperatorDivide)
    {
      auto left = JsonPointer(L"/foo/bar"s);
      auto right = JsonPointer(L"/bar/foo"s);
      auto result = left / right;
      Assert::AreEqual(L"/foo/bar/bar/foo"s, result.Path());
      Assert::AreEqual(L"/foo/bar"s, left.Path());
      Assert::AreEqual(L"/bar/foo"s, right.Path());

      result = left / L"0"s / L"1"s;
      Assert::AreEqual(L"/foo/bar/0/1"s, result.Path());
      Assert::AreEqual(L"/foo/bar"s, left.Path());

      ExpectException<exception>([&] { auto tmp = left / L"asd~"s; }, "Invalid reference token \"asd~\"! After a '~', there must be either '0' or '1'!");
      ExpectException<exception>([&] { auto tmp = left / L"asd~2"s; }, "Invalid reference token \"asd~2\"! After a '~', there must be either '0' or '1'!");

      result = left / 0 / 1;
      Assert::AreEqual(L"/foo/bar/0/1"s, result.Path());
      Assert::AreEqual(L"/foo/bar"s, left.Path());
    }

    TEST_METHOD(TestOperatorDivideAssign)
    {
      auto left = JsonPointer(L"/foo/bar"s);
      auto right = JsonPointer(L"/bar/foo"s);
      left /= right;
      Assert::AreEqual(L"/foo/bar/bar/foo"s, left.Path());
      Assert::AreEqual(L"/bar/foo"s, right.Path());

      (left /= L"0"s) /= L"1"s;
      Assert::AreEqual(L"/foo/bar/bar/foo/0/1"s, left.Path());

      ExpectException<exception>([&] { auto tmp = left /= L"asd~"s; }, "Invalid reference token \"asd~\"! After a '~', there must be either '0' or '1'!");
      ExpectException<exception>([&] { auto tmp = left /= L"asd~2"s; }, "Invalid reference token \"asd~2\"! After a '~', there must be either '0' or '1'!");

      (left /= 0) /= 1;
      Assert::AreEqual(L"/foo/bar/bar/foo/0/1/0/1"s, left.Path());
    }

    TEST_METHOD(TestLiteral)
    {
      auto ptr = L"/foo/0"_JsonPointer;
      Assert::AreEqual(L"/foo/0"s, ptr.Path());

      ptr = L""_JsonPointer;
      Assert::AreEqual(L""s, ptr.Path());

      ExpectException<exception>([&] { auto tmp = L"|foo/0"_JsonPointer; }, "Path must be empty or start with '/'! It started with \"|foo\"!");
      ExpectException<exception>([&] { auto tmp = L"/foo/asd~"_JsonPointer; }, "Invalid reference token \"asd~\"! After a '~', there must be either '0' or '1'!");
      ExpectException<exception>([&] { auto tmp = L"/foo/asd~2"_JsonPointer; }, "Invalid reference token \"asd~2\"! After a '~', there must be either '0' or '1'!");
    }
  };
}