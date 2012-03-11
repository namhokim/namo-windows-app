using System;
using System.Security.Cryptography;

// http://msdn.microsoft.com/en-us/library/system.security.cryptography.oid.aspx

namespace System.Net.SNMP
{
    class Program
    {
        static void Main(string[] args)
        {
            // Assign values to strings.
            string sysDesc = "1.3.6.1.2.1.1.1.0";
            string sysDescName = "sysDesc";

            Oid o1 = new Oid(sysDesc, sysDescName);
            Console.WriteLine("{0}", o1.Value);
        }
    }
}
