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
  /// Interaction logic for DeleteButton.xaml
  /// </summary>
  public partial class DeleteButton : Button
  {
    static DeleteButton()
    {
      DefaultStyleKeyProperty.OverrideMetadata(typeof(DeleteButton), new FrameworkPropertyMetadata(typeof(DeleteButton)));
    }

    public DeleteButton()
    {
      InitializeComponent();
    }
  }
}
