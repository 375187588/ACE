#pragma once
#include "ace/FILE_IO.h" 
#include "ace/FILE_Connector.h" 
#include "ace/Event_Handler.h" 
#include "ace/Reactor.h" 
#include "ace/OS_NS_String.h" 
#include "UserHandler.h" 

class ACE_Message_Block;
class UserEventHandler : public ACE_Event_Handler
{
public:
	UserEventHandler(ACE_Reactor *reactor)
		: ACE_Event_Handler(reactor),
		logging_handler_(log_file_) { }

	virtual ~UserEventHandler() { }
	virtual int open();
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);
	virtual ACE_HANDLE get_handle() const;
	ACE_SOCK_Stream& peer() { return logging_handler_.peer(); }
	ACE_FILE_IO& log_file() { return log_file_; }

private:
	ACE_FILE_IO log_file_;
	UserHandler logging_handler_;
};
