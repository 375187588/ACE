#pragma once
#include "Acceptor.h" 

class Daemon : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	Daemon(void);
	virtual ~Daemon(void);

	virtual int init(int argc, ACE_TCHAR *argv[]);
	virtual int svc();
	//virtual int fini();

protected:
	ACE_INET_Addr m_cldAddr;
	ACE_INET_Addr m_sldAddr;
	Acceptor m_acceptor;
};

ACE_FACTORY_DECLARE(ACE_Local_Service, Daemon)
