#include <iostream>

#include <omniORB4/CORBA.h>
#include "simple.h"

using namespace std;
using namespace example;

int main(int argc, char **argv) {
	try {
		if (argc != 2) {
			cout << "Usage: client IOR" << endl;
			return 0;
		}

		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		CORBA::Object_var simpleBase = orb->string_to_object(argv[1]);
		Simple_var simple = Simple::_narrow(simpleBase);

		simple->Display("Hello from omniORB client!");

		orb->destroy();

	} catch (const CORBA::SystemException &ex) {
		cerr << ex._name() << endl;
		return 1;
	} catch (const exception &ex) {
		cerr << ex.what() << endl;
		return 1;
	}
}
