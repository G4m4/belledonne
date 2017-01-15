//------------------------------------------------------------------------------
// <copyright file="BrowserWindow.cs" company="Microsoft">
//     Copyright (c) Microsoft.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

namespace BelledonneVSIX
{
  using System;
  using System.Runtime.InteropServices;
  using Microsoft.VisualStudio.Shell;

  /// <summary>
  /// This class implements the tool window exposed by this package and hosts a user control.
  /// </summary>
  /// <remarks>
  /// In Visual Studio tool windows are composed of a frame (implemented by the shell) and a pane,
  /// usually implemented by the package implementer.
  /// <para>
  /// This class derives from the ToolWindowPane class provided from the MPF in order to use its
  /// implementation of the IVsUIElementPane interface.
  /// </para>
  /// </remarks>
  [Guid("4c22a30b-b654-4e95-be7b-a79d8b269447")]
  public class BrowserWindow : ToolWindowPane
  {
    /// <summary>
    /// Initializes a new instance of the <see cref="BrowserWindow"/> class.
    /// </summary>
    public BrowserWindow() : base(null)
    {
      this.Caption = "BrowserWindow";

      // This is the user control hosted by the tool window; Note that, even if this class implements IDisposable,
      // we are not calling Dispose on this object. This is because ToolWindowPane calls Dispose on
      // the object returned by the Content property.
      this.Content = new BrowserWindowControl();
    }
  }
}
