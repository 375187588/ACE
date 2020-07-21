#pragma once

class AsynchOutputHandler : public ACE_Task<ACE_NULL_SYNCH>, public ACE_Service_Handler
{
public:
	AsynchOutputHandler(void);
	virtual ~AsynchOutputHandler(void);

	virtual int put(ACE_Message_Block *, ACE_Time_Value * = 0);
	virtual void open(ACE_HANDLE new_handle, ACE_Message_Block &message_block);
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);

protected:
	void start_write(ACE_Message_Block *mblk = 0);

protected:
	ACE_Asynch_Read_Stream reader_;
	ACE_Asynch_Write_Stream writer_;
	int can_write_;
};

typedef ACE_Unmanaged_Singleton<AsynchOutputHandler, ACE_Null_Mutex> OUTPUT_HANDLER;