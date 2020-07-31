#ifndef __REACTOR_LOGGING_SERVER_H__
#define __REACTOR_LOGGING_SERVER_H__

#include "ace/ACE.h" 
#include "ace/Reactor.h" 

#include "ace/Handle_Set.h" 
#include "ace/Hash_Map_Manager.h" 
#include "ace/Synch.h" 
//#include "Logging_Server.h" 
//#include "Logging_Handler.h" 

template <class ACCEPTOR>
class Reactor_Logging_Server : public ACCEPTOR
{
public:
  Reactor_Logging_Server(int argc, char *argv[], ACE_Reactor *reactor); 
}; 

template <class ACCEPTOR>
Reactor_Logging_Server<ACCEPTOR>::Reactor_Logging_Server(
  int argc, char *argv[], ACE_Reactor *reactor) : ACCEPTOR(reactor)
{
  u_short logger_port = argc > 1 ? atoi(argv[1]) : 0; 
  typename ACCEPTOR::PEER_ADDR server_addr;
  int result = 0; 

  if(logger_port != 0)
    result = server_addr.set(logger_port); 
  else 
    result = server_addr.set("ace_logger"); 

  if(result != -1)
    result = ACCEPTOR::open(server_addr); 

  if(result == -1)
    reactor->end_reactor_event_loop(); 
}

#endif 