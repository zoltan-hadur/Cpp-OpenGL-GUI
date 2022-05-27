using System.Collections.Generic;
using System.Collections.ObjectModel;

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

    public override string ToString() => $"{{ Object={{Pairs={Pairs.Count}}} }}";
  }
}
