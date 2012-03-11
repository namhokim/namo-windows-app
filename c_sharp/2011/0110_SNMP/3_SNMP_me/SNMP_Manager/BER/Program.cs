using System;
using System.DirectoryServices.Protocols;

// http://msdn.microsoft.com/ko-kr/library/system.directoryservices.protocols.berconverter.encode.aspx
// http://msdn.microsoft.com/ko-kr/library/system.directoryservices.protocols.berconverter.decode.aspx

namespace System.Net.SNMP
{
    class Program
    {
        static void Main(string[] args)
        {
            Byte[] int14 = BerConverter.Encode("{i}", 14);
            Byte[] berData = BerConverter.Encode("{s}", "HI");
            object[] objs = BerConverter.Decode("{i}", int14);
            Console.WriteLine("{0}", berData.GetLength(0));

            
        }
    }
}
