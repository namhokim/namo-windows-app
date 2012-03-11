#include <Windows.h>
#include <cstdio>

int main() {
	ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
	BOOL res = ::(L"C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);
	printf("Result : %d\nFree Bytes avlilable: %u%u\nTotal Number of Bytes : %u%u\nTotal Number of free bytes : %u%u\n",
		res, freeBytesAvailable.HighPart, freeBytesAvailable.LowPart,
		totalNumberOfBytes.HighPart, totalNumberOfBytes.LowPart,
		totalNumberOfFreeBytes.HighPart, totalNumberOfFreeBytes.LowPart);
}