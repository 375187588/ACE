#include "ParticipantAcceptor.h"
#include "Participant.h"
#include "ChatRoom.h"

ParticipantAcceptor::ParticipantAcceptor(ACE_Reactor* reactor)
{
	this->reactor(reactor);
}
ParticipantAcceptor::~ParticipantAcceptor()
{
	handle_close(ACE_INVALID_HANDLE, 0);
}

int ParticipantAcceptor::open(const ACE_INET_Addr& addr)
{
	//1 地址可重用
	if (accepter.open(addr, 1) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR,"%p\n","accept.open"),-1);
	}
	return reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
}
ACE_HANDLE ParticipantAcceptor::get_handle() const
{
	return accepter.get_handle();
}
int ParticipantAcceptor::handle_input(ACE_HANDLE h)
{
	//创建一个参与者
	Participant* user = new Participant(reactor());

	if (accepter.accept(user->socket()) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "accept.accept"), -1);
	}
	if (user->open() == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "accept.open"), -1);
		user->handle_close(ACE_INVALID_HANDLE,0);
	}
	else
	{
		ChatRoom::instance()->join(user);
	}

	return 0;
}
int ParticipantAcceptor::handle_close(ACE_HANDLE h, ACE_Reactor_Mask closeMask)
{
	if (get_handle() != ACE_INVALID_HANDLE)
	{
		ACE_Reactor_Mask m = ACE_Event_Handler::ACCEPT_MASK |
			ACE_Event_Handler::DONT_CALL;

		reactor()->remove_handler(this,m);
		accepter.close();
	}
	return 0;
}