FlushIpNetTable 함수
 - 로컬 컴퓨터의 특정 인터페이스의 ARP 테이블로부터 모든 ARP 엔트리를 삭제한다.
 
 arp -d 명령으로 OS에 구현되어 있음
 > ARP -d inet_addr [if_addr]
 
 인터페이스의 인덱스 번호는 route명령으로 알 수 있다.
 > route print
===========================================================================
인터페이스 목록
 14...10 0b a9 bc 89 28 ......Intel(R) Centrino(R) Advanced-N 6205
 12...f0 de f1 d9 fd 15 ......Intel(R) 82579LM Gigabit Network Connection
  1...........................Software Loopback Interface 1
 13...00 00 00 00 00 00 00 e0 Microsoft ISATAP Adapter
 11...00 00 00 00 00 00 00 e0 Microsoft Teredo Tunneling Adapter
 15...00 00 00 00 00 00 00 e0 Microsoft ISATAP Adapter #2
===========================================================================
...
 △ 부분이 인터페이스 인덱스임
 
 
refs. http://msdn.microsoft.com/en-us/library/windows/desktop/aa365905(v=vs.85).aspx