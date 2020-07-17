#include "Client.h"

/*
int Client::connect_to_server()
{
	//初始化与服务器的连接
	ACE_DEBUG((LM_DEBUG, "(%P|%t) Starting connect to %s:%d\n",
		remote_addr_.get_host_name(), remote_addr_.get_port_number()));
	if (connector_.connect(client_stream_, remote_addr_) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p\n", "connection failed"), -1);
	}
	else
		ACE_DEBUG((LM_DEBUG, "(%P|%t) connected to %s\n", remote_addr_.get_host_name()));

	return 0;
}

int Client::send_to_server()
{
	for (int i = 0; i < 100; i++)
	{
		if (client_stream_.send_n(data_buf_,
			ACE_OS::strlen(data_buf_) + 1, 0) == -1)
		{
			ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p\n", "send_n"), 0);
			break;
		}
	}
	//关闭连接
	close();
	return 0;
}
//关闭网络连接
int Client::close()
{
	if (client_stream_.close() == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p\n", "close"), -1);
	}
	return 0;
}
*/

#include "ace/OS.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Message_Block.h"
#include "ace/Thread_Manager.h"
#include "ace/INET_Addr.h"
#include "Constants.h"
#include "Client.h"

namespace ACE_Client
{
	Client::Client(const char *server)
	{
		addr = ACE_INET_Addr(SERVER_PORT_NUM, server);
	}

	int Client::svc(void)
	{
		ACE_SOCK_Stream stream;
		ACE_SOCK_Connector connector;

		if (connector.connect(stream, addr) < 0)
		{
			return -1;
		}
		else
		{
			while (1)
			{
				ACE_Message_Block *mb = NULL;
				if (this->getq(mb) == -1)
				{
					break;
				}

				ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Sending %s\n"), &(mb->rd_ptr()[4])));

				stream.send_n(mb->rd_ptr(), mb->length());
				mb->release();

				ACE_OS::sleep(TIME_INTERVAL);
			}
		}
		stream.close();

		return 0;
	}

	char *Client::put_task(char *msg_t, char *msg_s, size_t msg_len)
	{
		for (int i = 0; i < 4; i++)
		{
			msg_t[i] = (char)((msg_len >> (8 * i)) & 0xff);
		}
		ACE_OS::memcpy(&msg_t[4], msg_s, msg_len);

		ACE_Message_Block *mb = NULL;
		ACE_NEW_RETURN(mb, ACE_Message_Block(msg_len + 4, ACE_Message_Block::MB_DATA, 0, msg_t), 0);
		mb->wr_ptr(msg_len + 4);
		this->putq(mb);

		return msg_t;
	}

	void Client::send_tasks(void)
	{
		this->activate();
		ACE_Thread_Manager::instance()->wait();
	}
}
