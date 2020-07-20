#pragma once
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Log_Msg.h>
//Ω” ‹’ﬂ
class ParticipantAcceptor : public ACE_Event_Handler
{
public:
	ParticipantAcceptor(ACE_Reactor* reactor = ACE_Reactor::instance());
	virtual ~ParticipantAcceptor();

	int open(const ACE_INET_Addr& addr);
	virtual ACE_HANDLE get_handle() const;
	virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask closeMask);

private:
	ACE_SOCK_Acceptor accepter;
};

