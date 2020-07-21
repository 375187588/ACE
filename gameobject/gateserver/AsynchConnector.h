#pragma once
#include "AsynchOutputHandler.h" 

class AsynchConnector : public ACE_Asynch_Connector<AsynchOutputHandler>
{
public:
	AsynchConnector(void);
	virtual ~AsynchConnector(void);
	enum { INITIAL_RETRY_DELAY = 3, MAX_RETRY_DELAY = 60, };

	virtual int validate_connection(
		const ACE_Asynch_Connect::Result &result,
		const ACE_INET_Addr &remote,
		const ACE_INET_Addr &local);

	virtual void handle_time_out(const ACE_Time_Value &, const void *) { connect(remote_addr_); }
	int reconnect() { return connect(remote_addr_); }

protected:
	virtual AsynchOutputHandler* make_handler()
	{
		return OUTPUT_HANDLER::instance();
	}

protected:
	ACE_INET_Addr remote_addr_;
	int retry_delay_;
};

typedef ACE_Unmanaged_Singleton<AsynchConnector, ACE_Null_Mutex> CLD_CONNECTOR;