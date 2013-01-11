using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SQLServerNDT.Properties;
using System.Collections;

namespace SQLServerNDT.Forms
{
    public partial class FormConnection : Form
    {
        #region Properties
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
        #endregion

        public FormConnection()
        {
            InitializeComponent();
            LoadFromSetting();
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            comboBoxServerName.SelectedIndex = 0;
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
                this.Close();
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            SaveToSettings();
        }

        private void LoadFromSetting()
        {
            //if (Settings.Default.ServerNames == null)
            //{
            //    Settings.Default.ServerNames =
            //        new System.Collections.Specialized.StringCollection();
            //}
            //Array values = Enum.GetValues(typeof(AuthenticationTypes));
            //values.Cast(typeof(object));
            //ArrayList al = new ArrayList(Settings.Default.ServerNames);
            //this.comboBoxAuthenticationType.DataSource = new ArrayList(Settings.Default.ServerNames);
            //this.comboBoxAuthenticationType.Items.AddRange(Settings.Default.ServerNames.GetEnumerator());
        }

        private void SaveToSettings()
        {
            Settings.Default.ServerNames.Add(comboBoxServerName.Text);
            Settings.Default.IsWindowsAuthentication = this.IsWindowsAuthentication;
            Settings.Default.Save();
        }
    }
}
