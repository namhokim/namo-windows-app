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
            list.Add(new QueryObject { Display = "연결정보보기", Query = @"SELECT S.SPID, RTRIM(S.LOGINAME) as LOGINAME, S.LOGIN_TIME, S.LAST_BATCH, C.CLIENT_NET_ADDRESS FROM sys.sysprocesses S, sys.dm_exec_connections C WHERE S.spid = C.SESSION_ID;" });
            list.Add(new QueryObject { Display = "CPU병목보기 - runnable_tasks_count가 높은 값일 수록 CPU병목", Query = @"select scheduler_id, current_tasks_count, runnable_tasks_count from sys.dm_os_schedulers where scheduler_id < 255;" });
            list.Add(new QueryObject { Display = "CPU를 가장 많이 소비하는 대상보기 - 캐시된 일괄처리, 프로시저", Query = @"select top 50 SUM(qs.total_worker_time) as total_cpu_time, SUM(qs.execution_count) as total_execution_count, COUNT(*) as number_of_statements, CONVERT(VARCHAR(64), qs.plan_handle, 1) AS plan_handle from sys.dm_exec_query_stats qs group by qs.plan_handle order by SUM(qs.total_worker_time) desc;" });
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
