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
    public partial class FormSearch : Form
    {
        public string IPAddress { get; private set; }

        public FormSearch()
        {
            InitializeComponent();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.IPAddress = textBoxIPAddress.Text;
            this.Close();
        }

        private void textBoxIPAddress_TextChanged(object sender, EventArgs e)
        {
            buttonOK.Enabled = (textBoxIPAddress.Text.Length > 0);
        }

        private void listBoxAdded_SelectedIndexChanged(object sender, EventArgs e)
        {
            listBox_SelectToMyValue(listBoxAdded, listBoxRemoved);
        }

        private void listBoxRemoved_SelectedIndexChanged(object sender, EventArgs e)
        {
            listBox_SelectToMyValue(listBoxRemoved, listBoxAdded);
        }

        private void listBox_SelectToMyValue(ListBox my, ListBox you)
        {
            if (my.SelectedItem != null)
            {
                you.SelectedItem = null;
                textBoxIPAddress.Text = my.SelectedItem.ToString();
            }
        }

        private void buttonRecord_Click(object sender, EventArgs e)
        {
            // 기록

            // 이전 기록 내용이 있다면 변화 UI에 반영
        }

    }
}
