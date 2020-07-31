// gateserver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//AIO
#include <iostream>
#include <string>
#include <thread>
#include "Common.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Daemon.h"
#include "Input.h"
#include "Ouput.h"

ACE_THR_FUNC_RETURN controller(void *);

static ACE_THR_FUNC_RETURN controller(void *arg)
{
	ACE_Proactor* t = reinterpret_cast<ACE_Proactor*>(arg);

	while (1)
	{
		std::string user_input;
		std::getline(std::cin, user_input, '\n');
		if (user_input == "quit")
		{
			t->proactor_end_event_loop();
			break;
		}
	}

	return 0;
}
int main(int argc, char* argv[])
{
	ACE::debug(false);
	const wchar_t * p = L"MAIN";
	MessageBox(0, p, p, 0);
	//_CrtSetBreakAlloc(462);
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//AIO
	//ACE_STATIC_SVC_REGISTER(Client_Logging_Daemon); 
	ACE_Service_Config::open(argc, argv, ACE_DEFAULT_LOGGER_KEY/*, 0*/);
	std::thread t(controller, ACE_Proactor::instance());
	t.detach();
	ACE_Proactor::instance()->proactor_run_event_loop();
	ACE_Proactor::instance()->close_singleton();
	//
	//ACE_Service_Config::current()->close();
	//ACE_Service_Config::singleton()->close();
	//delete ACE_Proactor::instance();

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
