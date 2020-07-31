#pragma once
#include "Common.h"
#include "Output.h"

class Acceptor;
class Input : public ACE_Service_Handler
{
public:
	Input(Acceptor* pAcceptor = nullptr);
	virtual ~Input();
	virtual void open(ACE_HANDLE ,ACE_Message_Block &);
	virtual void handle_read_stream(const ReadStream &result);

private:
	Acceptor* m_pAcceptor;
	ACE_Message_Block* m_pMsgBlock;
	ACE_Asynch_Read_Stream m_readStream;
};

