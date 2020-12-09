#include "stdafx.h"

#include "JsonIndentSize.h"

using namespace std;

namespace Json4CPP
{
  JsonIndentSize::JsonIndentSize(uint8_t size)
  {
    _size = size;
  }

  bool JsonIndentSize::IsActive(wostream& os)
  {
    return os.iword(_valueIndex) & 0b100000000;
  }

  uint8_t JsonIndentSize::GetSize(wostream& os)
  {
    return os.iword(_valueIndex) & 0b011111111;
  }

  void JsonIndentSize::ResetState(wostream& os)
  {
    os.iword(_valueIndex) = 0;
  }

  wostream& operator<<(wostream& os, JsonIndentSize const& jsonIndentSize)
  {
    os.iword(jsonIndentSize._valueIndex) |= 0b100000000;                        // Set active flag
    os.iword(jsonIndentSize._valueIndex) |= 0b011111111 & jsonIndentSize._size; // Set size
    return os;
  }
}