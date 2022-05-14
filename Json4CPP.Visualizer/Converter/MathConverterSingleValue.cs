using NCalc;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Json4CPP.Visualizer.Converter
{
  public class MathConverterSingleValue : IValueConverter
  {
    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
      if (value == System.Windows.DependencyProperty.UnsetValue)
      {
        return System.Windows.DependencyProperty.UnsetValue;
      }

      double wValue;
      if (!double.TryParse(value.ToString(), out wValue))
      {
        throw new ArgumentException($"Parameter is not a number! Actual value is \"{value.ToString()}\"", nameof(value));
      }
      if (double.IsNaN(wValue))
      {
        return System.Windows.DependencyProperty.UnsetValue;
      }

      var wFormat = parameter as string;
      if (wFormat == null)
      {
        throw new ArgumentException("Parameter is not a string!", nameof(parameter));
      }
      if (!wFormat.Contains("{0}"))
      {
        throw new ArgumentException($"Parameter is not a valid format string! It must contain the value \"{{0}}\"! Actual value is \"{wFormat}\"", nameof(parameter));
      }

      try
      {
        wFormat = wFormat.Replace("{0}", wValue.ToString());
        var wExpression = new Expression(wFormat);
        var wResult = wExpression.Evaluate();

        if (targetType == typeof(System.Windows.CornerRadius))
        {
          return new System.Windows.CornerRadius(System.Convert.ToDouble(wResult));
        }
        else
        {
          return wResult;
        }
      }
      catch (Exception ex)
      {
        return System.Windows.DependencyProperty.UnsetValue;
      }
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
      throw new NotImplementedException();
    }
  }
}
