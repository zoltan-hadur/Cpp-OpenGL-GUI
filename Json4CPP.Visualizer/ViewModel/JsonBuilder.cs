using Microsoft.VisualStudio.Debugger;
using Microsoft.VisualStudio.Debugger.Evaluation;
using System;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer.ViewModel
{

  public abstract class JsonBuilder
  {
    public static JsonVM Build(DkmSuccessEvaluationResult result)
    {
      JsonVM wJson = null;
      switch (result.Type)
      {
        case "Json4CPP::Json":
          wJson = BuildJson(result);
          break;
        case "Json4CPP::JsonObject":
          wJson = BuildJsonObject(result);
          break;
        case "Json4CPP::JsonArray":
          wJson = BuildJsonArray(result);
          break;
      }
      return wJson;
    }

    private static JsonVM BuildJson(DkmSuccessEvaluationResult result)
    {
      JsonVM wJson;
      var wWhichResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._value._Which");
      var wWhich = int.Parse(wWhichResult.Value.Replace(wWhichResult.EditableValue, string.Empty).Trim());
      var wValueResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._value{string.Concat(Enumerable.Repeat("._Tail", wWhich))}._Head");
      var wType = (JsonType)wWhich;
      switch (wType)
      {
        default:
          wJson = new JsonVM()
          {
            Value = result.Value
          };
          break;
        case JsonType.Object:
          wJson = BuildJsonObject(wValueResult);
          break;
        case JsonType.Array:
          wJson = BuildJsonArray(wValueResult);
          break;
      }
      return wJson;
    }

    private static JsonVM BuildJsonObject(DkmSuccessEvaluationResult result)
    {
      var wJsonObject = new JsonObjectVM();
      var wSizeResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._pairs.size()");
      var wSize = int.Parse(wSizeResult.Value);
      for (int i = 0; i < wSize; i++)
      {
        var wFirstResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}[{i}].first");
        var wSecondResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}[{i}].second");
        wJsonObject.Pairs.Add(new PairVM
        {
          Key = wFirstResult.Value,
          Value = BuildJson(wSecondResult)
        });
      }
      return new JsonVM
      {
        Value = wJsonObject
      };
    }

    private static JsonVM BuildJsonArray(DkmSuccessEvaluationResult result)
    {
      var wJsonArray = new JsonArrayVM();
      var wSizeResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._values.size()");
      var wSize = int.Parse(wSizeResult.Value);
      for (int i = 0; i < wSize; i++)
      {
        var wValueResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}[{i}]");
        wJsonArray.Values.Add(BuildJson(wValueResult));
      }
      return new JsonVM
      {
        Value = wJsonArray
      };
    }
  }
}
