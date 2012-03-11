#include <Windows.h>

// Global variations
LPCTSTR g_name = TEXT("Billing{7D331537-A3FD-430a-94AD-F3386908D196}");

int main() {
	HANDLE hEvent = ::OpenEvent(EVENT_MODIFY_STATE, FALSE, g_name);
	if(NULL!=hEvent) {
		::SetEvent(hEvent);
	}

	return 0;
}