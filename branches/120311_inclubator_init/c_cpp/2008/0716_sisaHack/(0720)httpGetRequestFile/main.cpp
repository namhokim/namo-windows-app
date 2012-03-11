#include "stdinModlue.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	if (2 == argc) stdinEntry(argv[1]);
	else stdinEntry("nam17845");
	return 0;
}