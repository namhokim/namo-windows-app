using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DeviceTracer.Persistence.Domain
{
    public class Device
    {
        public virtual String Id { get; set; }
        public virtual String MacAddress { get; set; }
        public virtual String UserName { get; set; }
        public virtual String ModelName { get; set; }
        public virtual String Comment { get; set; }
        public virtual DateTime CreatedDateTime { get; set; }
        public virtual DateTime UpdatedDateTime { get; set; }

        public Device() {}

        public Device(string macAddress)
        {
            this.MacAddress = macAddress;
            this.CreatedDateTime = DateTime.Now;
        }
    }
}
