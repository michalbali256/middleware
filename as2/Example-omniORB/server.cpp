#include <iostream>
#include <exception>
#include <omniORB4/CORBA.h>
#include "simple.h"

using namespace std;
using namespace example;

class SimpleServant : public POA_example::Simple {
public:
	void Display(const char *text) throw (CORBA::SystemException) {
		cout << "omniORB server: " << text << endl;
	};
};

int main(int argc, char **argv) {
	try {
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		CORBA::Object_var rootPOABase = orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(rootPOABase);
		PortableServer::POAManager_var rootPOAManager = rootPOA->the_POAManager();

		PortableServer::Servant_var<SimpleServant> simpleServant = new SimpleServant;
		Simple_var simple = simpleServant->_this();

		CORBA::String_var simple_ior = orb->object_to_string(simple);
		cout << simple_ior << endl;

		rootPOAManager->activate();
		orb->run();

		orb->destroy();

	} catch (const CORBA::SystemException &ex) {
		cerr << ex._name() << endl;
		return 1;
	} catch (const exception &ex) {
		cerr << ex.what() << endl;
		return 1;
	}
}
