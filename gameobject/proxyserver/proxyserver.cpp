// ACELogServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ace/Log_Msg.h" 
#include "ace/Reactor.h" 
#include "ace/Select_Reactor.h" 
#include "ace/Thread_Manager.h" 
#include "Reactor_Logging_Server.h" 
#include "RealEventAcceptor.h" 

#include "ace/Log_Msg.h" 
#include "ace/Reactor.h" 
#include "ace/WFMO_Reactor.h" 
#include "ace/Thread_Manager.h" 
#include "Reactor_Logging_Server.h" 
#include "RealWFMOEventAcceptor.h" 

#include "ace/streams.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"
#include "ace/SString.h"
#include "ace/OS_NS_time.h"
#include "ace/Time_Value.h"

#include <string>
#include <thread>
#include "Common.h"
#include "UserHandler.h"

ACE_THR_FUNC_RETURN controller(void *);
ACE_THR_FUNC_RETURN event_loop(void *);

class Quit_Handler : public ACE_Event_Handler
{
public:
	Quit_Handler(ACE_Reactor *r) : ACE_Event_Handler(r) { }
	virtual int handle_exception(ACE_HANDLE) {
		reactor()->end_reactor_event_loop();
		return -1;
	}

	virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask)
	{
		reactor()->end_reactor_event_loop();
		delete this;
		return 0;
	}


	virtual ~Quit_Handler() { }
};

ACE_THR_FUNC_RETURN event_loop(void *);

int main(int argc, char* argv[])
{
	const size_t N_THREADS = 4;
	ACE_WFMO_Reactor wfmo_reactor;
	ACE_Reactor r(&wfmo_reactor);
	Reactor_Logging_Server<RealWFMOEventAcceptor> *server = 0;
	ACE_NEW_RETURN(server,
		Reactor_Logging_Server<RealWFMOEventAcceptor>(argc, argv, &r),
		1);

	Quit_Handler quit_handler(&r);
	ACE_Thread_Manager::instance()->spawn_n(N_THREADS, event_loop, &r);
	//ACE_Thread_Manager::instance()->spawn(controller, &r);
	std::thread t1(controller, &r);
	t1.join();

	return ACE_Thread_Manager::instance()->wait();
}

static ACE_THR_FUNC_RETURN event_loop(void *arg)
{
	ACE_Reactor* reactor = reinterpret_cast<ACE_Reactor*>(arg);
	//多线程不能是线程自己
	//reactor->owner(ACE_OS::thr_self()); 
	reactor->run_reactor_event_loop();
	return 0;
}


//int main(int argc, char *argv[])
//{
//	ACE_Select_Reactor select_reactor;
//	ACE_Reactor reactor(&select_reactor);
//	Reactor_Logging_Server<RealEventAcceptor> *server = 0;
//	ACE_NEW_RETURN(server,
//		Reactor_Logging_Server<RealEventAcceptor>(argc, argv, &reactor),
//		1);
//
//	ACE_Thread_Manager::instance()->spawn(event_loop, &reactor);
//	ACE_Thread_Manager::instance()->spawn(controller, &reactor);
//	//if(reactor.run_reactor_event_loop() == -1)
//	//  ACE_ERROR_RETURN((LM_ERROR, "%p\n", "run_reactor_event_loop()"), 1); 
//
//	return ACE_Thread_Manager::instance()->wait();
//}

//static ACE_THR_FUNC_RETURN event_loop(void *arg)
//{
//	//ACE_Reactor *reactor = ACE_static_cast(ACE_Reactor*, arg);
//	ACE_Reactor* reactor = reinterpret_cast<ACE_Reactor*>(arg);
//	reactor->owner(ACE_OS::thr_self());
//	reactor->run_reactor_event_loop();
//	return 0;
//}

static ACE_THR_FUNC_RETURN controller(void *arg)
{
	ACE_Reactor* reactor = reinterpret_cast<ACE_Reactor*>(arg);
	Quit_Handler *quit_handler = 0;
	ACE_NEW_RETURN(quit_handler, Quit_Handler(reactor), 0);

	while (1)
	{
		std::string user_input;
		std::getline(std::cin, user_input, '\n');
		if (user_input == "quit")
		{
			//reactor->notify(quit_handler);
			reactor->end_reactor_event_loop();
			break;
		}
	}

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
