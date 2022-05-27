using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;

namespace Json4CPP.Visualizer.ViewModel
{
  /// <summary>
  /// <see cref="Value"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObjectVM"/></item>
  /// <item><see cref="JsonArrayVM"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class JsonVM : ViewModelBase
  {
    private object mValue;
    public object Value
    {
      get { return mValue; }
      set
      {
        if (mValue != null)
        {
          switch (mValue)
          {
            case JsonObjectVM wObject:
              wObject.Pairs.CollectionChanged -= CollectionChanged;
              break;
            case JsonArrayVM wArray:
              wArray.Values.CollectionChanged -= CollectionChanged;
              break;
          }
        }
        Set(ref mValue, value);
        if (mValue != null)
        {
          switch (mValue)
          {
            case JsonObjectVM wObject:
              wObject.Pairs.CollectionChanged += CollectionChanged;
              break;
            case JsonArrayVM wArray:
              wArray.Values.CollectionChanged += CollectionChanged;
              break;
          }
        }
      }
    }

    private void CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
    {
      OnPropertyChanged(nameof(Value));
    }

    public bool IsObject => Value is JsonObjectVM;
    public bool IsArray => Value is JsonArrayVM;

    public JsonVM()
    {
      PropertyChanged += JsonVM_PropertyChanged;
    }

    private void JsonVM_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      switch (e.PropertyName)
      {
        case nameof(Value):
          OnPropertyChanged(nameof(IsObject));
          OnPropertyChanged(nameof(IsArray));
          break;
      }
    }

    public override string ToString() => Value.ToString();
  }
}
