#include "Common.h"
#include "Ouput.h"
#include "Connector.h" 

Ouput::Ouput(void)
	: m_nCanWrite(0)
{
}

Ouput::~Ouput(void)
{
	outLog("Ouput::~Ouput(void)\n");
}

void Ouput::open(ACE_HANDLE h, ACE_Message_Block &m)
{
	outLog("open new output handler\n");
	ACE_SOCK_Stream temp_peer(h);
	int bufsiz = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
	temp_peer.set_option(SOL_SOCKET, SO_SNDBUF, &bufsiz, sizeof bufsiz);

	m_reader.open(*this, h, 0, proactor());
	m_writer.open(*this, h, 0, proactor());

	ACE_Message_Block *pAmb = 0;
	ACE_NEW(pAmb, ACE_Message_Block(1));
	m_reader.read(*pAmb, 1);

	ACE_Sig_Action no_sigpipe((ACE_SignalHandler)SIG_IGN);
	no_sigpipe.register_action(SIGPIPE, 0);
	m_nCanWrite = 1;
	startWrite(0);
}

void Ouput::startWrite(ACE_Message_Block *pAmb)
{
	if (pAmb == nullptr)
	{
		ACE_Time_Value v(0);
		getq(pAmb, &v);
	}

	if (pAmb != nullptr)
	{
		m_nCanWrite = 0;
		if (m_writer.write(*pAmb, pAmb->length()) == -1)
			ungetq(pAmb);
	}
}

int Ouput::put(ACE_Message_Block *pAmb, ACE_Time_Value *pTimeOut)
{
	if (m_nCanWrite)
	{
		startWrite(pAmb);
		return 0;
	}

	return putq(pAmb, pTimeOut);
}

void Ouput::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	//outLog("write complete.\n"); 
	ACE_Message_Block &mb = result.message_block();
	if (!result.success())
	{
		mb.rd_ptr(mb.base());
		ungetq(&mb);
	}
	else
	{
		m_nCanWrite = (handle() == result.handle());
		if (mb.length() == 0)
		{
			mb.release();
			if (m_nCanWrite)
			{
				startWrite();
			}
		}
		else if (m_nCanWrite)
		{
			startWrite(&mb);
		}
		else
		{
			mb.rd_ptr(mb.base());
			ungetq(&mb);
		}
	}
}

void Ouput::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	outLog("disconnect with server\n");
	result.message_block().release();
	m_writer.cancel();
	ACE_OS::closesocket(result.handle());
	handle(ACE_INVALID_HANDLE);
	m_nCanWrite = 0;
	CLD_CONNECTOR::instance()->reconnect();
}
