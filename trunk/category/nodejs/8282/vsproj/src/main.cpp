#include <stdio.h>		// for fprint, stdout, stderr
#include <stdlib.h>		// for EXIT_SUCCESS, EXIT_FAILURE
#include "whoConnect.h"

const int ArgNone	= 1;
const int ArgOne	= 2;
const int ArgTwo	= 3;

int StdoutSendMessage(unsigned int SessionId, const wchar_t* message);
int StdoutCurrentUsers();

int wmain(int argc, wchar_t *argv[])
{
	switch(argc) {
		case ArgOne:
			return wts::DisconnectSession(_wtoi(argv[1]));
		case ArgTwo:
			return StdoutSendMessage(_wtoi(argv[1]), argv[2]);
		case ArgNone:
		default:
			return StdoutCurrentUsers();
	}

	return EXIT_FAILURE;
};

int StdoutSendMessage(unsigned int SessionId, const wchar_t* message)
{
	wts::SendMessageToSesstionID(SessionId, message);
	return EXIT_SUCCESS;
}

int StdoutCurrentUsers()
{
	Json::Value out;
	int ret = wts::ShowCurrentConnectedUser(out);

	// out
	Json::FastWriter w;
	printf(w.write(out).c_str());
	
	return ret;
}