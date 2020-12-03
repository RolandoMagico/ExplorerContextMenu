// Copyright (c) 2020 Roland Reinl.
// All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace ContextMenu
{
  using System;
  using System.Collections.Generic;
  using System.IO;
  using System.Linq;
  using System.Windows.Forms;
  using ContextQuickie;

  /// <summary>
  /// Main window of the application.
  /// </summary>
  public partial class MainWindow : Form
  {
    /// <summary>
    /// Initializes a new instance of the <see cref="MainWindow"/> class.
    /// </summary>
    public MainWindow()
    {
      this.InitializeComponent();
    }

    private void Form1_Load(object sender, EventArgs e)
    {
      foreach (var driveInfo in DriveInfo.GetDrives())
      {
        TreeNode driveNode = this.treeViewNavigator.Nodes.Add(driveInfo.Name);
        driveNode.Tag = driveInfo.Name;
        if (Directory.GetDirectories(driveInfo.Name).Any() || Directory.GetFiles(driveInfo.Name).Any())
        {
          driveNode.Nodes.Add(new TreeNode());
        }
      }
    }

    /// <summary>
    /// Handles the event <see cref="TreeView.BeforeExpand"/> of <see cref="treeViewNavigator"/>.
    /// </summary>
    /// <param name="sender">The sender.</param>
    /// <param name="e">The event arguments.</param>
    private void HandleEvent_treeViewNavigator_BeforeExpand(object sender, TreeViewCancelEventArgs e)
    {
      if ((e.Node.Nodes.Count == 1) && (e.Node.Nodes[0].Tag == null))
      {
        e.Node.Nodes.Clear();
        string selectedDirectory = e.Node.Tag as string;
        foreach (string directory in Directory.GetDirectories(selectedDirectory))
        {
          TreeNode directoryNode = new TreeNode(Path.GetFileName(directory));
          directoryNode.Tag = directory;
          try
          {
            if (Directory.GetDirectories(directory).Any() || Directory.GetFiles(directory).Any())
            {
              directoryNode.Nodes.Add(new TreeNode());
            }

            e.Node.Nodes.Add(directoryNode);
          }
          catch
          {
          }
        }

        foreach (string file in Directory.GetFiles(selectedDirectory))
        {
          TreeNode fileNode = new TreeNode(Path.GetFileName(file));
          fileNode.Tag = file;
          e.Node.Nodes.Add(fileNode);
        }
      }
    }

    /// <summary>
    /// Handles the event <see cref="TreeView.NodeMouseClick"/> of <see cref="treeViewNavigator"/>.
    /// </summary>
    /// <param name="sender">The sender.</param>
    /// <param name="e">The event arguments.</param>
    private void HandleEvent_treeViewNavigator_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
    {
      if ((e.Button == MouseButtons.Right) && (e.Node.Tag is string path))
      {
        List<string> paths = new List<string> { path };
        NetExplorerContextMenu menu = new NetExplorerContextMenu(paths);

        if (menu.Childs.Any())
        {
          e.Node.ContextMenuStrip = new ContextMenuStrip();
          foreach (NetExplorerContextMenuEntry childEntry in menu.Childs)
          {
            e.Node.ContextMenuStrip.Items.Add(this.ConvertContextMenuEntryToToolStipItem(childEntry));
          }
        }
      }
    }

    private void MenuItem_Click(object sender, EventArgs e)
    {
      if ((sender is ToolStripItem toolStripItem) && (toolStripItem.Tag is NetExplorerContextMenuEntry entry))
      {
        entry.ExecuteCommand();
      }
    }

    private ToolStripItem ConvertContextMenuEntryToToolStipItem(NetExplorerContextMenuEntry entry)
    {
      ToolStripItem result = null;
      if (entry.IsSeperator)
      {
        result = new ToolStripSeparator();
      }
      else
      {
        ToolStripMenuItem toolStripMenuItem = new ToolStripMenuItem(entry.Text);
        toolStripMenuItem.Tag = entry;
        toolStripMenuItem.ToolTipText = entry.HelpText;
        if (entry.Childs.Any())
        {
          foreach (NetExplorerContextMenuEntry childEntry in entry.Childs)
          {
            toolStripMenuItem.DropDownItems.Add(this.ConvertContextMenuEntryToToolStipItem(childEntry));
          }
        }
        else
        {
          toolStripMenuItem.Click += this.MenuItem_Click;
        }

        toolStripMenuItem.Image = entry.Icon;

        result = toolStripMenuItem;
      }

      return result;
    }
  }
}
