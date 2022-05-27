using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;

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

    public JsonArrayVM()
    {
      Values.CollectionChanged += Values_CollectionChanged;
    }

    private void Values_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
    {
      // Default value when added from the UI
      if (e.NewItems != null &&
          e.NewItems.Count == 1 &&
          e.NewItems[0] is JsonVM wNewValue &&
          wNewValue.Value == null)
      {
        wNewValue.Value = "null";
      }
    }

    public override string ToString() => $"{{ Array={{Values={Values.Count}}} }}";
  }
}
