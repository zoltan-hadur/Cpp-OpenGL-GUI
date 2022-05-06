using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Debugger;
using Microsoft.VisualStudio.Debugger.Evaluation;
using Microsoft.VisualStudio.Debugger.Interop;
using System;
using System.Diagnostics;

namespace Json4CPP.Visualizer
{
  /// <summary>
  /// Json4CPP visualizer service implementation.
  /// </summary>
  public class Json4CPPVisualizerService : IJson4CPPVisualizerService, IVsCppDebugUIVisualizer
  {
    int IVsCppDebugUIVisualizer.DisplayValue(uint ownerHwnd, uint visualizerId, IDebugProperty3 debugProperty)
    {
      try
      {
        var wResult = DkmSuccessEvaluationResult.ExtractFromProperty(debugProperty);
        var wWindow = new VisualizerWindow(new ViewModel(wResult));
        wWindow.ShowDialog();
      }
      catch (Exception e)
      {
        Debug.Fail("Visualization failed: " + e.Message);
        return e.HResult;
      }
      return VSConstants.S_OK;
    }

    public static DkmSuccessEvaluationResult EvaluateExpression(
      DkmSuccessEvaluationResult result,
      string expression,
      DkmEvaluationFlags flagsToRemove = DkmEvaluationFlags.None,
      DkmEvaluationFlags flagsToAdd = DkmEvaluationFlags.None)
    {
      DkmSuccessEvaluationResult wResult = null;
      var wWorkList = DkmWorkList.Create(null);
      var wCompletionRoutine = new DkmCompletionRoutine<DkmEvaluateExpressionAsyncResult>(wExpressionResult =>
      {
        wResult = wExpressionResult.ResultObject as DkmSuccessEvaluationResult;
      });

      var wContext = DkmInspectionContext.Create(
        InspectionSession: result.InspectionSession,
        RuntimeInstance: result.RuntimeInstance,
        Thread: result.InspectionContext.Thread,
        Timeout: 1000,
        EvaluationFlags: result.InspectionContext.EvaluationFlags & ~flagsToRemove | flagsToAdd,
        FuncEvalFlags: result.InspectionContext.FuncEvalFlags,
        Radix: 10,
        Language: result.Language,
        ReturnValue: null);

      var wExpression = DkmLanguageExpression.Create(result.Language, wContext.EvaluationFlags, expression, null);
      wContext.EvaluateExpression(wWorkList, wExpression, result.StackFrame, wCompletionRoutine);
      wWorkList.Execute();
      Debug.Assert(wResult != null, $"EvaluateExpression failed: {expression}");
      return wResult;
    }
  }
}
