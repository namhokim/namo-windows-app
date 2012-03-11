using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;   // for DllImport

namespace UsingByCsharp
{
    class Program
    {
        // struct for UNICODE
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct LocalizedName
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string value;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct SOME_INFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
            public string szIPAddr;

            public LocalizedName szLocalizedName;

            [MarshalAs(UnmanagedType.I4)]
            public Int32 status;
        }

        [DllImport("DllByClang.dll", EntryPoint = "GetSomeInfo")]
        public static extern bool GetPrtSystemInfo(string ip, [Out] IntPtr lpSOME_INFO, ref uint dwSize);

        static void Main(string[] args)
        {
            uint dwSize = 0;
            IntPtr pAddr = new IntPtr();
            Console.WriteLine("C# : struct Address : " + pAddr.ToInt32());
            if (GetPrtSystemInfo("10.16.11.23", pAddr, ref dwSize))
            {
                Type type = typeof(SOME_INFO);
                int size = Marshal.SizeOf(type);
                int alloc_size = size * (int)dwSize;

                pAddr = Marshal.AllocHGlobal(alloc_size);

                PrintDatatypes();
                Console.WriteLine("C# : struct Address : " + pAddr.ToInt32());

                
                if (GetPrtSystemInfo("10.16.11.23", pAddr, ref dwSize))
                {
                    SOME_INFO[] info = new SOME_INFO[dwSize];
                    int offset = pAddr.ToInt32();

                    int increment = size;
                    for (int i = 0; i < dwSize; i++)
                    {
                        info[i] = (SOME_INFO)Marshal.PtrToStructure(new IntPtr(offset), type);
                        Console.WriteLine(info[i].szIPAddr);
                        Console.WriteLine(info[i].szLocalizedName.value);
                        Console.WriteLine(info[i].status);
                        Console.WriteLine();
                        offset += increment;
                    }
                    
                }
                Marshal.FreeHGlobal(pAddr);
            }
            else
            {
                Console.WriteLine("Get failed");
            }
        }

        static void PrintDatatypes()
        {
            Type type = typeof(SOME_INFO);
            int size = Marshal.SizeOf(type);
            SOME_INFO si = new SOME_INFO();
            int size_si = Marshal.SizeOf(si);

            Console.WriteLine("Marshal.SizeOf(typeof(SOME_INFO)) : " + size);
            Console.WriteLine("Marshal.SizeOf(new SOME_INFO()) : " + size_si);
        }
    }
}
