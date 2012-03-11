using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SQLite;

namespace SQLite_NET
{
    class Program
    {
        static void Main(string[] args)
        {
            SQLiteConnection conn = new SQLiteConnection();
            conn.ConnectionString = "Data Source=dot_net.db;Version=3;password=pw";
            try
            {
                conn.Open();
                SQLiteCommand cmd = conn.CreateCommand();
                //cmd.CommandText = "insert into userInfo (userId, userName) values (4, 'bum');";
                //cmd.ExecuteNonQuery();
                cmd.CommandText = "select userID, userName from userInfo";
                SQLiteDataReader r = cmd.ExecuteReader();

                while (r.Read())
                {
                    Console.WriteLine(r.GetName(0) + ":" + r.GetInt32(0) + "\t" + r.GetName(1) + ":" + r.GetString(1));
                }
                r.Close();
                
                conn.Close();
            }
            catch (SQLiteException e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
