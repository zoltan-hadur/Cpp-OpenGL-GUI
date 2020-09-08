#include "stdafx.h"

#include "JsonIndentChar.h"

using namespace std;

namespace Json4CPP
{
  JsonIndentChar::JsonIndentChar(wchar_t c)
  {
    _char = c;
  }

  bool JsonIndentChar::IsActive(std::wostream& os)
  {
    return os.iword(_valueIndex) & 0b100000000;
  }

  wchar_t JsonIndentChar::GetChar(std::wostream& os)
  {
    return os.iword(_valueIndex) & 0b011111111;
  }

  void JsonIndentChar::ResetState(wostream& os)
  {
    os.iword(_valueIndex) = 0;
  }

  wostream& operator<<(wostream& os, JsonIndentChar const& jsonIndentChar)
  {
    os.iword(jsonIndentChar._valueIndex) |= 0b100000000;                        // Set active flag
    os.iword(jsonIndentChar._valueIndex) |= 0b011111111 & jsonIndentChar._char; // Set char
    return os;
  }
}