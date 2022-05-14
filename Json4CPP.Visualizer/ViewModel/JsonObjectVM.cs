using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Json4CPP.Visualizer.ViewModel
{
  /// <summary>
  /// <see cref="Pairs"/> should contain only <see cref="PairVM"/>s.
  /// </summary>
  public class JsonObjectVM : ViewModelBase
  {
    private ObservableCollection<object> mPairs = new ObservableCollection<object>();
    public ObservableCollection<object> Pairs
    {
      get { return mPairs; }
      set { Set(ref mPairs, value); }
    }

    public override string ToString() => $"{{ Object={{Pairs={Pairs.Count}}} }}";
  }
}
