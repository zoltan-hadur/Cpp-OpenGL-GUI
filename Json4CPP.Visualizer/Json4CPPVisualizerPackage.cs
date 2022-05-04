using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;
using System.Threading;
using Task = System.Threading.Tasks.Task;

namespace Json4CPP.Visualizer
{
  /// <summary>
  /// Package providing the visualizer.
  /// </summary>
  [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
  [ProvideService(typeof(IJson4CPPVisualizerService), ServiceName = nameof(Json4CPPVisualizerService), IsAsyncQueryable = true)]
  [Guid(PackageGuidString)]
  public sealed class Json4CPPVisualizerPackage : AsyncPackage
  {
    /// <summary>
    /// Json4CPPVisualizerPackage GUID string.
    /// </summary>
    public const string PackageGuidString = "9EF6E3C5-1CE7-42AD-BE3D-E851B495B273";

    /// <inheritdoc />
    protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
    {
      await base.InitializeAsync(cancellationToken, progress);
      AddService(typeof(IJson4CPPVisualizerService), (sc, ct, st) => Task.FromResult<object>(new Json4CPPVisualizerService()), true);
    }
  }
}
