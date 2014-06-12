using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DeviceTracer.Interop
{
    using NUnit.Framework;

    [TestFixture]
    class IphlpapiTests
    {
        [Test]
        public void TestIphlpapi()
        {
            IEnumerable<MibIpRow> entries = Iphlpapi.GetIpNetTable();
            Assert.AreNotEqual(0, entries.Count());
            foreach (var row in entries)
            {
                Console.WriteLine("{0}\t{1}\t{2}\t{3}", row.Index, row.Addr, row.PhysAddr, row.Type);
            }
        }
    }
}
