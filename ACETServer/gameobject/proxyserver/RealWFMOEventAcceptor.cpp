
#include "Common.h" 
#include "RealWFMOEventAcceptor.h" 
#include "RealUserWFMOEventHandler.h" 


int RealWFMOEventAcceptor::handle_input(ACE_HANDLE)
{
	RealUserWFMOEventHandler *peer_handler = 0;
	ACE_NEW_RETURN(peer_handler,
		RealUserWFMOEventHandler(reactor()), -1);

	if (acceptor().accept(peer_handler->peer()) == -1)
	{
		delete peer_handler;
		return -1;
	}
	else if (peer_handler->open() == -1)
	{
		peer_handler->handle_close();
		return -1;
	}

	return 0;
}

