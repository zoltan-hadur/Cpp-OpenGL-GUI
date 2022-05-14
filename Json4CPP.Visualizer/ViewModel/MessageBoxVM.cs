using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer.ViewModel
{
  public class MessageBoxVM : ViewModelBase
  {
    private string mMessage;
    public string Message
    {
      get { return mMessage; }
      set { Set(ref mMessage, value); }
    }

    public MessageBoxVM(string message)
    {
      Message = message;
    }
  }
}
