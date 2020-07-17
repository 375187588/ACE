#include "Server.h"

/*
int Server::handle_connection()
{
	//从客户端读取数据
	for (int i = 0; i < NO_ITERATIONS; i++)
	{
		int byte_count = 0;
		if ((byte_count = new_stream_.recv_n(data_buf_, SIZE_DATA, 0)) == -1)
		{
			ACE_ERROR((LM_ERROR, "%p\n", "Error in recv"));
		}
		else
		{
			data_buf_[byte_count] = 0;
			ACE_DEBUG((LM_DEBUG, "Server received %s \n", data_buf_));
		}
	}
	if (new_stream_.close() == -1)
	{
		ACE_ERROR((LM_ERROR, "%p\n", "close"));
	}

	return 0;
}

int Server::accept_connections()
{
	if (peer_acceptor_.get_local_addr(server_addr_) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "Error in get_local_addr"), 1);
	}
	ACE_DEBUG((LM_DEBUG, "Starting server at port %d\n", server_addr_.get_port_number()));
	while (1)
	{
		ACE_Time_Value timeout(ACE_DEFAULT_TIMEOUT);
		if (peer_acceptor_.accept(new_stream_, &client_addr_, &timeout) == -1)
		{
			ACE_ERROR((LM_ERROR, "%p\n", "accept"));
			continue;
		}
		else
		{
			ACE_DEBUG((LM_DEBUG,
				"Connection established with remote %s:%d\n",
				client_addr_.get_host_name(), client_addr_.get_port_number()
				));
			//处理连接
			handle_connection();
		}
	}
}
*/
#include "ace/Reactor.h"
#include "Server.h"
namespace ACE_Server
{
	int Server::svc(void)
	{
		int result = ACE_Reactor::instance()->run_reactor_event_loop();
		if (result == -1)
			return -1;

		return 0;
	}
}
