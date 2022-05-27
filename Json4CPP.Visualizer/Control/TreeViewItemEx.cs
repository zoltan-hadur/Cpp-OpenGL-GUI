using Json4CPP.Visualizer.Converter;
using Json4CPP.Visualizer.Extension;
using System;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Shapes;

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

    public static readonly DependencyProperty EditModeProperty = DependencyProperty.Register(nameof(EditMode), typeof(bool), typeof(TreeViewItemEx), new PropertyMetadata(false));

    public bool EditMode
    {
      get => (bool)GetValue(EditModeProperty);
      set => SetValue(EditModeProperty, value);
    }

    public static readonly DependencyProperty EmptyExpandableProperty = DependencyProperty.Register(nameof(EmptyExpandable), typeof(bool), typeof(TreeViewItemEx), new PropertyMetadata(false));

    public bool EmptyExpandable
    {
      get => (bool)GetValue(EmptyExpandableProperty);
      set => SetValue(EmptyExpandableProperty, value);
    }

    public static readonly DependencyProperty DeletableProperty = DependencyProperty.Register(nameof(Deletable), typeof(bool), typeof(TreeViewItemEx), new PropertyMetadata(true));

    public bool Deletable
    {
      get => (bool)GetValue(DeletableProperty);
      set => SetValue(DeletableProperty, value);
    }

    static TreeViewItemEx()
    {
      DefaultStyleKeyProperty.OverrideMetadata(typeof(TreeViewItemEx), new FrameworkPropertyMetadata(typeof(TreeViewItemEx)));
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
    }

    private void OnIsMouseOverChanged(object sender, EventArgs e)
    {
      var wIsMouseReallyOver = IsMouseOver && !(sender as TreeViewItemEx).IsMouseOverTrueForAnyChild() && !(GetTemplateChild("Part_NewItem") as Button).IsMouseOver;
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

    public override void OnApplyTemplate()
    {
      base.OnApplyTemplate();
      if (GetTemplateChild("Part_DeleteItem") is Button wDeleteItem)
      {
        wDeleteItem.Click += Part_DeleteItem_Click;
      }
    }

    private void Part_DeleteItem_Click(object sender, RoutedEventArgs e)
    {
      var wParent = this.FindParent<TreeViewItemEx>();
      if (wParent != null && wParent.Items is IEditableCollectionView wItems)
      {
        wItems.Remove(DataContext);
      }
    }
  }
}
