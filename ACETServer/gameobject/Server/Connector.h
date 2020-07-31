#pragma once
#include "Output.h" 

class Connector : public ACE_Asynch_Connector<Output>
{
public:
	Connector();
	virtual ~Connector();

public:
	virtual void handle_time_out(const ACE_Time_Value &, const void *);
	virtual int validate_connection(
		const ACE_Asynch_Connect::Result &result,
		const ACE_INET_Addr &remote,
		const ACE_INET_Addr &local);

	int reconnect() 
	{
		outLog("int reconnect ================ \n");
		cancel();
		return connect(m_remoteAddr); 
	}

	virtual Output* make_handler()
	{
		return MutexOutputHandle::instance();
	}

protected:
	ACE_INET_Addr m_remoteAddr;
	int m_nRetryDelay;
	long m_lTimeHandle;
};

using MutextConnector = ACE_Unmanaged_Singleton<Connector, ACE_Null_Mutex>;