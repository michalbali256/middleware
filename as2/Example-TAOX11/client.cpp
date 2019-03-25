#include <iostream>
#include <exception>

#include "simpleC.h"

int main(int argc, char **argv) {
	try {
		if (argc != 2) {
			std::cerr << "Usage: client IOR" << std::endl;
			return 1;
		}

		IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);

		IDL::traits<CORBA::Object>::ref_type simpleBase = orb->string_to_object(argv[1]);
		IDL::traits<example::Simple>::ref_type simple = IDL::traits<example::Simple>::narrow(simpleBase);

		simple->Display("Hello from TAO C++11 client !");

		orb->destroy();
	} catch (const CORBA::SystemException &ex) {
		std::cerr << ex << std::endl;
		return 1;
	} catch (const std::exception &ex){
		std::cerr << ex.what() << std::endl;
		return 1;
	}
}
