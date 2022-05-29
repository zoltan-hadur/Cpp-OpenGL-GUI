namespace Json4CPP.Visualizer.ViewModels
{
  //             0               1             2     3       4        5           6
  //std::variant<std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray>
  public enum JsonType
  {
    Null    = 0,
    String  = 1,
    Boolean = 2,
    Real    = 3,
    Integer = 4,
    Object  = 5,
    Array   = 6
  }
}
