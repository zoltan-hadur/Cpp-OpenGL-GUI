using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer.Interfaces
{
  public interface IEditableCollection
  {
    void RemoveItem(object obj);
    void AddItem();
  }
}
