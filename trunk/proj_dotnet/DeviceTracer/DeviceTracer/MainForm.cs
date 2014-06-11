using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DeviceTracer
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void buttonSearch_Click(object sender, EventArgs e)
        {
            var search = new FormSearch();
            DialogResult result = search.ShowDialog();
            labelForTest.Text = result.ToString() + " : " + search.IPAddress;
        }
    }
}
