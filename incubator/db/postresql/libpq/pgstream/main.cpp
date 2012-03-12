#include <iostream>
#include "pgstream.h"

int main() {
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

	return 0;
}