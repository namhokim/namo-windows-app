using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SQLServerNDT
{
    public static class QueryLists
    {
        public static void SetItems(System.Collections.IList list)
        {
            list.Add(new QueryObject { Display = "연결정보보기", Query = @"SELECT S.SPID, RTRIM(S.LOGINAME) as LOGINAME, S.LOGIN_TIME, S.LAST_BATCH, C.CLIENT_NET_ADDRESS FROM sys.sysprocesses S, sys.dm_exec_connections C WHERE S.spid = C.SESSION_ID" });
            //list.Add(new QueryObject { Display = "이거2", Query = "저거2" });
            //list.Add(new QueryObject { Display = "이거3", Query = "저거3" });
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
