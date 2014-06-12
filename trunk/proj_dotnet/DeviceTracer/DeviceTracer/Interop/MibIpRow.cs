using System.Net;
using System.Net.NetworkInformation;

namespace DeviceTracer.Interop
{
    public class MibIpRow
    {
        public int Index { get; set; }
        public PhysicalAddress PhysAddr { get; set; }
        public IPAddress Addr { get; set; }
        public MibIpNetType Type { get; set; }
    }
}
