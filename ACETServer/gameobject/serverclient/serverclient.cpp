// serverclient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "ace/Asynch_IO.h"
#include "ace/OS_main.h"
#include "ace/Proactor.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/OS.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"
#include "ace/Containers.h"
#include "ace/SOCK_SEQPACK_Association.h"

ACE_DLList<ACE_Asynch_Write_Stream> wList;

class Service_Handler :public ACE_Service_Handler
{
public:
	Service_Handler()
	{

	}
	~Service_Handler(void)
	{
		if (this->handle() != ACE_INVALID_HANDLE)
			ACE_OS::closesocket(this->handle());
	}

	virtual void open(ACE_HANDLE h, ACE_Message_Block &message_block)
	{
		if (rs_.open(*this, h))
		{
			ACE_ERROR((LM_ERROR, "%p/n", "ACE_Asynch_Read_Stream::open"));
			return;
		}

		if (ws_.open(*this))
		{
			ACE_ERROR((LM_ERROR, "%p/n", "ACE_Asynch_Write_Stream::open"));
			return;
		}

		if (post_recv() == -1)
			return;

		addresses(remote_address, local_address);
		remote_address.addr_to_string(peer_name, MAXHOSTNAMELEN);

		ACE_INET_Addr addr;
		ACE_SOCK_SEQPACK_Association ass = ACE_SOCK_SEQPACK_Association(h);
		size_t addr_size = sizeof ACE_INET_Addr;
		ass.get_remote_addrs(&addr, addr_size);
		ACE_OS::printf("fd:%d ip:%d port:%d\n", (int)h, addr.get_ip_address(), addr.get_port_number());
		//ACE_DEBUG((LM_DEBUG,ACE_TEXT("peer:%s\n"),peer_name));
	}
protected:
	int post_recv(void)
	{
		ACE_Message_Block *mb = 0;
		ACE_NEW_RETURN(mb, ACE_Message_Block(512), -1);

		if (rs_.read(*mb, mb->space()) == -1)
		{
			ACE_ERROR_RETURN((LM_ERROR, "%p/n", "ACE_Asynch_Read_Stream::read"), -1);
		}
		return 0;
	}
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
	{
		ACE_Message_Block &mb = result.message_block();
		if (result.success() && result.bytes_transferred() != 0)
		{
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("recv:%s\n"), mb.rd_ptr()));
			if (ws_.write(*mb.duplicate(), result.message_block().length()) == -1)
			{
				ACE_ERROR((LM_ERROR, "%p\n", "ACE_Asynch_Write_Stream::write"));
			}
			mb.release();
			post_recv();
		}
		else
		{
			mb.release();
			delete this;
		}
	}
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
	{
		result.message_block().release();
	}
private:
	ACE_Asynch_Read_Stream rs_;
	ACE_Asynch_Write_Stream ws_;

	ACE_HANDLE handle_;

	ACE_TCHAR peer_name[MAXHOSTNAMELEN];
	ACE_INET_Addr remote_address;
	ACE_INET_Addr local_address;
};

#include <ace/OS.h>
#include <ace/Task.h>

class TTcpNetThread : public ACE_Task_Base
{
public:
	/// 运行
	int open();

	/// 停止运行
	int close();
protected:
	/// 线程函数
	virtual int svc();
};

int TTcpNetThread::open() { return this->activate(); }

int TTcpNetThread::close()
{
	ACE_Proactor::instance()->proactor_end_event_loop(); // 终止ACE_Proactor循环

	this->wait(); // 等待清理现场

	return 0;
}

int TTcpNetThread::svc()
{
	ACE_Proactor::instance()->proactor_run_event_loop();

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("Network fin\n")));

	return 0;
}


#define CLIENT_THREAD_NUM 4

int main(int argc, char *argv[])
{
	ACE_Asynch_Acceptor<Service_Handler> acceptor;

	if (acceptor.open(ACE_INET_Addr(7878), 0, 1) == -1)
	{
		return -1;
	}

	TTcpNetThread netThread[CLIENT_THREAD_NUM];

	for (int i = 0; i < CLIENT_THREAD_NUM; i++)
	{
		netThread[i].open();
	}
	while (getchar())
	{
		ACE_OS::sleep(1);
	}
	//ACE_Proactor::instance()->proactor_run_event_loop();

	return 0;
};

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
