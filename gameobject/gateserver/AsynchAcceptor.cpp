#include "Common.h"
#include "AsynchAcceptor.h"

AsynchAcceptor::AsynchAcceptor(void)
{
}

AsynchAcceptor::~AsynchAcceptor(void)
{
}

void AsynchAcceptor::close()
{
	ACE_Unbounded_Set_Iterator<AsynchInputHandler *> iter(clients_.begin());
	AsynchInputHandler **ih;
	while (iter.next(ih))
		delete *ih;
}

AsynchInputHandler* AsynchAcceptor::make_handler()
{
	printf("accept new client\n");
	AsynchInputHandler *ih = 0;
	ACE_NEW_RETURN(ih, AsynchInputHandler(this), 0);
	if (clients_.insert(ih) == -1)
	{
		delete ih;
		return 0;
	}
	return ih;
}