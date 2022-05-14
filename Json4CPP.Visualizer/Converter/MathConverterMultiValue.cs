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
  public class MathConverterMultiValue : IMultiValueConverter
  {
    public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
    {
      if(values.Any(wValue => wValue == System.Windows.DependencyProperty.UnsetValue))
      {
        return System.Windows.DependencyProperty.UnsetValue;
      }

      var wValues = values.Select((wValue, wIndex) =>
      {
        if (double.TryParse(wValue.ToString(), out var wDoubleValue))
        {
          return wDoubleValue;
        }
        throw new ArgumentException($"Parameter {nameof(values)}[{wIndex}] is not a number! Actual value is \"{wValue.ToString()}\"", nameof(values));
      }).ToList();
      if (wValues.Any(wValue => double.IsNaN(wValue)))
      {
        return System.Windows.DependencyProperty.UnsetValue;
      }

      var wFormat = parameter as string;
      if (wFormat == null)
      {
        throw new ArgumentException("Parameter is not a string!", nameof(parameter));
      }
      for(int i = 0; i < wValues.Count; i++)
      {
        if (!wFormat.Contains($"{i}"))
        {
          throw new ArgumentException($"Parameter is not a valid format string! It must contain the value \"{i}\" as there are {wValues.Count} elements! Actual value is \"{wFormat}\"", nameof(parameter));
        }
        wFormat = wFormat.Replace($"{{{i}}}", wValues[i].ToString());
      }

      try
      {
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

    public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
    {
      throw new NotImplementedException();
    }
  }
}
