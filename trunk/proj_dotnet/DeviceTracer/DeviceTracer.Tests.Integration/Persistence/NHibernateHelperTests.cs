using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DeviceTracer.Persistence
{
    using NUnit.Framework;

    [TestFixture]
    class NHibernateHelperTests
    {
        [Test]
        public void TestOpenSession()
        {
            using(var session = NHibernateHelper.OpenSession())
            {
                Assert.NotNull(session);
            }
        }
    }
}
