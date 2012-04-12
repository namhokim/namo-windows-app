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

        // refs. http://social.msdn.microsoft.com/Forums/eu/netfxnetcom/thread/bb587794-f0a0-432e-a8b9-8d37cbf5b140
        public static List<string> Who()
        {
            List<string> r = new List<string>();

             //Look for established connections
            IPGlobalProperties ipProperties = IPGlobalProperties.GetIPGlobalProperties();
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
                        r.Add(info.RemoteEndPoint.Address.ToString());
                    }
                }   //foreach
                return r;
            }   //try
            catch (Exception)
            {
                return r;
            }  
        }

        public static string WhoWithHtml()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("<html>\n<title>누가 원격데스크탑을 하고 있나?</title>\n<body>\n<ol>");
            List<string> lists = Who();
            foreach (string li in lists)
            {
                sb.AppendFormat("<li>{0}</li>\n", li);
            }
            sb.Append("</ol>\n</body>\n</html>");
            return sb.ToString();
        }
    }
}
