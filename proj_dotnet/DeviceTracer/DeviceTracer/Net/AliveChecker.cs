using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace DeviceTracer.Net
{
    public class AliveChecker
    {
        /// <summary>
        /// 1.3.6.1.2.1.1.1.0
        /// </summary>
        private static readonly byte[] SysDescr = new byte[] { 0x30, 0x26, 0x02, 0x01, 0x00, 0x04, 0x06, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0xA0, 0x19, 0x02, 0x01, 0x02, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x30, 0x0E, 0x30, 0x0C, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x02, 0x01, 0x01, 0x01, 0x00, 0x05, 0x00 };
        private static readonly int SnmpPort = 161;

        public void Broadcast(int timeout)
        {
            //Creates a UdpClient for reading incoming data.
            UdpClient client = new UdpClient();

            client.Client.EnableBroadcast = true;

            //Creates an IPEndPoint to record the IP Address and port number of the sender. 
            // The IPEndPoint will allow you to read datagrams sent from any source.
            IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Broadcast, SnmpPort);

            try
            {
                client.Send(SysDescr, SysDescr.Length, RemoteIpEndPoint);

                // Blocks until a message returns on this socket from a remote host.
                Byte[] receiveBytes = client.Receive(ref RemoteIpEndPoint);

                Console.WriteLine("This message was sent from " +
                                            RemoteIpEndPoint.Address.ToString() +
                                            " on their port number " +
                                            RemoteIpEndPoint.Port.ToString());
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
