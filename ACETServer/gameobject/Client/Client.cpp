#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "ace/Message_Queue.h"
#include "ace/Asynch_IO.h"
#include "ace/OS.h"
#include "ace/Proactor.h"
#include "ace/Asynch_Connector.h"
struct PacketHeader
{
	ACE_HANDLE handler;
	int data_length;
};
class Service_Handler : public ACE_Service_Handler
{
public:
	Service_Handler()
	{
		ACE_OS::printf("Service_Handler constructed for connector \n");
	}
	~Service_Handler()
	{
		if (this->handle() != ACE_INVALID_HANDLE)
			ACE_OS::closesocket(this->handle());
		ACE_OS::printf("one Service_Handler for connecter destructed");
	}

	//把当前时间发送到远端
	void send_to_accepter(void);

	virtual void open(ACE_HANDLE h, ACE_Message_Block&)
	{
		//在OPEN函数中完成读写操作
		this->handle(h);
		//connect
		if (this->writer_.open(*this) != 0)
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),
				ACE_TEXT("Service_Handler open")));
			delete this;
			return;
		}
		ACE_OS::printf("connceted\n");
		send_to_accepter();
		return;
	}

	//writer_.write()异步写完成后会调用此函数
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
	{
		result.message_block().release();
		//ACE_OS::sleep(1);//发送完成之后一定要暂停，给对方一个处理的时间。要不然一直发送对方接收不过来就会丢失。更为常见的情况不是暂停时间，而是接收对端发来的确认信息。就像本文一开始说的那样。
		//上次发送完毕之后再接着发送一次,这次发送完成之后又会调用
		//handle_write_stream，所以会一直发送
		send_to_accepter();
		return;
	}

private:
	//用来向远端发送内容
	ACE_Asynch_Write_Stream writer_;
};

struct CmdHdr
{
	int cmd;
	int length;
};

struct CmdLogin : public CmdHdr
{
	int userID;
	char szPsw[20];
};

//把当前时间发送到远端
void Service_Handler::send_to_accepter(void)
{
	time_t now = ACE_OS::gettimeofday().sec();
	ACE_Message_Block *mb = new ACE_Message_Block(4096);
	//获取当前时间的字符串格式
	
	//std::string str = ctime(&now);

	char buffer[4096];
	memset(buffer, 0, sizeof(buffer));
	CmdLogin* pLogin = (CmdLogin*)buffer;
	pLogin->cmd = 1119;
	pLogin->length = sizeof(CmdLogin);
	strcpy(pLogin->szPsw, "8888");
	pLogin->userID = 2222;
	mb->copy(buffer,pLogin->length);
	int n = mb->length();
	std::cout << "n:" << n << " length:" << pLogin->length << std::endl;
	if (this->writer_.write(*mb, pLogin->length) != 0)
	{
		ACE_OS::printf("Begin write fail in open\n");
		delete this;
		return;
	}
	else
	{
		ACE_OS::printf("sended %s\n", mb->rd_ptr());
	}
}

int main(int argc, char *argv[])
{
	ACE_INET_Addr addr;
	addr.set(8889, ACE_LOCALHOST);

	char addr_buffer[50] = { 0 };
	addr.addr_to_string(addr_buffer, sizeof(addr_buffer));
	cout << "connecting[" << addr_buffer << "]...." << endl;

	ACE_Asynch_Connector<Service_Handler> connector;
	connector.open();
	if (connector.connect(addr) == -1)
		return -1;

	ACE_Proactor::instance()->proactor_run_event_loop();

	return 0;
}
