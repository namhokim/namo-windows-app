using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using wcr_console;

namespace wcr
{
    public partial class WebService : ServiceBase
    {
        private WebServer m_ws;

        public WebService()
        {
            InitializeComponent();
            m_ws = new WebServer();
        }

        protected override void OnStart(string[] args)
        {
            m_ws.Start();
        }

        protected override void OnStop()
        {
            m_ws.Stop();
        }
    }
}
