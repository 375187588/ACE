#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "ace/Message_Queue.h"
#include "ace/Asynch_IO.h"
#include "ace/OS.h"
#include "ace/Proactor.h"
#include "ace/Asynch_Acceptor.h"

class Service_Handler : public ACE_Service_Handler
{
public:
	Service_Handler()
	{
		ACE_OS::printf("Service_Handler constructed for accepter \n");
	}
	~Service_Handler()
	{
		if (this->handle() != ACE_INVALID_HANDLE)
			ACE_OS::closesocket(this->handle());
		ACE_OS::printf("one Service_Handler for accepter destructed");
	}
	//异步读取消息
	void reveive_from_connector(void);

	virtual void open(ACE_HANDLE h, ACE_Message_Block&)
	{
		this->handle(h);
		if (this->reader_.open(*this) != 0)
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),
				ACE_TEXT("Service_Handler open")));
			delete this;
			return;
		}
		//异步读取
		reveive_from_connector();
		return;
	}

	//reader_.read()异步读完成后会调用此函数
	virtual void handle_read_stream
	(const ACE_Asynch_Read_Stream::Result &result)
	{
		ACE_Message_Block &mb = result.message_block();
		if (!result.success() || result.bytes_transferred() == 0)
		{
			mb.release();
			delete this;
			return;
		}
		//输出读取内容
		ACE_OS::printf("received:%s\n", mb.rd_ptr());
		mb.release();
		//继续下一次异步读取
		reveive_from_connector();
	}

private:
	ACE_Asynch_Read_Stream reader_;
	char buffer[8192];
};

void Service_Handler::reveive_from_connector(void)
{
	ACE_Message_Block *mb = new ACE_Message_Block(buffer, 4096);
	if (this->reader_.read(*mb, mb->space()) != 0)
	{
		ACE_OS::printf("Begin read fail\n");
		delete this;
		return;
	}
}

int main(int argc, char *argv[])
{
	int port = 3000;
	ACE_Asynch_Acceptor<Service_Handler> acceptor;

	if (acceptor.open(ACE_INET_Addr(port)) == -1)
		return -1;
	cout << "listenning localhost[" << port << "]...." << endl;

	ACE_Proactor::instance()->proactor_run_event_loop();

	return 0;
}
