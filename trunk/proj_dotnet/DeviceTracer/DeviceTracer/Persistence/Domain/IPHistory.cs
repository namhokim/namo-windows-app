using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DeviceTracer.Persistence.Domain
{
    public class IPHistory
    {
        public virtual String Id { get; set; }
        public virtual String MacAddress { get; set; }
        public virtual String IPAddress { get; set; }
        public virtual DateTime StartDateTime { get; set; }
        public virtual DateTime EndDateTime { get; set; }
    }
}
