using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Net;
using System.Net.NetworkInformation;
using System.Runtime.InteropServices;

namespace DeviceTracer.Interop
{
    public static class Iphlpapi
    {
        #region "GetIpNetTable wrapper"
        public static IEnumerable<MibIpRow> GetIpNetTable()
        {
            int bytesNeeded = GetIpNetTable_GetAllocSize();
            MIB_IPNETROW[] table = GetIpNetTable_GetData(bytesNeeded);
            return GetIpNetTable_ConvertToDotNetType(table);
        }

        #region "GetIpNetTable sub-method"
        private static int GetIpNetTable_GetAllocSize()
        {
            // The number of bytes needed.
            int bytesNeeded = 0;

            // The result from the API call.
            int result = IPHelper.GetIpNetTable(IntPtr.Zero, ref bytesNeeded, false);

            // Call the function, expecting an insufficient buffer.
            if (result != IPHelper.ERROR_INSUFFICIENT_BUFFER)
            {
                // Throw an exception.
                throw new Win32Exception(result);
            }
            else
            {
                return bytesNeeded;
            }
        }

        private static MIB_IPNETROW[] GetIpNetTable_GetData(int bytesNeeded)
        {
            // Allocate the memory, do it in a try/finally block, to ensure
            // that it is released.
            IntPtr buffer = IntPtr.Zero;

            try
            {
                // Allocate the memory.
                buffer = Marshal.AllocCoTaskMem(bytesNeeded);

                // Make the call again. If it did not succeed, then raise an error.
                int result = IPHelper.GetIpNetTable(buffer, ref bytesNeeded, false);

                // If the result is not 0 (no error), then throw an exception.
                if (result != IPHelper.NO_ERROR)
                {
                    // Throw an exception.
                    throw new Win32Exception(result);
                }

                // Now we have the buffer, we have to marshal it. We can read
                // the first 4 bytes to get the length of the buffer.
                int entries = Marshal.ReadInt32(buffer);

                // Increment the memory pointer by the size of the int.
                IntPtr currentBuffer = new IntPtr(buffer.ToInt64() + Marshal.SizeOf(typeof(int)));

                // Allocate an array of entries.
                MIB_IPNETROW[] table = new MIB_IPNETROW[entries];

                // Cycle through the entries.
                for (int index = 0; index < entries; index++)
                {
                    // Call PtrToStructure, getting the structure information.
                    table[index] = (MIB_IPNETROW)Marshal.PtrToStructure(
                        new IntPtr(currentBuffer.ToInt64() + (index * Marshal.SizeOf(typeof(MIB_IPNETROW)))),
                        typeof(MIB_IPNETROW));
                }
                return table;
            }
            finally
            {
                // Release the memory.
                Marshal.FreeCoTaskMem(buffer);
            }
        }

        private static IEnumerable<MibIpRow> GetIpNetTable_ConvertToDotNetType(MIB_IPNETROW[] table)
        {
            int entries = table.Length;

            IList<MibIpRow> list = new List<MibIpRow>(entries);

            for (int index = 0; index < entries; index++)
            {
                list.Add(new MibIpRow
                {
                    Addr = new IPAddress(BitConverter.GetBytes(table[index].dwAddr)),
                    Index = table[index].dwIndex,
                    PhysAddr = new PhysicalAddress(table[index].bPhysAddr),
                    Type = (MibIpNetType)table[index].dwType,
                });
            }

            return list;
        }
        #endregion
        #endregion

        #region "FlushIpNetTable wrapper"
        public static int FlushIpNetTable(int index)
        {
            int result = IPHelper.FlushIpNetTable(index);
            if (result != IPHelper.NO_ERROR)
            {
                // Throw an exception.
                throw new Win32Exception(result);
            }
            else
            {
                return result;
            }
        }

        public static int FlushIpNetTable()
        {
            IEnumerable<Int32> interfaces = FlushIpNetTable_GetInterfaceIndexs();
            foreach (int idx in interfaces)
            {
                FlushIpNetTable(idx);
            }
            return IPHelper.NO_ERROR;
        }

        #region "GetIpNetTable sub-method"
        private static IEnumerable<Int32> FlushIpNetTable_GetInterfaceIndexs()
        {
            var ifSet = new HashSet<int>();
            var list = GetIpNetTable();
            foreach (var item in list)
            {
                ifSet.Add(item.Index);
            }
            return ifSet;
        }
        #endregion
        #endregion

    }
}
