// crt_getpid.c
// This program uses _getpid to obtain
// the process ID and then prints the ID.
// refs. http://msdn.microsoft.com/en-us/library/t2y34y40(v=vs.90).aspx


#include <stdio.h>		// for printf
#include <process.h>	// for _getpid, system

int main( void )
{
	// If run from command line, shows different ID for 
	// command line than for operating system shell.

	printf( "Process id: %d\n", _getpid() );
	system( "pause" );
}