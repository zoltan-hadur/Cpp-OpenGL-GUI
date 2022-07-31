using Json4CPP.Visualizer.Interfaces;
using Microsoft.VisualStudio.Debugger.Evaluation;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;

namespace Json4CPP.Visualizer.ViewModels
{
  /// <summary>
  /// <see cref="Values"/> contains one of the following types wrapped in a <see cref="JsonVM"/>:
  /// <list type="bullet">
  /// <item><see cref="JsonObjectVM"/></item>
  /// <item><see cref="JsonArrayVM"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class JsonArrayVM : ViewModelBase, IEditableCollection
  {
    public DkmSuccessEvaluationResult Result { get; set; }

    private ObservableCollection<JsonVM> mValues = new ObservableCollection<JsonVM>();
    public ObservableCollection<JsonVM> Values
    {
      get { return mValues; }
    }

    public override string ToString() => $"{{ Array={{Values={Values.Count}}} }}";

    public void RemoveItem(object obj)
    {
      if (obj is JsonVM wJson && Values.IndexOf(wJson) is int wIndex && wIndex >= 0)
      {
        _ = Json4CPPVisualizerService.EvaluateExpression(
          Result,
          $"((Json4CPP.dll!{Result.Type}*)&{Result.FullName})->RemoveItem({wIndex})",
          flagsToRemove: DkmEvaluationFlags.NoSideEffects);
        Values.Remove(wJson);
        for (int i = wIndex; i < Values.Count; i++)
        {
          var wResult = Json4CPPVisualizerService.EvaluateExpression(Result, $"{Result.FullName}[{i}]");
          var wRebuiltJson = JsonBuilder.Build(wResult);
          Values[i] = wRebuiltJson;
        }
      }
    }

    public void AddItem()
    {
      _ = Json4CPPVisualizerService.EvaluateExpression(
        Result,
        $"((Json4CPP.dll!{Result.Type}*)&{Result.FullName})->AddItem()",
        flagsToRemove: DkmEvaluationFlags.NoSideEffects);
      var wResult = Json4CPPVisualizerService.EvaluateExpression(Result, $"{Result.FullName}[{Values.Count}]");
      var wJson = JsonBuilder.Build(wResult);
      Values.Add(wJson);
    }
  }
}
