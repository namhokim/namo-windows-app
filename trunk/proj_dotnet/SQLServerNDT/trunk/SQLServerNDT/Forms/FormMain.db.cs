using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace SQLServerNDT.Forms
{
    public partial class FormMain : Form
    {
        SqlConnection _connection = null;

        void ConnectDatabase()
        {
            if (_connection != null & _connection.State == ConnectionState.Connecting)
                _connection.Close();
        }
    }
}
