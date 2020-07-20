#pragma once
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>

//参与者
class Participant : public ACE_Event_Handler
{
public:
	static ACE_Time_Value maxMsgInterVal;

	Participant(ACE_Reactor* reactor = ACE_Reactor::instance());

	int open();
	virtual ACE_HANDLE get_handle() const;
	virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE);
	//关闭长时间不发信息的用户
	virtual int handle_timeout(const ACE_Time_Value& t, const void* = 0);
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask closeMask);

	ACE_SOCK_Stream& socket();

private:
	ACE_Time_Value lastMsgTime;
	ACE_SOCK_Stream sock;
};

