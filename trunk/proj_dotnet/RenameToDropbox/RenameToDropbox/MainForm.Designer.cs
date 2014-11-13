namespace RenameToDropbox
{
    partial class MainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.labelTargetDir = new System.Windows.Forms.Label();
            this.textBoxTargetDir = new System.Windows.Forms.TextBox();
            this.buttonTargetDir = new System.Windows.Forms.Button();
            this.buttonRename = new System.Windows.Forms.Button();
            this.textBoxOutput = new System.Windows.Forms.TextBox();
            this.gbType = new System.Windows.Forms.GroupBox();
            this.rbTypePhoto = new System.Windows.Forms.RadioButton();
            this.rbTypeOruxmap = new System.Windows.Forms.RadioButton();
            this.gbType.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelTargetDir
            // 
            this.labelTargetDir.AutoSize = true;
            this.labelTargetDir.Location = new System.Drawing.Point(14, 16);
            this.labelTargetDir.Name = "labelTargetDir";
            this.labelTargetDir.Size = new System.Drawing.Size(57, 12);
            this.labelTargetDir.TabIndex = 0;
            this.labelTargetDir.Text = "대상폴더:";
            // 
            // textBoxTargetDir
            // 
            this.textBoxTargetDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxTargetDir.Location = new System.Drawing.Point(77, 13);
            this.textBoxTargetDir.Name = "textBoxTargetDir";
            this.textBoxTargetDir.Size = new System.Drawing.Size(385, 21);
            this.textBoxTargetDir.TabIndex = 1;
            // 
            // buttonTargetDir
            // 
            this.buttonTargetDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonTargetDir.Location = new System.Drawing.Point(468, 11);
            this.buttonTargetDir.Name = "buttonTargetDir";
            this.buttonTargetDir.Size = new System.Drawing.Size(29, 23);
            this.buttonTargetDir.TabIndex = 2;
            this.buttonTargetDir.Text = "...";
            this.buttonTargetDir.UseVisualStyleBackColor = true;
            this.buttonTargetDir.Click += new System.EventHandler(this.buttonTargetDir_Click);
            // 
            // buttonRename
            // 
            this.buttonRename.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRename.Location = new System.Drawing.Point(374, 40);
            this.buttonRename.Name = "buttonRename";
            this.buttonRename.Size = new System.Drawing.Size(123, 33);
            this.buttonRename.TabIndex = 3;
            this.buttonRename.Text = "&Rename";
            this.buttonRename.UseVisualStyleBackColor = true;
            this.buttonRename.Click += new System.EventHandler(this.buttonRename_Click);
            // 
            // textBoxOutput
            // 
            this.textBoxOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxOutput.Location = new System.Drawing.Point(16, 84);
            this.textBoxOutput.MinimumSize = new System.Drawing.Size(481, 123);
            this.textBoxOutput.Multiline = true;
            this.textBoxOutput.Name = "textBoxOutput";
            this.textBoxOutput.ReadOnly = true;
            this.textBoxOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxOutput.Size = new System.Drawing.Size(481, 123);
            this.textBoxOutput.TabIndex = 4;
            // 
            // gbType
            // 
            this.gbType.Controls.Add(this.rbTypePhoto);
            this.gbType.Controls.Add(this.rbTypeOruxmap);
            this.gbType.Location = new System.Drawing.Point(16, 40);
            this.gbType.Name = "gbType";
            this.gbType.Size = new System.Drawing.Size(320, 38);
            this.gbType.TabIndex = 5;
            this.gbType.TabStop = false;
            this.gbType.Text = "타입";
            // 
            // rbTypePhoto
            // 
            this.rbTypePhoto.AutoSize = true;
            this.rbTypePhoto.Location = new System.Drawing.Point(98, 16);
            this.rbTypePhoto.Name = "rbTypePhoto";
            this.rbTypePhoto.Size = new System.Drawing.Size(55, 16);
            this.rbTypePhoto.TabIndex = 1;
            this.rbTypePhoto.Text = "Photo";
            this.rbTypePhoto.UseVisualStyleBackColor = true;
            // 
            // rbTypeOruxmap
            // 
            this.rbTypeOruxmap.AutoSize = true;
            this.rbTypeOruxmap.Checked = true;
            this.rbTypeOruxmap.Location = new System.Drawing.Point(16, 16);
            this.rbTypeOruxmap.Name = "rbTypeOruxmap";
            this.rbTypeOruxmap.Size = new System.Drawing.Size(75, 16);
            this.rbTypeOruxmap.TabIndex = 0;
            this.rbTypeOruxmap.TabStop = true;
            this.rbTypeOruxmap.Text = "Oruxmap";
            this.rbTypeOruxmap.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(509, 219);
            this.Controls.Add(this.gbType);
            this.Controls.Add(this.textBoxOutput);
            this.Controls.Add(this.buttonRename);
            this.Controls.Add(this.buttonTargetDir);
            this.Controls.Add(this.textBoxTargetDir);
            this.Controls.Add(this.labelTargetDir);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.Text = "Rename";
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainForm_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainForm_DragEnter);
            this.gbType.ResumeLayout(false);
            this.gbType.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelTargetDir;
        private System.Windows.Forms.TextBox textBoxTargetDir;
        private System.Windows.Forms.Button buttonTargetDir;
        private System.Windows.Forms.Button buttonRename;
        private System.Windows.Forms.TextBox textBoxOutput;
        private System.Windows.Forms.GroupBox gbType;
        private System.Windows.Forms.RadioButton rbTypePhoto;
        private System.Windows.Forms.RadioButton rbTypeOruxmap;
    }
}

