using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;

namespace Json4CPP.Visualizer.ViewModel
{
  /// <summary>
  /// <see cref="Pairs"/> contains only <see cref="PairVM"/>s.
  /// </summary>
  public class JsonObjectVM : ViewModelBase
  {
    private ObservableCollection<PairVM> mPairs = new ObservableCollection<PairVM>();
    public ObservableCollection<PairVM> Pairs
    {
      get { return mPairs; }
    }

    public JsonObjectVM()
    {
      Pairs.CollectionChanged += Pairs_CollectionChanged;
    }

    private void Pairs_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
    {
      // Default value when added from the UI
      if (e.NewItems != null &&
          e.NewItems.Count == 1 &&
          e.NewItems[0] is PairVM wNewPair &&
          wNewPair.Key == null &&
          wNewPair.Value == null)
      {
        var wUniqueIndex = Enumerable.Range(0, int.MaxValue).First(wIndex => !Pairs.Any(wPair => wPair.Key == $"L\"{wIndex}\""));
        wNewPair.Key = $"L\"{wUniqueIndex}\"";
        wNewPair.Value = new JsonVM { Value = "null" };
      }
    }

    public override string ToString() => $"{{ Object={{Pairs={Pairs.Count}}} }}";
  }
}
