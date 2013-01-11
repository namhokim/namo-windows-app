using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
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
            CloseDatabase();
            FormConnection conn = new FormConnection();
            if (conn.ShowDialog() == DialogResult.OK)
            {
                _connection = conn.Connection;
                this.buttonConnInfo.Enabled = true;
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            CloseDatabase();
            base.OnClosed(e);
        }

        void CloseDatabase()
        {
            if (_connection != null && _connection.State==ConnectionState.Open)
            {
                _connection.Close();
            }
        }

        private void buttonConnInfo_Click(object sender, EventArgs e)
        {
            string query = @"SELECT S.SPID, S.LOGINAME, S.LOGIN_TIME, S.LAST_BATCH, C.CLIENT_NET_ADDRESS FROM sys.sysprocesses S, sys.dm_exec_connections C WHERE S.spid = C.SESSION_ID";

            try
            {
                SqlDataAdapter da = new SqlDataAdapter(query, _connection);
                SqlCommandBuilder sqlCommandBuilder = new SqlCommandBuilder(da);
                DataTable dt = new DataTable();
                da.Fill(new DataTable());
                this.dataGridView.DataSource = dt;
            }
            catch (SqlException ex)
            {
                MessageBox.Show(this, ex.Message, "쿼리 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            //this._connection.
            //;this.dataGridView.DataSource = 
        }
    }
}
