using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Cache;
using System.IO;
using System.Diagnostics;

namespace HttpChecker
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length > 0)
            {
                string url = args[0];
                HttpWebRequest webRequest = (HttpWebRequest)WebRequest.Create(url);
                webRequest.AllowAutoRedirect = false;
                webRequest.CachePolicy = new RequestCachePolicy(RequestCacheLevel.NoCacheNoStore);
                webRequest.KeepAlive = true;

                Stopwatch sw = new Stopwatch();
                HttpWebResponse response = null;

                try
                {
                    sw.Start();
                    response = (HttpWebResponse)webRequest.GetResponse();
                    sw.Stop();
                    Console.WriteLine("{0:yyyy-MM-dd HH:mm:ss} {1}=>{2}({3}) - {4} ms", System.DateTime.Now, url, (int)response.StatusCode, response.StatusCode, sw.ElapsedMilliseconds);
                    response.Close();
                }
                catch (Exception e)
                {
                    sw.Stop();
                    Console.WriteLine("{0:yyyy-MM-dd HH:mm:ss} {1} - {2} ms", System.DateTime.Now, e.Message, sw.ElapsedMilliseconds);
                }
                finally
                {
                    if(null!=response) response.Close();
                }
            }
        }
    }
}
