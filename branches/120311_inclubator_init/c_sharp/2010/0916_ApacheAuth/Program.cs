using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ApacheAuth
{
    class Program
    {
        static void Main(string[] args)
        {
            Author aa = new Author("http://10.16.11.169/test/");
            bool isAuthed = aa.Validate("lsh", "123");
            if (isAuthed)
            {
                Console.WriteLine("Valid");
            }
            else
            {
                Console.WriteLine("Invalid!!");
            }

        }
    }
}
