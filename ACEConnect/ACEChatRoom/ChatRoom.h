#pragma once
#include <list>
#include "Participant.h"

class ChatRoom
{
public:
	virtual ~ChatRoom();
	void join(Participant* user);
	void leave(Participant* user);
	void forwardMsg(const char* msg);
	static ChatRoom* instance();

private:
	ChatRoom();
	ChatRoom(const ChatRoom&);
	ChatRoom& operator==(const ChatRoom&);

private:
	static ChatRoom* room;
	std::list<Participant*> users;
};

