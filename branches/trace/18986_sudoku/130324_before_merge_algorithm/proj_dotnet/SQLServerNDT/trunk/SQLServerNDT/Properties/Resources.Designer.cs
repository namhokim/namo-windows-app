﻿//------------------------------------------------------------------------------
// <auto-generated>
//     이 코드는 도구를 사용하여 생성되었습니다.
//     런타임 버전:2.0.50727.5466
//
//     파일 내용을 변경하면 잘못된 동작이 발생할 수 있으며, 코드를 다시 생성하면
//     이러한 변경 내용이 손실됩니다.
// </auto-generated>
//------------------------------------------------------------------------------

namespace SQLServerNDT.Properties {
    using System;
    
    
    /// <summary>
    ///   지역화된 문자열 등을 찾기 위한 강력한 형식의 리소스 클래스입니다.
    /// </summary>
    // 이 클래스는 ResGen 또는 Visual Studio와 같은 도구를 통해 StronglyTypedResourceBuilder
    // 클래스에서 자동으로 생성되었습니다.
    // 멤버를 추가하거나 제거하려면 .ResX 파일을 편집한 다음 /str 옵션을 사용하여 ResGen을
    // 다시 실행하거나 VS 프로젝트를 다시 빌드하십시오.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "2.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    internal class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   이 클래스에서 사용하는 캐시된 ResourceManager 인스턴스를 반환합니다.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("SQLServerNDT.Properties.Resources", typeof(Resources).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   이 강력한 형식의 리소스 클래스를 사용하여 모든 리소스 조회에 대한 현재 스레드의 CurrentUICulture
        ///   속성을 재정의합니다.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   연결정보보기과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q01_D {
            get {
                return ResourceManager.GetString("Q01_D", resourceCulture);
            }
        }
        
        /// <summary>
        ///   SELECT S.SPID, RTRIM(S.LOGINAME) as LOGINAME, S.LOGIN_TIME, S.LAST_BATCH, C.CLIENT_NET_ADDRESS
        /// FROM sys.sysprocesses S, sys.dm_exec_connections C WHERE S.spid = C.SESSION_ID;과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q01_Q {
            get {
                return ResourceManager.GetString("Q01_Q", resourceCulture);
            }
        }
        
        /// <summary>
        ///   CPU병목보기 - runnable_tasks_count가 높은 값일 수록 CPU병목과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q02_D {
            get {
                return ResourceManager.GetString("Q02_D", resourceCulture);
            }
        }
        
        /// <summary>
        ///   select scheduler_id, current_tasks_count, runnable_tasks_count
        /// from sys.dm_os_schedulers where scheduler_id &lt; 255;과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q02_Q {
            get {
                return ResourceManager.GetString("Q02_Q", resourceCulture);
            }
        }
        
        /// <summary>
        ///   CPU를 가장 많이 소비하는 대상보기 - 캐시된 일괄처리, 프로시저과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q03_D {
            get {
                return ResourceManager.GetString("Q03_D", resourceCulture);
            }
        }
        
        /// <summary>
        ///   select top 50
        /// SUM(qs.total_worker_time) as total_cpu_time, SUM(qs.execution_count) as total_execution_count, COUNT(*) as number_of_statements, CONVERT(VARCHAR(64), qs.plan_handle, 1) AS plan_handle
        /// from sys.dm_exec_query_stats qs
        /// group by qs.plan_handle
        /// order by SUM(qs.total_worker_time) desc;과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q03_Q {
            get {
                return ResourceManager.GetString("Q03_Q", resourceCulture);
            }
        }
        
        /// <summary>
        ///   쿼리 재컴파일 횟수(상위 25건의 저장 프로시저)과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q04_D {
            get {
                return ResourceManager.GetString("Q04_D", resourceCulture);
            }
        }
        
        /// <summary>
        ///   select top 25
        /// sql_text.text, plan_generation_num, execution_count, dbid, objectid
        /// from sys.dm_exec_query_stats a
        /// cross apply sys.dm_exec_sql_text(sql_handle) as sql_text
        /// where plan_generation_num &gt; 1
        /// order by plan_generation_num desc;과(와) 유사한 지역화된 문자열을 찾습니다.
        /// </summary>
        internal static string Q04_Q {
            get {
                return ResourceManager.GetString("Q04_Q", resourceCulture);
            }
        }
    }
}
