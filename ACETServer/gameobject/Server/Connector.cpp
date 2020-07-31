#include "Connector.h"

Connector::Connector():
	m_nRetryDelay(INIT_RETRYDELAY)
{
	ACE::debug(OPEN_SYS_LOG);
	open();	
}

Connector::~Connector()
{
	ACE_Proactor::instance()->cancel_timer(this->m_lTimeHandle);
	outLog("Connector::~Connector()\n");
}

void Connector::handle_time_out(const ACE_Time_Value &, const void *)
{
	outLog("Connector handle_time_out in:%d\n", m_remoteAddr);
	cancel();
	int nResult = connect(m_remoteAddr);
	if (nResult == -1)
	{
		outLog("handle_time_out 超时 连接远程失败:%d\n", nResult);
	}
	else
	{
		outLog("handle_time_out 超时 连接远程成功:%d\n", nResult);
		ACE_Proactor::instance()->cancel_timer(this->m_lTimeHandle);
	}
}

int Connector::validate_connection(
	const ACE_Asynch_Connect::Result &result,
	const ACE_INET_Addr &remote,
	const ACE_INET_Addr &local)
{
	outLog("Connector validate_connection in\n");
	m_remoteAddr = remote;
	if (!result.success())
	{
		outLog("validate_connection fail\n");
		ACE_Time_Value delay(m_nRetryDelay);
		m_nRetryDelay *= 2;
		if (m_nRetryDelay > MAX_RETRYDELAY)
			m_nRetryDelay = MAX_RETRYDELAY;
		if(ACE_Proactor::instance())
		{
			m_lTimeHandle = ACE_Proactor::instance()->schedule_timer(*this, nullptr, delay);
			outLog("validate_connection fail schedule_timer\n");
		}
		else
		{
			outLog("validate_connection fail schedule_timer fail\n");
		}
		return -1;
	}
	m_nRetryDelay = INIT_RETRYDELAY;

	return 0;
}