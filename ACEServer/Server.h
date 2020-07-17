#pragma once

/*
//练习ACE TCP连接server端
#include<ace/SOCK_Acceptor.h>
#include<ace/SOCK_Stream.h>
#include<ace/Log_Msg.h>
#include<ace/OS_NS_stdlib.h>
#include<ace/Time_Value.h>
#define SIZE_DATA 18
#define SIZE_BUF 8192
#define NO_ITERATIONS 5

class Server
{
public:
	Server(int port) :server_addr_(port), peer_acceptor_(server_addr_)
	{
		data_buf_ = new char[SIZE_BUF];
	}
	int handle_connection();
	int accept_connections();
	~Server() {}

private:
	char *data_buf_;
	ACE_INET_Addr server_addr_;
	ACE_INET_Addr client_addr_;
	//用于被动的连接建立，基于BSD accept()和listen()调用
	ACE_SOCK_Acceptor peer_acceptor_; 
	ACE_SOCK_Stream new_stream_;
};
*/

#include "ace/Task.h"

namespace ACE_Server
{
	class Server : public ACE_Task_Base
	{
	public:
		virtual int svc(void);
	};
}