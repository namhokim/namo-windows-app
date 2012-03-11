using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Net.NetworkInformation;
using System.Text;
using System.Collections;

namespace PingTest
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamWriter file = new StreamWriter(getFilename());

            Stopwatch sw = new Stopwatch();
            sw.Start();

            if (args.Length > 0)
            {
                ping(args[0]);

                string output = ping(args[0]);
                Console.WriteLine(output);
                file.WriteLine(output);
            }
            else
            {
                //Console.WriteLine("Usage: PingTest target_name");
                string target = null;
                while ( null != (target = Console.ReadLine()) )
                {
                    if (target.Length > 0)
                    {
                        string output = ping(target);
                        Console.WriteLine(output);
                        file.WriteLine(output);
                    }
                }
            }

            sw.Stop();
            Console.WriteLine("Elapsed: {0} ms", sw.ElapsedMilliseconds);
            file.WriteLine("Elapsed: {0} ms", sw.ElapsedMilliseconds);

            if (null != file)
            {
                file.Flush();
                file.Close();
            }
        }

        static string getFilename()
        {
            return String.Format(
                    "{0:yyyyMMdd_HHmmss_fff}.txt", System.DateTime.Now);
        }

        static string ping(string target)
        {
            Ping pingSender = new Ping();
            PingOptions options = new PingOptions();

            // Use the default Ttl value which is 128,
            // but change the fragmentation behavior.
            options.DontFragment = true;

            // Create a buffer of 32 bytes of data to be transmitted.
            string data = "abcdefghijklmnopqrstuvwabcdefghi";
            Byte[] buffer = Encoding.ASCII.GetBytes(data);
            int timeout = 4000;
            PingReply reply = pingSender.Send(target, timeout, buffer, options);

            string output = "";
            if (reply.Status == IPStatus.Success)
            {
                output = String.Format(
                    "Reply from {0}: bytes={1} time={2}ms TTL={3}",
                    reply.Address.ToString(), reply.Buffer.Length,
                    reply.RoundtripTime, reply.Options.Ttl);
            }
            else
            {
                output = String.Format("Reply from {0}: {1}", target, reply.Status);
            }
            return output;
        }
    }
}