﻿using Json4CPP.Visualizer.Interfaces;
using Microsoft.VisualStudio.Debugger.Evaluation;
using System.ComponentModel;
using System.Linq;

namespace Json4CPP.Visualizer.ViewModels
{
  /// <summary>
  /// <see cref="string"/> Key, <see cref="JsonVM"/> Value pair.<para/>
  /// <see cref="Value"/> should contain one of the following types:
  /// <list type="bullet">
  /// <item><see cref="JsonObjectVM"/></item>
  /// <item><see cref="JsonArrayVM"/></item>
  /// <item><see cref="string"/></item>
  /// </list>
  /// </summary>
  public class PairVM : ViewModelBase, IEditableCollection
  {
    private DkmSuccessEvaluationResult mResult;

    private JsonObjectVM mParent;

    private string mKey;
    public string Key
    {
      get => mKey;
      set => Set(ref mKey, value);
    }

    private JsonVM mValue;
    public JsonVM Value
    {
      get => mValue;
      set
      {
        if (mValue != null)
        {
          mValue.PropertyChanged -= Value_PropertyChanged;
        }
        Set(ref mValue, value);
        if (mValue != null)
        {
          mValue.PropertyChanged += Value_PropertyChanged;
        }
      }
    }

    private void Value_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      switch (e.PropertyName)
      {
        case nameof(Value.IsObject):
        case nameof(Value.IsArray):
          OnPropertyChanged(nameof(IsObject));
          OnPropertyChanged(nameof(IsArray));
          break;
        case nameof(Value.Value):
          OnPropertyChanged(nameof(Value));
          break;
      }
    }

    public bool IsObject => Value.IsObject;
    public bool IsArray => Value.IsArray;

    protected PairVM()
    {

    }

    public PairVM(DkmSuccessEvaluationResult result, JsonObjectVM parent)
    {
      mResult = result;
      mParent = parent;

      var wFirstResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}.first");
      var wSecondResult = Json4CPPVisualizerService.EvaluateExpression(mResult, $"{mResult.FullName}.second");
      mKey = wFirstResult.Value;
      mValue = JsonBuilder.Build(wSecondResult);

      PropertyChanged += PairVM_PropertyChanged;
    }

    private void PairVM_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
      switch (e.PropertyName)
      {
        case nameof(Key):
          {
            if (!HasError(nameof(Key)))
            {
              mParent.ChangeKey(mParent.Pairs.IndexOf(this), Key);
            }
            break;
          }
        case nameof(Value):
          OnPropertyChanged(nameof(IsObject));
          OnPropertyChanged(nameof(IsArray));
          break;
      }
    }

    public override string ToString() => $"{Key}: {Value}";

    protected override void OnValidate(string propertyName)
    {
      base.OnValidate(propertyName);
      switch (propertyName)
      {
        case nameof(Key):
          {
            if (Key == null)
            {
              AddError(propertyName, $"{nameof(Key)} can't be null!");
            }
            if (!(Key.StartsWith("L\"") && Key.EndsWith("\"") && Key.Length >= 3))
            {
              AddError(propertyName, $"{nameof(Key)} must be enclosed within \"L\"\" and \"\"\"!");
            }
            if (mParent.Pairs.Any(wPair => wPair != this && wPair?.Key == Key) == true)
            {
              AddError(propertyName, $"{nameof(Key)} must be unique!");
            }
          }
          break;
      }
    }

    public void RemoveItem(object obj)
    {
      if (Value is IEditableCollection wValue)
      {
        wValue.RemoveItem(obj);
      }
    }

    public void AddItem()
    {
      if (Value is IEditableCollection wValue)
      {
        wValue.AddItem();
      }
    }
  }
}