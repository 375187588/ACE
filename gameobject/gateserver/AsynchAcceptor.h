#pragma once
#include "AsynchInputHandler.h" 

class AsynchAcceptor : public ACE_Asynch_Acceptor<AsynchInputHandler>
{
public:
	AsynchAcceptor(void);
	virtual ~AsynchAcceptor(void);

	void close();
	void remove(AsynchInputHandler *ih) { clients_.remove(ih); }

protected:
	virtual AsynchInputHandler* make_handler();

protected:
	ACE_Unbounded_Set<AsynchInputHandler *> clients_;
};
