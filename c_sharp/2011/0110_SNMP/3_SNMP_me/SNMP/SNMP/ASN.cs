using System;
using System.Text;

namespace System.Net.SNMP
{
    ///////////////////////////////////////////////////////////////////////////////
    //                                                                           //
    // SNMP Application Syntax Values                                            //
    //                                                                           //
    ///////////////////////////////////////////////////////////////////////////////
    public class ASN
    {
        public const int Universal = 0x00;
        public const int Application = 0x40;
        public const int Context = 0x80;
        public const int Private = 0xC0;

        public const int Primitive = 0x00;
        public const int Constructor = 0x20;

        public enum Type
        {
            // SNMP Simple Syntax Values
            Integer = Universal | Primitive | 0x02,
            Bits = Universal | Primitive | 0x03,
            OctetString = Universal | Primitive | 0x04,
            Null = Universal | Primitive | 0x05,
            ObjectIdentifier = Universal | Primitive | 0x06,
            Integer32 = Universal | Primitive | 0x07,

            // SNMP Constructor Syntax Values
            Sequence = Universal | Constructor | 0x10,  // 0x30
            SequenceOf = Sequence,                      // 0x30

            // SNMP Application Syntax Values
            IPAddress = Application | Primitive | 0x00, // 0x40
            Counter32 = Application | Primitive | 0x01, // 0x41
            Gauge32 = Application | Primitive | 0x02,   // 0x42
            TimeTicks = Application | Primitive | 0x03, // 0x43
            Opaque = Application | Primitive | 0x04,    // 0x44
            Counter64 = Application | Primitive | 0x06, // 0x06
            UInteger32 = Application | Primitive | 0x07,// 0x07
        }

        // Type from <snmp.h> L.84 struct AsnAny
        public abstract class Variation
        {
            public Type asnType { get; set; }
        }

        // ASN_INTEGER, ASN_INTEGER32
        public class Integer32 : Variation
        {
            public int value;

            public Integer32()
            {
                this.asnType = Type.Integer32;
            }

            public Integer32(byte[] packet, int startIdx, int length)
            {
                this.asnType = Type.Integer32;
                SetValue(packet, startIdx, length);
            }

            public void SetValue(byte[] packet, int startIdx, int length)
            {
                this.value = 0;

                for (int i = 0; i < length; ++i)
                {
                    if (i > 0) value <<= 8;
                    value |= packet[startIdx + i];
                }
            }
        }

        // ASN_UNSIGNED32
        public class Unsigned32 : Variation
        {
            public uint value;

            public Unsigned32()
            {
                this.asnType = Type.UInteger32;
            }
        }

        // ASN_COUNTER64
        public class Counter64 : Variation
        {
            public ulong value;

            public Counter64()
            {
                this.asnType = Type.Counter64;
            }
        }

        // ASN_OCTETSTRING
        public class OctetString : Variation
        {
            public string value;

            public OctetString()
            {
                this.asnType = Type.OctetString;
            }
        }

        // ASN_BITS
        public class Bits : Variation
        {
            public string value;

            public Bits()
            {
                this.asnType = Type.Bits;
            }
        }

        // ASN_OBJECTIDENTIFIER
        public class ObjectIdentifier : Variation
        {
            public string value;

            public ObjectIdentifier()
            {
                this.asnType = Type.ObjectIdentifier;
            }
            //SetValue

            public void SetValue(byte[] packet, int startIdx, int length)
            {
                StringBuilder sb = new StringBuilder (length*2-1);

                for (int i = 0; i < length; ++i)
                {
                    if (i > 0) sb.Append(".");
                    if (i == 0 && packet[i + startIdx] == 0x2B)
                    {
                        sb.Append("1.3");
                    }
                    else
                    {
                        sb.Append(packet[i + startIdx]);
                    }
                }
                value = sb.ToString();
            }
        }

        // ASN_SEQUENCE
        public class Sequence : Variation
        {
            public string value;

            public Sequence()
            {
                this.asnType = Type.Sequence;
            }
        }

        // ASN_IPADDRESS
        public class IPAddress : Variation
        {
            public string value;

            public IPAddress()
            {
                this.asnType = Type.IPAddress;
            }
        }

        // ASN_COUNTER32
        public class Counter32 : Variation
        {
            public uint value;

            public Counter32()
            {
                this.asnType = Type.Counter32;
            }
        }

        // ASN_GAUGE32
        public class Gauge32 : Variation
        {
            public uint value;

            public Gauge32()
            {
                this.asnType = Type.Gauge32;
            }
        }

        // ASN_TIMETICKS
        public class TimeTicks : Variation
        {
            public uint value;

            public TimeTicks()
            {
                this.asnType = Type.TimeTicks;
            }
        }

        // ASN_OPAQUE
        public class Opaque : Variation
        {
            public string value;

            public Opaque()
            {
                this.asnType = Type.Opaque;
            }
        }

    }
}
