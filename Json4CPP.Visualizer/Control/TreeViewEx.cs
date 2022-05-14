using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace Json4CPP.Visualizer.Control
{
  public class TreeViewEx : TreeView
  {
    protected override DependencyObject GetContainerForItemOverride()
    {
      return new TreeViewItemEx();
    }

    protected override bool IsItemItsOwnContainerOverride(object item)
    {
      return item is TreeViewItemEx;
    }
  }
}
