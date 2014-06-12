﻿using System;
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
    }
}
