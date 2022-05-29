using Json4CPP.Visualizer.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace Json4CPP.Visualizer.Extensions
{
  public static class Extensions
  {
    public static T FindParent<T>(this DependencyObject child) where T : DependencyObject
    {
      var wParent = VisualTreeHelper.GetParent(child);
      if (wParent == null)
      {
        return null;
      }
      if (wParent is T)
      {
        return wParent as T;
      }
      else
      {
        return FindParent<T>(wParent);
      }
    }

    public static bool IsMouseOverTrueForAnyChild(this TreeViewItemEx item)
    {
      foreach (var wItem in item.Items)
      {
        if (item.ItemContainerGenerator.ContainerFromItem(wItem) is TreeViewItemEx wTreeViewItem)
        {
          if (wTreeViewItem.IsMouseOver)
          {
            return true;
          }
          if (IsMouseOverTrueForAnyChild(wTreeViewItem))
          {
            return true;
          }
        }
      }
      return false;
    }
  }
}
