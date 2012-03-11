using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;


namespace ReadRegistry
{
    class Program
    {
        static void Main(string[] args)
        {
            // HLKM 접근 권한이 있어야 하지는 않다. (query 권한만 쓴다면)
            //string user = "namhokim";
            if (args.Length > 0)
            {
                string sid = Username2SID(args[0]);
                Console.WriteLine(args[0] + " is " + sid);
            }
            else
            {
                Console.WriteLine("need to username parameter");
            }
            // Create a RegistryKey, which will access the HKEY_USERS
            // key in the registry of this machine.
//            RegistryKey rk = Registry.Users;

            // Print out the keys.
//            PrintKeys(rk);
        }

        const string RegProfileList = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList";
        const string RegProfileImagePath = "ProfileImagePath";

        static string Username2SID(string username)
        {
            RegistryKey hKLM = Registry.LocalMachine.OpenSubKey(RegProfileList, false);
            // Retrieve all the subkeys for the specified key.
            String[] names = hKLM.GetSubKeyNames();

            // Print the contents of the array to the console.
            foreach (String s in names)
            {
                string subkey = RegProfileList + System.IO.Path.DirectorySeparatorChar + s;
                RegistryKey pl = Registry.LocalMachine.OpenSubKey(subkey, false);
                string val = pl.GetValue(RegProfileImagePath).ToString();

                if (val.Contains(username))
                {
                    pl.Close();
                    hKLM.Close();

                    return s;
                }

                pl.Close();
            }

            hKLM.Close();

            return "";
        }

    }
}
