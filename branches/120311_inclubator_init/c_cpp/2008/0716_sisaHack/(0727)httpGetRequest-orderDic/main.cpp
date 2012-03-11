#include "keygenModule.h"
#include <stdio.h>

void Usage()
{
	printf("Usage:\n");
	printf(" httpGetRequest [start String]\n");
	printf(" httpGetRequest [id] [start String]\n");
	printf(" httpGetRequest [id] [start String] [end String]\n");
}

int main(int argc, char* argv[])
{
	if(argc == 2) controlEntry2("nam17845",argv[1]);
	else if(argc == 3) controlEntry2(argv[1], argv[2]);
	else if(argc == 4) controlEntry3(argv[1], argv[2],argv[3]);
	else {
		//printf("%d\n", findIndexDictionary('9'));
		Usage();
		return 1;
	}

	//Display(3, Buffer);
	//DigitControl(4,6);
	return 0;
}