namespace DeviceTracer
{
    partial class FormSearch
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormSearch));
            this.buttonRecord = new System.Windows.Forms.Button();
            this.groupBoxAdded = new System.Windows.Forms.GroupBox();
            this.groupBoxRemoved = new System.Windows.Forms.GroupBox();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.textBoxIPAddress = new System.Windows.Forms.TextBox();
            this.labelIPAddress = new System.Windows.Forms.Label();
            this.listBoxAdded = new System.Windows.Forms.ListBox();
            this.labelAddedCount = new System.Windows.Forms.Label();
            this.listBoxRemoved = new System.Windows.Forms.ListBox();
            this.labelRemovedCount = new System.Windows.Forms.Label();
            this.labelAddedCountValue = new System.Windows.Forms.Label();
            this.labelRemovedCountValue = new System.Windows.Forms.Label();
            this.groupBoxAdded.SuspendLayout();
            this.groupBoxRemoved.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonRecord
            // 
            this.buttonRecord.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRecord.Location = new System.Drawing.Point(456, 33);
            this.buttonRecord.Name = "buttonRecord";
            this.buttonRecord.Size = new System.Drawing.Size(116, 60);
            this.buttonRecord.TabIndex = 0;
            this.buttonRecord.Text = "&Record";
            this.buttonRecord.UseVisualStyleBackColor = true;
            // 
            // groupBoxAdded
            // 
            this.groupBoxAdded.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
            this.groupBoxAdded.Controls.Add(this.labelAddedCountValue);
            this.groupBoxAdded.Controls.Add(this.labelAddedCount);
            this.groupBoxAdded.Controls.Add(this.listBoxAdded);
            this.groupBoxAdded.Location = new System.Drawing.Point(19, 12);
            this.groupBoxAdded.Name = "groupBoxAdded";
            this.groupBoxAdded.Size = new System.Drawing.Size(200, 338);
            this.groupBoxAdded.TabIndex = 1;
            this.groupBoxAdded.TabStop = false;
            this.groupBoxAdded.Text = "Added";
            // 
            // groupBoxRemoved
            // 
            this.groupBoxRemoved.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
            this.groupBoxRemoved.Controls.Add(this.labelRemovedCountValue);
            this.groupBoxRemoved.Controls.Add(this.labelRemovedCount);
            this.groupBoxRemoved.Controls.Add(this.listBoxRemoved);
            this.groupBoxRemoved.Location = new System.Drawing.Point(226, 13);
            this.groupBoxRemoved.Name = "groupBoxRemoved";
            this.groupBoxRemoved.Size = new System.Drawing.Size(200, 337);
            this.groupBoxRemoved.TabIndex = 2;
            this.groupBoxRemoved.TabStop = false;
            this.groupBoxRemoved.Text = "Removed";
            // 
            // buttonOK
            // 
            this.buttonOK.Enabled = false;
            this.buttonOK.Location = new System.Drawing.Point(497, 154);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 3;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(497, 183);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 4;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // textBoxIPAddress
            // 
            this.textBoxIPAddress.Location = new System.Drawing.Point(472, 240);
            this.textBoxIPAddress.Name = "textBoxIPAddress";
            this.textBoxIPAddress.ReadOnly = true;
            this.textBoxIPAddress.Size = new System.Drawing.Size(100, 21);
            this.textBoxIPAddress.TabIndex = 5;
            this.textBoxIPAddress.TextChanged += new System.EventHandler(this.textBoxIPAddress_TextChanged);
            // 
            // labelIPAddress
            // 
            this.labelIPAddress.AutoSize = true;
            this.labelIPAddress.Location = new System.Drawing.Point(442, 225);
            this.labelIPAddress.Name = "labelIPAddress";
            this.labelIPAddress.Size = new System.Drawing.Size(75, 12);
            this.labelIPAddress.TabIndex = 4;
            this.labelIPAddress.Text = "Choosen IP:";
            // 
            // listBoxAdded
            // 
            this.listBoxAdded.FormattingEnabled = true;
            this.listBoxAdded.ItemHeight = 12;
            this.listBoxAdded.Items.AddRange(new object[] {
            "10.17.1.23",
            "10.17.1.33"});
            this.listBoxAdded.Location = new System.Drawing.Point(16, 42);
            this.listBoxAdded.Name = "listBoxAdded";
            this.listBoxAdded.Size = new System.Drawing.Size(166, 280);
            this.listBoxAdded.TabIndex = 1;
            this.listBoxAdded.SelectedIndexChanged += new System.EventHandler(this.listBoxAdded_SelectedIndexChanged);
            // 
            // labelAddedCount
            // 
            this.labelAddedCount.AutoSize = true;
            this.labelAddedCount.Location = new System.Drawing.Point(16, 24);
            this.labelAddedCount.Name = "labelAddedCount";
            this.labelAddedCount.Size = new System.Drawing.Size(42, 12);
            this.labelAddedCount.TabIndex = 1;
            this.labelAddedCount.Text = "Count:";
            // 
            // listBoxRemoved
            // 
            this.listBoxRemoved.FormattingEnabled = true;
            this.listBoxRemoved.ItemHeight = 12;
            this.listBoxRemoved.Items.AddRange(new object[] {
            "10.17.1.23",
            "10.17.1.33"});
            this.listBoxRemoved.Location = new System.Drawing.Point(16, 41);
            this.listBoxRemoved.Name = "listBoxRemoved";
            this.listBoxRemoved.Size = new System.Drawing.Size(165, 280);
            this.listBoxRemoved.TabIndex = 2;
            this.listBoxRemoved.SelectedIndexChanged += new System.EventHandler(this.listBoxRemoved_SelectedIndexChanged);
            // 
            // labelRemovedCount
            // 
            this.labelRemovedCount.AutoSize = true;
            this.labelRemovedCount.Location = new System.Drawing.Point(16, 22);
            this.labelRemovedCount.Name = "labelRemovedCount";
            this.labelRemovedCount.Size = new System.Drawing.Size(42, 12);
            this.labelRemovedCount.TabIndex = 1;
            this.labelRemovedCount.Text = "Count:";
            // 
            // labelAddedCountValue
            // 
            this.labelAddedCountValue.AutoSize = true;
            this.labelAddedCountValue.Location = new System.Drawing.Point(65, 24);
            this.labelAddedCountValue.Name = "labelAddedCountValue";
            this.labelAddedCountValue.Size = new System.Drawing.Size(11, 12);
            this.labelAddedCountValue.TabIndex = 1000;
            this.labelAddedCountValue.Text = "0";
            // 
            // labelRemovedCountValue
            // 
            this.labelRemovedCountValue.AutoSize = true;
            this.labelRemovedCountValue.Location = new System.Drawing.Point(64, 23);
            this.labelRemovedCountValue.Name = "labelRemovedCountValue";
            this.labelRemovedCountValue.Size = new System.Drawing.Size(11, 12);
            this.labelRemovedCountValue.TabIndex = 1000;
            this.labelRemovedCountValue.Text = "0";
            // 
            // FormSearch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 362);
            this.Controls.Add(this.labelIPAddress);
            this.Controls.Add(this.textBoxIPAddress);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.groupBoxRemoved);
            this.Controls.Add(this.groupBoxAdded);
            this.Controls.Add(this.buttonRecord);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FormSearch";
            this.Text = "Search Device";
            this.groupBoxAdded.ResumeLayout(false);
            this.groupBoxAdded.PerformLayout();
            this.groupBoxRemoved.ResumeLayout(false);
            this.groupBoxRemoved.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonRecord;
        private System.Windows.Forms.GroupBox groupBoxAdded;
        private System.Windows.Forms.GroupBox groupBoxRemoved;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.TextBox textBoxIPAddress;
        private System.Windows.Forms.Label labelIPAddress;
        private System.Windows.Forms.Label labelAddedCountValue;
        private System.Windows.Forms.Label labelAddedCount;
        private System.Windows.Forms.ListBox listBoxAdded;
        private System.Windows.Forms.Label labelRemovedCountValue;
        private System.Windows.Forms.Label labelRemovedCount;
        private System.Windows.Forms.ListBox listBoxRemoved;
    }
}