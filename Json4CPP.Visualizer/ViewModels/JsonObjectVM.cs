using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;

namespace Json4CPP.Visualizer.ViewModels
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
      if (e.OldItems != null) foreach(PairVM wPair in e.OldItems)
      {
        wPair.Parent = null;
      }
      if (e.NewItems != null) foreach (PairVM wPair in e.NewItems)
      {
        wPair.Parent = this;
      }

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
