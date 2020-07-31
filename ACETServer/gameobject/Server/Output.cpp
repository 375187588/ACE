#include "Output.h"
#include "Connector.h"

Output::Output():
	m_bCantWrite(false)
{
	ACE::debug(OPEN_SYS_LOG);
}

Output::~Output()
{
	if (ACE_INVALID_HANDLE != this->handle())
	{
		ACE_OS::closesocket(this->handle());
	}
	outLog("Output::~Output()\n");
}

void Output::startWrite(ACE_Message_Block* pmb)
{
	if (nullptr == pmb)
	{
		ACE_Time_Value n(0);
		getq(pmb, &n);
	}
	else if (nullptr != pmb)
	{
		m_bCantWrite = false;
		if (m_writeStream.write(*pmb, pmb->length()) == -1)
		{
			ungetq(pmb);
		}
	}
}

void Output::open(ACE_HANDLE h, ACE_Message_Block& mb)
{
	//在OPEN函数中完成读写操作
	this->handle(h);

	ACE_SOCK_Stream ass(h);
	int nBufSize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
	ass.set_option(SOL_SOCKET, SO_SNDBUF, &nBufSize, sizeof(nBufSize));

	//发送
	if (this->m_writeStream.open(*this,h,0, ACE_Proactor::instance()) != 0)
	{
		outLog("m_writeStream Output::open fail\n");
		delete this;
		return;
	}	
	//接收
	if (this->m_readStream.open(*this, h, 0, ACE_Proactor::instance()) != 0)
	{
		outLog("m_readStream Output::open fail\n");
		delete this;
		return;
	}

	ACE_Message_Block *pmb = 0;
	ACE_NEW(pmb, ACE_Message_Block(1));
	m_readStream.read(*pmb, 1);

	ACE_Sig_Action s((ACE_SignalHandler)SIG_IGN);
	s.register_action(SIGPIPE, 0);
	m_bCantWrite = true;
	startWrite(nullptr);

	return;
}

void Output::handle_write_stream(const WriteStream &result)
{
	ACE_Message_Block &mb = result.message_block();
	if (!result.success())
	{
		mb.rd_ptr(mb.base());
		ungetq(&mb);
	}
	else
	{
		m_bCantWrite = (handle() == result.handle());
		if (mb.length() == 0)
		{
			mb.release();
			if (m_bCantWrite)
			{
				startWrite();
			}
		}
		else if (m_bCantWrite)
		{
			startWrite(&mb);
		}
		else
		{
			mb.rd_ptr(mb.base());
			ungetq(&mb);
		}
	}

	return;
}

int Output::put(ACE_Message_Block* pmb, ACE_Time_Value* ptv)
{
	if (m_bCantWrite)
	{
		startWrite(pmb);
		return 0;
	}

	return putq(pmb, ptv);
}

void Output::handle_read_stream(const ReadStream &result)
{
	outLog("Output::handle_read_stream\n");
	result.message_block().release();
	m_writeStream.cancel();
	ACE_OS::closesocket(result.handle());
	handle(ACE_INVALID_HANDLE);
	m_bCantWrite = false;

	if (MutextConnector::instance())
	{
		MutextConnector::instance()->reconnect();
	}	
}