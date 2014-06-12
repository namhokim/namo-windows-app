using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;

namespace DeviceTracer.Interop
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct MIB_IPNETROW
    {
        /// <summary>
        /// The index of the adapter.
        /// </summary>
        public int dwIndex;
        /// <summary>
        /// The length, in bytes, of the physical address.
        /// </summary>
        public int dwPhysAddrLen;
        /// <summary>
        /// The physical address.
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = IPHelper.MAXLEN_PHYSADDR)]
        public byte[] bPhysAddr;
        /// <summary>
        /// The IPv4 address.
        /// </summary>
        public int dwAddr;
        /// <summary>
        /// The type of ARP entry. This type can be one of the following values.
        /// </summary>
        public int dwType;
    }
}
