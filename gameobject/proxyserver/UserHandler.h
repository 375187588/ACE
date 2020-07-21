#pragma once
#include "ace/FILE_IO.h" 
#include "ace/SOCK_Stream.h" 

class ACE_Message_Block;
class UserHandler
{
public:
	UserHandler(ACE_FILE_IO &log_file) : log_file_(log_file) {}
	UserHandler(ACE_FILE_IO &log_file, ACE_HANDLE handle) : log_file_(log_file)
	{
		logging_peer_.set_handle(handle);
	}
	UserHandler(ACE_FILE_IO &log_file, const ACE_SOCK_Stream &logging_peer)
		: log_file_(log_file), logging_peer_(logging_peer) {}

	ACE_SOCK_Stream& peer() { return logging_peer_; }

	int close() { return logging_peer_.close(); }
	int recv_log_record(ACE_Message_Block* &mblk);
	int write_log_record(ACE_Message_Block *mblk);
	int log_record();

private:
	ACE_FILE_IO &log_file_;
	ACE_SOCK_Stream logging_peer_;
};
