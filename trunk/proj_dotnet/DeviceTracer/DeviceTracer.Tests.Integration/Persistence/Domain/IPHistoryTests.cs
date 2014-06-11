using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NHibernate;

namespace DeviceTracer.Persistence.Domain
{
    using NUnit.Framework;

    [TestFixture]
    class IPHistoryTests
    {
        [Test]
        public void TestSave()
        {
            using (var session = NHibernateHelper.OpenSession())
            {
                using (ITransaction transaction = session.BeginTransaction())
                {
                    // Create a IPHistory...
                    var ipHistory = new IPHistory
                    {
                        MacAddress = "f0-de-f1-6e-11-7b",
                        IPAddress = "10.17.1.133",
                    };

                    // And save it to the database
                    var guid = session.Save(ipHistory);
                    session.Flush();

                    // Get
                    var deviceSaved = session.Get<IPHistory>(guid);
                    //Console.WriteLine("Object ID : {0}", guid.ToString());
                    Assert.AreEqual(ipHistory, deviceSaved);

                    transaction.Rollback();
                    //transaction.Commit();
                }
            }
        }
    }
}
