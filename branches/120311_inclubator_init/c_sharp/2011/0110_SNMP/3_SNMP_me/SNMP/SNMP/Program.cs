using System;
using System.Text;
using System.Net.SNMP;

namespace Application
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
            int commlength, miblength, datatype, datalength, datastart;
            string mib = "1.3.6.1.2.1.1.1.0";
            string output;

            byte[] response = Request.Get(PDU.Type.Get, ip, "public", mib, 5000);
            if (response[0] == Request.SocketException)
            {
                int errCode = BitConverter.ToInt32(response, 1);
                int errMsgLen = BitConverter.ToInt32(response, 5);
                string errMsg = Encoding.Unicode.GetString(response, 9, errMsgLen);
                Console.WriteLine("No response from {0} - code : {1} / message : {2}", ip, errCode, errMsg);
                return;
            }

            // 클래스로
            Message m = new Message(response);
            Console.WriteLine("msg length : {0}", m.messageLength);
            Console.WriteLine("version : {0}", m.version);
            Console.WriteLine("community : {0}", m.community);
            Console.WriteLine("PDU legnth : {0}", m.pdu.length);
            Console.WriteLine("requestID : {0}", m.pdu.requestID);
            Console.WriteLine("errorStatus : {0}", m.pdu.errorStatus);
            Console.WriteLine("errorIndex : {0}", m.pdu.errorIndex);

            // 패킷에서 커뮤니티 이름과 MIB 길이를 구함
            commlength = Convert.ToInt16(response[6]);
            miblength = Convert.ToInt16(response[23 + commlength]);

            // SNMP 응답에서 MIB data를 추출
            datatype = Convert.ToInt16(response[24 + commlength + miblength]);
            datalength = Convert.ToInt16(response[25 + commlength + miblength]);
            datastart = 26 + commlength + miblength;
            output = Encoding.ASCII.GetString(response, 44, 123);
            Console.WriteLine(" sysName - Datatype: {0}, Value: {1}", datatype, output);
        }
    }
}
