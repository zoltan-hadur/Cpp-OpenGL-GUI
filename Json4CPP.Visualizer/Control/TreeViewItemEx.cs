using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Json4CPP.Visualizer.Control
{
  public class TreeViewItemEx : TreeViewItem
  {
    private EventHandler mOnIsMouseOverChanged;

    public static readonly DependencyProperty IsMouseReallyOverProperty = DependencyProperty.Register(nameof(IsMouseReallyOver), typeof(bool), typeof(TreeViewItemEx), new PropertyMetadata(false));

    public bool IsMouseReallyOver
    {
      get => (bool)GetValue(IsMouseReallyOverProperty);
      set => SetValue(IsMouseReallyOverProperty, value);
    }

    public TreeViewItemEx() : base()
    {
      mOnIsMouseOverChanged = new EventHandler(OnIsMouseOverChanged);
      var wDescriptor = DependencyPropertyDescriptor.FromProperty(IsMouseOverProperty, typeof(TreeViewItemEx));
      wDescriptor.AddValueChanged(this, mOnIsMouseOverChanged);
      RoutedEventHandler wHandler = null;
      wHandler = (object sender, RoutedEventArgs e) =>
      {
        wDescriptor.RemoveValueChanged(this, mOnIsMouseOverChanged);
        Unloaded -= wHandler;
      };
      Unloaded += wHandler;

      RoutedEventHandler wHandler2 = null;
      wHandler2 = (object sender, RoutedEventArgs e) =>
      {
        var wGrid = VisualTreeHelper.GetChild(this, 0) as Grid;
        if (wGrid != null)
        {
          var wBorder = VisualTreeHelper.GetChild(wGrid, 1) as Border;
          if (wBorder != null)
          {
            Grid.SetColumnSpan(wBorder, 2);
          }
        }
        Loaded -= wHandler2;
      };
      Loaded += wHandler2;
    }

    private void OnIsMouseOverChanged(object sender, EventArgs e)
    {
      var wIsMouseReallyOver = IsMouseOver && !IsMouseOverTrueForAnyChild(sender as TreeViewItemEx);
      if (wIsMouseReallyOver == IsMouseReallyOver)
      {
        return;
      }

      if (!wIsMouseReallyOver)
      {
        IsMouseReallyOver = wIsMouseReallyOver;
      }

      var wParent = FindParent<TreeViewItemEx>(this);
      while(wParent != null)
      {
        wParent.OnIsMouseOverChanged(wParent, EventArgs.Empty);
        wParent = FindParent<TreeViewItemEx>(wParent);
      }

      if (wIsMouseReallyOver)
      {
        IsMouseReallyOver = wIsMouseReallyOver;
      }
    }

    private static T FindParent<T>(DependencyObject child) where T : DependencyObject
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

    private bool IsMouseOverTrueForAnyChild(TreeViewItemEx item)
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
