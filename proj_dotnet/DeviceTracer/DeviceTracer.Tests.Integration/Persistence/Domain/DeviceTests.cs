using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NHibernate;

namespace DeviceTracer.Persistence.Domain
{
    using NUnit.Framework;

    [TestFixture]
    class DeviceTests
    {
        [Test]
        public void TestSave()
        {
            using (var session = NHibernateHelper.OpenSession())
            {
                using (ITransaction transaction = session.BeginTransaction())
                {
                    // Create a Device...
                    var device = new Device("f0-de-f1-6e-11-7b");

                    // And save it to the database
                    var guid = session.Save(device);
                    session.Flush();

                    // Get
                    var deviceSaved = session.Get<Device>(guid);
                    //Console.WriteLine("Object ID : {0}", guid.ToString());
                    Assert.AreEqual(device, deviceSaved);

                    transaction.Rollback();
                    //transaction.Commit();
                }
            }
        }

    }
}
