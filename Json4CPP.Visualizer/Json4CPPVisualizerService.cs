using Microsoft.VisualStudio;
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
        var wWindow = new VisualizerWindow(new ViewModel
        {
          Expression = wResult.FullName,
          Json = JsonBuilder.Build(wResult)
        });
        wWindow.ShowDialog();
      }
      catch (Exception e)
      {
        Debug.Fail("Visualization failed: " + e.Message);
        return e.HResult;
      }
      return VSConstants.S_OK;
    }
  }
}
