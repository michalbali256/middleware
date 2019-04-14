#include <exception>
#include <memory>
#include <omniORB4/CORBA.h>
#include "master.h"
#include "server_impl.h"

#include <iostream>
#include <map>
#include <mutex>
using namespace std;



class server_servant : public POA_master::server_i, public virtual disconnect_provider {
public:
	
	virtual void disconnect (size_t hash)
	{
		std::lock_guard<std::mutex> lock(inst_impls_mutex);
		inst_impls.erase(hash);
	}
	
	virtual CORBA::Short ping(CORBA::Short val)
	{
		return val;
	}
	
    virtual master::instance_i_ptr connect(char*& peer, CORBA::LongLong& key)
	{
		size_t hash = std::hash<std::string>()(peer);
		size_t k = hash % 100000000;
		
		if(k != key)
		{
			exception_text = "Wrong key, it should be " + std::to_string(k);
			master::server_i::connection_e(exception_text.c_str())._raise();
		}
		
		//the random key that will be later enforced by the instance
		key = random() % 100000000;
		
		auto inst = new instance_impl(*this);
		
		inst->hash = hash;
		inst->key = key;
		
		//create random 8-character string
		inst->peer = CORBA::string_dup("someeigh");
		for(size_t i = 0; i < 8; ++i)
			inst->peer[i] = (char)( random() % 26 + 65);
		
		//return the string
		CORBA::string_free(peer);
		peer = CORBA::string_dup(inst->peer);
		
		//add the newly created instance to a map
		std::lock_guard<std::mutex> lock(inst_impls_mutex);
		inst_impls[hash] = master::instance_i::_duplicate(inst->_this());
		
		return inst_impls[hash];
	}
private:
	std::string exception_text;
	
	std::map<size_t, master::instance_i_var> inst_impls;
	
	std::mutex inst_impls_mutex;
};

int main(int argc, char **argv) {
	
	int seed = time(NULL);
    srand(seed);
	
	try {
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		CORBA::Object_var rootPOABase = orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(rootPOABase);
		PortableServer::POAManager_var rootPOAManager = rootPOA->the_POAManager();

		PortableServer::Servant_var<server_servant> server_serv = new server_servant;
		master::server_i_var server = server_serv->_this();

		CORBA::String_var ior = orb->object_to_string(server);
		cout << ior << endl;

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
