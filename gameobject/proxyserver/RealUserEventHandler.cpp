
#include "Common.h" 
#include "RealUserEventHandler.h"


int RealUserEventHandler::open()
{
	int result = UserEventHandler::open();
	if (result == -1)
		return result;

	ACE_Time_Value reschedule(max_client_timeout_.sec() / 4);
	result = reactor()->schedule_timer(this, 0, max_client_timeout_, reschedule);
	return result;
}

int RealUserEventHandler::handle_input(ACE_HANDLE h)
{
	time_of_last_log_record_ = reactor()->timer_queue()->gettimeofday();
	return UserEventHandler::handle_input(h);
}

int RealUserEventHandler::handle_timeout(const ACE_Time_Value &now, const void*)
{
	ACE_DEBUG((LM_DEBUG,
		"handle_timeout, now = %d, last = %d, timeout = %d\n",
		now.sec(), time_of_last_log_record_.sec(), max_client_timeout_.sec()));

	if (now - time_of_last_log_record_ >= max_client_timeout_)
		reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK);
	return 0;
}

int RealUserEventHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
	reactor()->cancel_timer(this);
	std::cout << "RealUserEventHandler::handle_close" << std::endl;
	return UserEventHandler::handle_close(h, mask);
}