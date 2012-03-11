using System;
using System.Data;  // for DataTable

namespace DataTable_sample
{
    class Program
    {
        static DataTable table = new DataTable("dev_map");

        // 한번만 호출
        static void Init()
        {
            table.Columns.Add(new DataColumn("name", typeof(string)));
            table.Columns.Add(new DataColumn("code", typeof(string)));

            table.Rows.Add("Aficio MP 2510", "M154");
            table.Rows.Add("Aficio MP 3010", "M154");
            table.Rows.Add("Aficio MP C2800", "M172");
            table.Rows.Add("Aficio MP C3300", "M172");
            table.Rows.Add("Aficio MP 5000", "M163");
            table.Rows.Add("Aficio MP 5000B", "M163");
        }

        static string SearchIn(string dev_name)
        {
            //string query = "'" + dev_name + "' LIKE '*name*'";
            //DataRow[] result = table.Select(query);
            DataRow[] result = table.Select("'%2510' like [name]");

            return "22";
        }

        static void Main(string[] args)
        {
            Init();
            string dev = "Aficio MP 5000 1.21";
            Console.WriteLine("{0} is {1}", dev, SearchIn(dev));

        }
    }
}
