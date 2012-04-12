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
            List<string> lists = RemoteDesktopFinder.Who();
            foreach (string li in lists)
            {
                Console.WriteLine(li);
            }
        }
    }
}
