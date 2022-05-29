using Json4CPP.Visualizer.Controls;
using Json4CPP.Visualizer.ViewModels;
using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;

namespace Json4CPP.Visualizer.Views
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
      foreach (var wItem in TreeViewEx.Items)
      {
        if (TreeViewEx.ItemContainerGenerator.ContainerFromItem(wItem) is TreeViewItemEx wTreeViewItem)
        {
          SetIsExpanded(wTreeViewItem, isExpanded);
        }
      }
    }

    private void SetIsExpanded(TreeViewItemEx item, bool isExpanded)
    {
      if (item.HasItems || item.EmptyExpandable)
      {
        item.IsExpanded = isExpanded;
        item.UpdateLayout();
        foreach (var wItem in item.Items)
        {
          if (item.ItemContainerGenerator.ContainerFromItem(wItem) is TreeViewItemEx wTreeViewItem)
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
