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

        private void dataGridView_DataError(object sender, DataGridViewDataErrorEventArgs anError)
        {
            dataGridView.Rows[anError.RowIndex].ErrorText = anError.Exception.Message;
            anError.Cancel = true;

            // http://msdn.microsoft.com/en-us/library/system.windows.forms.datagridview.dataerror.aspx
            //MessageBox.Show("Error happened " + anError.Context.ToString());

            if (anError.Context == DataGridViewDataErrorContexts.Commit)
            {
                MessageBox.Show("Commit error");
            }
            if (anError.Context == DataGridViewDataErrorContexts.CurrentCellChange)
            {
                MessageBox.Show("Cell change");
            }
            if (anError.Context == DataGridViewDataErrorContexts.Parsing)
            {
                MessageBox.Show("parsing error");
            }
            if (anError.Context == DataGridViewDataErrorContexts.LeaveControl)
            {
                MessageBox.Show("leave control error");
            }

            if ((anError.Exception) is ConstraintException)
            {
                DataGridView view = (DataGridView)sender;
                view.Rows[anError.RowIndex].ErrorText = "an error";
                view.Rows[anError.RowIndex].Cells[anError.ColumnIndex].ErrorText = "an error";

                anError.ThrowException = false;
            }
        }

        private void splitContainer_SplitterMoved(object sender, SplitterEventArgs e)
        {
            // prevent for focus at splitter
            textBoxQuery.Focus();
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Main Windows size
            // http://stackoverflow.com/questions/1873658/net-winforms-remember-windows-size-and-location
            if (WindowState == FormWindowState.Maximized)
            {
                Properties.Settings.Default.Location = RestoreBounds.Location;
                Properties.Settings.Default.Size = RestoreBounds.Size;
                Properties.Settings.Default.Maximised = true;
            }
            else
            {
                Properties.Settings.Default.Location = Location;
                Properties.Settings.Default.Size = Size;
                Properties.Settings.Default.Maximised = false;
            }
            Properties.Settings.Default.Save();

            // Splitter info
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            if (Properties.Settings.Default.Maximised)
            {
                WindowState = FormWindowState.Maximized;
            }

            try
            {
                Location = Properties.Settings.Default.Location;
                Size = Properties.Settings.Default.Size;
            }
            catch (NullReferenceException)
            {
                // first runtime, then using default size and location
            }
        }

    }
}
