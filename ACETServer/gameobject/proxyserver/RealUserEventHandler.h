#pragma once
#include "ace/Timer_Queue.h" 
#include "UserEventHandler.h" 

class RealUserEventHandler : public UserEventHandler
{
public:
	enum { MAX_CLIENT_TIMEOUT = 3600 };

	RealUserEventHandler(
		ACE_Reactor *reactor,
		const ACE_Time_Value &max_client_timeout =
		ACE_Time_Value(MAX_CLIENT_TIMEOUT))
		: UserEventHandler(reactor),
		time_of_last_log_record_(0),
		max_client_timeout_(max_client_timeout) { }

	virtual ~RealUserEventHandler() { }
	virtual int open();
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
	virtual int handle_timeout(const ACE_Time_Value &tv, const void *act = 0);
	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);

private:
	ACE_Time_Value time_of_last_log_record_;
	const ACE_Time_Value max_client_timeout_;
};
