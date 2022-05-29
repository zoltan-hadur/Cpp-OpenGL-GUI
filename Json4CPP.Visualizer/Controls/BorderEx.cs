using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace Json4CPP.Visualizer.Controls
{
  public class BorderEx : Border
  {
    public static readonly DependencyProperty AngleProperty = DependencyProperty.Register(nameof(Angle), typeof(double), typeof(BorderEx), new PropertyMetadata(0.0));
    public static readonly DependencyProperty ScaleProperty = DependencyProperty.Register(nameof(Scale), typeof(double), typeof(BorderEx), new PropertyMetadata(1.0));

    public double Angle
    {
      get => (double)GetValue(AngleProperty);
      set => SetValue(AngleProperty, value);
    }

    public double Scale
    {
      get => (double)GetValue(ScaleProperty);
      set => SetValue(ScaleProperty, value);
    }

    static BorderEx()
    {
      DefaultStyleKeyProperty.OverrideMetadata(typeof(BorderEx), new FrameworkPropertyMetadata(typeof(BorderEx)));
    }
  }
}
