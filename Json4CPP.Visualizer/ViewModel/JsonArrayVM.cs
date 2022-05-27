using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Json4CPP.Visualizer.ViewModel
{
  /// <summary>
  /// <see cref="Values"/> contains one of the following types wrapped in a <see cref="JsonVM"/>:
  /// <list type="bullet">
  /// <item><see cref="JsonObjectVM"/></item>
  /// <item><see cref="JsonArrayVM"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class JsonArrayVM : ViewModelBase
  {
    private ObservableCollection<JsonVM> mValues = new ObservableCollection<JsonVM>();
    public ObservableCollection<JsonVM> Values
    {
      get { return mValues; }
    }

    public override string ToString() => $"{{ Array={{Values={Values.Count}}} }}";
  }
}
