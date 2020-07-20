// ACEConnector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Log_Msg.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Null_Mutex.h>

class AcceptorHandler :public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>
{
public:
	typedef ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH> Parent;
	enum {
		BUF_SIZE = 512
	};

	virtual int handle_input(ACE_HANDLE h)
	{
		ssize_t n = peer().recv(buf, BUF_SIZE);
		if (n <= 0)
		{
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "peer().recv()"), -1);
		}
		if (peer().send(buf, n) == -1)
		{
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "peer().send()"), -1);
		}
		return 0;
	}

private:
	char buf[BUF_SIZE];
};

typedef ACE_Acceptor<AcceptorHandler, ACE_SOCK_Acceptor> MyAcceptor;

int main(int argc, char *argv[])
{
	ACE_INET_Addr addr(8868);
	MyAcceptor acceptor(addr, ACE_Reactor::instance());
	ACE_Reactor::instance()->run_reactor_event_loop();
	std::cout << "Hello World!\n";
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
