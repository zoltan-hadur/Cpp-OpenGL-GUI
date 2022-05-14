using Json4CPP.Visualizer.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer.View
{
  public interface IView<T> where T : ViewModelBase
  {
    T ViewModel { get; set; }
  }
}
