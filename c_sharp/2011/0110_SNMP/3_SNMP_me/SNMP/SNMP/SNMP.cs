using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Collections;

///////////////////////////////////////////////////////////////////////////////
// follow to belows
// SNMPv1 (RFC 1157) - http://www.faqs.org/rfcs/rfc1157.html
// SNMPv2 (RFC 1905) - http://www.faqs.org/rfcs/rfc1905.html
///////////////////////////////////////////////////////////////////////////////

namespace System.Net.SNMP
{
    public class Request
    {
        // Miscellaneous definitions
        public const int SocketException = 0xff;

        const int DefaultSnmpPortUDP = 161;
        const int DefaultSnmpPortIPX = 36879;
        const int DefaultSnmpTrapPortUDP = 162;
        const int DefaultSnmpTrapPortIPX = 36880;

        const int PacketSize = 1024;
        const int MibSize = 1024;
        const int MaxOID_Len = 128;

        //[STAThread]
        /// <summary>
        /// Get 또는 GetNext 패킷을 요청하여 Response 패킷 데이터를 받음
        /// </summary>
        /// <param name="type">PDU.Get 또는 PDU.GetNext</param>
        /// <param name="host">SNMP Agent 주소</param>
        /// <param name="community">커뮤니티 이름(예. "public")</param>
        /// <param name="mibstring">문자열 MIB(예. "1.3.6.1.2.1.1.1.0")</param>
        /// <param name="timeout">접속시 최대 기다리는 시간(ms)</param>
        /// <returns>SNMP Response 패킷</returns>
        public static byte[] Get(PDU.Type type, string host, string community, string mibstring, int timeout)
        {
            byte[] packet = new byte[PacketSize];
            byte[] mib = new byte[MibSize];

            int miblen = Util.StrMibToOid(mibstring, ref mib);
            int comlen = community.Length;  // null?

            int snmplen = 29 + comlen + miblen - 1; // 전체 SNMP 패킷의 길이
            int pos = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // SNMP 시퀀스 시작
            packet[pos++] = 0x30;                           // 시작
            packet[pos++] = Convert.ToByte(snmplen - 2);    // 시퀀스 크기

            // SNMP 버전
            packet[pos++] = 0x02;   // 정수 타입
            packet[pos++] = 0x01;   // 길이
            packet[pos++] = (byte)Version.Type.v1;   // SNMP 버전1

            // 커뮤니티 이름   
            packet[pos++] = 0x04;   // 정수 타입
            packet[pos++] = Convert.ToByte(comlen); // 커뮤니티 길이

            // 커뮤니티 이름을 byte 배열로 변환
            byte[] data = Encoding.ASCII.GetBytes(community);
            for (int i = 0; i < data.Length; ++i)
            {
                packet[pos++] = data[i];
            }

            // GetRequest 혹은 GetNextRequest value
            packet[pos++] = Convert.ToByte(type);

            packet[pos++] = Convert.ToByte(20 + miblen - 1);    // MIB의 전체 크기

            // 요청 ID
            packet[pos++] = 0x02;   // 정수 타입
            packet[pos++] = 0x04;   // 길이
            packet[pos++] = 0x01;   // SNMP 요청 ID
            packet[pos++] = 0x2E;
            packet[pos++] = 0x6E;
            packet[pos++] = 0x4A;

            // 에러 상태
            packet[pos++] = 0x02;   // 정수 타입
            packet[pos++] = 0x01;   // 길이
            packet[pos++] = 0x00;   // SNMP 에러 상태

            // Error index
            packet[pos++] = 0x02;   // 정수 타입
            packet[pos++] = 0x01;   // 길이
            packet[pos++] = 0x00;   // SNMP 에러 인덱스

            // 변수 바인딩 시작
            packet[pos++] = 0x30;

            packet[pos++] = Convert.ToByte(6 + miblen - 1); // 바인딩 변수 크기
            packet[pos++] = 0X30;                           // 첫 변수 바인딩 시퀀스의 시작
            packet[pos++] = Convert.ToByte(6 + miblen - 1 - 2);
            packet[pos++] = 0x06;
            packet[pos++] = Convert.ToByte(miblen - 1);       // 길이

            // MIB의 시작
            packet[pos++] = 0x2b;

            // 패킷내의 MIB 배열 위치
            for (int i = 2; i < miblen; ++i) packet[pos++] = Convert.ToByte(mib[i]);
            packet[pos++] = 0x05;   // Null 오브젝트 값
            packet[pos++] = 0x00;   // Null

            ///////////////////////////////////////////////////////////////////////////////
            // 목적지에 패킷을 보냄
            Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            sock.SetSocketOption(SocketOptionLevel.Socket,
                SocketOptionName.ReceiveTimeout, timeout);
            IPHostEntry ihe = Dns.GetHostEntry(host);
            IPEndPoint iep = new IPEndPoint(ihe.AddressList[0], DefaultSnmpPortUDP);
            EndPoint ep = (EndPoint)iep;

            try
            {
                // SNMP Agent에 요청
                sock.SendTo(packet, snmplen, SocketFlags.None, iep);

                // 패킷에서 응답을 받음
                int recv = sock.ReceiveFrom(packet, ref ep);
            }
            catch (SocketException e)
            {
                byte[] byteErrCode = BitConverter.GetBytes(e.ErrorCode);
                byte[] byteErrMessage = Encoding.Unicode.GetBytes(e.Message);
                byte[] byteMsgSize = BitConverter.GetBytes(byteErrMessage.Length);

                int ppos = 0;
                packet[ppos++] = Request.SocketException;
                for (int i = 0; i < byteErrCode.Length; ++i)
                    packet[ppos++] = byteErrCode[i];

                for (int i = 0; i < byteMsgSize.Length; ++i)
                    packet[ppos++] = byteMsgSize[i];

                for (int i = 0; i < byteErrMessage.Length; ++i)
                    packet[ppos++] = byteErrMessage[i];
            }
            finally
            {
                sock.Close();
            }
            ///////////////////////////////////////////////////////////////////////////////

            return packet;
        }
    }   /* class Request */

    public class Version
    {
        public enum Type
        {
            v1 = 0,
            v2c = 1,
            v3 = 2,
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //                                                                           //
    // PDU Type Values                                                           //
    //                                                                           //
    ///////////////////////////////////////////////////////////////////////////////
    // Protocol Data Unit
    public class PDU
    {
        public const int RequestOffset = 0;
        public const int RequestLengthOffset = 1;
        public const int RequestID_TypeOffset = 2;

        public enum Type
        {
            Get = ASN.Context | ASN.Constructor | 0x0,      //0xA0 : get-request
            GetNext = ASN.Context | ASN.Constructor | 0x1,  //0xA1 : get-next-request
            Response = ASN.Context | ASN.Constructor | 0x2,  //0xA2,      // get-response
            //Set = 0xA3,           // set-request
            //V1Trap = 0xA4,
            //GetBulk = 0xA5,
            //Inform = 0xA6,
            //Trap = 0xA7,          // trap
        }

        // Control Fields
        public PDU.Type typePDU { get; set; }
        public int length { get; set; }
        public int requestID { get; set; }
        public ErrorCodes errorStatus { get; set; }
        public int errorIndex { get; set; }

        // Variation Sequence List
        ArrayList varBindList = new ArrayList();

        // method
        public void SetPacket(byte[] pdu_part_packet)
        {
            if (pdu_part_packet == null || pdu_part_packet.Length < 3)
                return;

            // Get Offset
            int req_ID_len = pdu_part_packet[RequestID_TypeOffset + 1];
            int err_stat_offset = RequestID_TypeOffset + 2 + req_ID_len;
            int err_stat_len = pdu_part_packet[err_stat_offset + 1];
            int err_idx_offset = err_stat_offset + 2 + err_stat_len;
            int err_idx_len = pdu_part_packet[err_idx_offset + 1];

            ASN.Type req_ID_type = (ASN.Type)pdu_part_packet[RequestID_TypeOffset];
            ASN.Type err_stat_type = (ASN.Type)pdu_part_packet[err_stat_offset];
            ASN.Type err_idx_type = (ASN.Type)pdu_part_packet[err_idx_offset];
            ASN.Integer32 asnInt32 = new ASN.Integer32();

            /// Set Attributes
            typePDU = (PDU.Type)pdu_part_packet[RequestOffset];
            length = Convert.ToInt16(pdu_part_packet[RequestLengthOffset]);


            if (req_ID_type == ASN.Type.Integer)
            {
                asnInt32.SetValue(pdu_part_packet, RequestID_TypeOffset + 2, req_ID_len);
                requestID = asnInt32.value;
            }

            if (err_stat_type == ASN.Type.Integer)
            {
                asnInt32.SetValue(pdu_part_packet, err_stat_offset + 2, err_stat_len);
                errorStatus = (ErrorCodes)asnInt32.value;
            }

            if (err_idx_type == ASN.Type.Integer)
            {
                asnInt32.SetValue(pdu_part_packet, err_idx_offset + 2, err_idx_len);
                errorIndex = asnInt32.value;
            }

            /// Set Variation Sequence
            int start_of_var = err_idx_offset + 2 + err_idx_len + 4;
            int size_of_var = pdu_part_packet[start_of_var - 1];
            byte[] varBindPacket = new byte[size_of_var];
            for (int i = 0; i < size_of_var; ++i)
            {
                varBindPacket[i] = pdu_part_packet[start_of_var + i];
            }
            SetVarBindList(varBindPacket);
        }

        void SetVarBindList(byte[] variable_packet)
        {
            int idx = 0;
            ASN.Type OID_type = (ASN.Type)variable_packet[idx++];
            int oid_len = variable_packet[idx++];
            ASN.ObjectIdentifier oid = new ASN.ObjectIdentifier();
            oid.SetValue(variable_packet, idx, oid_len);
        }
    }   /* public class PDU */

    ///////////////////////////////////////////////////////////////////////////////
    //                                                                           //
    // SNMP Error Codes                                                          //
    //                                                                           //
    ///////////////////////////////////////////////////////////////////////////////
    public enum ErrorCodes
    {
        // SNMPv1
        NoError = 0,
        TooBig = 1,
        NoSuchName = 2,
        BadValue = 3,
        ReadOnly = 4,
        GenErr = 5,
        // SNMPv2
        NoAccess = 6,
        WrongType = 7,
        WrongLength = 8,
        WrongEncoding = 9,
        WrongValue = 10,
        NoCreation = 11,
        InconsistentValue = 12,
        ResourceUnavailable = 13,
        CommitFailed = 14,
        UndoFailed = 15,
        AuthorizationError = 16,
        NotWritable = 17,
        InconsistentName = 18,
    }

    public class Util
    {
        public static int StrMibToOid(string mibstr, ref byte[] mib)
        {
            string[] mibvals = mibstr.Split('.');
            int miblen = mibvals.Length;    // 최초 길이
            int tot_miblen = miblen;        // 최종 MIB 길이
            int cnt = 0;

            // 문자열 MIB를 정수의 byte 배열로 바꾼다
            for (int i = 0; i < miblen; ++i)
            {
                int temp = Convert.ToInt16(mibvals[i]);
                if (temp > 127)
                {
                    mib[cnt] = Convert.ToByte(128 + (temp / 128));
                    mib[cnt + 1] = Convert.ToByte(temp - ((temp / 128) * 128));
                    cnt += 2;
                    ++tot_miblen;
                }
                else
                {
                    mib[cnt] = Convert.ToByte(temp);
                    ++cnt;
                }
            }

            return tot_miblen;
        }
    }   /* public class Util */

    public class Message
    {
        // constants
        const byte UDP_SNMP_Message = 0x30;
        const int MsgLenOffset = 1; // message length offset (fixed)
        const int VerTypeOffset = 2;// version type offset (fixed)
        const int VerLenOffset = 3; // version length offset (fixed)
        const int ComTypeOffset = 4; // community type offset (variated by value of VerLenOffset)
        const int ComLenOffset = 5; // community length offset (variated by value of VerLenOffset)

        public Message()
        {
        }
        public Message(byte[] packet)
        {
            SetPacket(packet);
        }

        public bool SetPacket(byte[] packet)
        {
            if (packet==null || packet.Length<3 || UDP_SNMP_Message != packet[0])
                return false;

            // Get Offset
            int verLen = packet[VerLenOffset];
            int comLen = packet[ComLenOffset + verLen];
            int dataOffset = ComLenOffset + verLen + comLen + 1;
            int size_of_pdu = Convert.ToInt16(packet[dataOffset + 1]) + 2;
            ASN.Type verType = (ASN.Type)packet[VerTypeOffset];
            ASN.Type comType = (ASN.Type)packet[ComTypeOffset + verLen];

            /// Set Attributes
            // Message Length
            this.messageLength = Convert.ToInt16(packet[MsgLenOffset]);

            // Version
            if ( ASN.Type.Integer==verType)
                this.version = (Version.Type)packet[VerLenOffset + verLen];

            // Community
            if ( ASN.Type.OctetString == comType)
                this.community =
                    Encoding.ASCII.GetString(packet, ComLenOffset + verLen + 1, comLen);

            // PDU
            byte[] pdu_packet = new byte[size_of_pdu];
            for(int i=0; i<size_of_pdu; ++i) {
                pdu_packet[i] = packet[dataOffset+i];
            }
            pdu.SetPacket(pdu_packet);
            
            
            return true;
        }

        byte[] GetBytes()
        {
            return null;
        }

        // fields
        public int messageLength { get; set; }
        //public int version { get; set; }
        public Version.Type version { get; set; }
        public string community { get; set; }
        
        // PDU
        public PDU pdu = new PDU();
        

    }   /* public class Message */

}   /* namespace System.Net.SNMP */
