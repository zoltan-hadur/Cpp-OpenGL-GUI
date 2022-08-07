using Microsoft.VisualStudio.Debugger;
using Microsoft.VisualStudio.Debugger.Evaluation;
using System;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer.ViewModels
{

  public abstract class JsonBuilder
  {
    public static JsonVM Build(DkmSuccessEvaluationResult result)
    {
      JsonVM wJson = null;
      switch (result.Type)
      {
        case "Json4CPP::Json":
          {
            var wWhichResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._value._Which");
            var wWhich = int.Parse(wWhichResult.Value.Replace(wWhichResult.EditableValue, string.Empty).Trim());
            var wValueResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._value{string.Concat(Enumerable.Repeat("._Tail", wWhich))}._Head");
            var wType = (JsonType)wWhich;
            switch (wType)
            {
              default:
                wJson = new JsonVM(result, result.Value);
                break;
              case JsonType.Object:
                wJson = new JsonVM(null, new JsonObjectVM(wValueResult));
                break;
              case JsonType.Array:
                wJson = new JsonVM(null, new JsonArrayVM(wValueResult));
                break;
            }
            break;
          }
        case "Json4CPP::JsonObject":
          {
            wJson = new JsonVM(null, new JsonObjectVM(result));
            break;
          }
        case "Json4CPP::JsonArray":
          {
            wJson = new JsonVM(null, new JsonArrayVM(result));
            break;
          }
      }
      return wJson;
    }
  }
}
