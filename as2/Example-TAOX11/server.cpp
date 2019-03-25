#include <iostream>
#include <exception>

#include "simpleS.h"

class SimpleServant: public CORBA::servant_traits<example::Simple>::base_type {
public:
	virtual void Display(const std::string &text) {
		std::cout << "TAO C++11 server: " << text << std::endl;
	}
};

int main(int argc, char **argv) {
	try {
		IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);

		IDL::traits<CORBA::Object>::ref_type rootPOABase = orb ->resolve_initial_references("RootPOA");
		IDL::traits<PortableServer::POA>::ref_type rootPOA = IDL::traits<PortableServer::POA>::narrow(rootPOABase);
		IDL::traits<PortableServer::POAManager>::ref_type rootPOAManager = rootPOA->the_POAManager();

		CORBA::servant_traits<example::Simple>::ref_type simpleServant = CORBA::make_reference<SimpleServant>();
		IDL::traits<example::Simple>::ref_type simple = simpleServant->_this();

		std::cout << orb ->object_to_string(simple) << std::endl;

		rootPOAManager->activate();
		orb->run();

		orb->destroy();
	} catch (const CORBA::SystemException &ex) {
		std::cerr << ex << std::endl;
		return 1;
	} catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
}
