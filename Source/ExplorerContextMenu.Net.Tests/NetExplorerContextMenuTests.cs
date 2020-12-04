// Copyright (c) 2020 Roland Reinl.
// All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace ExplorerContextMenu.Net.Tests
{
  using System;
  using System.Collections.Generic;
  using System.IO;
  using System.Linq;
  using ContextQuickie;
  using Xunit;

  /// <summary>
  /// Unit tests for the <see cref="NetExplorerContextMenu"/> class.
  /// </summary>
  public class NetExplorerContextMenuTests : IDisposable
  {
    private readonly List<string> createdFiles = new List<string>();

    private readonly List<string> createdDirectories = new List<string>();

    /// <summary>
    /// Test that an empty menu is created if no paths are passed.
    /// </summary>
    [StaFact]
    public void TestEmptyPathsList()
    {
      NetExplorerContextMenu menu = new NetExplorerContextMenu(new List<string>());
      Assert.Empty(menu.Childs);
    }

    /// <summary>
    /// Test that a non empty menu is created if one file is passed.
    /// </summary>
    [StaFact]
    public void TestOneFileAsPaths()
    {
      NetExplorerContextMenu menu = new NetExplorerContextMenu(new List<string> { this.CreateTempFile() });
      Assert.NotEmpty(menu.Childs);
    }

    /// <summary>
    /// Test that a non empty menu is created if two files are passed.
    /// </summary>
    [StaFact]
    public void TestTwoFilesAsPaths()
    {
      NetExplorerContextMenu menu = new NetExplorerContextMenu(new List<string> { this.CreateTempFile(), this.CreateTempFile() });
      Assert.NotEmpty(menu.Childs);
    }

    /// <summary>
    /// Test that a non empty menu is created if one directory is passed.
    /// </summary>
    [StaFact]
    public void TestOneDirectoryAsPaths()
    {
      NetExplorerContextMenu menu = new NetExplorerContextMenu(new List<string> { this.CreateTempDirectory() });
      Assert.NotEmpty(menu.Childs);
    }

    /// <summary>
    /// Test that a non empty menu is created if two directories are passed.
    /// </summary>
    [StaFact]
    public void TestTwoDirectoriesAsPaths()
    {
      NetExplorerContextMenu menu = new NetExplorerContextMenu(new List<string> { this.CreateTempDirectory() });
      Assert.NotEmpty(menu.Childs);
    }

    /// <summary>
    /// Test that a non empty menu is created if two directories with different parents are passed.
    /// </summary>
    [StaFact]
    public void TestTwoDirectoriesWithDifferentParentsAsPaths()
    {
      NetExplorerContextMenu menu = new NetExplorerContextMenu(new List<string> { this.CreateTempDirectory(), Path.GetPathRoot(this.CreateTempDirectory()) });
      Assert.NotEmpty(menu.Childs);
    }

    /// <summary>
    /// Test if Beyond Compare "Select left file" is present with one file.
    /// This tests will fail if Beyond compare is not installed.
    /// </summary>
    [StaFact]
    public void TestBeyondCompareWithOneFile()
    {
      NetExplorerContextMenu menu = new NetExplorerContextMenu(new List<string> { this.CreateTempFile() });
      Assert.Single(menu.Childs.Where(child => child.Text == "Select &Left File for Compare"));
    }

    /// <inheritdoc/>
    public void Dispose()
    {
      this.createdFiles.ForEach(File.Delete);
      this.createdDirectories.ForEach(Directory.Delete);
    }

    private string CreateTempFile()
    {
      string path = Path.GetTempFileName();
      this.createdFiles.Add(path);
      return path;
    }

    private string CreateTempDirectory()
    {
      string path = Path.GetTempFileName();
      File.Delete(path);
      Directory.CreateDirectory(path);
      this.createdDirectories.Add(path);
      return path;
    }
  }
}
