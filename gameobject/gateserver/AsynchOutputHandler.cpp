#include "Common.h"
#include "AsynchOutputHandler.h"
#include "AsynchConnector.h" 

AsynchOutputHandler::AsynchOutputHandler(void)
	: can_write_(0)
{
}

AsynchOutputHandler::~AsynchOutputHandler(void)
{
}


void AsynchOutputHandler::open(ACE_HANDLE new_handle, ACE_Message_Block &message_block)
{
	printf("open new output handler\n");
	ACE_SOCK_Stream temp_peer(new_handle);
	int bufsiz = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
	temp_peer.set_option(SOL_SOCKET, SO_SNDBUF, &bufsiz, sizeof bufsiz);

	reader_.open(*this, new_handle, 0, proactor());
	writer_.open(*this, new_handle, 0, proactor());

	ACE_Message_Block *mb = 0;
	ACE_NEW(mb, ACE_Message_Block(1));
	reader_.read(*mb, 1);

	ACE_Sig_Action no_sigpipe((ACE_SignalHandler)SIG_IGN);
	no_sigpipe.register_action(SIGPIPE, 0);
	can_write_ = 1;
	start_write(0);
}

void AsynchOutputHandler::start_write(ACE_Message_Block *mblk)
{
	if (mblk == 0)
	{
		ACE_Time_Value nonblock(0);
		getq(mblk, &nonblock);
	}

	if (mblk != 0)
	{
		can_write_ = 0;
		if (writer_.write(*mblk, mblk->length()) == -1)
			ungetq(mblk);
	}
}

int AsynchOutputHandler::put(ACE_Message_Block *mb, ACE_Time_Value *timeout)
{
	if (can_write_)
	{
		start_write(mb);
		return 0;
	}

	return putq(mb, timeout);
}

void AsynchOutputHandler::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	//printf("write complete.\n"); 
	ACE_Message_Block &mblk = result.message_block();
	if (!result.success())
	{
		mblk.rd_ptr(mblk.base());
		ungetq(&mblk);
	}
	else
	{
		can_write_ = (handle() == result.handle());
		if (mblk.length() == 0)
		{
			mblk.release();
			if (can_write_)
				start_write();
		}
		else if (can_write_)
			start_write(&mblk);
		else
		{
			mblk.rd_ptr(mblk.base());
			ungetq(&mblk);
		}
	}
}

void AsynchOutputHandler::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	printf("disconnect with server\n");
	result.message_block().release();
	writer_.cancel();
	ACE_OS::closesocket(result.handle());
	handle(ACE_INVALID_HANDLE);
	can_write_ = 0;
	CLD_CONNECTOR::instance()->reconnect();
}
