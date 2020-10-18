namespace ContextMenu
{
  partial class MainWindow
  {
    /// <summary>
    /// Erforderliche Designervariable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Verwendete Ressourcen bereinigen.
    /// </summary>
    /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Vom Windows Form-Designer generierter Code

    /// <summary>
    /// Erforderliche Methode für die Designerunterstützung.
    /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
    /// </summary>
    private void InitializeComponent()
    {
      this.treeViewNavigator = new System.Windows.Forms.TreeView();
      this.SuspendLayout();
      // 
      // treeViewNavigator
      // 
      this.treeViewNavigator.Dock = System.Windows.Forms.DockStyle.Fill;
      this.treeViewNavigator.Location = new System.Drawing.Point(0, 0);
      this.treeViewNavigator.Name = "treeViewNavigator";
      this.treeViewNavigator.Size = new System.Drawing.Size(800, 450);
      this.treeViewNavigator.TabIndex = 0;
      this.treeViewNavigator.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.HandleEvent_treeViewNavigator_BeforeExpand);
      this.treeViewNavigator.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.HandleEvent_treeViewNavigator_NodeMouseClick);
      // 
      // MainWindow
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(800, 450);
      this.Controls.Add(this.treeViewNavigator);
      this.Name = "MainWindow";
      this.Text = "Context Menu Example";
      this.Load += new System.EventHandler(this.Form1_Load);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.TreeView treeViewNavigator;
  }
}

