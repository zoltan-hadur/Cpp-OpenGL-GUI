using Microsoft.VisualStudio.Debugger;
using Microsoft.VisualStudio.Debugger.Evaluation;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer
{
  /// <summary>
  /// <see cref="string"/> Key, <see cref="Json"/> Value pair.<para/>
  /// <see cref="Value"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObject"/></item>
  /// <item><see cref="JsonArray"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class Pair
  {
    public string Key { get; set; }
    public Json Value { get; set; }
    public override string ToString() => $"{Key}: {Value}";
  }

  /// <summary>
  /// <see cref="Value"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObject"/></item>
  /// <item><see cref="JsonArray"/></item>
  /// <item><see cref="Pair"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class Json
  {
    public object Value { get; set; }
    /// <summary>
    /// Returns the appropriate children according to the actual type of <see cref="Value"/>.
    /// </summary>
    public List<Json> Children
    {
      get
      {
        if (Value is JsonObject wObject)
        {
          return wObject.Pairs;
        }
        else if (Value is JsonArray wArray)
        {
          return wArray.Values;
        }
        else if (Value is Pair wPair)
        {
          return wPair.Value.Children;
        }
        else
        {
          return new List<Json>();
        }
      }
    }
    public override string ToString() => Value.ToString();
  }

  /// <summary>
  /// <see cref="Pairs"/> should contain only <see cref="Pair"/>s.
  /// </summary>
  public class JsonObject
  {
    public List<Json> Pairs { get; set; } = new List<Json>();
    public override string ToString() => $"{{ Object={{Pairs={Pairs.Count}}} }}";
  }

  /// <summary>
  /// <see cref="Values"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObject"/></item>
  /// <item><see cref="JsonArray"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class JsonArray
  {
    public List<Json> Values { get; set; } = new List<Json>();
    public override string ToString() => $"{{ Array={{Values={Values.Count}}} }}";
  }

  //             0               1             2     3       4        5           6
  //std::variant<std::nullptr_t, std::wstring, bool, double, int64_t, JsonObject, JsonArray>
  public enum JsonType
  {
    Null    = 0,
    String  = 1,
    Boolean = 2,
    Real    = 3,
    Integer = 4,
    Object  = 5,
    Array   = 6
  }

  public abstract class JsonBuilder
  {
    public static Json Build(DkmSuccessEvaluationResult result)
    {
      Json wJson = null;
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

    private static Json BuildJson(DkmSuccessEvaluationResult result)
    {
      Json wJson;
      var wWhichResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._value._Which");
      var wWhich = int.Parse(wWhichResult.Value.Replace(wWhichResult.EditableValue, string.Empty).Trim());
      var wValueResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._value{string.Concat(Enumerable.Repeat("._Tail", wWhich))}._Head");
      var wType = (JsonType)wWhich;
      switch (wType)
      {
        default:
          wJson = new Json()
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

    private static Json BuildJsonObject(DkmSuccessEvaluationResult result)
    {
      var wJsonObject = new JsonObject();
      var wSizeResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._pairs.size()");
      var wSize = int.Parse(wSizeResult.Value);
      for (int i = 0; i < wSize; i++)
      {
        var wFirstResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}[{i}].first");
        var wSecondResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}[{i}].second");
        wJsonObject.Pairs.Add(new Json
        {
          Value = new Pair
          {
            Key = wFirstResult.Value,
            Value = BuildJson(wSecondResult)
          }
        });
      }
      return new Json
      {
        Value = wJsonObject
      };
    }

    private static Json BuildJsonArray(DkmSuccessEvaluationResult result)
    {
      var wJsonArray = new JsonArray();
      var wSizeResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}._values.size()");
      var wSize = int.Parse(wSizeResult.Value);
      for (int i = 0; i < wSize; i++)
      {
        var wValueResult = Json4CPPVisualizerService.EvaluateExpression(result, $"{result.FullName}[{i}]");
        wJsonArray.Values.Add(BuildJson(wValueResult));
      }
      return new Json
      {
        Value = wJsonArray
      };
    }
  }
}
