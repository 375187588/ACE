
#ifndef __LOGGING_ACCEPTOR_WFMO_H__
#define __LOGGING_ACCEPTOR_WFMO_H__

#include "Logging_Acceptor_Ex.h" 

class Logging_Acceptor_WFMO : public Logging_Acceptor_Ex
{
public:
  //typedef ACE_INET_Addr PEER_ADDR; 
  Logging_Acceptor_WFMO(ACE_Reactor *r = ACE_Reactor::instance()) : Logging_Acceptor_Ex(r) { }
  virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE); 

protected:
  virtual ~Logging_Acceptor_WFMO() {}
}; 

#endif 