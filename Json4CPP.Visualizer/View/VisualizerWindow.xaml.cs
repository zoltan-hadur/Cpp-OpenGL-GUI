using Json4CPP.Visualizer.ViewModel;
using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;

namespace Json4CPP.Visualizer.View
{
  /// <summary>
  /// Interaction logic for VisualizerWindow.xaml
  /// </summary>
  public partial class VisualizerWindow : Window, IView<VisualizerWindowVM>
  {
    public VisualizerWindowVM ViewModel
    {
      get => DataContext as VisualizerWindowVM;
      set => DataContext = value;
    }

    public VisualizerWindow(VisualizerWindowVM viewModel)
    {
      InitializeComponent();
      ViewModel = viewModel;
    }

    private void ExpandAll_Click(object sender, RoutedEventArgs e)
    {
      SetIsExpandedAll(true);
    }

    private void CollapseAll_Click(object sender, RoutedEventArgs e)
    {
      SetIsExpandedAll(false);
    }

    private void SetIsExpandedAll(bool isExpanded)
    {
      foreach (var wItem in TreeView.Items)
      {
        if (TreeView.ItemContainerGenerator.ContainerFromItem(wItem) is TreeViewItem wTreeViewItem)
        {
          SetIsExpanded(wTreeViewItem, isExpanded);
        }
      }
    }

    private void SetIsExpanded(TreeViewItem item, bool isExpanded)
    {
      if (item.HasItems)
      {
        item.IsExpanded = isExpanded;
        item.UpdateLayout();
        foreach (var wItem in item.Items)
        {
          if (item.ItemContainerGenerator.ContainerFromItem(wItem) is TreeViewItem wTreeViewItem)
          {
            SetIsExpanded(wTreeViewItem, isExpanded);
          }
        }
      }
    }

    private void Stringify_Click(object sender, RoutedEventArgs e)
    {
      MessageBox.Show(ViewModel.Stringify());
    }
  }
}
