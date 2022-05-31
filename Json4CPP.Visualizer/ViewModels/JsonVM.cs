using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;

namespace Json4CPP.Visualizer.ViewModels
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

    protected override void OnValidate(string propertyName)
    {
      base.OnValidate(propertyName);
      switch (propertyName)
      {
        case nameof(Value):
          {
            if (Value is string wValue &&                                                                     // Value is string and it is either
               (
                 Convert.ToInt32(wValue.StartsWith("L\"") && wValue.EndsWith("\"") && wValue.Length >= 3) +   // a valid string, or
                 Convert.ToInt32(double.TryParse(wValue, out _)) +                                            // a valid number, or
                 Convert.ToInt32(wValue == "true" || wValue == "false") +                                     // a valid boolean, or
                 Convert.ToInt32(wValue == "null")                                                            // null
               )
               != 1)  // If it is not exactly one of the above, then it is invalid
            {
              AddError(propertyName, $"{nameof(Value)} must be one of the following:"         + Environment.NewLine +
                                      "  string: must be enclosed within \"L\"\" and \"\"\"!" + Environment.NewLine +
                                      "  number: must be a valid number!"                     + Environment.NewLine +
                                      "  boolean: must be either \"true\" or \"false\"!"      + Environment.NewLine +
                                      "  null: must be \"null\"!");
            }
          }
          break;
      }
    }
  }
}
