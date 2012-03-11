using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.OleDb;

namespace OracleConn
{
    class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                ConnLoop();
            }
        }

        static void ConnLoop()
        {
            string dbPwdtextBox1 = "Print_123!";
            string dbIdtextBox1 = "sindoh";
            string dbIptextBox1 = "10.5.29.223";
            string m_strConn = @"Provider=OraOLEDB.Oracle.1;Password=" + dbPwdtextBox1
                + ";Persist Security Info=True;User ID=" + dbIdtextBox1
                + ";Data Source=" + dbIptextBox1 + ":1531/SINDOH";

            try
            {
                OleDbConnection m_oledbConn;
                string m_strSql;
                OleDbCommand m_oledbComm;
                OleDbDataReader m_oledbDataReader;
                m_oledbConn = new OleDbConnection(m_strConn);

                m_strSql = "select count(*) as JOB_CNT from pod_job_info";

                m_oledbComm = new OleDbCommand(m_strSql, m_oledbConn);
                m_oledbConn.Open();
                m_oledbDataReader = m_oledbComm.ExecuteReader();

                if (m_oledbDataReader.HasRows)
                {
                    while (m_oledbDataReader.Read())
                    {
                        Console.WriteLine(m_oledbDataReader["JOB_CNT"].ToString());
                    }
                }
                //m_oledbDataReader.Close();
                //m_oledbConn.Close();

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
