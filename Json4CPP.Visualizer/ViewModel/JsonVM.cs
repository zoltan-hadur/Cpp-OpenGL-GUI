using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace Json4CPP.Visualizer.ViewModel
{
  /// <summary>
  /// <see cref="Value"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObjectVM"/></item>
  /// <item><see cref="JsonArrayVM"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class JsonVM : ViewModelBase
  {
    private object mValue;
    public object Value
    {
      get { return mValue; }
      set { Set(ref mValue, value); }
    }

    public bool IsObject => Value is JsonObjectVM;
    public bool IsArray => Value is JsonArrayVM;

    private ObservableCollection<object> mEmptyChildren = new ObservableCollection<object>();

    /// <summary>
    /// Returns the appropriate children according to the actual type of <see cref="Value"/>.
    /// </summary>
    public ObservableCollection<object> Children
    {
      get
      {
        if (Value is JsonObjectVM wObject)
        {
          return wObject.Pairs;
        }
        else if (Value is JsonArrayVM wArray)
        {
          return wArray.Values;
        }
        else
        {
          return mEmptyChildren;
        }
      }
    }

    public JsonVM()
    {
      PropertyChanged += JsonVM_PropertyChanged;
    }

    private void JsonVM_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      switch (e.PropertyName)
      {
        case nameof(Value):
          OnPropertyChanged(nameof(IsObject));
          OnPropertyChanged(nameof(IsArray));
          OnPropertyChanged(nameof(Children));
          break;
      }
    }

    public override string ToString() => Value.ToString();
  }
}
