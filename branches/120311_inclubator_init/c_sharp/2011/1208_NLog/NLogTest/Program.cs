using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NLog;

namespace NLogTest
{
    class Program
    {
        static void Main(string[] args)
        {
            Logger logger = LogManager.GetCurrentClassLogger();

            string arg1 = "Prog";
            if (args.Length > 0)
            {
                arg1 = args[0];
            }

            for (int i = 1; i <= 20000; ++i)
            {
                logger.Info("{0}\t{1} Test Message", arg1, i);
            }
        }
    }
}
