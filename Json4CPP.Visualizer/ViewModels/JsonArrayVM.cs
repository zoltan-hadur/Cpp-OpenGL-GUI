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
    private DkmSuccessEvaluationResult mResult;

    private ObservableCollection<JsonVM> mValues = new ObservableCollection<JsonVM>();
    public ObservableCollection<JsonVM> Values
    {
      get => mValues;
    }

    protected JsonArrayVM()
    {

    }

    public JsonArrayVM(DkmSuccessEvaluationResult result)
    {
      mResult = result;

      // Query the size of the c++ object
      var wSizeResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}._values.size()");
      var wSize = int.Parse(wSizeResult.Value);

      // Query each element of the c++ object and add it to the c# ui
      for (int i = 0; i < wSize; i++)
      {
        var wValueResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}[{i}]");
        Values.Add(JsonBuilder.Build(wValueResult));
      }
    }

    public override string ToString() => $"{{ Array={{Values={Values.Count}}} }}";

    public void RemoveItem(object obj)
    {
      if (obj is JsonVM wJson && Values.IndexOf(wJson) is int wIndex && wIndex >= 0)
      {
        // Remove item from the c++ object
        _ = Json4CPPVisualizerService.EvaluateExpression(
          mResult,
          $"((Json4CPP.dll!{mResult.Type}*)&{mResult.FullName})->RemoveItem({wIndex})",
          flagsToRemove: DkmEvaluationFlags.NoSideEffects);

        // Remove item from the c# ui
        Values.Remove(wJson);

        // Refresh the remaining ones
        for (int i = wIndex; i < Values.Count; i++)
        {
          var wResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}[{i}]");
          var wRebuiltJson = JsonBuilder.Build(wResult);
          Values[i] = wRebuiltJson;
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
      var wResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}[{Values.Count}]");
      Values.Add(JsonBuilder.Build(wResult));
    }
  }
}
