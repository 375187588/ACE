// ACE.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

// serverMain.cpp

// 如果是WIN32平台上报错
// f:\ace\ace_wrappers\ace\config-win32-common.h(23): fatal error C1189: #error :  Please define WIN32 in your project settings.
// 可以文件头添加WIN32的预定义解决
#ifndef WIN32
#define WIN32
#endif

// 针对Debug版和Release版进行不同预处理
#ifdef _DEBUG
#pragma comment(lib,"ACEd.lib")
#else
#pragma comment(lib,"ACE.lib")
#endif

#include <ace\Log_Msg.h>
#include <ace\Time_Value.h>
#include <ace\SOCK_Acceptor.h>
#include <ace\SOCK_Stream.h>
#include <ace\OS_NS_stdlib.h>

#define SIZE_DATA 18
#define SIZE_BUF 1024
#define NO_ITERATIONS 5

class Server
{
public:
	Server(int port) : server_addr_(port), peer_acceptor_(server_addr_)
	{
		data_buf_ = new char[SIZE_BUF];
	}

	int handle_connection()
	{
		for (int i = 0; i < NO_ITERATIONS; i++)
		{
			int byte_count = 0;
			if ((byte_count = new_stream_.recv_n(data_buf_, SIZE_DATA, 0)) == -1)
				ACE_ERROR((LM_ERROR, "%p\n", "Error in recv"));
			else
			{
				data_buf_[byte_count] = 0;
				ACE_DEBUG((LM_DEBUG, "Server received %s \n", data_buf_));
			}
		}
		if (new_stream_.close() == -1)
			ACE_ERROR((LM_ERROR, "%p\n", "close"));
		return 0;
	}

	int accept_connections()
	{
		if (peer_acceptor_.get_local_addr(server_addr_) == -1)
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "Error in get_local_addr"), 1);
		ACE_DEBUG((LM_DEBUG, "Starting server at port %d\n",
			server_addr_.get_port_number()));
		while (1)
		{
			ACE_Time_Value timeout(ACE_DEFAULT_TIMEOUT);
			if (peer_acceptor_.accept(new_stream_, &client_addr_, &timeout) == -1)
			{
				ACE_ERROR((LM_ERROR, "%p\n", "accept"));
				continue;
			}
			else
			{
				ACE_DEBUG((LM_DEBUG,
					"Connection established with remote %s:%d\n",
					client_addr_.get_host_name(), client_addr_.get_port_number()));
				handle_connection();
			}
		}
	}
private:
	char *data_buf_;
	ACE_INET_Addr server_addr_;
	ACE_INET_Addr client_addr_;
	ACE_SOCK_Acceptor peer_acceptor_;
	ACE_SOCK_Stream new_stream_;
};

int main(int argc, char *argv[])
{
	//if (argc < 2)
	//{
	//	ACE_ERROR((LM_ERROR, "Usage %s <port_num>", argv[0]));
	//	ACE_OS::exit(1);
	//}
	//Server server(ACE_OS::atoi(argv[1]));
	Server server(ACE_OS::atoi("8888"));
	server.accept_connections();
	getchar();
	return 0;
}

//int main()
//{
//    std::cout << "Hello World!\n";
//}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
