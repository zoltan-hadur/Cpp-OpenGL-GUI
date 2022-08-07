using Json4CPP.Visualizer.Interfaces;
using Microsoft.VisualStudio.Debugger.Evaluation;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;

namespace Json4CPP.Visualizer.ViewModels
{
  /// <summary>
  /// <see cref="Pairs"/> contains only <see cref="PairVM"/>s.
  /// </summary>
  public class JsonObjectVM : ViewModelBase, IEditableCollection
  {
    private DkmSuccessEvaluationResult mResult;

    private ObservableCollection<PairVM> mPairs = new ObservableCollection<PairVM>();
    public ObservableCollection<PairVM> Pairs
    {
      get => mPairs;
    }

    protected JsonObjectVM()
    {

    }

    public JsonObjectVM(DkmSuccessEvaluationResult result)
    {
      mResult = result;

      // Query the size of the c++ object
      var wSizeResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}._pairs.size()");
      var wSize = int.Parse(wSizeResult.Value);

      // Query each element of the c++ object and add it to the c# ui
      for (int i = 0; i < wSize; i++)
      {
        var wPairResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}[{i}]");
        Pairs.Add(new PairVM(wPairResult, this));
      }
    }

    public override string ToString() => $"{{ Object={{Pairs={Pairs.Count}}} }}";

    public void RemoveItem(object obj)
    {
      if (obj is PairVM wPair && Pairs.IndexOf(wPair) is int wIndex && wIndex >= 0)
      {
        // Remove item from the c++ object
        _ = Json4CPPVisualizerService.EvaluateExpression(
          mResult,
          $"((Json4CPP.dll!{mResult.Type}*)&{mResult.FullName})->RemoveItem({wPair.Key})",
          flagsToRemove: DkmEvaluationFlags.NoSideEffects);

        // Remove item from the c# ui
        Pairs.Remove(wPair);

        // Refresh the remaining ones
        for (int i = wIndex; i < Pairs.Count; i++)
        {
          var wPairResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}[{i}]");
          Pairs[i] = new PairVM(wPairResult, this);
        }
      }
    }

    public void AddItem()
    {
      // Add item to the c++ object
      _ = Json4CPPVisualizerService.EvaluateExpression(
        mResult,
        $"((Json4CPP.dll!{mResult.Type}*)&{mResult.FullName})->AddItem()",
        flagsToRemove: DkmEvaluationFlags.NoSideEffects);

      // Query and add the newly added item from the c++ object to the c# ui
      var wPairResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}[{Pairs.Count}]");
      Pairs.Add(new PairVM(wPairResult, this));
    }
  }
}
