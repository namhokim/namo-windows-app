using System;
using System.Security.Cryptography;

// http://msdn.microsoft.com/en-us/library/system.security.cryptography.asnencodeddata.aspx

namespace System.Net.SNMP
{
    class Program
    {
        static void Main(string[] args)
        {
            byte[] rawData = new byte[12];
            rawData[0] = 0x06;
            rawData[1] = 0x08;
            rawData[2] = 0x2b;
            rawData[3] = 0x06;
            rawData[4] = 0x01;
            rawData[5] = 0x02;
            rawData[6] = 0x01;
            rawData[7] = 0x01;
            rawData[8] = 0x01;
            rawData[9] = 0x00;
            rawData[10] = 0x05;
            rawData[11] = 0x00;
            AsnEncodedData aed = new AsnEncodedData("1.3.6.1.2.1.1.1.0", rawData);
            Console.WriteLine("{0} - {1}", aed.Format(true), aed.GetType());
        }
    }
}
