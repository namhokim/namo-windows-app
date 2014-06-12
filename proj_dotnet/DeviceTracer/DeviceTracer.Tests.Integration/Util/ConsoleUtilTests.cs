using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DeviceTracer.Util
{
    using NUnit.Framework;

    [TestFixture]
    class ConsoleUtilTests
    {
        [Test]
        public void IsRunAsAdminTest()
        {
            // 현재 NUnit이 관리자 모드가 아니라고 가정한다.
            Assert.False(ConsoleUtil.IsRunAsAdmin());
        }

        [Test]
        public void RunAsAdminTest()
        {
            ConsoleUtil.RunAsAdmin("arp", "-d *");

        }
    }
}
