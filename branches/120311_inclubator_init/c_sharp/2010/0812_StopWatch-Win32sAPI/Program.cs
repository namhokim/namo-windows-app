using System;

namespace Namo
{
    class Program
    {
        static void Main(string[] args)
        {
            Namo.Time.StopWatch sw = new Namo.Time.StopWatch();
            sw.Start();
            System.Threading.Thread.Sleep(300);
            sw.Stop();
            Console.WriteLine("Lap time : {0}", sw.LapTime());
        }
    }
}
