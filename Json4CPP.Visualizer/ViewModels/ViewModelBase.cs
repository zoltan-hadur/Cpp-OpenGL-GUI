using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer.ViewModels
{
  public abstract class ViewModelBase : INotifyPropertyChanged, INotifyDataErrorInfo
  {
    public event PropertyChangedEventHandler PropertyChanged;

    protected void Set<T>(ref T field, T value, [CallerMemberName] string propertyName = null)
    {
      if (EqualityComparer<T>.Default.Equals(field, value)) return;
      field = value;
      ClearErrors(propertyName);
      OnValidate(propertyName);
      OnPropertyChanged(propertyName);
    }

    protected void OnPropertyChanged([CallerMemberName] string propertyName = null)
    {
      PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }

    private readonly Dictionary<string, HashSet<string>> mErrors = new Dictionary<string, HashSet<string>>();

    public event EventHandler<DataErrorsChangedEventArgs> ErrorsChanged;

    public bool HasErrors => mErrors.Any();

    public IEnumerable GetErrors(string propertyName) => mErrors.TryGetValue(propertyName, out var wErrors) ? wErrors : null;

    private void OnErrorsChanged(string propertyName) => ErrorsChanged?.Invoke(this, new DataErrorsChangedEventArgs(propertyName));

    virtual protected void OnValidate(string propertyName)
    {

    }

    protected void AddError(string propertyName, string error)
    {
      HashSet<string> wErrors;
      if (!mErrors.TryGetValue(propertyName, out wErrors))
      {
        wErrors = new HashSet<string>();
        mErrors[propertyName] = wErrors;
      }
      if (wErrors.Add(error))
      {
        OnErrorsChanged(propertyName);
      }
    }

    private void ClearErrors(string propertyName)
    {
      if (mErrors.Remove(propertyName))
      {
        OnErrorsChanged(propertyName);
      }
    }

    protected bool HasError(string propertyName) => mErrors.TryGetValue(propertyName, out var wErrors) ? wErrors.Any() : false;
  }
}
