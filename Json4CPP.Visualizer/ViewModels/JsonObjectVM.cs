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
    public DkmSuccessEvaluationResult Result { get; set; }

    private ObservableCollection<PairVM> mPairs = new ObservableCollection<PairVM>();
    public ObservableCollection<PairVM> Pairs
    {
      get { return mPairs; }
    }

    public override string ToString() => $"{{ Object={{Pairs={Pairs.Count}}} }}";

    public void RemoveItem(object obj)
    {
      if (obj is PairVM wPair && Pairs.IndexOf(wPair) is int wIndex && wIndex >= 0)
      {
        _ = Json4CPPVisualizerService.EvaluateExpression(
          Result,
          $"((Json4CPP.dll!{Result.Type}*)&{Result.FullName})->RemoveItem({wPair.Key})",
          flagsToRemove: DkmEvaluationFlags.NoSideEffects);
        Pairs.Remove(wPair);
        for (int i = wIndex; i < Pairs.Count; i++)
        {
          var wPairResult = Json4CPPVisualizerService.EvaluateExpression(Result, $"{Result.FullName}[{i}]");
          var wFirstResult = Json4CPPVisualizerService.EvaluateExpression(wPairResult, $"{wPairResult.FullName}.first");
          var wSecondResult = Json4CPPVisualizerService.EvaluateExpression(wPairResult, $"{wPairResult.FullName}.second");
          Pairs[i] = null;
          Pairs[i] = new PairVM()
          {
            Parent = this,
            Result = wPairResult,
            Key = wFirstResult.Value,
            Value = JsonBuilder.Build(wSecondResult)
          };
        }
      }
    }

    public void AddItem()
    {
      _ = Json4CPPVisualizerService.EvaluateExpression(
        Result,
        $"((Json4CPP.dll!{Result.Type}*)&{Result.FullName})->AddItem()",
        flagsToRemove: DkmEvaluationFlags.NoSideEffects);
      var wPairResult = Json4CPPVisualizerService.EvaluateExpression(Result, $"{Result.FullName}[{Pairs.Count}]");
      var wFirstResult = Json4CPPVisualizerService.EvaluateExpression(wPairResult, $"{wPairResult.FullName}.first");
      var wSecondResult = Json4CPPVisualizerService.EvaluateExpression(wPairResult, $"{wPairResult.FullName}.second");
      Pairs.Add(new PairVM()
      {
        Parent = this,
        Result = wPairResult,
        Key = wFirstResult.Value,
        Value = JsonBuilder.Build(wSecondResult)
      });
    }
  }
}
