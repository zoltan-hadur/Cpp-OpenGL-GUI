#include "stdafx.h"

#include "Json.h"
#include "JsonBuilder.h"
#include "Helper.h"

using namespace std;

namespace Json4CPP
{
  JsonBuilder::JsonBuilder() : JsonBuilder(nullptr) {}

  JsonBuilder::JsonBuilder(VALUE                          value ) { visit([&](auto arg) { _value = arg; }, value);              }
  JsonBuilder::JsonBuilder(nullptr_t                      value ) { _value = value;                                             }
  JsonBuilder::JsonBuilder(const wchar_t*                 value ) { _value = wstring(value);                                    }
  JsonBuilder::JsonBuilder(wstring                        value ) { _value = value;                                             }
  JsonBuilder::JsonBuilder(bool                           value ) { _value = value;                                             }
  JsonBuilder::JsonBuilder(char                           value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(int8_t                         value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(uint8_t                        value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(int16_t                        value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(uint16_t                       value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(int32_t                        value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(uint32_t                       value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(int64_t                        value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(uint64_t                       value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(float                          value ) { _value = double(value);                                     }
  JsonBuilder::JsonBuilder(double                         value ) { _value = value;                                             }
  JsonBuilder::JsonBuilder(JsonObject                     value ) { _value = value;                                             }
  JsonBuilder::JsonBuilder(JsonArray                      value ) { _value = value;                                             }
  JsonBuilder::JsonBuilder(Json                           value ) : JsonBuilder(value._value) {                                 }
  JsonBuilder::JsonBuilder(initializer_list<JsonBuilder>  values) { _value = vector<JsonBuilder>(values.begin(), values.end()); }
  JsonBuilder::JsonBuilder(vector<JsonBuilder>            values) { _value = values;                                            }

  JsonBuilderType JsonBuilder::Type() const { return GetType(_value); }
  bool JsonBuilder::Is(JsonBuilderType type) const { return Type() == type; }
}