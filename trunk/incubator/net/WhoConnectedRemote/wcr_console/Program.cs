using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace wcr_console
{
    class Program
    {
        static void Main(string[] args)
        {
            //Console.WriteLine(RemoteDesktopFinder.WhoWithHtml());
            WebServer ws = new WebServer();
            Console.WriteLine("Start the service");
            ws.Start();

            // 대기
            Console.WriteLine("Press Any key to stop the service");
            Console.ReadKey(true);

            Console.WriteLine("Stop the service");
            ws.Stop();
        }
    }
}
