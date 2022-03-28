#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include "JsonArray.h"
#include "Json.h"
#include "JsonPointer.h"

#include <vector>
#include <memory>
#include <iostream>
#include <functional>

namespace Json4CPP
{
  class JSON_API JsonPatch
  {
  private:
    class OperationBase
    {
    protected:
      JsonPointer _path;
    public:
      OperationBase(Json const& json);
      virtual explicit operator JsonObject() const = 0;
      virtual void Execute(Json& json) const = 0;
    };

    class OperationAdd final : public OperationBase
    {
    protected:
      Json _value;
    public:
      OperationAdd(Json const& json);
      virtual explicit operator JsonObject() const;
      virtual void Execute(Json& json) const;
    };

    class OperationRemove final : public OperationBase
    {
    protected:

    public:
      OperationRemove(Json const& json);
      virtual explicit operator JsonObject() const;
      virtual void Execute(Json& json) const;
    };

    class OperationReplace final : public OperationBase
    {
    protected:
      Json _value;
    public:
      OperationReplace(Json const& json);
      virtual explicit operator JsonObject() const;
      virtual void Execute(Json& json) const;
    };

    class OperationMove final : public OperationBase
    {
    protected:
      JsonPointer _from;
    public:
      OperationMove(Json const& json);
      virtual explicit operator JsonObject() const;
      virtual void Execute(Json& json) const;
    };

    class OperationCopy final : public OperationBase
    {
    protected:
      JsonPointer _from;
    public:
      OperationCopy(Json const& json);
      virtual explicit operator JsonObject() const;
      virtual void Execute(Json& json) const;
    };

    class OperationTest final : public OperationBase
    {
    protected:
      Json _value;
    public:
      OperationTest(Json const& json);
      virtual explicit operator JsonObject() const;
      virtual void Execute(Json& json) const;
    };

#pragma warning(suppress: 4251)
    std::vector<std::shared_ptr<OperationBase>> _operations;
  public:
    JsonPatch();
    JsonPatch(JsonArray const& array);

    Json Apply(Json const& json) const;

    explicit operator JsonArray() const;

    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonPatch const& patch);
    JSON_API friend std::wistream& operator>>(std::wistream& is, JsonPatch      & patch);

    JSON_API friend bool operator==(JsonPatch const& left, JsonPatch const& right);
    JSON_API friend bool operator!=(JsonPatch const& left, JsonPatch const& right);
  };

  JSON_API JsonPatch operator""_JsonPatch(wchar_t const* value, std::size_t size);
}