#pragma once
#include "ace/Event_Handler.h" 
#include "ace/INET_Addr.h" 
#include "ace/Log_Record.h" 
#include "ace/Reactor.h" 
#include "ace/SOCK_Acceptor.h" 
#include "ace/SOCK_Stream.h" 
#include "EventAcceptor.h" 

class RealEventAcceptor : public EventAcceptor
{
public:
	//typedef ACE_INET_Addr PEER_ADDR; 
	RealEventAcceptor(ACE_Reactor *r = ACE_Reactor::instance()) : EventAcceptor(r) { }
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);

protected:
	virtual ~RealEventAcceptor() {}
};
