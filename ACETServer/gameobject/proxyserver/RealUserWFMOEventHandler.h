#pragma once
#include "RealUserEventHandler.h" 

class RealUserWFMOEventHandler : public RealUserEventHandler
{
public:
	RealUserWFMOEventHandler(ACE_Reactor *reactor)
		: RealUserEventHandler(reactor) { }

	virtual ~RealUserWFMOEventHandler() { }
	virtual int handle_input(ACE_HANDLE h)
	{
		ACE_GUARD_RETURN(ACE_SYNCH_MUTEX, monitor, lock_, -1);
		return RealUserEventHandler::handle_input(h);
	}

private:
	ACE_Thread_Mutex lock_;
};
