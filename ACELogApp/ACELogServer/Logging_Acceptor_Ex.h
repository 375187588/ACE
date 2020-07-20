
#ifndef __LOGGING_ACCEPTOR_EX_H__
#define __LOGGING_ACCEPTOR_EX_H__

#include "ace/Event_Handler.h" 
#include "ace/INET_Addr.h" 
#include "ace/Log_Record.h" 
#include "ace/Reactor.h" 
#include "ace/SOCK_Acceptor.h" 
#include "ace/SOCK_Stream.h" 
#include "Logging_Acceptor.h" 

class Logging_Acceptor_Ex : public Logging_Acceptor
{
public:
  //typedef ACE_INET_Addr PEER_ADDR; 
  Logging_Acceptor_Ex(ACE_Reactor *r = ACE_Reactor::instance()) : Logging_Acceptor(r) { }
  virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE); 

protected:
  virtual ~Logging_Acceptor_Ex() {}
}; 

#endif 