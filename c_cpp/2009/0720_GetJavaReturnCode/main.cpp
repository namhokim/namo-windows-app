#include <cstdlib>	// for system()
#include <io.h>		// for dup2()
#include <iostream>	// for cout, enld
#include "SomeClassExitCode.h"	// ���� ���ǵ� �ڵ尪

int main(int argc, char *argv[])
{
	using namespace std;
	
	errno_t err;
	// standard out to file
	FILE * fo;
	if( (err=fopen_s(&fo, "stdout.txt", "a+")) != 0) {
		cout << "stdout file redirection error\n";
		exit(CppAbnormalExit);
	}
	else {
		fseek(fo, SEEK_END, 0);
		_dup2( _fileno(fo), _fileno(stdout) );
	}
	
	// standard error to file
	FILE * fe;
	if( (err=fopen_s(&fe, "stderr.txt", "a+")) != 0) {
		cout << "stdout file redirection error\n";
		exit(CppAbnormalExit);
	}
	else {
		fseek(fe, SEEK_END, 0);
		_dup2( _fileno(fe), _fileno(stderr) );
	}
	
	// RUN
	int exitCode;
	
	if (argc == 1) {
		exitCode = system("java 2SomeClass");
	}
	else {
		exitCode = system("java 1SomeClass withError");
	}
	cout << "Call result : " << exitCode << endl;
	if(exitCode == NormalExit) cout << "���������� ����\r\n";
	else cout << "�������� ����\r\n";
	
	fclose(fo);
	fclose(fe);
	
	return NormalExit;
}