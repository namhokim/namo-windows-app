using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace ApacheAuth
{
    public class Author
    {
        private string m_web_uri;
        private WebClient wc = null;

        public Author(string web_uri)
        {
            m_web_uri = web_uri;

            wc = new WebClient();
        }

        public bool Validate(string id, string password)
        {
            NetworkCredential nc = new NetworkCredential(id, password);
            wc.Credentials = nc;

            try
            {
                byte[] response = wc.DownloadData(m_web_uri);
                return true;
            }
            catch (System.Net.WebException e)
            {
                return false;
            }
        }
    }
}
