using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SQLServerNDT.Forms
{
    public partial class FormConnection : Form
    {
        public FormConnection()
        {
            InitializeComponent();
            //MyInitializeComponent();
        }

        private void MyInitializeComponent()
        {
            Array values = Enum.GetValues(typeof(AuthenticationTypes));
            //values.Cast(typeof(object));
            this.comboBoxAuthenticationType.Items.AddRange((object[])values);
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            comboBoxServerName.SelectedIndex = 0;
            comboBoxAuthenticationType.SelectedIndex = 0;
        }

        private void comboBoxAuthenticationType_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool bSQLAuth = (comboBoxAuthenticationType.SelectedIndex == 1);
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
    }
}
