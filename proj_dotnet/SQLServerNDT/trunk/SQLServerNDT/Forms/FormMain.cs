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
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void buttonConnectLocal_Click(object sender, EventArgs e)
        {
            FormConnection conn = new FormConnection();
            _connection = conn.Connection;
        }
    }
}
