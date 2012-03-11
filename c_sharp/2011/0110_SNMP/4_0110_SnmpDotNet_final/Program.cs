using System;
using System.Net;
using System.Collections.Generic;
using Lextm.SharpSnmpLib;
using Lextm.SharpSnmpLib.Messaging;

namespace SnmpDotNet
{
    class Program
    {
        static void Main(string[] args)
        {
            string ip = "10.16.11.23";

            long l = 0;
            long lBeginTick = DateTime.Now.Ticks;
            while (true)
            {
                Console.WriteLine("Try {0} - lab time {1}",
                    l++,
                    ((DateTime.Now.Ticks - lBeginTick) / 10000) / (double)1000);
                Test(ip);
                Console.WriteLine("");
                System.Threading.Thread.Sleep(1000);
                //break;
            }
        }

        static void Test(string ip)
        {
            Variable var = new Variable(new ObjectIdentifier("1.3.6.1.2.1.1.1.0"));
            List<Variable> variables = new List<Variable>();
            variables.Add(var);

            IPHostEntry ihe = Dns.GetHostEntry(ip);
            IPEndPoint iep = new IPEndPoint(ihe.AddressList[0], 161);

            GetRequestMessage req = new GetRequestMessage(1, VersionCode.V1, new OctetString("public"), variables);
            ISnmpMessage msg = req.GetResponse(3000, iep);

            Console.WriteLine("{0}", msg.Pdu().Variables[0].Data.ToString());
        }
    }
}
