using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using wcr_console;

namespace wcr
{
    public partial class _Default : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            List<string> list = RemoteDesktopFinder.Who();
            foreach (string li in list)
            {
                list.Add(li);
            }
        }
    }
}
