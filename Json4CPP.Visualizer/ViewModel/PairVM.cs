namespace Json4CPP.Visualizer.ViewModel
{
  /// <summary>
  /// <see cref="string"/> Key, <see cref="JsonVM"/> Value pair.<para/>
  /// <see cref="Value"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObjectVM"/></item>
  /// <item><see cref="JsonArrayVM"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class PairVM : ViewModelBase
  {
    private string mKey;
    public string Key
    {
      get { return mKey; }
      set { Set(ref mKey, value); }
    }

    private JsonVM mValue;
    public JsonVM Value
    {
      get { return mValue; }
      set { Set(ref mValue, value); }
    }
  }
}
