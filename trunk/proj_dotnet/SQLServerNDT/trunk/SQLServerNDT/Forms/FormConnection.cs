using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Data.Common;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SQLServerNDT.Properties;
using SQLServerNDT.Extensions;

namespace SQLServerNDT.Forms
{
    public partial class FormConnection : Form
    {
        #region Properties
        public SqlConnection Connection { get { return _connection; } }
        public bool IsWindowsAuthentication
        {
            get{
                return comboBoxAuthenticationType.SelectedIndex == 0;
            }
        }
        public bool IsSQLServerAuthentication
        {
            get
            {
                return comboBoxAuthenticationType.SelectedIndex == 1;
            }
        }
        private int GetIndexFromAuthenticationTypeSetting
        {
            get
            {
                return (Settings.Default.IsWindowsAuthentication ? 0 : 1);
            }
        }
        #endregion

        SqlConnection _connection = null;

        public FormConnection()
        {
            InitializeComponent();
            LoadFromSetting();
            InitializeUI();
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            comboBoxAuthenticationType.SelectedIndex =
                Settings.Default.IsWindowsAuthentication ? 0 : 1;
        }

        private void comboBoxAuthenticationType_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool bSQLAuth = this.IsSQLServerAuthentication;
            comboBoxID.Enabled = bSQLAuth;
            textBoxPassword.Enabled = bSQLAuth;
        }

        private void Form_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                this.DialogResult = DialogResult.Cancel;
                this.Close();
            }
        }

        private void LoadFromSetting()
        {
            this.comboBoxServerName.DataSource = Settings.Default.ServerNames;
            this.comboBoxAuthenticationType.SelectedIndex = GetIndexFromAuthenticationTypeSetting;
            if (Settings.Default.Users != null)
                this.comboBoxID.DataSource = Settings.Default.Users;
            if (Settings.Default.Password != null)
                this.textBoxPassword.Text = Settings.Default.Password;
        }

        public void SaveToSettings()
        {
            // Server Name
            if (comboBoxServerName.Text.Length > 0)
            {
                Settings.Default.ServerNames.Clear();
                Settings.Default.ServerNames.Add(comboBoxServerName.Text);
            }

            // Authentication Type
            Settings.Default.IsWindowsAuthentication = this.IsWindowsAuthentication;

            // User
            if (comboBoxID.Text.Length > 0)
            {
                try
                {
                    Settings.Default.Users.Clear();
                }
                catch (NullReferenceException)
                {
                    Settings.Default.Users = new StringCollection();
                }
                Settings.Default.Users.Add(comboBoxID.Text);
            }

            // Password
            Settings.Default.Password = this.textBoxPassword.Text;
            Settings.Default.Save();
        }

        private void InitializeUI()
        {
            if (this.comboBoxServerName.Items.Count > 0)
                this.comboBoxServerName.SelectedIndex = 0;
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (backgroundWorker.IsBusy) return;

            LockUI(true);

            _connection = new SqlConnection(MakeConnectionString());
            //MessageBox.Show(_connection.ConnectionString + "=>" + _connection.ConnectionTimeout
            //    + "," + _connection.Database);
            backgroundWorker.RunWorkerAsync();
        }

        private string MakeConnectionString()
        {
            // Integrated Security=SSPI
            const string ConnFormatSQL = @"Data Source={0};Initial Catalog=master;"
                + "Persist Security Info=True;User ID={1};Password={2};Connection Timeout={3}";
            const string ConnFormatWin = @"Data Source={0};Initial Catalog=master;"
                + "Persist Security Info=True;Integrated Security=SSPI;Connection Timeout={1}";
            int timeout = 3;
            string connStr, server;
            if (comboBoxServerName.Text == "(local)") server = "localhost";
            else server = comboBoxServerName.Text;
            if (this.IsSQLServerAuthentication)
            {
                connStr = string.Format(ConnFormatSQL,
                    server, comboBoxID.Text, textBoxPassword.Text, timeout);
            }
            else
            {
                connStr = string.Format(ConnFormatWin, server, timeout);
            }
            return connStr;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            if (backgroundWorker.IsBusy)
            {
                LockUI(false);
            }
            else
            {
                this.DialogResult = DialogResult.Cancel;
                this.Close();
            }
        }

        private void backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                _connection.Open();
            }
            catch (Exception ex)
            {
                progressBar.Visible = false;
                MessageBox.Show(this, ex.Message, "서버에 연결", MessageBoxButtons.OK, MessageBoxIcon.Error);
                _connection = null;
            }
        }

        private void backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            LockUI(false);
            if (_connection!=null && _connection.State == ConnectionState.Open)
            {
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        void LockUI(bool bLock)
        {
            progressBar.Visible = bLock;

            comboBoxServerName.Enabled = !bLock;
            comboBoxAuthenticationType.Enabled = !bLock;
            comboBoxID.Enabled = !bLock;
            textBoxPassword.Enabled = !bLock;
            buttonConnect.Enabled = !bLock;
        }

        protected override void OnClosed(EventArgs e)
        {
            SaveToSettings();
            base.OnClosed(e);
        }
    }
}
