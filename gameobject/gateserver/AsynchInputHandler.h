#pragma once

class AsynchAcceptor;
class AsynchInputHandler : public ACE_Service_Handler
{
public:
	AsynchInputHandler(AsynchAcceptor *acc = 0);
	virtual ~AsynchInputHandler(void);

	virtual void open(ACE_HANDLE new_handle, ACE_Message_Block &message_block);

protected:
	enum { LOG_HEADER_SIZE = 8, };
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);

	AsynchAcceptor *acceptor_;
	ACE_Message_Block *mblk_;
	ACE_Asynch_Read_Stream reader_;
};
