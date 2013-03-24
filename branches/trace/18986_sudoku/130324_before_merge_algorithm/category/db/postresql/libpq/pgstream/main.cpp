#include <iostream>
#include <Windows.h>
#include "pgstream.h"

#pragma comment(lib, "ws2_32.lib")

int winsock_init();
void repeat();

int main() {
	//winsock_init();
	unsigned int c = 0;
	while(1) {
		std::cout << ++c << ">\n";
		if(c%1000==0) Sleep(10000);
		repeat();
	}

	return 0;
}

int winsock_init()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

	return WSAStartup(wVersionRequested, &wsaData);
}

void repeat()
{
	pg_cnx cnx;
	std::string subject;
	try {
		cnx.connect("dbname=mail host=localhost port=5433");
		cnx.set_option("bind_variables", false);
		cnx.set_option("nested_transactions", true);
		cnx.set_option("prepare_statements", false);
		pg_stream s("SELECT IP_ADDRESS FROM DEVICE", cnx);
		while(!s.eos()) {
			char ip[48+1];
			s >> ip;
			std::cout << ip << std::endl;
		}
	}
	catch(pg_excpt p) {
		if (!p.query().empty())
			std::cerr << p.query() << "\n";
		std::cerr << p.errmsg();
		if (!p.errcode().empty()) {
			std::cerr << "(ERR=" << p.errcode() << ")" << std::endl;
		}
	}
}