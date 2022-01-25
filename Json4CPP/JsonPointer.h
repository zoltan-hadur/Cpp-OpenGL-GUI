#pragma once

#ifdef JSON4CPP_EXPORTS
#define JSON_API __declspec(dllexport)
#else
#define JSON_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include <iostream>

namespace Json4CPP
{
  class JSON_API Json;
  class JSON_API JsonObject;
  class JSON_API JsonArray;

  class JSON_API JsonPointer
  {
  private:
#pragma warning(suppress: 4251)
    std::vector<std::wstring> _encodedTokens;
    std::vector<std::wstring> ExtractEncodedTokens(std::wstring const& path) const;
  public:
    JsonPointer();
    JsonPointer(wchar_t      const* path);
    JsonPointer(std::wstring const& path);

    std::wstring Path() const;
    void Path(std::wstring const& path);
    JsonPointer Parent() const;
    bool Empty() const;

    Json      & Navigate(Json      & json) const;
    Json const& Navigate(Json const& json) const;

    JSON_API friend std::wostream& operator<<(std::wostream& os, JsonPointer const& ptr);
    JSON_API friend std::wistream& operator>>(std::wistream& is, JsonPointer      & ptr);

    JSON_API friend bool         operator==(JsonPointer const& left, JsonPointer const& right);
    JSON_API friend bool         operator!=(JsonPointer const& left, JsonPointer const& right);
    JSON_API friend JsonPointer  operator/ (JsonPointer const& left, JsonPointer const& right);
    JSON_API friend JsonPointer  operator/ (JsonPointer const& left, std::wstring && encodedToken);
    JSON_API friend JsonPointer  operator/ (JsonPointer const& left, int64_t arrayIndex);
    JSON_API friend JsonPointer& operator/=(JsonPointer      & left, JsonPointer const& right);
    JSON_API friend JsonPointer& operator/=(JsonPointer      & left, std::wstring && encodedToken);
    JSON_API friend JsonPointer& operator/=(JsonPointer      & left, int64_t arrayIndex);
  };

  JSON_API JsonPointer operator""_JsonPointer(wchar_t const* value, std::size_t size);
}