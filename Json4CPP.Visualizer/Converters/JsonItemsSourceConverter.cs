using Json4CPP.Visualizer.ViewModels;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace Json4CPP.Visualizer.Converters
{
  public class JsonItemsSourceConverter : IValueConverter
  {
    private static ReadOnlyObservableCollection<object> mEmptyChildren = new ReadOnlyObservableCollection<object>(new ObservableCollection<object>());

    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
      if (value == null)
      {
        return DependencyProperty.UnsetValue;
      }
      else if (value is JsonVM wJson)
      {
        switch (wJson.Value)
        {
          case JsonObjectVM wObject: return wObject.Pairs;
          case JsonArrayVM wArray: return wArray.Values;
          default: return mEmptyChildren;
        }
      }
      else
      {
        throw new ArgumentException($"Parameter's type is not {typeof(JsonVM)}", nameof(value));
      }
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
      throw new NotImplementedException();
    }
  }
}
