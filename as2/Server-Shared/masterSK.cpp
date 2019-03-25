// This file is generated by omniidl (C++ backend)- omniORB_4_2. Do not edit.

#include "master.h"
#include <omniORB4/IOP_S.h>
#include <omniORB4/IOP_C.h>
#include <omniORB4/callDescriptor.h>
#include <omniORB4/callHandle.h>
#include <omniORB4/objTracker.h>


OMNI_USING_NAMESPACE(omni)

static const char* _0RL_library_version = omniORB_4_2;



_init_in_def_( const ::CORBA::ULong consts::MAX_LEN = 8U; )

void
master::count_t::operator>>= (cdrStream& _n) const
{
  _pd__d >>= _n;


  switch(_pd__d) {
    case vshort:
      _pd_short_value >>= _n;
      break;
    case vlong:
      _pd_long_value >>= _n;
      break;
    case vlong_long:
      _pd_long_long_value >>= _n;
      break;
    default:
      _pd_ulong_value >>= _n;
      break;

  }
}

void
master::count_t::operator<<= (cdrStream& _n)
{
  _release_member();
  
  (switch_t&)_pd__d <<= _n;

  switch(_pd__d) {
    case vshort:
      _pd__default = 0;
      (::CORBA::Short&)_pd_short_value <<= _n;
      break;
    case vlong:
      _pd__default = 0;
      (::CORBA::Long&)_pd_long_value <<= _n;
      break;
    case vlong_long:
      _pd__default = 0;
      (::CORBA::LongLong&)_pd_long_long_value <<= _n;
      break;
    default:
      _pd__default = 1;
      (::CORBA::ULong&)_pd_ulong_value <<= _n;
      break;

  }
  _pd__initialised = 1;
}

void
master::request_t::operator>>= (cdrStream &_n) const
{
  (const count_t&) index >>= _n;
  (const ::CORBA::Any&) data >>= _n;

}

void
master::request_t::operator<<= (cdrStream &_n)
{
  (count_t&)index <<= _n;
  (::CORBA::Any&)data <<= _n;

}

::CORBA::Exception::insertExceptionToAny master::server_i::connection_e::insertToAnyFn = 0;
::CORBA::Exception::insertExceptionToAnyNCP master::server_i::connection_e::insertToAnyFnNCP = 0;

master::server_i::connection_e::connection_e(const master::server_i::connection_e& _s) : ::CORBA::UserException(_s)
{
  cause = _s.cause;

}

master::server_i::connection_e::connection_e(const char* _cause)
{
  pd_insertToAnyFn    = master::server_i::connection_e::insertToAnyFn;
  pd_insertToAnyFnNCP = master::server_i::connection_e::insertToAnyFnNCP;
  cause = _cause;

}



master::server_i::connection_e& master::server_i::connection_e::operator=(const master::server_i::connection_e& _s)
{
  if (&_s != this) {
    ((::CORBA::UserException*) this)->operator=(_s);
    cause = _s.cause;

  }
  return *this;
}

master::server_i::connection_e::~connection_e() {}

void master::server_i::connection_e::_raise() const { throw *this; }

const char* master::server_i::connection_e::_PD_repoId = "IDL:master/server_i/connection_e:1.0";
const char* master::server_i::connection_e::_PD_typeId = "Exception/UserException/master::server_i::connection_e";

master::server_i::connection_e* master::server_i::connection_e::_downcast(::CORBA::Exception* _e) {
  return (connection_e*) _NP_is_a(_e, _PD_typeId);
}

const master::server_i::connection_e* master::server_i::connection_e::_downcast(const ::CORBA::Exception* _e) {
  return (const connection_e*) _NP_is_a(_e, _PD_typeId);
}

::CORBA::Exception* master::server_i::connection_e::_NP_duplicate() const {
  return new connection_e(*this);
}

const char* master::server_i::connection_e::_NP_typeId() const {
  return _PD_typeId;
}

const char* master::server_i::connection_e::_NP_repoId(int* _size) const {
  *_size = sizeof("IDL:master/server_i/connection_e:1.0");
  return _PD_repoId;
}
 
void master::server_i::connection_e::_NP_marshal(cdrStream& _s) const {
  *this >>= _s;
}

void
master::server_i::connection_e::operator>>= (cdrStream& _n) const
{
  _n.marshalString(cause,0);

}

void
master::server_i::connection_e::operator<<= (cdrStream& _n)
{
  cause = _n.unmarshalString(0);

}

master::server_i_ptr master::server_i_Helper::_nil() {
  return ::master::server_i::_nil();
}

::CORBA::Boolean master::server_i_Helper::is_nil(::master::server_i_ptr p) {
  return ::CORBA::is_nil(p);

}

void master::server_i_Helper::release(::master::server_i_ptr p) {
  ::CORBA::release(p);
}

void master::server_i_Helper::marshalObjRef(::master::server_i_ptr obj, cdrStream& s) {
  ::master::server_i::_marshalObjRef(obj, s);
}

master::server_i_ptr master::server_i_Helper::unmarshalObjRef(cdrStream& s) {
  return ::master::server_i::_unmarshalObjRef(s);
}

void master::server_i_Helper::duplicate(::master::server_i_ptr obj) {
  if (obj && !obj->_NP_is_nil())  omni::duplicateObjRef(obj);
}

master::server_i_ptr
master::server_i::_duplicate(::master::server_i_ptr obj)
{
  if (obj && !obj->_NP_is_nil())  omni::duplicateObjRef(obj);
  return obj;
}

master::server_i_ptr
master::server_i::_narrow(::CORBA::Object_ptr obj)
{
  if (!obj || obj->_NP_is_nil() || obj->_NP_is_pseudo()) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_realNarrow(_PD_repoId);
  return e ? e : _nil();
}


master::server_i_ptr
master::server_i::_unchecked_narrow(::CORBA::Object_ptr obj)
{
  if (!obj || obj->_NP_is_nil() || obj->_NP_is_pseudo()) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_uncheckedNarrow(_PD_repoId);
  return e ? e : _nil();
}

master::server_i_ptr
master::server_i::_nil()
{
#ifdef OMNI_UNLOADABLE_STUBS
  static _objref_server_i _the_nil_obj;
  return &_the_nil_obj;
#else
  static _objref_server_i* _the_nil_ptr = 0;
  if (!_the_nil_ptr) {
    omni::nilRefLock().lock();
    if (!_the_nil_ptr) {
      _the_nil_ptr = new _objref_server_i;
      registerNilCorbaObject(_the_nil_ptr);
    }
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
#endif
}

const char* master::server_i::_PD_repoId = "IDL:master/server_i:1.0";


master::_objref_server_i::~_objref_server_i() {
  
}


master::_objref_server_i::_objref_server_i(omniIOR* ior, omniIdentity* id) :
   omniObjRef(::master::server_i::_PD_repoId, ior, id, 1)
   
   
{
  _PR_setobj(this);
}

void*
master::_objref_server_i::_ptrToObjRef(const char* id)
{
  if (id == ::master::server_i::_PD_repoId)
    return (::master::server_i_ptr) this;
  
  if (id == ::CORBA::Object::_PD_repoId)
    return (::CORBA::Object_ptr) this;

  if (omni::strMatch(id, ::master::server_i::_PD_repoId))
    return (::master::server_i_ptr) this;
  
  if (omni::strMatch(id, ::CORBA::Object::_PD_repoId))
    return (::CORBA::Object_ptr) this;

  return 0;
}


//
// Code for master::server_i::ping

// Proxy call descriptor class. Mangled signature:
//  _cshort_i_cshort
class _0RL_cd_03d70e627809b364_00000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_03d70e627809b364_00000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  ::CORBA::Short arg_0;
  ::CORBA::Short result;
};

void _0RL_cd_03d70e627809b364_00000000::marshalArguments(cdrStream& _n)
{
  arg_0 >>= _n;

}

void _0RL_cd_03d70e627809b364_00000000::unmarshalArguments(cdrStream& _n)
{
  (::CORBA::Short&)arg_0 <<= _n;

}

void _0RL_cd_03d70e627809b364_00000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_03d70e627809b364_00000000::unmarshalReturnedValues(cdrStream& _n)
{
  (::CORBA::Short&)result <<= _n;

}

const char* const _0RL_cd_03d70e627809b364_00000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_10000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_03d70e627809b364_00000000* tcd = (_0RL_cd_03d70e627809b364_00000000*)cd;
  master::_impl_server_i* impl = (master::_impl_server_i*) svnt->_ptrToInterface(master::server_i::_PD_repoId);
  tcd->result = impl->ping(tcd->arg_0);


}

::CORBA::Short master::_objref_server_i::ping(::CORBA::Short val)
{
  _0RL_cd_03d70e627809b364_00000000 _call_desc(_0RL_lcfn_03d70e627809b364_10000000, "ping", 5);
  _call_desc.arg_0 = val;

  _invoke(_call_desc);
  return _call_desc.result;


}


//
// Code for master::server_i::connect

// Proxy call descriptor class. Mangled signature:
//  _cmaster_minstance__i_n_c8string_n_clonglong_e_cmaster_mserver__i_mconnection__e
class _0RL_cd_03d70e627809b364_20000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_03d70e627809b364_20000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 1, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  void userException(cdrStream&, _OMNI_NS(IOP_C)*, const char*);
  static const char* const _user_exns[];

  ::CORBA::String_var arg_0_;
  char** arg_0;
  ::CORBA::LongLong arg_1_;
  ::CORBA::LongLong* arg_1;
  master::instance_i_var result;
};

void _0RL_cd_03d70e627809b364_20000000::marshalArguments(cdrStream& _n)
{
  _n.marshalString(*arg_0,8);
  *arg_1 >>= _n;

}

void _0RL_cd_03d70e627809b364_20000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = _n.unmarshalString(8);
  arg_0 = &arg_0_.inout();
  (::CORBA::LongLong&)arg_1_ <<= _n;
  arg_1 = &arg_1_;

}

void _0RL_cd_03d70e627809b364_20000000::marshalReturnedValues(cdrStream& _n)
{
  master::instance_i_Helper::marshalObjRef(result,_n);
  _n.marshalString(*arg_0,8);
  *arg_1 >>= _n;

}

void _0RL_cd_03d70e627809b364_20000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = master::instance_i_Helper::unmarshalObjRef(_n);
  arg_0_ = *arg_0;
  *arg_0 = (char*) _CORBA_String_helper::empty_string;
  *arg_0 = _n.unmarshalString(8);
  (::CORBA::LongLong&)*arg_1 <<= _n;

}

const char* const _0RL_cd_03d70e627809b364_20000000::_user_exns[] = {
  master::server_i::connection_e::_PD_repoId
};

void _0RL_cd_03d70e627809b364_20000000::userException(cdrStream& s, _OMNI_NS(IOP_C)* iop_client, const char* repoId)
{
  if (omni::strMatch(repoId, master::server_i::connection_e::_PD_repoId)) {
    master::server_i::connection_e _ex;
    _ex <<= s;
    if (iop_client) iop_client->RequestCompleted();
    throw _ex;
  }


  else {
    if (iop_client) iop_client->RequestCompleted(1);
    OMNIORB_THROW(UNKNOWN,UNKNOWN_UserException,
                  (::CORBA::CompletionStatus)s.completion());
  }
}

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_30000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_03d70e627809b364_20000000* tcd = (_0RL_cd_03d70e627809b364_20000000*)cd;
  master::_impl_server_i* impl = (master::_impl_server_i*) svnt->_ptrToInterface(master::server_i::_PD_repoId);
#ifdef HAS_Cplusplus_catch_exception_by_base
  tcd->result = impl->connect(*tcd->arg_0, *tcd->arg_1);
#else
  if (!cd->is_upcall())
    tcd->result = impl->connect(*tcd->arg_0, *tcd->arg_1);
  else {
    try {
      tcd->result = impl->connect(*tcd->arg_0, *tcd->arg_1);
    }
    catch(master::server_i::connection_e& ex) {
      throw omniORB::StubUserException(ex._NP_duplicate());
    }


  }
#endif


}

master::instance_i_ptr master::_objref_server_i::connect(::CORBA::String_INOUT_arg peer, ::CORBA::LongLong& key)
{
  _0RL_cd_03d70e627809b364_20000000 _call_desc(_0RL_lcfn_03d70e627809b364_30000000, "connect", 8);
  _call_desc.arg_0 = &(char*&) peer;
  _call_desc.arg_1 = &(::CORBA::LongLong&) key;

  _invoke(_call_desc);
  return _call_desc.result._retn();


}

master::_pof_server_i::~_pof_server_i() {}


omniObjRef*
master::_pof_server_i::newObjRef(omniIOR* ior, omniIdentity* id)
{
  return new ::master::_objref_server_i(ior, id);
}


::CORBA::Boolean
master::_pof_server_i::is_a(const char* id) const
{
  if (omni::ptrStrMatch(id, ::master::server_i::_PD_repoId))
    return 1;
  
  return 0;
}

const master::_pof_server_i _the_pof_master_mserver__i;

master::_impl_server_i::~_impl_server_i() {}


::CORBA::Boolean
master::_impl_server_i::_dispatch(omniCallHandle& _handle)
{
  const char* op = _handle.operation_name();

  if (omni::strMatch(op, "ping")) {

    _0RL_cd_03d70e627809b364_00000000 _call_desc(_0RL_lcfn_03d70e627809b364_10000000, "ping", 5, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if (omni::strMatch(op, "connect")) {

    _0RL_cd_03d70e627809b364_20000000 _call_desc(_0RL_lcfn_03d70e627809b364_30000000, "connect", 8, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }


  return 0;
}

void*
master::_impl_server_i::_ptrToInterface(const char* id)
{
  if (id == ::master::server_i::_PD_repoId)
    return (::master::_impl_server_i*) this;
  
  if (id == ::CORBA::Object::_PD_repoId)
    return (void*) 1;

  if (omni::strMatch(id, ::master::server_i::_PD_repoId))
    return (::master::_impl_server_i*) this;
  
  if (omni::strMatch(id, ::CORBA::Object::_PD_repoId))
    return (void*) 1;
  return 0;
}

const char*
master::_impl_server_i::_mostDerivedRepoId()
{
  return ::master::server_i::_PD_repoId;
}

::CORBA::Exception::insertExceptionToAny master::instance_i::protocol_e::insertToAnyFn = 0;
::CORBA::Exception::insertExceptionToAnyNCP master::instance_i::protocol_e::insertToAnyFnNCP = 0;

master::instance_i::protocol_e::protocol_e(const master::instance_i::protocol_e& _s) : ::CORBA::UserException(_s)
{
  cause = _s.cause;

}

master::instance_i::protocol_e::protocol_e(const char* _cause)
{
  pd_insertToAnyFn    = master::instance_i::protocol_e::insertToAnyFn;
  pd_insertToAnyFnNCP = master::instance_i::protocol_e::insertToAnyFnNCP;
  cause = _cause;

}



master::instance_i::protocol_e& master::instance_i::protocol_e::operator=(const master::instance_i::protocol_e& _s)
{
  if (&_s != this) {
    ((::CORBA::UserException*) this)->operator=(_s);
    cause = _s.cause;

  }
  return *this;
}

master::instance_i::protocol_e::~protocol_e() {}

void master::instance_i::protocol_e::_raise() const { throw *this; }

const char* master::instance_i::protocol_e::_PD_repoId = "IDL:master/instance_i/protocol_e:1.0";
const char* master::instance_i::protocol_e::_PD_typeId = "Exception/UserException/master::instance_i::protocol_e";

master::instance_i::protocol_e* master::instance_i::protocol_e::_downcast(::CORBA::Exception* _e) {
  return (protocol_e*) _NP_is_a(_e, _PD_typeId);
}

const master::instance_i::protocol_e* master::instance_i::protocol_e::_downcast(const ::CORBA::Exception* _e) {
  return (const protocol_e*) _NP_is_a(_e, _PD_typeId);
}

::CORBA::Exception* master::instance_i::protocol_e::_NP_duplicate() const {
  return new protocol_e(*this);
}

const char* master::instance_i::protocol_e::_NP_typeId() const {
  return _PD_typeId;
}

const char* master::instance_i::protocol_e::_NP_repoId(int* _size) const {
  *_size = sizeof("IDL:master/instance_i/protocol_e:1.0");
  return _PD_repoId;
}
 
void master::instance_i::protocol_e::_NP_marshal(cdrStream& _s) const {
  *this >>= _s;
}

void
master::instance_i::protocol_e::operator>>= (cdrStream& _n) const
{
  _n.marshalString(cause,0);

}

void
master::instance_i::protocol_e::operator<<= (cdrStream& _n)
{
  cause = _n.unmarshalString(0);

}

master::instance_i_ptr master::instance_i_Helper::_nil() {
  return ::master::instance_i::_nil();
}

::CORBA::Boolean master::instance_i_Helper::is_nil(::master::instance_i_ptr p) {
  return ::CORBA::is_nil(p);

}

void master::instance_i_Helper::release(::master::instance_i_ptr p) {
  ::CORBA::release(p);
}

void master::instance_i_Helper::marshalObjRef(::master::instance_i_ptr obj, cdrStream& s) {
  ::master::instance_i::_marshalObjRef(obj, s);
}

master::instance_i_ptr master::instance_i_Helper::unmarshalObjRef(cdrStream& s) {
  return ::master::instance_i::_unmarshalObjRef(s);
}

void master::instance_i_Helper::duplicate(::master::instance_i_ptr obj) {
  if (obj && !obj->_NP_is_nil())  omni::duplicateObjRef(obj);
}

master::instance_i_ptr
master::instance_i::_duplicate(::master::instance_i_ptr obj)
{
  if (obj && !obj->_NP_is_nil())  omni::duplicateObjRef(obj);
  return obj;
}

master::instance_i_ptr
master::instance_i::_narrow(::CORBA::Object_ptr obj)
{
  if (!obj || obj->_NP_is_nil() || obj->_NP_is_pseudo()) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_realNarrow(_PD_repoId);
  return e ? e : _nil();
}


master::instance_i_ptr
master::instance_i::_unchecked_narrow(::CORBA::Object_ptr obj)
{
  if (!obj || obj->_NP_is_nil() || obj->_NP_is_pseudo()) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_uncheckedNarrow(_PD_repoId);
  return e ? e : _nil();
}

master::instance_i_ptr
master::instance_i::_nil()
{
#ifdef OMNI_UNLOADABLE_STUBS
  static _objref_instance_i _the_nil_obj;
  return &_the_nil_obj;
#else
  static _objref_instance_i* _the_nil_ptr = 0;
  if (!_the_nil_ptr) {
    omni::nilRefLock().lock();
    if (!_the_nil_ptr) {
      _the_nil_ptr = new _objref_instance_i;
      registerNilCorbaObject(_the_nil_ptr);
    }
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
#endif
}

const char* master::instance_i::_PD_repoId = "IDL:master/instance_i:1.0";


master::_objref_instance_i::~_objref_instance_i() {
  
}


master::_objref_instance_i::_objref_instance_i(omniIOR* ior, omniIdentity* id) :
   omniObjRef(::master::instance_i::_PD_repoId, ior, id, 1)
   
   
{
  _PR_setobj(this);
}

void*
master::_objref_instance_i::_ptrToObjRef(const char* id)
{
  if (id == ::master::instance_i::_PD_repoId)
    return (::master::instance_i_ptr) this;
  
  if (id == ::CORBA::Object::_PD_repoId)
    return (::CORBA::Object_ptr) this;

  if (omni::strMatch(id, ::master::instance_i::_PD_repoId))
    return (::master::instance_i_ptr) this;
  
  if (omni::strMatch(id, ::CORBA::Object::_PD_repoId))
    return (::CORBA::Object_ptr) this;

  return 0;
}


//
// Code for master::instance_i::_get_idle

// Proxy call descriptor class. Mangled signature:
//  _cboolean
class _0RL_cd_03d70e627809b364_40000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_03d70e627809b364_40000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  ::CORBA::Boolean result;
};

void _0RL_cd_03d70e627809b364_40000000::marshalReturnedValues(cdrStream& _n)
{
  _n.marshalBoolean(result);

}

void _0RL_cd_03d70e627809b364_40000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = _n.unmarshalBoolean();

}

const char* const _0RL_cd_03d70e627809b364_40000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_50000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_03d70e627809b364_40000000* tcd = (_0RL_cd_03d70e627809b364_40000000*)cd;
  master::_impl_instance_i* impl = (master::_impl_instance_i*) svnt->_ptrToInterface(master::instance_i::_PD_repoId);
  tcd->result = impl->idle();


}

::CORBA::Boolean master::_objref_instance_i::idle()
{
  _0RL_cd_03d70e627809b364_40000000 _call_desc(_0RL_lcfn_03d70e627809b364_50000000, "_get_idle", 10);


  _invoke(_call_desc);
  return _call_desc.result;


}


//
// Code for master::instance_i::_get_ready

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_60000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_03d70e627809b364_40000000* tcd = (_0RL_cd_03d70e627809b364_40000000*)cd;
  master::_impl_instance_i* impl = (master::_impl_instance_i*) svnt->_ptrToInterface(master::instance_i::_PD_repoId);
  tcd->result = impl->ready();


}

::CORBA::Boolean master::_objref_instance_i::ready()
{
  _0RL_cd_03d70e627809b364_40000000 _call_desc(_0RL_lcfn_03d70e627809b364_60000000, "_get_ready", 11);


  _invoke(_call_desc);
  return _call_desc.result;


}


//
// Code for master::instance_i::_set_ready

// Proxy call descriptor class. Mangled signature:
//  void_i_cboolean
class _0RL_cd_03d70e627809b364_70000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_03d70e627809b364_70000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  static const char* const _user_exns[];

  ::CORBA::Boolean arg_0;
};

void _0RL_cd_03d70e627809b364_70000000::marshalArguments(cdrStream& _n)
{
  _n.marshalBoolean(arg_0);

}

void _0RL_cd_03d70e627809b364_70000000::unmarshalArguments(cdrStream& _n)
{
  arg_0 = _n.unmarshalBoolean();

}

const char* const _0RL_cd_03d70e627809b364_70000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_80000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_03d70e627809b364_70000000* tcd = (_0RL_cd_03d70e627809b364_70000000*)cd;
  master::_impl_instance_i* impl = (master::_impl_instance_i*) svnt->_ptrToInterface(master::instance_i::_PD_repoId);
  impl->ready(tcd->arg_0);


}

void master::_objref_instance_i::ready(::CORBA::Boolean _v)
{
  _0RL_cd_03d70e627809b364_70000000 _call_desc(_0RL_lcfn_03d70e627809b364_80000000, "_set_ready", 11);
  _call_desc.arg_0 = _v;

  _invoke(_call_desc);



}


//
// Code for master::instance_i::get_status

// Proxy call descriptor class. Mangled signature:
//  void_i_cstring_n_cmaster_mcount__t_o_cmaster_moctet__sequence__t_e_cmaster_minstance__i_mprotocol__e
class _0RL_cd_03d70e627809b364_90000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_03d70e627809b364_90000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 1, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  void userException(cdrStream&, _OMNI_NS(IOP_C)*, const char*);
  static const char* const _user_exns[];

  ::CORBA::String_var arg_0_;
  const char* arg_0;
  master::count_t arg_1_;
  master::count_t* arg_1;
  master::octet_sequence_t_var arg_2;
};

void _0RL_cd_03d70e627809b364_90000000::marshalArguments(cdrStream& _n)
{
  _n.marshalString(arg_0,0);
  (const master::count_t&) *arg_1 >>= _n;

}

void _0RL_cd_03d70e627809b364_90000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = _n.unmarshalString(0);
  arg_0 = arg_0_.in();
  (master::count_t&)arg_1_ <<= _n;
  arg_1 = &arg_1_;

}

void _0RL_cd_03d70e627809b364_90000000::marshalReturnedValues(cdrStream& _n)
{
  (const master::count_t&) *arg_1 >>= _n;
  (const master::octet_sequence_t&) arg_2 >>= _n;

}

void _0RL_cd_03d70e627809b364_90000000::unmarshalReturnedValues(cdrStream& _n)
{
  (master::count_t&)*arg_1 <<= _n;
  arg_2 = new master::octet_sequence_t;
  (master::octet_sequence_t&)arg_2 <<= _n;

}

const char* const _0RL_cd_03d70e627809b364_90000000::_user_exns[] = {
  master::instance_i::protocol_e::_PD_repoId
};

void _0RL_cd_03d70e627809b364_90000000::userException(cdrStream& s, _OMNI_NS(IOP_C)* iop_client, const char* repoId)
{
  if (omni::strMatch(repoId, master::instance_i::protocol_e::_PD_repoId)) {
    master::instance_i::protocol_e _ex;
    _ex <<= s;
    if (iop_client) iop_client->RequestCompleted();
    throw _ex;
  }


  else {
    if (iop_client) iop_client->RequestCompleted(1);
    OMNIORB_THROW(UNKNOWN,UNKNOWN_UserException,
                  (::CORBA::CompletionStatus)s.completion());
  }
}

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_a0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_03d70e627809b364_90000000* tcd = (_0RL_cd_03d70e627809b364_90000000*)cd;
  master::_impl_instance_i* impl = (master::_impl_instance_i*) svnt->_ptrToInterface(master::instance_i::_PD_repoId);
#ifdef HAS_Cplusplus_catch_exception_by_base
  impl->get_status(tcd->arg_0, *tcd->arg_1, tcd->arg_2.out());
#else
  if (!cd->is_upcall())
    impl->get_status(tcd->arg_0, *tcd->arg_1, tcd->arg_2.out());
  else {
    try {
      impl->get_status(tcd->arg_0, *tcd->arg_1, tcd->arg_2.out());
    }
    catch(master::instance_i::protocol_e& ex) {
      throw omniORB::StubUserException(ex._NP_duplicate());
    }


  }
#endif


}

void master::_objref_instance_i::get_status(const char* s_key, ::master::count_t& cnt, ::master::octet_sequence_t_out status)
{
  _0RL_cd_03d70e627809b364_90000000 _call_desc(_0RL_lcfn_03d70e627809b364_a0000000, "get_status", 11);
  _call_desc.arg_0 = s_key;
  _call_desc.arg_1 = &(::master::count_t&) cnt;

  _invoke(_call_desc);
  status = _call_desc.arg_2._retn();


}


//
// Code for master::instance_i::request

// Proxy call descriptor class. Mangled signature:
//  _cboolean_i_cmaster_mrequest__t_e_cmaster_minstance__i_mprotocol__e
class _0RL_cd_03d70e627809b364_b0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_03d70e627809b364_b0000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 1, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  void userException(cdrStream&, _OMNI_NS(IOP_C)*, const char*);
  static const char* const _user_exns[];

  master::request_t_var arg_0_;
  const master::request_t* arg_0;
  ::CORBA::Boolean result;
};

void _0RL_cd_03d70e627809b364_b0000000::marshalArguments(cdrStream& _n)
{
  (const master::request_t&) *arg_0 >>= _n;

}

void _0RL_cd_03d70e627809b364_b0000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = new master::request_t;
  (master::request_t&)arg_0_ <<= _n;
  arg_0 = &arg_0_.in();

}

void _0RL_cd_03d70e627809b364_b0000000::marshalReturnedValues(cdrStream& _n)
{
  _n.marshalBoolean(result);

}

void _0RL_cd_03d70e627809b364_b0000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = _n.unmarshalBoolean();

}

const char* const _0RL_cd_03d70e627809b364_b0000000::_user_exns[] = {
  master::instance_i::protocol_e::_PD_repoId
};

void _0RL_cd_03d70e627809b364_b0000000::userException(cdrStream& s, _OMNI_NS(IOP_C)* iop_client, const char* repoId)
{
  if (omni::strMatch(repoId, master::instance_i::protocol_e::_PD_repoId)) {
    master::instance_i::protocol_e _ex;
    _ex <<= s;
    if (iop_client) iop_client->RequestCompleted();
    throw _ex;
  }


  else {
    if (iop_client) iop_client->RequestCompleted(1);
    OMNIORB_THROW(UNKNOWN,UNKNOWN_UserException,
                  (::CORBA::CompletionStatus)s.completion());
  }
}

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_c0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_03d70e627809b364_b0000000* tcd = (_0RL_cd_03d70e627809b364_b0000000*)cd;
  master::_impl_instance_i* impl = (master::_impl_instance_i*) svnt->_ptrToInterface(master::instance_i::_PD_repoId);
#ifdef HAS_Cplusplus_catch_exception_by_base
  tcd->result = impl->request(*tcd->arg_0);
#else
  if (!cd->is_upcall())
    tcd->result = impl->request(*tcd->arg_0);
  else {
    try {
      tcd->result = impl->request(*tcd->arg_0);
    }
    catch(master::instance_i::protocol_e& ex) {
      throw omniORB::StubUserException(ex._NP_duplicate());
    }


  }
#endif


}

::CORBA::Boolean master::_objref_instance_i::request(const ::master::request_t& req)
{
  _0RL_cd_03d70e627809b364_b0000000 _call_desc(_0RL_lcfn_03d70e627809b364_c0000000, "request", 8);
  _call_desc.arg_0 = &(::master::request_t&) req;

  _invoke(_call_desc);
  return _call_desc.result;


}


//
// Code for master::instance_i::disconnect

// Proxy call descriptor class. Mangled signature:
//  _wvoid
class _0RL_cd_03d70e627809b364_d0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_03d70e627809b364_d0000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 1, _user_exns, 0, upcall)
  {
    
  }
  
  
    
  
  static const char* const _user_exns[];

  
};

const char* const _0RL_cd_03d70e627809b364_d0000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_03d70e627809b364_e0000000(omniCallDescriptor*, omniServant* svnt)
{
  
  master::_impl_instance_i* impl = (master::_impl_instance_i*) svnt->_ptrToInterface(master::instance_i::_PD_repoId);
  impl->disconnect();


}

void master::_objref_instance_i::disconnect()
{
  _0RL_cd_03d70e627809b364_d0000000 _call_desc(_0RL_lcfn_03d70e627809b364_e0000000, "disconnect", 11);


  _invoke(_call_desc);



}

master::_pof_instance_i::~_pof_instance_i() {}


omniObjRef*
master::_pof_instance_i::newObjRef(omniIOR* ior, omniIdentity* id)
{
  return new ::master::_objref_instance_i(ior, id);
}


::CORBA::Boolean
master::_pof_instance_i::is_a(const char* id) const
{
  if (omni::ptrStrMatch(id, ::master::instance_i::_PD_repoId))
    return 1;
  
  return 0;
}

const master::_pof_instance_i _the_pof_master_minstance__i;

master::_impl_instance_i::~_impl_instance_i() {}


::CORBA::Boolean
master::_impl_instance_i::_dispatch(omniCallHandle& _handle)
{
  const char* op = _handle.operation_name();

  if (omni::strMatch(op, "_get_idle")) {

    _0RL_cd_03d70e627809b364_40000000 _call_desc(_0RL_lcfn_03d70e627809b364_50000000, "_get_idle", 10, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if (omni::strMatch(op, "_get_ready")) {

    _0RL_cd_03d70e627809b364_40000000 _call_desc(_0RL_lcfn_03d70e627809b364_60000000, "_get_ready", 11, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if (omni::strMatch(op, "_set_ready")) {

    _0RL_cd_03d70e627809b364_70000000 _call_desc(_0RL_lcfn_03d70e627809b364_80000000, "_set_ready", 11, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if (omni::strMatch(op, "get_status")) {

    _0RL_cd_03d70e627809b364_90000000 _call_desc(_0RL_lcfn_03d70e627809b364_a0000000, "get_status", 11, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if (omni::strMatch(op, "request")) {

    _0RL_cd_03d70e627809b364_b0000000 _call_desc(_0RL_lcfn_03d70e627809b364_c0000000, "request", 8, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if (omni::strMatch(op, "disconnect")) {

    _0RL_cd_03d70e627809b364_d0000000 _call_desc(_0RL_lcfn_03d70e627809b364_e0000000, "disconnect", 11, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }


  return 0;
}

void*
master::_impl_instance_i::_ptrToInterface(const char* id)
{
  if (id == ::master::instance_i::_PD_repoId)
    return (::master::_impl_instance_i*) this;
  
  if (id == ::CORBA::Object::_PD_repoId)
    return (void*) 1;

  if (omni::strMatch(id, ::master::instance_i::_PD_repoId))
    return (::master::_impl_instance_i*) this;
  
  if (omni::strMatch(id, ::CORBA::Object::_PD_repoId))
    return (void*) 1;
  return 0;
}

const char*
master::_impl_instance_i::_mostDerivedRepoId()
{
  return ::master::instance_i::_PD_repoId;
}

POA_master::server_i::~server_i() {}

POA_master::instance_i::~instance_i() {}

