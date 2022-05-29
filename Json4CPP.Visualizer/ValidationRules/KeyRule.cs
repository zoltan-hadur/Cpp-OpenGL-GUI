using Json4CPP.Visualizer.ViewModels;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace Json4CPP.Visualizer.ValidationRules
{
  public class KeyRuleWrapper : DependencyObject
  {
    public static readonly DependencyProperty ParentProperty = DependencyProperty.Register(nameof(Parent), typeof(object), typeof(KeyRuleWrapper));

    public object Parent
    {
      get { return GetValue(ParentProperty); }
      set { SetValue(ParentProperty, value); }
    }

    public static readonly DependencyProperty CurrentProperty = DependencyProperty.Register(nameof(Current), typeof(object), typeof(KeyRuleWrapper));

    public object Current
    {
      get { return GetValue(CurrentProperty); }
      set { SetValue(CurrentProperty, value); }
    }
  }

  public class KeyRule : ValidationRule
  {
    public KeyRuleWrapper Wrapper { get; set; }

    public override ValidationResult Validate(object value, CultureInfo cultureInfo)
    {
      var wParent = Wrapper.Parent is JsonVM wJsonParent ? wJsonParent.Value as JsonObjectVM :
                                                           Wrapper.Parent is PairVM wPairParent ? wPairParent.Value.Value as JsonObjectVM :
                                                                                                  null;
      if (wParent == null)
      {
        throw new Exception($"{nameof(Wrapper)}.{nameof(Wrapper.Parent)} is not valid!");
      }

      var wCurrent = Wrapper.Current is PairVM wPairCurrent ? wPairCurrent :
                                                              null;
      if (wCurrent == null)
      {
        throw new Exception($"{nameof(Wrapper)}.{nameof(Wrapper.Current)} is not valid!");
      }

      var wKey = value as string;
      if (wKey == null)
      {
        return new ValidationResult(false, "Key is null!");
      }
      if (!(wKey.StartsWith("L\"") && wKey.EndsWith("\"")))
      {
        return new ValidationResult(false, "Key must start with 'L\"' and end with '\"'.!");
      }
      if (wParent.Pairs.Any(wPair => wPair != wCurrent && wPair.Key == wKey))
      {
        return new ValidationResult(false, "Key must be unique!");
      }
      return ValidationResult.ValidResult;
    }
  }
}
