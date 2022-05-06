using System.Windows;
using System.Windows.Controls;

namespace Json4CPP.Visualizer
{
  /// <summary>
  /// Interaction logic for VisualizerWindow.xaml
  /// </summary>
  public partial class VisualizerWindow : Window, IView<ViewModel>
  {
    public ViewModel ViewModel
    {
      get => DataContext as ViewModel;
      set => DataContext = value;
    }

    public VisualizerWindow(ViewModel viewModel)
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

    private void Stringify_Click(object sender, RoutedEventArgs e)
    {
      MessageBox.Show(ViewModel.Stringify());
    }
  }
}
