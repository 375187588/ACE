
#ifndef __LOGGING_HANDLER_H__
#define __LOGGING_HANDLER_H__

#include "ace/FILE_IO.h" 
#include "ace/SOCK_Stream.h" 

class ACE_Message_Block;
class UserHandler
{
public:
	UserHandler(ACE_HANDLE handle)
	{
		logging_peer_.set_handle(handle);
	}
	ACE_SOCK_Stream& peer() { return logging_peer_; }

	int close() { return logging_peer_.close(); }
	int recv_log_record(ACE_Message_Block* &mblk);

private:
	ACE_SOCK_Stream logging_peer_;
};

#endif 