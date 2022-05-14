namespace Json4CPP.Visualizer.ViewModel
{
  public class VisualizerWindowVMDesign : VisualizerWindowVM
  {
    public VisualizerWindowVMDesign()
    {
      var wInnerInnerJsonObject = new JsonObjectVM();
      wInnerInnerJsonObject.Pairs.Add(new PairVM { Key = "L\"Key1\"", Value = new JsonVM { Value = "1" } });
      wInnerInnerJsonObject.Pairs.Add(new PairVM { Key = "L\"Key2\"", Value = new JsonVM { Value = "2" } });
      var wInnerInnerJsonArray = new JsonArrayVM();
      wInnerInnerJsonArray.Values.Add(new JsonVM { Value = "1" });
      wInnerInnerJsonArray.Values.Add(new JsonVM { Value = "2" });
      wInnerInnerJsonArray.Values.Add(new JsonVM { Value = "3" });
      var wInnerJsonObject = new JsonObjectVM();
      wInnerJsonObject.Pairs.Add(new PairVM { Key = "L\"Key1\"", Value = new JsonVM { Value = "1" } });
      wInnerJsonObject.Pairs.Add(new PairVM { Key = "L\"Key2\"", Value = new JsonVM { Value = "2" } });
      wInnerJsonObject.Pairs.Add(new PairVM { Key = "L\"Key3\"", Value = new JsonVM { Value = wInnerInnerJsonObject } });
      wInnerJsonObject.Pairs.Add(new PairVM { Key = "L\"Key4\"", Value = new JsonVM { Value = wInnerInnerJsonArray } });
      var wInnerJsonArray = new JsonArrayVM();
      wInnerJsonArray.Values.Add(new JsonVM { Value = "1" });
      wInnerJsonArray.Values.Add(new JsonVM { Value = "2" });
      wInnerJsonArray.Values.Add(new JsonVM { Value = "3" });
      wInnerJsonArray.Values.Add(new JsonVM { Value = wInnerInnerJsonObject });
      wInnerJsonArray.Values.Add(new JsonVM { Value = wInnerInnerJsonArray });
      var wJsonObject = new JsonObjectVM();
      wJsonObject.Pairs.Add(new PairVM { Key = "L\"Null\"", Value = new JsonVM { Value = "null" } });
      wJsonObject.Pairs.Add(new PairVM { Key = "L\"String\"", Value = new JsonVM { Value = "L\"Test\"" } });
      wJsonObject.Pairs.Add(new PairVM { Key = "L\"Boolean\"", Value = new JsonVM { Value = "true" } });
      wJsonObject.Pairs.Add(new PairVM { Key = "L\"Real\"", Value = new JsonVM { Value = "13.37" } });
      wJsonObject.Pairs.Add(new PairVM { Key = "L\"Integer\"", Value = new JsonVM { Value = "1337" } });
      wJsonObject.Pairs.Add(new PairVM { Key = "L\"Object\"", Value = new JsonVM { Value = wInnerJsonObject } });
      wJsonObject.Pairs.Add(new PairVM { Key = "L\"Array\"", Value = new JsonVM { Value = wInnerJsonArray } });
      Expression = "object._value._Tail._Tail._Tail._Tail._Tail._Head";
      Json = new JsonVM { Value = wJsonObject };
    }
  }
}
