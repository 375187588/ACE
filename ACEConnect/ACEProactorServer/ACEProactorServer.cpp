// ACEProactorServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ace/Asynch_IO.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Proactor.h"

class EchoService : public ACE_Service_Handler
{
public:
	~EchoService()
	{
		if (this->handle() != ACE_INVALID_HANDLE)
		{
			ACE_OS::closesocket(this->handle());
		}
	}
	virtual void open(ACE_HANDLE h, ACE_Message_Block&)
	{
		handle(h);
		if (this->reader_.open(*this) != 0 ||
			this->writer_.open(*this) != 0)
		{
			ACE_ERROR((LM_ERROR, "%p\n", "open()"));
			delete this;
			return;
		}
		ACE_Message_Block* mb;
		ACE_NEW_NORETURN(mb, ACE_Message_Block(512));
		if (this->reader_.read(*mb, mb->space()) != 0)
		{
			ACE_ERROR((LM_ERROR, "%p\n", "read()"));
			mb->release();
			delete this;
			return;
		}
	}

	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result)
	{
		ACE_Message_Block& mb = result.message_block();
		if (!result.success() || result.bytes_transferred() == 0)
		{
			mb.release();
			delete this;
		}
		else
		{
			if (this->writer_.write(mb, mb.length()) != 0)
			{
				ACE_ERROR((LM_ERROR, "%p\n", "write()"));
				mb.release();
			}
			else
			{
				ACE_Message_Block* mblk;
				ACE_NEW_NORETURN(mblk, ACE_Message_Block(512));
				this->reader_.read(*mblk, mblk->space());
			}
		}
	}

	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result)
	{
		result.message_block().release();
	}

private:
	ACE_Asynch_Read_Stream reader_;
	ACE_Asynch_Write_Stream writer_;
};

int main(int argc, char *argv[])
{
	ACE_INET_Addr listen_addr(8868);
	ACE_Asynch_Acceptor<EchoService> aio_acceptor;
	if (0 != aio_acceptor.open(listen_addr,
		0,//bytes_to_read
		0,//pass_address
		ACE_DEFAULT_BACKLOG,//resuse_addr
		1,
		0,//proactor
		0))//valldate_new_connection
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "write()"),1);
	}
	ACE_Proactor::instance()->proactor_run_event_loop();
	getchar();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
