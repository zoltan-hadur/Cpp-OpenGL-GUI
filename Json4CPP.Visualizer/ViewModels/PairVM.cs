using System.Collections.Specialized;
using System.ComponentModel;

namespace Json4CPP.Visualizer.ViewModels
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
      set
      {
        if (mValue != null)
        {
          mValue.PropertyChanged -= Value_PropertyChanged;
        }
        Set(ref mValue, value);
        if (mValue != null)
        {
          mValue.PropertyChanged += Value_PropertyChanged;
        }
      }
    }

    private void Value_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      switch (e.PropertyName)
      {
        case nameof(Value.IsObject):
        case nameof(Value.IsArray):
          OnPropertyChanged(nameof(IsObject));
          OnPropertyChanged(nameof(IsArray));
          break;
        case nameof(Value.Value):
          OnPropertyChanged(nameof(Value));
          break;
      }
    }

    public bool IsObject => Value.IsObject;
    public bool IsArray => Value.IsArray;

    public PairVM()
    {
      PropertyChanged += PairVM_PropertyChanged;
    }

    private void PairVM_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      switch (e.PropertyName)
      {
        case nameof(Value):
          OnPropertyChanged(nameof(IsObject));
          OnPropertyChanged(nameof(IsArray));
          break;
      }
    }

    public override string ToString() => $"{Key}: {Value}";
  }
}
