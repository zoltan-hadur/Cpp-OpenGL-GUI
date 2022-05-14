using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Json4CPP.Visualizer.ViewModel
{
  /// <summary>
  /// <see cref="Values"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObjectVM"/></item>
  /// <item><see cref="JsonArrayVM"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class JsonArrayVM : ViewModelBase
  {
    private ObservableCollection<object> mValues = new ObservableCollection<object>();
    public ObservableCollection<object> Values
    {
      get { return mValues; }
      set { Set(ref mValues, value); }
    }

    public override string ToString() => $"{{ Array={{Values={Values.Count}}} }}";
  }
}
