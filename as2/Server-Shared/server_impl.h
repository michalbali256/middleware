#include <omniORB4/CORBA.h>
#include "master.h"

#include <cstring>

class disconnect_provider
{
    public:
    virtual void disconnect(size_t hash) = 0;
};

class instance_impl : public POA_master::instance_i
{
    size_t idle_counter_ = 0;
    bool ready_ = false;
    bool idle_ = false;
    
    master::count_t sent_c_;
    CORBA::Octet sent_o_;
    bool shrt = false;
    disconnect_provider & v_;
public:
    
    CORBA::String_var peer;
    CORBA::LongLong key;
    size_t hash;
    
    instance_impl(disconnect_provider & p) :v_(p) {} 
    
	virtual CORBA::Boolean idle() override
	{
        if (idle_)
            return true;
        ++idle_counter_;
        if(idle_counter_ < 3000)
            return false;
        else
        {
            idle_ = true;
            return true;
        }
	}
	
    virtual CORBA::Boolean ready() override
	{
		return ready_;
	}
	
    virtual void ready(CORBA::Boolean _v) override
    {
        ready_ = _v;
	}
	
    virtual void get_status(const char* s_key, master::count_t& cnt, master::octet_sequence_t_out status) override
	{
        if(!ready_)
        {
            master::instance_i::protocol_e("Ready not set.")._raise();
        }
        if(!idle_)
        {
            master::instance_i::protocol_e("Instance not idle yet.")._raise();
        }
        
        if(cnt._d() != master::vlong_long)
        {
            master::instance_i::protocol_e("Count is not long long")._raise();
        }
        if(strcmp(s_key, peer.inout()) != 0 )
        {
            master::instance_i::protocol_e("s_key does is not the same that was returned.")._raise();
        }
        if(cnt.long_long_value() != key)
        {
            master::instance_i::protocol_e("cnt does is not the same as the key that was returned")._raise();
        }
        
        
        status = new master::octet_sequence_t;
        status->length(10);
        for(size_t i = 0; i < 10; ++i)
			status[i] = (char)( random() % 255);
        
        int r = rand() % 10;
        
        if(rand()%2 == 0)
        {
            cnt.short_value(r);
            shrt = true;
        }
        else
        {
            cnt.long_value(r);
        }
        
        sent_c_ = cnt;
        sent_o_ = status[r];
	}
	
    virtual CORBA::Boolean request(const master::request_t& req) override
	{
        CORBA::Octet o;
        if(!req.data >>= CORBA::Any::to_octet(o))
            master::instance_i::protocol_e("Octet extraction from o was not successful.")
        
        if(o != sent_o_)
        {
            master::instance_i::protocol_e("Octets do not match")._raise();
        }
        if(shrt)
        {
            if(req.index._d() != master::vshort)
                master::instance_i::protocol_e("Request index type does not match")._raise();
            
            if(req.index.short_value() != sent_c_.short_value())
            {
                master::instance_i::protocol_e("Indices do not match")._raise();
            }
        }
        else
        {
            if(req.index._d() != master::vlong)
                master::instance_i::protocol_e("Request index type does not match")._raise();
            
            if(req.index.long_value() != sent_c_.long_value())
            {
                master::instance_i::protocol_e("Indices do not match")._raise();
            }
        }
        
		return true;
	}
	
    virtual void disconnect() override
	{
		v_.disconnect(hash);
	}
};