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

namespace Json4CPP.Visualizer.Control
{
  /// <summary>
  /// Interaction logic for PlusButton.xaml
  /// </summary>
  public partial class PlusButton : Button
  {
    public static readonly DependencyProperty AngleProperty = DependencyProperty.Register(nameof(Angle), typeof(double), typeof(PlusButton), new PropertyMetadata(0.0));

    public double Angle
    {
      get => (double)GetValue(AngleProperty);
      set => SetValue(AngleProperty, value);
    }

    static PlusButton()
    {
      DefaultStyleKeyProperty.OverrideMetadata(typeof(PlusButton), new FrameworkPropertyMetadata(typeof(PlusButton)));
    }

    public PlusButton()
    {
      InitializeComponent();
    }
  }
}
