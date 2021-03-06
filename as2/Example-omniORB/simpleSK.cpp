// This file is generated by omniidl (C++ backend)- omniORB_4_2. Do not edit.

#include "simple.h"
#include <omniORB4/IOP_S.h>
#include <omniORB4/IOP_C.h>
#include <omniORB4/callDescriptor.h>
#include <omniORB4/callHandle.h>
#include <omniORB4/objTracker.h>


OMNI_USING_NAMESPACE(omni)

static const char* _0RL_library_version = omniORB_4_2;



example::Simple_ptr example::Simple_Helper::_nil() {
  return ::example::Simple::_nil();
}

::CORBA::Boolean example::Simple_Helper::is_nil(::example::Simple_ptr p) {
  return ::CORBA::is_nil(p);

}

void example::Simple_Helper::release(::example::Simple_ptr p) {
  ::CORBA::release(p);
}

void example::Simple_Helper::marshalObjRef(::example::Simple_ptr obj, cdrStream& s) {
  ::example::Simple::_marshalObjRef(obj, s);
}

example::Simple_ptr example::Simple_Helper::unmarshalObjRef(cdrStream& s) {
  return ::example::Simple::_unmarshalObjRef(s);
}

void example::Simple_Helper::duplicate(::example::Simple_ptr obj) {
  if (obj && !obj->_NP_is_nil())  omni::duplicateObjRef(obj);
}

example::Simple_ptr
example::Simple::_duplicate(::example::Simple_ptr obj)
{
  if (obj && !obj->_NP_is_nil())  omni::duplicateObjRef(obj);
  return obj;
}

example::Simple_ptr
example::Simple::_narrow(::CORBA::Object_ptr obj)
{
  if (!obj || obj->_NP_is_nil() || obj->_NP_is_pseudo()) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_realNarrow(_PD_repoId);
  return e ? e : _nil();
}


example::Simple_ptr
example::Simple::_unchecked_narrow(::CORBA::Object_ptr obj)
{
  if (!obj || obj->_NP_is_nil() || obj->_NP_is_pseudo()) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_uncheckedNarrow(_PD_repoId);
  return e ? e : _nil();
}

example::Simple_ptr
example::Simple::_nil()
{
#ifdef OMNI_UNLOADABLE_STUBS
  static _objref_Simple _the_nil_obj;
  return &_the_nil_obj;
#else
  static _objref_Simple* _the_nil_ptr = 0;
  if (!_the_nil_ptr) {
    omni::nilRefLock().lock();
    if (!_the_nil_ptr) {
      _the_nil_ptr = new _objref_Simple;
      registerNilCorbaObject(_the_nil_ptr);
    }
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
#endif
}

const char* example::Simple::_PD_repoId = "IDL:example/Simple:1.0";


example::_objref_Simple::~_objref_Simple() {
  
}


example::_objref_Simple::_objref_Simple(omniIOR* ior, omniIdentity* id) :
   omniObjRef(::example::Simple::_PD_repoId, ior, id, 1)
   
   
{
  _PR_setobj(this);
}

void*
example::_objref_Simple::_ptrToObjRef(const char* id)
{
  if (id == ::example::Simple::_PD_repoId)
    return (::example::Simple_ptr) this;
  
  if (id == ::CORBA::Object::_PD_repoId)
    return (::CORBA::Object_ptr) this;

  if (omni::strMatch(id, ::example::Simple::_PD_repoId))
    return (::example::Simple_ptr) this;
  
  if (omni::strMatch(id, ::CORBA::Object::_PD_repoId))
    return (::CORBA::Object_ptr) this;

  return 0;
}


//
// Code for example::Simple::Display

// Proxy call descriptor class. Mangled signature:
//  void_i_cstring
class _0RL_cd_bc8c40de0d5fbc39_00000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_bc8c40de0d5fbc39_00000000(LocalCallFn lcfn, const char* op_, size_t oplen, _CORBA_Boolean upcall=0)
    : omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  static const char* const _user_exns[];

  ::CORBA::String_var arg_0_;
  const char* arg_0;
};

void _0RL_cd_bc8c40de0d5fbc39_00000000::marshalArguments(cdrStream& _n)
{
  _n.marshalString(arg_0,0);

}

void _0RL_cd_bc8c40de0d5fbc39_00000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = _n.unmarshalString(0);
  arg_0 = arg_0_.in();

}

const char* const _0RL_cd_bc8c40de0d5fbc39_00000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_bc8c40de0d5fbc39_10000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_bc8c40de0d5fbc39_00000000* tcd = (_0RL_cd_bc8c40de0d5fbc39_00000000*)cd;
  example::_impl_Simple* impl = (example::_impl_Simple*) svnt->_ptrToInterface(example::Simple::_PD_repoId);
  impl->Display(tcd->arg_0);


}

void example::_objref_Simple::Display(const char* sText)
{
  _0RL_cd_bc8c40de0d5fbc39_00000000 _call_desc(_0RL_lcfn_bc8c40de0d5fbc39_10000000, "Display", 8);
  _call_desc.arg_0 = sText;

  _invoke(_call_desc);



}

example::_pof_Simple::~_pof_Simple() {}


omniObjRef*
example::_pof_Simple::newObjRef(omniIOR* ior, omniIdentity* id)
{
  return new ::example::_objref_Simple(ior, id);
}


::CORBA::Boolean
example::_pof_Simple::is_a(const char* id) const
{
  if (omni::ptrStrMatch(id, ::example::Simple::_PD_repoId))
    return 1;
  
  return 0;
}

const example::_pof_Simple _the_pof_example_mSimple;

example::_impl_Simple::~_impl_Simple() {}


::CORBA::Boolean
example::_impl_Simple::_dispatch(omniCallHandle& _handle)
{
  const char* op = _handle.operation_name();

  if (omni::strMatch(op, "Display")) {

    _0RL_cd_bc8c40de0d5fbc39_00000000 _call_desc(_0RL_lcfn_bc8c40de0d5fbc39_10000000, "Display", 8, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }


  return 0;
}

void*
example::_impl_Simple::_ptrToInterface(const char* id)
{
  if (id == ::example::Simple::_PD_repoId)
    return (::example::_impl_Simple*) this;
  
  if (id == ::CORBA::Object::_PD_repoId)
    return (void*) 1;

  if (omni::strMatch(id, ::example::Simple::_PD_repoId))
    return (::example::_impl_Simple*) this;
  
  if (omni::strMatch(id, ::CORBA::Object::_PD_repoId))
    return (void*) 1;
  return 0;
}

const char*
example::_impl_Simple::_mostDerivedRepoId()
{
  return ::example::Simple::_PD_repoId;
}

POA_example::Simple::~Simple() {}

