using Microsoft.VisualStudio.Debugger.Evaluation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer.ViewModel
{
  public class VisualizerWindowVM : ViewModelBase
  {
    private DkmSuccessEvaluationResult mResult;

    private string mExpression;
    public string Expression
    {
      get => mExpression;
      set => Set(ref mExpression, value);
    }

    private JsonVM mJson;
    public JsonVM Json
    {
      get => mJson;
      set => Set(ref mJson, value);
    }

    protected VisualizerWindowVM()
    {

    }

    public VisualizerWindowVM(DkmSuccessEvaluationResult result)
    {
      mResult = result;
      Expression = result.FullName;
      Json = JsonBuilder.Build(result);
    }

    public string Stringify()
    {
      var wResult = Json4CPPVisualizerService.EvaluateExpression(
        mResult,
        $"((Json4CPP.dll!{mResult.Type}*)&{mResult.FullName})->Dump(2, L' ')",
        flagsToRemove: DkmEvaluationFlags.NoSideEffects);
      return wResult.GetUnderlyingString();
    }
  }
}
