#include "httpModule.h"
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
	HTTPDown("10.15.1.102:9090", "POD_Center/login.do");
	return 0;
}