using NCalc;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using Expression = NCalc.Expression;

namespace Json4CPP.Visualizer.Converters
{
  public class MathConverterMultiValue : IMultiValueConverter
  {
    public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
    {
      if(values.Any(wValue => wValue == DependencyProperty.UnsetValue || wValue is double wDoubleValue && double.IsNaN(wDoubleValue)))
      {
        return DependencyProperty.UnsetValue;
      }

      var wFormats = parameter is string ? new List<string> { parameter as string } :
                                           parameter is IEnumerable<string> wEnumerable ? wEnumerable.ToList() : null;
      if (wFormats == null)
      {
        throw new ArgumentException("Parameter is not a string or IEnumerable<string>!", nameof(parameter));
      }
      for(int i = 0; i < values.Length; i++)
      {
        if (!wFormats.Any(wFormat => wFormat.Contains($"{{{i}}}")))
        {
          throw new ArgumentException($"Parameter is not a valid format string(s)! It must contain the value \"{{{i}}}\" as there are {values.Length} elements! Actual value is \"[{string.Join(", ",wFormats)}]\"", nameof(parameter));
        }
      }
      for (int i = 0; i < values.Length; i++)
      {
        for (int j = 0; j < wFormats.Count; ++j)
        {
          wFormats[j] = wFormats[j].Replace($"{{{i}}}", values[i].ToString());
        }
      }

      try
      {
        if (wFormats.Count == 1)
        {
          var wExpression = new Expression(wFormats[0]);
          var wResult = wExpression.Evaluate();

          if (targetType == typeof(CornerRadius))
          {
            return new CornerRadius(System.Convert.ToDouble(wResult));
          }
          else
          {
            return wResult;
          }
        }
        else
        {
          var wResults = wFormats.Select(wFormat => new Expression(wFormat).Evaluate()).ToArray();

          if (targetType == typeof(Point))
          {
            return new Point(System.Convert.ToDouble(wResults[0]), System.Convert.ToDouble(wResults[1]));
          }
          else
          {
            return wResults;
          }
        }
      }
      catch (Exception ex)
      {
        System.Diagnostics.Debug.WriteLine(ex.ToString());
        return DependencyProperty.UnsetValue;
      }
    }

    public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
    {
      throw new NotImplementedException();
    }
  }
}
