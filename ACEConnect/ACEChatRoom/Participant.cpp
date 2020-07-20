#include "Participant.h"
#include <ace/Log_Msg.h>
#include <ace/Timer_Queue.h>
#include "ChatRoom.h"

ACE_Time_Value Participant::maxMsgInterVal = ACE_Time_Value(5);

Participant::Participant(ACE_Reactor* reactor)
{
	this->reactor(reactor);
}

int Participant::open()
{
	//一定要初始化这个时间
	this->lastMsgTime = reactor()->timer_queue()->gettimeofday();

	//时钟调度
	int result = reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
	if (result != 0)
		return result;

	result = reactor()->schedule_timer(this, 0, ACE_Time_Value::zero, maxMsgInterVal);

	return result;
}

ACE_HANDLE Participant::get_handle() const
{
	return sock.get_handle();
}

int Participant::handle_input(ACE_HANDLE h)
{
	char buf[512] = "";
	ssize_t recvBytes = sock.recv(buf, sizeof(buf));
	if (recvBytes <= 0)
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "sock.recv"), -1);
	}

	this->lastMsgTime = reactor()->timer_queue()->gettimeofday();

	ChatRoom::instance()->forwardMsg(buf);
	return 0;
}

//关闭长时间不发信息的用户
int Participant::handle_timeout(const ACE_Time_Value& t, const void* p)
{
	//5秒不发话，超时自动请出
	if (t - lastMsgTime > maxMsgInterVal)
	{
		reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK);
	}
	return 0;
}

int Participant::handle_close(ACE_HANDLE h, ACE_Reactor_Mask closeMask)
{
	if (get_handle() != ACE_INVALID_HANDLE)
	{
		ACE_Reactor_Mask m = ACE_Event_Handler::ACCEPT_MASK |
			ACE_Event_Handler::DONT_CALL;

		reactor()->cancel_timer(this);
		reactor()->remove_handler(this, m);
		sock.close();
		ChatRoom::instance()->leave(this);
		delete this;
	}
	return 0;
}


ACE_SOCK_Stream& Participant::socket()
{
	return sock;
}
