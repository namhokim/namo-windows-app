﻿using System;

namespace MultiCast
{
    delegate void dMe(int a);

    class Program
    {
        public static void Method1(int a)
        {
            Console.WriteLine("Method1 " + a);
        }

        public static void Method2(int a)
        {
            Console.WriteLine("Method2 " + a);
        }

        static void Main(string[] args)
        {
            dMe d;

            d = Method1;
            d += Method2;
            d(34);
        }
    }
}