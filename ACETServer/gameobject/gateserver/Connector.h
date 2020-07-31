#pragma once
#include "Ouput.h" 

class Connector : public ACE_Asynch_Connector<Ouput>
{
public:
	Connector();
	virtual ~Connector(void);
	enum { INITIAL_RETRY_DELAY = 3, MAX_RETRY_DELAY = 60, };
	static ACE_Time_Value MaxConnInterVal;

	virtual int validate_connection(
		const ACE_Asynch_Connect::Result &result,
		const ACE_INET_Addr &remote,
		const ACE_INET_Addr &local);

	virtual void handle_time_out(const ACE_Time_Value &, const void *) 
	{
		outLog("%s = %s\n","Connector","handle_time_out");

		connect(m_remoteAddr);
	}
	int reconnect() { return connect(m_remoteAddr); }

protected:
	virtual Ouput* make_handler()
	{
		this->m_lastConnTime = ACE_OS::gettimeofday();
		return OUTPUT_HANDLER::instance();
	}

protected:
	ACE_INET_Addr m_remoteAddr;
	ACE_Time_Value m_lastConnTime;
	int m_retryDelay;
};

typedef ACE_Unmanaged_Singleton<Connector, ACE_Null_Mutex> CLD_CONNECTOR;