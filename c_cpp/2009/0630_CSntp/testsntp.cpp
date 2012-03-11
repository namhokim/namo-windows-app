#include "stdafx.h"
#include <math.h>
#include "sntp.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* /*envp[]*/)
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed"));
		return 1;
	}
	else
	{
		if (argc < 2)
		{
			_tprintf(_T("testsntp host [/s]\n"));
			_tprintf(_T("where host is the NTP server address and\n"));
			_tprintf(_T("/s means synchronize the time instead of just displaying the retrieved values\n"));
			return 1;
		}

		BOOL bSynchroniseClock = FALSE;
		for (int i=0; i<argc && !bSynchroniseClock; i++)
			bSynchroniseClock = (_tcsicmp(argv[i], _T("/s")) == 0);

		//Initialise the winsock stack
		WSADATA wsaData;
		BYTE wsMajorVersion = 1;
		BYTE wsMinorVersion = 1;
		WORD wVersionRequested = MAKEWORD(wsMinorVersion, wsMajorVersion);   
		if (WSAStartup(wVersionRequested, &wsaData) != 0) 
		{
			_tprintf(_T("Failed to load winsock stack\n"));
			return 1;
		}
		if (LOBYTE(wsaData.wVersion) != wsMajorVersion || HIBYTE(wsaData.wVersion) != wsMinorVersion)
		{
			_tprintf(_T("Winsock stack does not support version which this program requires\n"));
			return 1;
		}

		//Do the actual NTP Query
		CSNTPClient sntp;
		NtpServerResponse response;
		if (sntp.GetServerTime(argv[1], response))
		{
			_tprintf(_T("Time was successfully retreived from NTP server\n"));
 
			SYSTEMTIME st1 = response.m_OriginateTime;
			SYSTEMTIME st2 = response.m_ReceiveTime;
			SYSTEMTIME st3 = response.m_TransmitTime;
			SYSTEMTIME st4 = response.m_DestinationTime;

			_tprintf(_T("                            DD/MM/YYYY  HH:MM:SS.MS\n"));
			_tprintf(_T("Client Originate Date was   %02d/%02d/%04d, %02d:%02d:%02d.%03d\n"), st1.wDay, st1.wMonth, st1.wYear, st1.wHour, st1.wMinute, st1.wSecond, st1.wMilliseconds);
			_tprintf(_T("Server Receive Date was     %02d/%02d/%04d, %02d:%02d:%02d.%03d\n"), st2.wDay, st2.wMonth, st2.wYear, st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);
			_tprintf(_T("Server Transmit Date was    %02d/%02d/%04d, %02d:%02d:%02d.%03d\n"), st3.wDay, st3.wMonth, st3.wYear, st3.wHour, st3.wMinute, st3.wSecond, st3.wMilliseconds);
			_tprintf(_T("Client Destination Date was %02d/%02d/%04d, %02d:%02d:%02d.%03d\n"), st4.wDay, st4.wMonth, st4.wYear, st4.wHour, st4.wMinute, st4.wSecond, st4.wMilliseconds);
  		_tprintf(_T("Round trip delay was %f seconds\n"), response.m_RoundTripDelay);
  		_tprintf(_T("Local clock offset was %f seconds\n"), response.m_LocalClockOffset);

			if (bSynchroniseClock)
			{
				if (fabs(response.m_LocalClockOffset) < 3600)
				{
					CNtpTime newTime(CNtpTime::GetCurrentTime() + response.m_LocalClockOffset);
					if (sntp.SetClientTime(newTime))
						_tprintf(_T("time was successfully synchronised\n"));
					else
						_tprintf(_T("Failed to set the local time\n"));
				}
				else
					_tprintf(_T("time difference was greater was 1 hour, not synchronizing clock\n"));
      } 
		}
		else
			_tprintf(_T("Failed to retreive time from NTP server, GetLastError returned %d\n"), GetLastError());

		//Don't forget to release out use of the winsock stack
		WSACleanup();
	}

	return 0;
}





