using System;
using System.Runtime.InteropServices;

namespace DeviceTracer.Interop
{
    public class IPHelper
    {
        public const int NO_ERROR = 0;

        /// <summary>
        /// The buffer pointed to by the pIpNetTable parameter is not large enough.
        /// The required size is returned in the DWORD variable pointed to
        /// by the pdwSize parameter.
        /// </summary>
        public const int ERROR_INSUFFICIENT_BUFFER = 122;

        /// <summary>
        /// An invalid parameter was passed to the function. This error is returned
        /// if the pdwSize parameter is NULL, or GetIpNetTable is unable to write
        /// to the memory pointed to by the pdwSize parameter.
        /// </summary>
        public const int ERROR_INVALID_PARAMETER = 87;

        /// <summary>
        /// The IPv4 transport is not configured on the local computer.
        /// </summary>
        public const int ERROR_NOT_SUPPORTED = 50;

        public const int MAXLEN_PHYSADDR = 8;

        /// <summary>
        /// The GetIpNetTable function retrieves the IP-to-physical address mapping table.
        /// </summary>
        /// <param name="pIpNetTable">A pointer to a buffer that receives the
        ///        IP-to-physical address mapping table as a MIB_IPNETTABLE structure.</param>
        /// <param name="pdwSize">On input, specifies the size of the buffer pointed to
        /// by the pIpNetTable parameter.
        /// <para>On output, if the buffer is not large enough to hold the returned mapping table,
        /// the function sets this parameter equal to the required buffer size</para></param>
        /// <param name="bOrder">A Boolean value that specifies whether the returned mapping
        /// table should be sorted in ascending order by IP address. If this parameter is TRUE,
        /// the table is sorted.</param>
        /// <returns>If the function succeeds, the return value is NO_ERROR.
        /// <para>If the function fails, the return value is one of the following error codes:
        /// ERROR_INSUFFICIENT_BUFFER, ERROR_INVALID_PARAMETER, ERROR_NOT_SUPPORTED or other code.
        /// </para>
        /// </returns>
        [DllImport("Iphlpapi.dll", EntryPoint = "GetIpNetTable")]
        internal static extern int GetIpNetTable(IntPtr pIpNetTable, ref int pdwSize, bool bOrder);

        [DllImport("Iphlpapi.dll", EntryPoint = "FlushIpNetTable")]
        internal static extern int FlushIpNetTable(int dwIfIndex);

        /// <summary>
        /// http://msdn.microsoft.com/en-us/library/windows/desktop/aa366358(v=vs.85).aspx
        /// </summary>
        /// <param name="DestIP">The destination IPv4 address, in the form of an IPAddr structure. The ARP request attempts to obtain the physical address that corresponds to this IPv4 address.</param>
        /// <param name="SrcIP">The source IPv4 address of the sender, in the form of an IPAddr structure. This parameter is optional and is used to select the interface to send the request on for the ARP entry. The caller may specify zero corresponding to the INADDR_ANY IPv4 address for this parameter.</param>
        /// <param name="pMacAddr">A pointer to an array of ULONG variables. This array must have at least two ULONG elements to hold an Ethernet or token ring physical address. The first six bytes of this array receive the physical address that corresponds to the IPv4 address specified by the DestIP parameter.</param>
        /// <param name="PhyAddrLen">On input, a pointer to a ULONG value that specifies the maximum buffer size, in bytes, the application has set aside to receive the physical address or MAC address. The buffer size should be at least 6 bytes for an Ethernet or token ring physical address</param>
        /// <returns>If the function succeeds, the return value is NO_ERROR.</returns>
        [DllImport("iphlpapi.dll", ExactSpelling = true)]
        public static extern int SendARP(uint DestIP, uint SrcIP, byte[] pMacAddr, ref int PhyAddrLen);
    }
}
