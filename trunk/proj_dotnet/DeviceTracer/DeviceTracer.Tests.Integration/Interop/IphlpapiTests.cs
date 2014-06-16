using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;

namespace DeviceTracer.Interop
{
    using NUnit.Framework;

    [TestFixture]
    class IphlpapiTests
    {
        [Test]
        public void TestGetIpNetTable()
        {
            IEnumerable<MibIpRow> entries = Iphlpapi.GetIpNetTable();
            Assert.AreNotEqual(0, entries.Count());
            foreach (var row in entries)
            {
                Console.WriteLine("{0}\t{1}\t{2}\t{3}", row.Index, row.Addr, row.PhysAddr, row.Type);
            }
        }

        [Test]
        public void TestFlushIpNetTable12()
        {
            try {
                Assert.AreEqual(IPHelper.NO_ERROR, Iphlpapi.FlushIpNetTable(12));
            }
            catch (Exception ex)
            {
                Assert.Fail("{0}. 작업을 수행할 권한(관리자)이 없습니다.", ex.Message);
            }
        }

        [Test]
        public void TestFlushIpNetTable()
        {
            try
            {
                Assert.AreEqual(IPHelper.NO_ERROR, Iphlpapi.FlushIpNetTable());
            }
            catch (Exception ex)
            {
                Assert.Fail("{0}. 작업을 수행할 권한(관리자)이 없습니다.", ex.Message);
            }
        }

        [Test]
        public void TestSendARP()
        {
            try
            {
                string ipAddress = "10.17.1.23";
                System.Net.NetworkInformation.PhysicalAddress phyAddr;
                Assert.AreEqual(IPHelper.NO_ERROR, Iphlpapi.SendARP(IPAddress.Parse(ipAddress), out phyAddr));
                Console.WriteLine("IP: {0} -> MAC: {1}", ipAddress, phyAddr);
            }
            catch (Exception ex)
            {
                Assert.Fail("{0}. 작업을 수행할 권한(관리자)이 없습니다.", ex.Message);
            }
        }
    }
}
