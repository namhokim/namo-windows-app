using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Text;

namespace wcr_console
{
    public class RemoteDesktopFinder
    {
        private const int port = 3389;

        private static Dictionary<string, string> IP_MAP = new Dictionary<string, string>() {
            {"10.17.1.133", "김남호"},
            {"10.17.1.182", "박성웅"},
        };

        // refs. http://social.msdn.microsoft.com/Forums/eu/netfxnetcom/thread/bb587794-f0a0-432e-a8b9-8d37cbf5b140
        public static List<string> Who()
        {
            List<string> r = new List<string>();

             //Look for established connections
            IPGlobalProperties ipProperties = IPGlobalProperties.GetIPGlobalProperties();
            // GetExtendedTcpTable function
            // refs. http://msdn.microsoft.com/en-us/library/windows/desktop/aa365928(v=vs.85).aspx
            TcpConnectionInformation[] connections = ipProperties.GetActiveTcpConnections();

            try
            {
                foreach (TcpConnectionInformation info in connections)
                {
                    if (port != 0
                        && !info.LocalEndPoint.Port.Equals(port)
                        /*&& !info.RemoteEndPoint.Port.Equals(port)*/)  // 내가 접속하고 있는 포트
                    {
                        continue;
                    }
                    else
                    {
                        string ip = info.RemoteEndPoint.Address.ToString();
                        if (IP_MAP.ContainsKey(ip))
                        {
                            ip = string.Format("{0} ({1})", ip, IP_MAP[ip]);
                        }
                        r.Add(ip);
                    }
                }   //foreach
                return r;
            }   //try
            catch (Exception)
            {
                return r;
            }  
        }
    }
}
