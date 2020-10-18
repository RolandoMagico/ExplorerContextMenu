// Copyright (c) 2020 Roland Reinl.
// All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace ContextMenu
{
  using System;
  using System.Windows.Forms;

  /// <summary>
  /// The entry class of the application.
  /// </summary>
  public static class Program
  {
    /// <summary>
    /// The entry method of the application.
    /// </summary>
    [STAThread]
    public static void Main()
    {
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new MainWindow());
    }
  }
}
