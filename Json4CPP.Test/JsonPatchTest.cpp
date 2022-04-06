#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Json4CPP;

namespace Json4CPP::Test
{
  TEST_CLASS(JsonPatchTest)
  {
    TEST_METHOD(TestConstructor)
    {
      auto patch = JsonPatch();
      Assert::AreEqual(JsonArray(), (JsonArray)patch);
      auto json = Json{
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
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
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
      }, result);
    }

    TEST_METHOD(TestConstructorJsonArray_ValueIsNotAnObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { 1, 3, 3, 7 } }); }, "Value: [1,3,3,7] is not an object!");
    }

    TEST_METHOD(TestConstructorJsonArray_KeyOpIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } } }); }, "Key 'op' is missing from object: {\"path\":\"/baz\",\"value\":\"qux\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OpIsNotAValidOperation)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"asdasd"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } } }); }, "Operation \"asdasd\" is not a valid operation! It must be one of \"add\", \"remove\", \"replace\", \"move\", \"copy\", or \"test\".");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationAdd_KeyPathIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"add"s }, { L"value"s, L"qux"s } } }); }, "Key 'path' is missing from object: {\"op\":\"add\",\"value\":\"qux\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationAdd_KeyPathIsNotString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"add"s }, { L"path"s, 1337 }, { L"value"s, L"qux"s } } }); }, "Path: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationAdd_KeyValueIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"add"s }, { L"path"s, L"/baz"s } } }); }, "Key 'value' is missing from object: {\"op\":\"add\",\"path\":\"/baz\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationAdd_)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } }
      });
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } }
      }, (JsonArray)patch);
    }

    TEST_METHOD(TestConstructorJsonArray_OperationRemove_KeyPathIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"remove"s } } }); }, "Key 'path' is missing from object: {\"op\":\"remove\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationRemove_KeyPathIsNotString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"remove"s }, { L"path"s, 1337 } } }); }, "Path: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationRemove_)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/baz"s } }
      });
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/baz"s } }
      }, (JsonArray)patch);
    }

    TEST_METHOD(TestConstructorJsonArray_OperationReplace_KeyPathIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"replace"s }, { L"value"s, L"qux"s } } }); }, "Key 'path' is missing from object: {\"op\":\"replace\",\"value\":\"qux\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationReplace_KeyPathIsNotAString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"replace"s }, { L"path"s, 1337 }, { L"value"s, L"qux"s } } }); }, "Path: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationReplace_KeyValueIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"replace"s }, { L"path"s, L"/baz"s } } }); }, "Key 'value' is missing from object: {\"op\":\"replace\",\"path\":\"/baz\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationReplace_)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } }
      });
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } }
      }, (JsonArray)patch);
    }

    TEST_METHOD(TestConstructorJsonArray_OperationMove_KeyPathIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"move"s }, { L"from"s, L"/qux"s } } }); }, "Key 'path' is missing from object: {\"op\":\"move\",\"from\":\"/qux\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationMove_KeyPathIsNotAString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"move"s }, { L"path"s, 1337 }, { L"from"s, L"/qux"s } } }); }, "Path: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationMove_KeyFromIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"move"s }, { L"path"s, L"/baz"s } } }); }, "Key 'from' is missing from object: {\"op\":\"move\",\"path\":\"/baz\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationMove_KeyFromIsNotAString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"move"s }, { L"path"s, L"/baz"s }, { L"from"s, 1337 } } }); }, "From: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationMove_KeyFromIsAPrefixOfKeyPath)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"move"s }, { L"path"s, L"/baz/foo"s }, { L"from"s, L"/baz"s } } }); }, "Location 'from': \"/baz\" is a prefix of location 'path': \"/baz/foo\"! A location cannot be moved into one of its children.");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationMove_)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"path"s, L"/baz"s }, { L"from"s, L"/qux"s } }
      });
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"move"s }, { L"path"s, L"/baz"s }, { L"from"s, L"/qux"s } }
      }, (JsonArray)patch);
    }

    TEST_METHOD(TestConstructorJsonArray_OperationCopy_KeyPathIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"copy"s }, { L"from"s, L"/qux"s } } }); }, "Key 'path' is missing from object: {\"op\":\"copy\",\"from\":\"/qux\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationCopy_KeyPathIsNotAString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"copy"s }, { L"path"s, 1337 }, { L"from"s, L"/qux"s } } }); }, "Path: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationCopy_KeyFromIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"copy"s }, { L"path"s, L"/baz"s } } }); }, "Key 'from' is missing from object: {\"op\":\"copy\",\"path\":\"/baz\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationCopy_KeyFromIsNotAString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"copy"s }, { L"path"s, L"/baz"s }, { L"from"s, 1337 } } }); }, "From: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationCopy_)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"path"s, L"/baz"s }, { L"from"s, L"/qux"s } }
      });
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"copy"s }, { L"path"s, L"/baz"s }, { L"from"s, L"/qux"s } }
      }, (JsonArray)patch);
    }

    TEST_METHOD(TestConstructorJsonArray_OperationTest_KeyPathIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"test"s }, { L"value"s, L"qux"s } } }); }, "Key 'path' is missing from object: {\"op\":\"test\",\"value\":\"qux\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationTest_KeyPathIsNotAString)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"test"s }, { L"path"s, 1337 }, { L"value"s, L"qux"s } } }); }, "Path: 1337 is not a string!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationTest_KeyValueIsMissingFromObject)
    {
      ExpectException<exception>([]() { auto patch = JsonPatch(JsonArray{ { { L"op"s, L"test"s }, { L"path"s, L"/baz"s } } }); }, "Key 'value' is missing from object: {\"op\":\"test\",\"path\":\"/baz\"}!");
    }

    TEST_METHOD(TestConstructorJsonArray_OperationTest_)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } }
      });
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } }
      }, (JsonArray)patch);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationIsTheRoot)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L""s }, { L"value"s, L"asdasd"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>(L"asdasd"s, result);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentDoesNotExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/b/c/d"s }, { L"value"s, L"asdasd"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"add\",\"path\":\"/b/c/d\",\"value\":\"asdasd\"} failed! Parent: \"/b/c\" does not exist!");
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnArrayAndTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/-"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 3, 7, L"1337"s }, result);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnObjectButTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/-"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 },
        { L"-"s, L"1337"s }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndex)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/1"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, L"1337"s, 3, 3, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSize)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/4"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 3, 7, L"1337"s }, result);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSizePlusOne)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/5"s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"add\",\"path\":\"/5\",\"value\":\"1337\"} failed! Target location: \"/5\" is out of range!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnArrayAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/a"s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"add\",\"path\":\"/a\",\"value\":\"1337\"} failed! Target location's parent: \"\" is an array, thus target : \"a\" must be either an index or '-'!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnObjectAndTargetLocationDoesNotExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/c"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 },
        { L"c"s, L"1337"s }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnObjectAndTargetLocationDoesExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/b"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, L"1337"s }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationAdd_TargetLocationsParentIsAnObjectAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s }, { L"path"s, L"/1"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 },
        { L"1"s, L"1337"s }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationIsTheRoot)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L""s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"remove\",\"path\":\"\"} failed! Target location: \"\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationDoesNotExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/a/b"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"remove\",\"path\":\"/a/b\"} failed! Target location: \"/a/b\" does not exist!");
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnArrayAndTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/-"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"remove\",\"path\":\"/-\"} failed! Target location: \"/-\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnObjectButTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"-"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/-"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"-"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/1"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 4, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSize)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/4"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"remove\",\"path\":\"/4\"} failed! Target location: \"/4\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSizePlusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/5"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"remove\",\"path\":\"/5\"} failed! Target location: \"/5\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnArrayAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/a"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"remove\",\"path\":\"/a\"} failed! Target location: \"/a\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnObjectAndTargetLocationDoesExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/b"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationRemove_TargetLocationsParentIsAnObjectAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"1"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s }, { L"path"s, L"/1"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"1"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationIsTheRoot)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L""s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"replace\",\"path\":\"\",\"value\":\"1337\"} failed! Target location: \"\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationDoesNotExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/a/b"s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"replace\",\"path\":\"/a/b\",\"value\":\"1337\"} failed! Target location: \"/a/b\" does not exist!");
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnArrayAndTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/-"s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"replace\",\"path\":\"/-\",\"value\":\"1337\"} failed! Target location: \"/-\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnObjectButTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{
        { L"-"s, 1 },
        { L"a"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/-"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"-"s, L"1337"s },
        { L"a"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"-"s, 1 },
        { L"a"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/1"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, L"1337"s, 4, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSizeMinusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/3"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 4, L"1337"s }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSize)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/4"s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"replace\",\"path\":\"/4\",\"value\":\"1337\"} failed! Target location: \"/4\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSizePlusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/5"s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"replace\",\"path\":\"/5\",\"value\":\"1337\"} failed! Target location: \"/5\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnArrayAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/a"s }, { L"value"s, L"1337"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"replace\",\"path\":\"/a\",\"value\":\"1337\"} failed! Target location: \"/a\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnObjectAndTargetLocationDoesExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/a"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, L"1337"s },
        { L"b"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationReplace_TargetLocationsParentIsAnObjectAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"1"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"replace"s }, { L"path"s, L"/1"s }, { L"value"s, L"1337"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"1"s, L"1337"s },
        { L"b"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationDoesNotExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/a/b"s }, { L"path"s, L"/c"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"move\",\"from\":\"/a/b\",\"path\":\"/c\"} failed! From location: \"/a/b\" does not exist!");
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnArrayAndFromLocationIsTheEndOfTheArray)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/-"s }, { L"path"s, L"/1"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"move\",\"from\":\"/-\",\"path\":\"/1\"} failed! From location: \"/-\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnObjectAndFromLocationIsTheEndOfTheArray)
    {
      auto json = Json{
        { L"-"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/-"s }, { L"path"s, L"/1"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"b"s, 2 },
        { L"1"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"-"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/-"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 4, 7, 3 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsAnIndexWhichEqualToTheArraysSizeMinusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/3"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 4, 7, 3 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsAnIndexWhichEqualToTheArraysSize)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/4"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"move\",\"from\":\"/1\",\"path\":\"/4\"} failed! Target location: \"/4\" is out of range!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsAnIndexWhichEqualToTheArraysSizePlusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/5"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"move\",\"from\":\"/1\",\"path\":\"/5\"} failed! Target location: \"/5\" is out of range!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/a"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"move\",\"from\":\"/1\",\"path\":\"/a\"} failed! Target location's parent: \"\" is an array, thus target : \"a\" must be either an index or '-'!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnObjectAndFromLocationIsAnIndexAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"1"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/2"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"b"s, 2 },
        { L"2"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnObjectAndFromLocationIsAnIndexAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{
        { L"1"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/asd"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"b"s, 2 },
        { L"asd"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnObjectAndFromLocationIsNotAnIndexAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/a"s }, { L"path"s, L"/2"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"b"s, 2 },
        { L"2"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnObjectAndFromLocationIsNotAnIndexAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/a"s }, { L"path"s, L"/asd"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"b"s, 2 },
        { L"asd"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsTheSameIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/1"s }, { L"path"s, L"/1"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationMove_FromLocationsParentIsAnObjectAndFromLocationIsNotAnIndexAndTargetLocationIsTheSame)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"move"s }, { L"from"s, L"/a"s }, { L"path"s, L"/a"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"b"s, 2 },
        { L"a"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationIsTheRootWhichIsAnArrayAndTargetLocationIsTheRoot)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L""s }, { L"path"s, L""s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationIsTheRootWhichIsAnObjectAndTargetLocationIsTheRoot)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L""s }, { L"path"s, L""s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationIsTheRootWhichIsAnArrayAndTargetLocationIsTheEndOnTheArray)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L""s }, { L"path"s, L"/-"s } }
        });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 3, 7, { 1, 3, 3, 7 } }, result);
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationIsTheRootWhichIsAnObjectAndTargetLocationIsTheEndOnTheArray)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L""s }, { L"path"s, L"/-"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 },
        { L"-"s, {
          { L"a"s, 1 },
          { L"b"s, 2 }
        } }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsAnIndexWhichEqualToTheArraysSizeMinusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/1"s }, { L"path"s, L"/3"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 4, 3, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsAnIndexWhichEqualToTheArraysSize)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/1"s }, { L"path"s, L"/4"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 4, 7, 3 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsAnIndexWhichEqualToTheArraysSizePlusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/1"s }, { L"path"s, L"/5"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"copy\",\"from\":\"/1\",\"path\":\"/5\"} failed! Target location: \"/5\" is out of range!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/1"s }, { L"path"s, L"/a"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"copy\",\"from\":\"/1\",\"path\":\"/a\"} failed! Target location's parent: \"\" is an array, thus target : \"a\" must be either an index or '-'!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnObjectAndFromLocationIsAnIndexAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"1"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/1"s }, { L"path"s, L"/2"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 },
        { L"2"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnObjectAndFromLocationIsAnIndexAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{
        { L"1"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/1"s }, { L"path"s, L"/asd"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 },
        { L"asd"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnObjectAndFromLocationIsNotAnIndexAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/a"s }, { L"path"s, L"/2"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 },
        { L"2"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnObjectAndFromLocationIsNotAnIndexAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/a"s }, { L"path"s, L"/asd"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 },
        { L"asd"s, 1 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnArrayAndFromLocationIsAnIndexAndTargetLocationIsTheSameIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/1"s }, { L"path"s, L"/1"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 3, 4, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationCopy_FromLocationsParentIsAnObjectAndFromLocationIsNotAnIndexAndTargetLocationIsTheSame)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"copy"s }, { L"from"s, L"/a"s }, { L"path"s, L"/a"s } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationIsTheRoot)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L""s }, { L"value"s, { 1, 3, 4, 7 } } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationDoesNotExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/a/b"s }, { L"value"s, L"asd"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/a/b\",\"value\":\"asd\"} failed! Target location: \"/a/b\" does not exist!");
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnArrayAndTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{ 1, 3, 3, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/-"s }, { L"value"s, L"asd"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/-\",\"value\":\"asd\"} failed! Target location: \"/-\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 3, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnObjectButTargetLocationIsTheEndOfTheArray)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"-"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/-"s }, { L"value"s, 2 } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"-"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"-"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/1"s }, { L"value"s, 3 } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSizeMinusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/3"s }, { L"value"s, 7 } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, result);
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSize)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/4"s }, { L"value"s, L"asd"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/4\",\"value\":\"asd\"} failed! Target location: \"/4\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexWhichEqualToTheArraysSizePlusOne)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/5"s }, { L"value"s, L"asd"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/5\",\"value\":\"asd\"} failed! Target location: \"/5\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnArrayAndTargetLocationIsNotAnIndex)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/a"s }, { L"value"s, L"asd"s } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/a\",\"value\":\"asd\"} failed! Target location: \"/a\" does not exist!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnObjectAndTargetLocationDoesExist)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/a"s }, { L"value"s, 1 } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnObjectAndTargetLocationIsAnIndex)
    {
      auto json = Json{
        { L"1"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/1"s }, { L"value"s, 1 } }
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 }
      }, result);
      Assert::AreEqual<Json>({
        { L"1"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnArrayAndTargetLocationIsAnIndexAndValueDoesNotMatch)
    {
      auto json = Json{ 1, 3, 4, 7 };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/1"s }, { L"value"s, 4 } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/1\",\"value\":4} failed! Target location value: 3 is not equal to 4!");
      Assert::AreEqual<Json>({ 1, 3, 4, 7 }, json);
    }

    TEST_METHOD(TestApply_OperationTest_TargetLocationsParentIsAnObjectAndTargetLocationIsNotAnIndexAndValueDoesNotMatch)
    {
      auto json = Json{
        { L"a"s, 1 },
        { L"b"s, 2 }
      };
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"test"s }, { L"path"s, L"/a"s }, { L"value"s, 2 } }
      });
      ExpectException<exception>([&]() { auto result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/a\",\"value\":2} failed! Target location value: 1 is not equal to 2!");
      Assert::AreEqual<Json>({
        { L"a"s, 1 },
        { L"b"s, 2 }
      }, json);
    }

    TEST_METHOD(TestApply_AllOperation)
    {
      auto json = Json{
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
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },

        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/String"s      }, { L"value"s, L"Test"s         } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Boolean"s     }, { L"value"s, true             } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key1"s }, { L"value"s, nullptr          } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key2"s }, { L"value"s, 2                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/0"s     }, { L"value"s, 1                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/1"s     }, { L"value"s, 3                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/2"s     }, { L"value"s, 13.37            } },
      });
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"Null", nullptr },
        { L"String", L"Test" },
        { L"Boolean", true },
        { L"Integer", 31337 },
        { L"Object", {
          { L"Key1", nullptr },
          { L"Key2", 2 },
          { L"Key3", 3 } }
        },
        { L"Array", { 1, 3, 13.37 } },
      }, result);
      Assert::AreEqual<Json>({
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
      }, json);
    }

    TEST_METHOD(TestConversionOperatorJsonArray)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },

        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/String"s      }, { L"value"s, L"Test"s         } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Boolean"s     }, { L"value"s, true             } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key1"s }, { L"value"s, nullptr          } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key2"s }, { L"value"s, 2                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/0"s     }, { L"value"s, 1                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/1"s     }, { L"value"s, 3                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/2"s     }, { L"value"s, 13.37            } },
      });
      auto result = (JsonArray)patch;
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },

        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/String"s      }, { L"value"s, L"Test"s         } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Boolean"s     }, { L"value"s, true             } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key1"s }, { L"value"s, nullptr          } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key2"s }, { L"value"s, 2                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/0"s     }, { L"value"s, 1                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/1"s     }, { L"value"s, 3                } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Array/2"s     }, { L"value"s, 13.37            } },
      }, result);
    }

    TEST_METHOD(TestOperatorInsertion)
    {
      auto patch = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      wostringstream os;
      os << JsonIndentChar(L'\t') << JsonIndentSize(1) << patch;
      auto result = os.str();
      Assert::AreEqual(
        L"["                                "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"add\","             "\r\n"
         "\t\t\"path\": \"/Object/Key3\","  "\r\n"
         "\t\t\"value\": 3"                 "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"remove\","          "\r\n"
         "\t\t\"path\": \"/Array/1\""       "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"replace\","         "\r\n"
         "\t\t\"path\": \"/Integer\","      "\r\n"
         "\t\t\"value\": 31337"             "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"move\","            "\r\n"
         "\t\t\"from\": \"/Real\","         "\r\n"
         "\t\t\"path\": \"/Array/-\""       "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"copy\","            "\r\n"
         "\t\t\"from\": \"/Null\","         "\r\n"
         "\t\t\"path\": \"/Object/Key1\""   "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"test\","            "\r\n"
         "\t\t\"path\": \"/Null\","         "\r\n"
         "\t\t\"value\": null"              "\r\n"
         "\t}"                              "\r\n"
         "]"s, result);
    }

    TEST_METHOD(TestOperatorExtraction)
    {
      auto is = wistringstream(
        L"["                                "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"add\","             "\r\n"
         "\t\t\"path\": \"/Object/Key3\","  "\r\n"
         "\t\t\"value\": 3"                 "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"remove\","          "\r\n"
         "\t\t\"path\": \"/Array/1\""       "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"replace\","         "\r\n"
         "\t\t\"path\": \"/Integer\","      "\r\n"
         "\t\t\"value\": 31337"             "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"move\","            "\r\n"
         "\t\t\"from\": \"/Real\","         "\r\n"
         "\t\t\"path\": \"/Array/-\""       "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"copy\","            "\r\n"
         "\t\t\"from\": \"/Null\","         "\r\n"
         "\t\t\"path\": \"/Object/Key1\""   "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"test\","            "\r\n"
         "\t\t\"path\": \"/Null\","         "\r\n"
         "\t\t\"value\": null"              "\r\n"
         "\t}"                              "\r\n"
         "]"s);
      JsonPatch result;
      is >> result;
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      }, (JsonArray)result);
    }

    TEST_METHOD(TestOperatorEqual)
    {
      auto patch1 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      auto patch2 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      Assert::IsTrue(patch1 == patch2);
    }

    TEST_METHOD(TestOperatorEqualArrayOrderMatters)
    {
      auto patch1 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      auto patch2 = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      Assert::IsFalse(patch1 == patch2);
    }

    TEST_METHOD(TestOperatorEqualObjectOrderDoesNotMatter)
    {
      auto patch1 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      auto patch2 = JsonPatch(JsonArray{
        { { L"path"s, L"/Object/Key3"s }, { L"op"s, L"add"s     }, { L"value"s, 3                } },
        { { L"path"s, L"/Array/1"s     }, { L"op"s, L"remove"s  }                                  },
        { { L"path"s, L"/Integer"s     }, { L"value"s, 31337            }, { L"op"s, L"replace"s } },
        { { L"op"s, L"move"s    }, { L"path"s , L"/Array/-"s     }, { L"from"s, L"/Real"s        } },
        { { L"path"s , L"/Object/Key1"s }, { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        } },
        { { L"value"s, nullptr          }, { L"path"s, L"/Null"s        }, { L"op"s, L"test"s    } }
      });
      Assert::IsTrue(patch1 == patch2);
    }

    TEST_METHOD(TestOperatorNotEqual)
    {
      auto patch1 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      auto patch2 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      Assert::IsFalse(patch1 != patch2);
    }

    TEST_METHOD(TestOperatorNotEqualArrayOrderMatters)
    {
      auto patch1 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      auto patch2 = JsonPatch(JsonArray{
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      Assert::IsTrue(patch1 != patch2);
    }

    TEST_METHOD(TestOperatorNotEqualObjectOrderDoesNotMatter)
    {
      auto patch1 = JsonPatch(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      });
      auto patch2 = JsonPatch(JsonArray{
        { { L"path"s, L"/Object/Key3"s }, { L"op"s, L"add"s     }, { L"value"s, 3                } },
        { { L"path"s, L"/Array/1"s     }, { L"op"s, L"remove"s  }                                  },
        { { L"path"s, L"/Integer"s     }, { L"value"s, 31337            }, { L"op"s, L"replace"s } },
        { { L"op"s, L"move"s    }, { L"path"s , L"/Array/-"s     }, { L"from"s, L"/Real"s        } },
        { { L"path"s , L"/Object/Key1"s }, { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        } },
        { { L"value"s, nullptr          }, { L"path"s, L"/Null"s        }, { L"op"s, L"test"s    } }
      });
      Assert::IsFalse(patch1 != patch2);
    }

    TEST_METHOD(TestLiteral)
    {
      auto result = 
        L"["                                "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"add\","             "\r\n"
         "\t\t\"path\": \"/Object/Key3\","  "\r\n"
         "\t\t\"value\": 3"                 "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"remove\","          "\r\n"
         "\t\t\"path\": \"/Array/1\""       "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"replace\","         "\r\n"
         "\t\t\"path\": \"/Integer\","      "\r\n"
         "\t\t\"value\": 31337"             "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"move\","            "\r\n"
         "\t\t\"from\": \"/Real\","         "\r\n"
         "\t\t\"path\": \"/Array/-\""       "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"copy\","            "\r\n"
         "\t\t\"from\": \"/Null\","         "\r\n"
         "\t\t\"path\": \"/Object/Key1\""   "\r\n"
         "\t},"                             "\r\n"
         "\t{"                              "\r\n"
         "\t\t\"op\": \"test\","            "\r\n"
         "\t\t\"path\": \"/Null\","         "\r\n"
         "\t\t\"value\": null"              "\r\n"
         "\t}"                              "\r\n"
         "]"_JsonPatch;
      Assert::AreEqual(JsonArray{
        { { L"op"s, L"add"s     }, { L"path"s, L"/Object/Key3"s }, { L"value"s, 3                } },
        { { L"op"s, L"remove"s  }, { L"path"s, L"/Array/1"s     }                                  },
        { { L"op"s, L"replace"s }, { L"path"s, L"/Integer"s     }, { L"value"s, 31337            } },
        { { L"op"s, L"move"s    }, { L"from"s, L"/Real"s        }, { L"path"s , L"/Array/-"s     } },
        { { L"op"s, L"copy"s    }, { L"from"s, L"/Null"s        }, { L"path"s , L"/Object/Key1"s } },
        { { L"op"s, L"test"s    }, { L"path"s, L"/Null"s        }, { L"value"s, nullptr          } }
      }, (JsonArray)result);
    }

    // https://datatracker.ietf.org/doc/html/rfc6902
    TEST_METHOD(TestExamples)
    {
      // A.1.  Adding an Object Member
      auto json = Json{
        { L"foo"s, L"bar"s }
      };
      auto patch = JsonPatch{ JsonArray {
        { {L"op"s, L"add"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } }
      } };
      auto result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"baz"s, L"qux"s },
        { L"foo"s, L"bar"s }
      }, result);


      // A.2.  Adding an Array Element
      json = Json{
        { L"foo"s, { L"bar"s, L"baz"s } }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"add"s }, { L"path"s, L"/foo/1"s }, { L"value"s, L"qux"s } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, { L"bar"s, L"qux"s, L"baz"s } }
      }, result);


      // A.3.  Removing an Object Member
      json = Json{
        { L"baz"s, L"qux"s },
        { L"foo"s, L"bar"s }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"remove"s }, { L"path"s, L"/baz"s } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, L"bar"s }
      }, result);


      // A.4.  Removing an Array Element
      json = Json{
        { L"foo"s, { L"bar"s, L"qux"s, L"baz"s } }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"remove"s }, { L"path"s, L"/foo/1"s } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, { L"bar"s, L"baz"s } }
      }, result);


      // A.5.  Replacing a Value
      json = Json{
        { L"baz"s, L"qux"s },
        { L"foo"s, L"bar"s }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"replace"s }, { L"path"s, L"/baz"s }, { L"value"s, L"boo"s } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"baz"s, L"boo"s },
        { L"foo"s, L"bar"s }
      }, result);


      // A.6.  Moving a Value
      json = Json{
        { L"foo"s, {
          { L"bar"s, L"baz"s },
          { L"waldo"s, L"fred"s }
        } },
        { L"qux"s, {
          { L"corge"s, L"grault"s }
        } }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"move"s }, { L"from"s, L"/foo/waldo"s }, { L"path"s, L"/qux/thud"s } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, {
          { L"bar"s, L"baz"s }
        } },
        { L"qux"s, {
          { L"corge"s, L"grault"s },
          { L"thud"s, L"fred"s }
        } }
      }, result);


      // A.7.  Moving an Array Element
      json = Json{
        { L"foo"s, { L"all"s, L"grass"s, L"cows"s, L"eat"s } }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"move"s }, { L"from"s, L"/foo/1"s }, { L"path"s, L"/foo/3"s } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, { L"all"s, L"cows"s, L"eat"s, L"grass"s } }
      }, result);


      // A.8.  Testing a Value: Success
      json = Json{
        { L"baz"s, L"qux"s },
        { L"foo"s, { L"a"s, 2, L"c"s } }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"test"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s } },
        { {L"op"s, L"test"s }, { L"path"s, L"/foo/1"s }, { L"value"s, 2 } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"baz"s, L"qux"s },
        { L"foo"s, { L"a"s, 2, L"c"s } }
      }, result);


      // A.9.  Testing a Value: Error
      json = Json{
        { L"baz"s, L"qux"s }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"test"s }, { L"path"s, L"/baz"s }, { L"value"s, L"bar"s } }
      } };
      ExpectException<exception>([&]() { result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/baz\",\"value\":\"bar\"} failed! Target location value: \"qux\" is not equal to \"bar\"!");

      // A.10.  Adding a Nested Member Object
      json = Json{
        { L"foo"s, L"bar"s }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"add"s }, { L"path"s, L"/child"s }, { L"value"s, { { L"grandchild"s, JsonObject{ } } } } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, L"bar"s },
        { L"child"s, {
          { L"grandchild"s, JsonObject{ } }
        } }
      }, result);


      // A.11.  Ignoring Unrecognized Elements
      json = Json{
        { L"foo"s, L"bar"s }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"add"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s }, { L"xyz"s, 123 } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, L"bar"s },
        { L"baz"s, L"qux"s }
      }, result);


      // A.12.  Adding to a Nonexistent Target
      json = Json{
        { L"foo"s, L"bar"s }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"add"s }, { L"path"s, L"/baz/bat"s }, { L"value"s, L"qux"s } }
      } };
      ExpectException<exception>([&]() { result = json.Patch(patch); }, "Operation: {\"op\":\"add\",\"path\":\"/baz/bat\",\"value\":\"qux\"} failed! Parent: \"/baz\" does not exist!");


      // A.13.  Invalid JSON Patch Document
      ExpectException<exception>([&]()
        {
          patch = JsonPatch{ JsonArray {
            { {L"op"s, L"add"s }, { L"path"s, L"/baz"s }, { L"value"s, L"qux"s }, { L"op"s, L"remove"s } }
          } };
        }, "Duplicated key: \"op\"!");


      // A.14.  ~ Escape Ordering
      json = Json{
        { L"/"s, 9 },
        { L"~1"s, 10 }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"test"s }, { L"path"s, L"/~01"s }, { L"value"s, 10 } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"/"s, 9 },
        { L"~1"s, 10 }
      }, result);


      // A.15.  Comparing Strings and Numbers
      json = Json{
        { L"/"s, 9 },
        { L"~1"s, 10 }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"test"s }, { L"path"s, L"/~01"s }, { L"value"s, L"10"s } }
      } };
      ExpectException<exception>([&]() { result = json.Patch(patch); }, "Operation: {\"op\":\"test\",\"path\":\"/~01\",\"value\":\"10\"} failed! Target location value: 10 is not equal to \"10\"!");


      // A.16.  Adding an Array Value
      json = Json{
        { L"foo"s, { L"bar"s } }
      };
      patch = JsonPatch{ JsonArray {
        { {L"op"s, L"add"s }, { L"path"s, L"/foo/-"s }, { L"value"s, { L"abc"s, L"def"s } } }
      } };
      result = json.Patch(patch);
      Assert::AreEqual<Json>({
        { L"foo"s, { L"bar"s, { L"abc"s, L"def"s } } },
      }, result);
    }
  };
}