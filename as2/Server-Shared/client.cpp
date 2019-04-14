#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <omniORB4/CORBA.h>
#include "master.h"

using namespace std;
using namespace std::chrono_literals;

int main(int argc, char **argv) {
	try {
		if (argc < 4) {
			cout << "Usage: client IOR peer key" << endl;
			return 0;
		}
		
		
		CORBA::String_var peer = CORBA::string_dup(argv[2]);
		CORBA::LongLong key;
		key = atoi(argv[3]);
		

		//corba initialization
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
		
		//get server variable from server
		CORBA::Object_var server_base = orb->string_to_object(argv[1]);
		master::server_i_var server = master::server_i::_narrow(server_base);
		
		//1. connect with the key and user name
		master::instance_i_var inst = server->connect(peer, key);
		
		std::cout << peer << "\n";
		std::cout << key << "\n";
		
		//2. wait until the instance is available
		inst->ready(true);
		
		while(!inst->idle())
			std::this_thread::sleep_for(1ms);
		
		std::cout << "Instance ready\n";
		
		//get octet sequence and index
		master::octet_sequence_t_var oct;
		
		master::count_t cnt;
		
		if(cnt._d() != master::vlong_long)
		{
			std::cout << "The value returned from instance was not of type long long.\n";
			inst->disconnect();
			orb->destroy();
		}
		
		cnt.long_long_value(key);
		inst->get_status(peer, cnt, oct);
		
		//get the index from the union count_t
		CORBA::ULongLong index;
		switch(cnt._d())
		{
			case master::switch_t::vshort :
				index = cnt.short_value();
				break;
			case master::switch_t::vlong :
				index = cnt.long_value();
				break;
		}
		
		//prepare the request
		master::request_t req;
		
		req.index = cnt;
		req.data <<= CORBA::Any::from_octet(oct[index]);
		//req.data.replace(CORBA::TypeCode::PR_octet_tc(), & oct[index]);
		
		//call the request
		auto ret = inst->request(req);
		
		std::cout << "Request returned: " << ret << "\n";
		
		inst->disconnect();
		orb->destroy();

	} catch (const CORBA::SystemException & ex) {
		cerr << ex._name() << endl;
		return 1;
	} catch (master::server_i::connection_e & e) {
		cerr << e.cause << endl;

	}
	catch(master::instance_i::protocol_e & e)
	{
		cerr << e.cause << endl;
	}
	 catch (const exception &ex) {
		cerr << ex.what() << endl;
		return 1;
	}
}
