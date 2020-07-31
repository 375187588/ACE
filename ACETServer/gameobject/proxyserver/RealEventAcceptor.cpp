
#include "Common.h"
#include "RealEventAcceptor.h" 
#include "RealUserEventHandler.h" 


int RealEventAcceptor::handle_input(ACE_HANDLE)
{
	RealUserEventHandler *peer_handler = 0;
	ACE_NEW_RETURN(peer_handler,
		RealUserEventHandler(reactor(), ACE_Time_Value(5)),
		-1);

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

