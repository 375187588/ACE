
#include "Common.h" 
#include "EventAcceptor.h" 
#include "UserEventHandler.h" 

int EventAcceptor::open(const ACE_INET_Addr &local_addr)
{
	if (acceptor_.open(local_addr) == -1)
		return -1;

	return reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
}

int EventAcceptor::handle_input(ACE_HANDLE)
{
	UserEventHandler *peer_handler = 0;
	ACE_NEW_RETURN(peer_handler,
		UserEventHandler(reactor()),
		-1);

	if (acceptor_.accept(peer_handler->peer()) == -1)
	{
		delete peer_handler;
		std::cout << "EventAcceptor acceptor_.accept" << std::endl;
		return -1;
	}
	else if (peer_handler->open() == -1)
	{
		peer_handler->handle_close();
		std::cout << "EventAcceptor acceptor_.open" << std::endl;
		return -1;
	}

	std::cout << "EventAcceptor handle_input" << std::endl;

	return 0;
}

int EventAcceptor::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
	acceptor_.close();
	delete this;
	std::cout << "EventAcceptor acceptor_.handle_close" << std::endl;
	return 0;
}

