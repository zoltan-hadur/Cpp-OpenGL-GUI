using Microsoft.VisualStudio.Debugger.Evaluation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Json4CPP.Visualizer
{
  public class ViewModel : ViewModelBase
  {
    private DkmSuccessEvaluationResult mResult;

    private string mExpression;
    public string Expression
    {
      get => mExpression;
      set => Set(ref mExpression, value);
    }

    private Json mJson;
    public Json Json
    {
      get => mJson;
      set => Set(ref mJson, value);
    }

    protected ViewModel()
    {

    }

    public ViewModel(DkmSuccessEvaluationResult result)
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

  public class DesignViewModel : ViewModel
  {
    public DesignViewModel()
    {
      var wInnerJsonObject = new JsonObject();
      wInnerJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Key1\"", Value = new Json { Value = "1" } } });
      wInnerJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Key1\"", Value = new Json { Value = "2" } } });
      var wInnerJsonArray = new JsonArray();
      wInnerJsonArray.Values.Add(new Json { Value = "1" });
      wInnerJsonArray.Values.Add(new Json { Value = "2" });
      wInnerJsonArray.Values.Add(new Json { Value = "3" });
      var wJsonObject = new JsonObject();
      wJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Null\"", Value = new Json { Value = "null" } } });
      wJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"String\"", Value = new Json { Value = "L\"Test\"" } } });
      wJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Boolean\"", Value = new Json { Value = "true" } } });
      wJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Real\"", Value = new Json { Value = "13.37" } } });
      wJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Integer\"", Value = new Json { Value = "1337" } } });
      wJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Object\"", Value = new Json { Value = wInnerJsonObject } } });
      wJsonObject.Pairs.Add(new Json { Value = new Pair { Key = "L\"Array\"", Value = new Json { Value = wInnerJsonArray } } });
      Expression = "object._value._Tail._Tail._Tail._Tail._Tail._Head";
      Json = new Json { Value = wJsonObject };
    }
  }
}
