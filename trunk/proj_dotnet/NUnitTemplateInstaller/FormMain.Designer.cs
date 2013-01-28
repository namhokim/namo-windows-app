namespace NUnitTemplateInstaller
{
    partial class FormMain
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
            this.labelVS2008ver = new System.Windows.Forms.Label();
            this.labelTypes = new System.Windows.Forms.Label();
            this.buttonInstall = new System.Windows.Forms.Button();
            this.buttonUninstall = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // labelVS2008ver
            // 
            this.labelVS2008ver.AutoSize = true;
            this.labelVS2008ver.Location = new System.Drawing.Point(13, 13);
            this.labelVS2008ver.Name = "labelVS2008ver";
            this.labelVS2008ver.Size = new System.Drawing.Size(168, 12);
            this.labelVS2008ver.TabIndex = 0;
            this.labelVS2008ver.Text = "Detected Visual Studio 2008 :";
            // 
            // labelTypes
            // 
            this.labelTypes.AutoSize = true;
            this.labelTypes.Location = new System.Drawing.Point(202, 12);
            this.labelTypes.Name = "labelTypes";
            this.labelTypes.Size = new System.Drawing.Size(74, 12);
            this.labelTypes.TabIndex = 1;
            this.labelTypes.Text = "{Type, bits}";
            // 
            // buttonInstall
            // 
            this.buttonInstall.Enabled = false;
            this.buttonInstall.Location = new System.Drawing.Point(48, 51);
            this.buttonInstall.Name = "buttonInstall";
            this.buttonInstall.Size = new System.Drawing.Size(126, 41);
            this.buttonInstall.TabIndex = 2;
            this.buttonInstall.Text = "&Install";
            this.buttonInstall.UseVisualStyleBackColor = true;
            // 
            // buttonUninstall
            // 
            this.buttonUninstall.Enabled = false;
            this.buttonUninstall.Location = new System.Drawing.Point(206, 51);
            this.buttonUninstall.Name = "buttonUninstall";
            this.buttonUninstall.Size = new System.Drawing.Size(126, 41);
            this.buttonUninstall.TabIndex = 3;
            this.buttonUninstall.Text = "&Uninstall";
            this.buttonUninstall.UseVisualStyleBackColor = true;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(386, 123);
            this.Controls.Add(this.buttonUninstall);
            this.Controls.Add(this.buttonInstall);
            this.Controls.Add(this.labelTypes);
            this.Controls.Add(this.labelVS2008ver);
            this.Name = "FormMain";
            this.ShowIcon = false;
            this.Text = "NUnit with ASP.NET MVC 2 Installer";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelVS2008ver;
        private System.Windows.Forms.Label labelTypes;
        private System.Windows.Forms.Button buttonInstall;
        private System.Windows.Forms.Button buttonUninstall;
    }
}

