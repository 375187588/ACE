#pragma once
/*
#include<ace/SOCK_Connector.h>
#include<ace/INET_Addr.h>
#include<ace/Log_Msg.h>
#include<ace/OS_NS_string.h>
#include<ace/OS_NS_stdlib.h>

#define SIZE_BUF 128
#define NO_ITERATIONS 10005

class Client
{
public:
	Client(int port) :remote_addr_(port)
	{
		remote_addr_.set(port, "127.0.0.1");
		data_buf_ = "Hello ACE Server from Client";
	}

	int connect_to_server();
	int send_to_server();
	int close();

private:
	ACE_SOCK_Stream client_stream_;
	ACE_INET_Addr remote_addr_;
	ACE_SOCK_CONNECTOR connector_;
	const char *data_buf_;
};
*/

#include "ace/Task.h"
#include "ace/INET_Addr.h"
#include "ace/Synch.h"

namespace ACE_Client
{
	class Client : public ACE_Task<ACE_NULL_SYNCH>
	{
	public:
		Client(const char *server);
		virtual int svc(void);
		char *put_task(char *msg_t, char *msg_s, size_t msg_len);
		void send_tasks(void);

	private:
		ACE_INET_Addr addr;
	};
}
