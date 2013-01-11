using System;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace SQLServerNDT.Forms
{
    public partial class FormMain : Form
    {
        private string _connectionString = null;

        public FormMain()
        {
            InitializeComponent();
        }

        private void buttonConnectLocal_Click(object sender, EventArgs e)
        {
            if (_connectionString == null)
            {
                FormConnection conn = new FormConnection();
                if (conn.ShowDialog() == DialogResult.OK)
                {
                    _connectionString = conn.ConnectionString;
                    RefreshUI_DB_OK(true);
                }
            }
            else
            {
                _connectionString = null;
                RefreshUI_DB_OK(false);
            }
        }

        void ClearDataGridView()
        {
            // http://stackoverflow.com/questions/7430933/datagridview-remove-all-columns
            this.dataGridView.Columns.Clear();
            this.dataGridView.DataSource = null;
        }

        private void buttonConnInfo_Click(object sender, EventArgs e)
        {
            string query = @"SELECT S.SPID, RTRIM(S.LOGINAME) as LOGINAME, S.LOGIN_TIME, S.LAST_BATCH, C.CLIENT_NET_ADDRESS FROM sys.sysprocesses S, sys.dm_exec_connections C WHERE S.spid = C.SESSION_ID";

            try
            {
                ClearDataGridView();
                using (SqlConnection _connection = new SqlConnection(_connectionString))
                {
                    _connection.Open();

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
                }
            }
            catch (Exception ex)
            {
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
