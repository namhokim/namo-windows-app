
namespace SQLServerNDT.Forms
{
    enum AuthenticationTypes
    {
        Windows, SQLServer,
    }

    public class AuthenticationType
    {
        AuthenticationTypes _type;

        public AuthenticationType()
        {
            _type = AuthenticationTypes.Windows;
        }

        public override string ToString()
        {
            return _type.ToString();
        }

    }
}
