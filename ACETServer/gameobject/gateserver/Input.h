#pragma once

class Acceptor;
class Input : public ACE_Service_Handler
{
public:
	Input(Acceptor *acc = 0);
	virtual ~Input(void);

	virtual void open(ACE_HANDLE new_handle, ACE_Message_Block &message_block);

protected:
	enum { LOG_HEADER_SIZE = 8, };
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);

	Acceptor *m_acceptor;
	ACE_Message_Block *m_pAmb;
	ACE_Asynch_Read_Stream m_reader;
};
