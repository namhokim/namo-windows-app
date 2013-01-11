namespace SQLServerNDT.Forms
{
    partial class FormConnection
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormConnection));
            this.labelServerName = new System.Windows.Forms.Label();
            this.labelAuthentication = new System.Windows.Forms.Label();
            this.labelLogin = new System.Windows.Forms.Label();
            this.labelPassword = new System.Windows.Forms.Label();
            this.labelSeperator = new System.Windows.Forms.Label();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.comboBoxServerName = new System.Windows.Forms.ComboBox();
            this.comboBoxAuthenticationType = new System.Windows.Forms.ComboBox();
            this.comboBoxID = new System.Windows.Forms.ComboBox();
            this.textBoxPassword = new System.Windows.Forms.TextBox();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.backgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.SuspendLayout();
            // 
            // labelServerName
            // 
            this.labelServerName.AutoSize = true;
            this.labelServerName.Location = new System.Drawing.Point(29, 17);
            this.labelServerName.Name = "labelServerName";
            this.labelServerName.Size = new System.Drawing.Size(79, 12);
            this.labelServerName.TabIndex = 0;
            this.labelServerName.Text = "서버 이름(&S):";
            // 
            // labelAuthentication
            // 
            this.labelAuthentication.AutoSize = true;
            this.labelAuthentication.Location = new System.Drawing.Point(30, 44);
            this.labelAuthentication.Name = "labelAuthentication";
            this.labelAuthentication.Size = new System.Drawing.Size(51, 12);
            this.labelAuthentication.TabIndex = 1;
            this.labelAuthentication.Text = "인증(&A):";
            // 
            // labelLogin
            // 
            this.labelLogin.AutoSize = true;
            this.labelLogin.Location = new System.Drawing.Point(46, 74);
            this.labelLogin.Name = "labelLogin";
            this.labelLogin.Size = new System.Drawing.Size(62, 12);
            this.labelLogin.TabIndex = 2;
            this.labelLogin.Text = "로그인(&L):";
            // 
            // labelPassword
            // 
            this.labelPassword.AutoSize = true;
            this.labelPassword.Location = new System.Drawing.Point(46, 100);
            this.labelPassword.Name = "labelPassword";
            this.labelPassword.Size = new System.Drawing.Size(51, 12);
            this.labelPassword.TabIndex = 3;
            this.labelPassword.Text = "암호(&P):";
            // 
            // labelSeperator
            // 
            this.labelSeperator.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelSeperator.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.labelSeperator.Location = new System.Drawing.Point(24, 135);
            this.labelSeperator.Name = "labelSeperator";
            this.labelSeperator.Size = new System.Drawing.Size(475, 2);
            this.labelSeperator.TabIndex = 4;
            this.labelSeperator.Text = "labelSeperator";
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(303, 149);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(100, 30);
            this.buttonConnect.TabIndex = 5;
            this.buttonConnect.Text = "연결(&C)";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(409, 149);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(100, 30);
            this.buttonCancel.TabIndex = 6;
            this.buttonCancel.Text = "취소";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // comboBoxServerName
            // 
            this.comboBoxServerName.FormattingEnabled = true;
            this.comboBoxServerName.Location = new System.Drawing.Point(209, 12);
            this.comboBoxServerName.Name = "comboBoxServerName";
            this.comboBoxServerName.Size = new System.Drawing.Size(300, 20);
            this.comboBoxServerName.TabIndex = 7;
            // 
            // comboBoxAuthenticationType
            // 
            this.comboBoxAuthenticationType.FormattingEnabled = true;
            this.comboBoxAuthenticationType.Items.AddRange(new object[] {
            "Windows 인증",
            "SQL Server 인증"});
            this.comboBoxAuthenticationType.Location = new System.Drawing.Point(209, 41);
            this.comboBoxAuthenticationType.Name = "comboBoxAuthenticationType";
            this.comboBoxAuthenticationType.Size = new System.Drawing.Size(300, 20);
            this.comboBoxAuthenticationType.TabIndex = 8;
            this.comboBoxAuthenticationType.SelectedIndexChanged += new System.EventHandler(this.comboBoxAuthenticationType_SelectedIndexChanged);
            // 
            // comboBoxID
            // 
            this.comboBoxID.FormattingEnabled = true;
            this.comboBoxID.Location = new System.Drawing.Point(229, 67);
            this.comboBoxID.Name = "comboBoxID";
            this.comboBoxID.Size = new System.Drawing.Size(280, 20);
            this.comboBoxID.TabIndex = 9;
            // 
            // textBoxPassword
            // 
            this.textBoxPassword.Location = new System.Drawing.Point(229, 93);
            this.textBoxPassword.MaxLength = 10000;
            this.textBoxPassword.Name = "textBoxPassword";
            this.textBoxPassword.Size = new System.Drawing.Size(280, 21);
            this.textBoxPassword.TabIndex = 10;
            this.textBoxPassword.UseSystemPasswordChar = true;
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(24, 123);
            this.progressBar.MarqueeAnimationSpeed = 10;
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(475, 8);
            this.progressBar.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            this.progressBar.TabIndex = 11;
            this.progressBar.Visible = false;
            // 
            // backgroundWorker
            // 
            this.backgroundWorker.WorkerReportsProgress = true;
            this.backgroundWorker.WorkerSupportsCancellation = true;
            this.backgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_DoWork);
            this.backgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorker_RunWorkerCompleted);
            // 
            // FormConnection
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(521, 191);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.textBoxPassword);
            this.Controls.Add(this.comboBoxID);
            this.Controls.Add(this.comboBoxAuthenticationType);
            this.Controls.Add(this.comboBoxServerName);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.labelSeperator);
            this.Controls.Add(this.labelPassword);
            this.Controls.Add(this.labelLogin);
            this.Controls.Add(this.labelAuthentication);
            this.Controls.Add(this.labelServerName);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormConnection";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "서버에 연결";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form_KeyDown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelServerName;
        private System.Windows.Forms.Label labelAuthentication;
        private System.Windows.Forms.Label labelLogin;
        private System.Windows.Forms.Label labelPassword;
        private System.Windows.Forms.Label labelSeperator;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.ComboBox comboBoxServerName;
        private System.Windows.Forms.ComboBox comboBoxAuthenticationType;
        private System.Windows.Forms.ComboBox comboBoxID;
        private System.Windows.Forms.TextBox textBoxPassword;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.ComponentModel.BackgroundWorker backgroundWorker;
    }
}