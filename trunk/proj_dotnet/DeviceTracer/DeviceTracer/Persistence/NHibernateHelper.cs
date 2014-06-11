using NHibernate;
using NHibernate.Cfg;

namespace DeviceTracer.Persistence
{
    public static class NHibernateHelper
    {
        private static ISessionFactory _sessionFactory =
            (new Configuration())
            .Configure().AddAssembly("DeviceTracer").BuildSessionFactory();

        private static ISessionFactory SessionFactory
        {
            get
            {
                return _sessionFactory;
            }
        }

        public static ISession OpenSession()
        {
            return SessionFactory.OpenSession();
        }
    }
}
