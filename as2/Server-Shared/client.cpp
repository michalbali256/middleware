#include <iostream>
#include <string>

#include <omniORB4/CORBA.h>
#include "master.h"

using namespace std;


int main(int argc, char **argv) {
	try {
		if (argc <= 2) {
			cout << "Usage: client IOR" << endl;
			return 0;
		}

		CORBA::String_var s = CORBA::string_dup("balim");
		CORBA::LongLong key;
		if(argc == 3)
		{
			key = atoi(argv[2]);
		}

		
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		CORBA::Object_var server_base = orb->string_to_object(argv[1]);
		master::server_i_var server = master::server_i::_narrow(server_base);

		

		master::instance_i_ptr inst = server->connect(s, key);

		std::cout << s << "\n";
		std::cout << key << "\n";
		orb->destroy();

	} catch (const CORBA::SystemException &ex) {
		cerr << "HERE" << endl;
		cerr << ex._name() << endl;
		return 1;
	} catch (master::server_i::connection_e e) {
		cerr << e.cause << endl;

	}
	 catch (const exception &ex) {
		cerr << "HERE2" << endl;
		cerr << ex.what() << endl;
		return 1;
	}
}
