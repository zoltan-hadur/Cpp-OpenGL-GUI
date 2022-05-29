using System.Windows;

namespace Json4CPP.Visualizer.ValidationRules
{
  // https://social.technet.microsoft.com/wiki/contents/articles/31422.wpf-passing-a-data-bound-value-to-a-validation-rule.aspx
  public class BindingProxy : Freezable
  {
    public static readonly DependencyProperty DataProperty = DependencyProperty.Register("Data", typeof(object), typeof(BindingProxy), new PropertyMetadata(null));

    public object Data
    {
      get { return GetValue(DataProperty); }
      set { SetValue(DataProperty, value); }
    }

    protected override Freezable CreateInstanceCore()
    {
      return new BindingProxy();
    }
  }
}
