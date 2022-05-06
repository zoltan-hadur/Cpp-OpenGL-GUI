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

namespace Json4CPP.Visualizer
{
  /// <summary>
  /// Interaction logic for MessageBox.xaml
  /// </summary>
  public partial class MessageBox : Window
  {
    public string Message { get; set; }

    public MessageBox(string message)
    {
      Message = message;
      InitializeComponent();
    }

    public static void Show(string message)
    {
      var wMessageBox = new MessageBox(message);
      wMessageBox.ShowDialog();
    }
  }
}
