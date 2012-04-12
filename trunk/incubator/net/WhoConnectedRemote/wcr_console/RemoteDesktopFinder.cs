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
                        && !info.RemoteEndPoint.Port.Equals(port))
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
    }
}
