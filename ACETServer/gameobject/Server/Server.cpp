#define _CRT_SECURE_NO_WARNINGS
#include "Output.h"
#include "Input.h"
//#include "MemoryMgr.hpp"
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
#include <crtdbg.h>
#include "Daemon.h"

int main(int argc, char *argv[])
{
	ACE::debug(OPEN_SYS_LOG);
	const wchar_t * p = L"MAIN";
	MessageBox(0, p,p, 0);
	//_CrtSetBreakAlloc(444);
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	ACE_Service_Config::open(argc, argv, ACE_DEFAULT_LOGGER_KEY);
	std::thread t(controller, ACE_Proactor::instance());
	t.join();
	//ACE_Reactor::instance()->run_reactor_event_loop();
	if (ACE_Proactor::instance())
	{
		ACE_Proactor::instance()->proactor_run_event_loop();
		ACE_Proactor::instance()->close_singleton();

		ACE_Service_Config::current()->close();
		ACE_Service_Config::singleton()->close();
	}
	return 0;
}
