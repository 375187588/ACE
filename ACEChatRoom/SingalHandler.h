#pragma once
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Signal.h>

class SingalHandler : public ACE_Event_Handler
{
public:
	SingalHandler(ACE_Reactor* reactor = ACE_Reactor::instance());
	virtual ~SingalHandler();

	virtual int handle_signal(int signum, siginfo_t*, ucontext_t*);
};

