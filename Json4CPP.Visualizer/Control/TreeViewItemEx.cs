using Json4CPP.Visualizer.Extension;
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
      var wIsMouseReallyOver = IsMouseOver && !(sender as TreeViewItemEx).IsMouseOverTrueForAnyChild();
      if (wIsMouseReallyOver == IsMouseReallyOver)
      {
        return;
      }

      if (!wIsMouseReallyOver)
      {
        IsMouseReallyOver = wIsMouseReallyOver;
      }

      var wParent = this.FindParent<TreeViewItemEx>();
      while(wParent != null)
      {
        wParent.OnIsMouseOverChanged(wParent, EventArgs.Empty);
        wParent = wParent.FindParent<TreeViewItemEx>();
      }

      if (wIsMouseReallyOver)
      {
        IsMouseReallyOver = wIsMouseReallyOver;
      }
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
