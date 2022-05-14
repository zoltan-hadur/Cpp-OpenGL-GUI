using Json4CPP.Visualizer.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Json4CPP.Visualizer.View
{
  /// <summary>
  /// Interaction logic for MessageBox.xaml
  /// </summary>
  public partial class MessageBox : Window, IView<MessageBoxVM>
  {
    public MessageBoxVM ViewModel
    {
      get => DataContext as MessageBoxVM;
      set => DataContext = value;
    }

    public MessageBox(MessageBoxVM viewModel)
    {
      InitializeComponent();
      ViewModel = viewModel;
    }

    public static void Show(string message)
    {
      var wMessageBox = new MessageBox(new MessageBoxVM(message));
      wMessageBox.ShowDialog();
    }
  }
}
