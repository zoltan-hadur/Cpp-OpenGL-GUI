#include "stdafx.h"

#include "JsonIndentChar.h"

namespace Json4CPP
{
  JsonIndentChar::JsonIndentChar(wchar_t c)
  {
    _char = c;
  }

  bool JsonIndentChar::IsActive(std::wostream& os)
  {
    return static_cast<bool>(os.iword(_activeIndex));
  }

  wchar_t JsonIndentChar::GetChar(std::wostream& os)
  {
    return static_cast<wchar_t>(os.iword(_charIndex));
  }

  void JsonIndentChar::ResetState(std::wostream& os)
  {
    os.iword(_activeIndex) = false;
    os.iword(_charIndex)   = 0;
  }

  std::wostream& operator<<(std::wostream& os, JsonIndentChar const& jsonIndentChar)
  {
    os.iword(jsonIndentChar._activeIndex) |= true;
    os.iword(jsonIndentChar._charIndex)   |= jsonIndentChar._char;
    return os;
  }
}