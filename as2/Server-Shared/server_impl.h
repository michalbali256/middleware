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
    
    CORBA::String_var exception_message_;
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
            exception_message_ = CORBA::string_dup("Ready not set.");
            master::instance_i::protocol_e(exception_message_)._raise();
        }
        if(!idle_)
        {
            exception_message_ = CORBA::string_dup("Instance not idle yet.");
            master::instance_i::protocol_e(exception_message_)._raise();
        }
        if(cnt._d() != master::vlong_long)
        {
            exception_message_ = CORBA::string_dup("count is not long long");
            master::instance_i::protocol_e(exception_message_)._raise();
        }
        if(strcmp(s_key, peer.inout()) != 0 )
        {
            std::string s = "s_key does is not the same that was returned. s_key" + std::string(s_key) + " " + std::string(peer.inout());
            exception_message_ = CORBA::string_dup(s.c_str());
            master::instance_i::protocol_e(exception_message_)._raise();
        }
        if(cnt.long_long_value() != key)
        {
            exception_message_ = CORBA::string_dup("cnt does is not the same as the key that was returned");
            master::instance_i::protocol_e(exception_message_)._raise();
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
        req.data >>= CORBA::Any::to_octet(o);
        
        if(o != sent_o_)
        {
            exception_message_ = CORBA::string_dup("octets do not match");
            master::instance_i::protocol_e(exception_message_)._raise();
        }
        if(shrt)
        {
            if(req.index.short_value() != sent_c_.short_value())
            {
                exception_message_ = CORBA::string_dup("indices do not match");
                master::instance_i::protocol_e(exception_message_)._raise();
            }
        }
        else
        {
            if(req.index.long_value() != sent_c_.long_value())
            {
                exception_message_ = CORBA::string_dup("indices do not match");
                master::instance_i::protocol_e(exception_message_)._raise();
            }
        }
        
		return true;
	}
	
    virtual void disconnect() override
	{
		v_.disconnect(hash);
	}
};