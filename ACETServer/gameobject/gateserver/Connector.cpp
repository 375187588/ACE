#include "Common.h"
#include "Connector.h"

ACE_Time_Value Connector::MaxConnInterVal = ACE_Time_Value(5);

Connector::Connector() : 
	m_retryDelay(INITIAL_RETRY_DELAY)
{
	open();
}

Connector::~Connector(void)
{
	delete this;
	outLog("Connector::~Connector(void)\n");
}

int Connector::validate_connection(
	const ACE_Asynch_Connect::Result &result,
	const ACE_INET_Addr &remote,
	const ACE_INET_Addr &local)
{
	outLog("validate new connection to server.\n");
	m_remoteAddr = remote;
	if (!result.success())
	{
		ACE_Time_Value delay(m_retryDelay);
		m_retryDelay *= 2;
		if (m_retryDelay > MAX_RETRY_DELAY)
			m_retryDelay = MAX_RETRY_DELAY;
		outLog("%s = %s\n","validate_connection","!result.success()");
		if (proactor())
		{
			outLog("proactor proactor proactor to server.\n");
			proactor()->schedule_timer(*this, 0, delay);
		}
		else
		{
			outLog("fail proactor() fail.\n");
		}
		return -1;
	}
	m_retryDelay = INITIAL_RETRY_DELAY;

	return 0;
}