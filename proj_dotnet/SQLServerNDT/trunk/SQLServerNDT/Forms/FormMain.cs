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
            InitializeQueryCombo();
        }

        private void InitializeQueryCombo(){
            QueryLists.SetItems(comboBoxQueries.Items);
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

        private void ClearDataGridView()
        {
            // http://stackoverflow.com/questions/7430933/datagridview-remove-all-columns
            this.dataGridView.Columns.Clear();
            this.dataGridView.DataSource = null;
        }

        private void RefreshUI_DB_OK(bool bConnected)
        {
            if (bConnected)
            {
                this.buttonConnect.Text = "끊기(&D)";
                this.comboBoxQueries.Enabled = true;
            }
            else
            {
                this.buttonConnect.Text = "연결(&C)";
                this.comboBoxQueries.Enabled = false;
            }
        }

        private void comboBoxQueries_SelectedIndexChanged(object sender, EventArgs e)
        {
            const int NO_SELECTED = -1;

            if (comboBoxQueries.SelectedIndex == NO_SELECTED)
            {
                textBoxQuery.Text = "";
            }
            else
            {
                var qo = comboBoxQueries.SelectedItem as QueryObject;
                textBoxQuery.Text = qo.Query;
            }
        }

        private void textBoxQuery_TextChanged(object sender, EventArgs e)
        {
            buttonExecuteQuery.Enabled = (comboBoxQueries.Enabled && textBoxQuery.TextLength > 0);
        }

        private void buttonExecuteQuery_Click(object sender, EventArgs e)
        {
            ClearDataGridView();
            try
            {
                ExecuteQuery(_connectionString, textBoxQuery.Text, this.dataGridView);
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message, "쿼리실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ExecuteQuery(string connectionString, string query, DataGridView dataGridView)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                conn.Open();

                // http://social.msdn.microsoft.com/Forums/en-US/csharpgeneral/thread/e6698cad-15f7-41e7-82c3-cc71c17f30e2/
                SqlDataAdapter da = new SqlDataAdapter(query, conn);
                SqlCommandBuilder sqlCommandBuilder = new SqlCommandBuilder(da);

                // Populate a new data table and bind it to the BindingSource.
                DataTable dt = new DataTable
                {
                    Locale = System.Globalization.CultureInfo.InvariantCulture
                };
                da.Fill(dt);
                dataGridView.DataSource = dt;

                // Resize the DataGridView columns to fit the newly loaded content.
                dataGridView.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);
            }
        }

        private void textBoxQuery_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.A)
            {
                textBoxQuery.SelectAll();
            }
        }

    }
}
