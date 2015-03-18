using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DeviceTracer.Net
{
    using NUnit.Framework;

    [TestFixture]
    class AliveCheckerTests
    {
        AliveChecker checker;

        [SetUp]
        public void Init()
        {
            checker = new AliveChecker();
        }

        [Test]
        public void Intance_notNull()
        {
            Assert.NotNull(checker);
        }

        [Test]
        public void TestBroadcast()
        {
            checker.Broadcast(1000);
        }

    }
}
