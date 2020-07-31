#pragma once

class Ouput : 
	public ACE_Task<ACE_NULL_SYNCH>, 
	public ACE_Service_Handler
{
public:
	Ouput(void);
	virtual ~Ouput(void);

	virtual int put(ACE_Message_Block *, ACE_Time_Value * = 0);
	virtual void open(ACE_HANDLE h, ACE_Message_Block &m);
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);

protected:
	void startWrite(ACE_Message_Block *pAmb = 0);

protected:
	ACE_Asynch_Read_Stream m_reader;
	ACE_Asynch_Write_Stream m_writer;
	int m_nCanWrite;
};

typedef ACE_Unmanaged_Singleton<Ouput, ACE_Null_Mutex> OUTPUT_HANDLER;