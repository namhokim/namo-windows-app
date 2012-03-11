using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeforeLog
{
    class Program
    {
        static void Main(string[] args)
        {
            string arg1 = "Prog";
            if (args.Length > 0)
            {
                arg1 = args[0];
            }

            for (int i = 1; i <= 20000; ++i)
            {
                log(string.Format("{0}\t{1} Test Message", arg1, i));
            }
        }

        private static void log(string message)
        {
            try
            {
                string m_strLogPrefix = AppDomain.CurrentDomain.BaseDirectory + @"LOG\";
                string m_strLogExt = @".LOG";
                DateTime dtNow = DateTime.Now;
                string strDate = dtNow.ToString("yyyy-MM-dd");
                string strTime = dtNow.ToString("HH:mm:ss");
                string strPath = String.Format("{0}{1}{2}", m_strLogPrefix, strDate, m_strLogExt);
                string strDir = System.IO.Path.GetDirectoryName(strPath);
                System.IO.DirectoryInfo diDir = new System.IO.DirectoryInfo(strDir);

                if (!diDir.Exists)
                {
                    diDir.Create();
                    diDir = new System.IO.DirectoryInfo(strDir);
                }

                if (diDir.Exists)
                {
                    System.IO.StreamWriter swStream = System.IO.File.AppendText(strPath);
                    string strLog = String.Format("{0} -  {1}", strTime, message);
                    swStream.WriteLine(strLog);
                    swStream.Close(); ;
                }
            }
            catch (System.Exception)
            {
                //Debug.WriteLine(e.Message);
            }
        }
    }
}
