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
        private SqlConnection _connection = null;
        private bool IsConnected { get { return _connection != null && _connection.State == ConnectionState.Open; } }
        private bool IsClosed { get { return _connection != null && _connection.State == ConnectionState.Closed; } }

        public FormMain()
        {
            InitializeComponent();
        }

        private void buttonConnectLocal_Click(object sender, EventArgs e)
        {
            if (IsConnected)
            {
                CloseDatabase();
                //this.buttonConnInfo.Enabled = false;
                //this.buttonConnect.Text = "연결(&C)";
                //RefreshUI_DB_OK(true);
            }
            else
            {
                FormConnection conn = new FormConnection();
                if (conn.ShowDialog() == DialogResult.OK)
                {
                    _connection = conn.Connection;
                    _connection.StateChange += (s, ea) =>
                    {
                        if (ea.OriginalState == ConnectionState.Open
                            && ea.CurrentState == ConnectionState.Closed)
                        {
                            RefreshUI_DB_OK(false);
                            ClearDataGridView();
                        }
                    };
                    RefreshUI_DB_OK(true);
                }
                else
                {
                    RefreshUI_DB_OK(false);
                }
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            CloseDatabase();
            base.OnClosed(e);
        }

        void CloseDatabase()
        {
            if (_connection != null && IsConnected)
            {
                _connection.Close();
                _connection = null;
            }
        }

        void ClearDataGridView()
        {
            // http://stackoverflow.com/questions/7430933/datagridview-remove-all-columns
            this.dataGridView.Columns.Clear();
            this.dataGridView.DataSource = null;
            this.dataGridView.AutoGenerateColumns = false;
            //DataGridViewTextBoxColumn col = new DataGridViewTextBoxColumn();
            //this.dataGridView.Columns.Add(col);
        }

        private void buttonConnInfo_Click(object sender, EventArgs e)
        {
            string query = @"SELECT S.SPID, RTRIM(S.LOGINAME) as LOGINAME, S.LOGIN_TIME, S.LAST_BATCH, C.CLIENT_NET_ADDRESS FROM sys.sysprocesses S, sys.dm_exec_connections C WHERE S.spid = C.SESSION_ID";

            try
            {
                if (_connection == null) throw new NullReferenceException("데이터베이스와 연결이 되지 않았습니다.");
                // http://social.msdn.microsoft.com/Forums/en-US/csharpgeneral/thread/e6698cad-15f7-41e7-82c3-cc71c17f30e2/
                SqlDataAdapter da = new SqlDataAdapter(query, _connection);
                SqlCommandBuilder sqlCommandBuilder = new SqlCommandBuilder(da);

                // Populate a new data table and bind it to the BindingSource.
                DataTable dt = new DataTable
                {
                    Locale = System.Globalization.CultureInfo.InvariantCulture
                };
                da.Fill(dt);
                this.dataGridView.DataSource = dt;

                // Resize the DataGridView columns to fit the newly loaded content.
                this.dataGridView.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);
                // Fit with Windows
                //this.dataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;

                // you can make it grid readonly.
                this.dataGridView.ReadOnly = true;

                this.dataGridView.BackgroundColor = SystemColors.ControlDark;
            }
            catch (NullReferenceException nex)
            {
                RefreshUI_DB_OK(false);
                MessageBox.Show(this, nex.Message, "연결문제", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (Exception ex)
            {
                if (IsClosed)
                {
                    RefreshUI_DB_OK(false);
                }
                MessageBox.Show(this, ex.Message, "쿼리실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }

        void RefreshUI_DB_OK(bool bConnected)
        {
            if (bConnected)
            {
                this.buttonConnInfo.Enabled = true;
                this.buttonConnect.Text = "끊기(&D)";
            }
            else
            {
                this.buttonConnInfo.Enabled = false;
                this.buttonConnect.Text = "연결(&C)";
            }

        }

    }
}
