// ACEServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
//#include "ace/Log_Msg.h"
//#include <ace\Log_Msg.h>
//#include <ace\Time_Value.h>
//#include <ace\SOCK_Acceptor.h>
//#include <ace\SOCK_Stream.h>
//#include <ace\OS_NS_stdlib.h>

#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/TP_Reactor.h"
#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/OS.h"
#include "Request_Handler.h"
#include "Constants.h"
#include "Server.h"

using namespace ACE_Server;

int main(int argc, char *argv[])
{
	/*ACE_DEBUG((LM_INFO, ACE_TEXT("ACE Server\n")));
	Server server(ACE_OS::atoi("6789"));
	server.accept_connections();*/

	ACE_TP_Reactor tp_reactor;
	ACE_Reactor reactor(&tp_reactor, 1);
	ACE_Reactor::instance(&reactor, 1);

	ACE_Acceptor<Request_Handler, ACE_SOCK_ACCEPTOR> acceptor;
	ACE_INET_Addr addr(SERVER_PORT_NUM);
	if (acceptor.open(addr) == -1)
		return -1;

	Server server_tp;
	server_tp.activate(THR_NEW_LWP | THR_JOINABLE, SERVER_THREAD_POOL_SIZE);
	ACE_Thread_Manager::instance()->wait();

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
