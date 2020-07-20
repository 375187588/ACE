#include "SingalHandler.h"
#include <ace/Log_Msg.h>
#include "ChatRoom.h"

SingalHandler::SingalHandler(ACE_Reactor* reactor)
{
	this->reactor(reactor);
	ACE_Sig_Set signals;
	signals.fill_set();

	this->reactor()->register_handler(signals, this);
}

SingalHandler::~SingalHandler()
{

}

int SingalHandler::handle_signal(int signum, siginfo_t*, ucontext_t*)
{
	switch (signum)
	{
	case SIGINT:
		ACE_DEBUG((LM_DEBUG, "signal SIGINT,bug not be terminated!\n"));
		break;
	case SIGUSR1:
		ACE_DEBUG((LM_DEBUG, "signal SIGUSR1,broadcast greeting...!\n"));
		ChatRoom::instance()->forwardMsg("hello every on!\n");
		break;
	case SIGBREAK:
		ACE_DEBUG((LM_DEBUG, "signal SIGUSR2,shutdown chat room...\n"));
		this->reactor()->end_reactor_event_loop();
		break;
	default:
		break;
	}
	return 0;
}