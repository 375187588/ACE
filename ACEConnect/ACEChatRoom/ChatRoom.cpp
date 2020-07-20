#include "ChatRoom.h"

ChatRoom* ChatRoom::room = nullptr;
ChatRoom* ChatRoom::instance()
{
	if (nullptr == room)
	{
		room = new ChatRoom();
	}
	return room;
}
ChatRoom::~ChatRoom()
{

}
void ChatRoom::join(Participant* user)
{
	users.push_back(user);
}
void ChatRoom::leave(Participant* user)
{
	std::list<Participant*>::iterator it = users.begin();
	while (it != users.end())
	{
		if (*it == user)
		{
			users.erase(it);
			break;
		}
		it++;
	}
}
void ChatRoom::forwardMsg(const char* msg)
{
	std::list<Participant*>::const_iterator it = users.begin();
	while (it != users.end())
	{
		ACE_SOCK_Stream& sock = (*it)->socket();
		if (sock.send(msg, std::strlen(msg)) == -1)
		{
			(*it)->handle_close(ACE_INVALID_HANDLE, 0);
		}
		it++;
	}
}

ChatRoom::ChatRoom()
{

}
ChatRoom::ChatRoom(const ChatRoom&)
{

}

ChatRoom& ChatRoom::operator==(const ChatRoom&)
{
	return *this;
}

	