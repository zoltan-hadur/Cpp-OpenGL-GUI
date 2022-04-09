#include "stdafx.h"

#include "JsonIndentSize.h"

namespace Json4CPP
{
  JsonIndentSize::JsonIndentSize(uint8_t size)
  {
    _size = size;
  }

  bool JsonIndentSize::IsActive(std::wostream& os)
  {
    return static_cast<bool>(os.iword(_activeIndex));
  }

  uint8_t JsonIndentSize::GetSize(std::wostream& os)
  {
    return static_cast<uint8_t>(os.iword(_sizeIndex));
  }

  void JsonIndentSize::ResetState(std::wostream& os)
  {
    os.iword(_activeIndex) = false;
    os.iword(_sizeIndex)   = 0;
  }

  std::wostream& operator<<(std::wostream& os, JsonIndentSize const& jsonIndentSize)
  {
    os.iword(jsonIndentSize._activeIndex) |= true;
    os.iword(jsonIndentSize._sizeIndex)   |= jsonIndentSize._size;
    return os;
  }
}