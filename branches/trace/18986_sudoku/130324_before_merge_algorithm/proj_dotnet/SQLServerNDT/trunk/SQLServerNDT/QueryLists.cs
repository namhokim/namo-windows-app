using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SQLServerNDT.Properties;

namespace SQLServerNDT
{
    public static class QueryLists
    {
        public static void SetItems(System.Collections.IList list)
        {

            list.Add(new QueryObject { Display = Resources.Q01_D, Query = Resources.Q01_Q });
            list.Add(new QueryObject { Display = Resources.Q02_D, Query = Resources.Q02_Q });
            list.Add(new QueryObject { Display = Resources.Q03_D, Query = Resources.Q03_Q });
            list.Add(new QueryObject { Display = Resources.Q04_D, Query = Resources.Q04_Q });
            //list.Add(new QueryObject { Display = "표시", Query = @"쿼리" });
        }
    }

    public class QueryObject
    {
        public string Display {get;set;}
        public string Query {get;set;}
        public override string ToString()
        {
            return Display;
        }
    }
}
