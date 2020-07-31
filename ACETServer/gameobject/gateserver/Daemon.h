#pragma once
#include "Acceptor.h" 

class Daemon : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	Daemon(void);
	virtual ~Daemon(void);

	virtual int init(int argc, ACE_TCHAR *argv[]);
	virtual int fini();
	virtual int info(ACE_TCHAR **bufferp, size_t length = 0) const;
	virtual int svc();

protected:
	ACE_INET_Addr m_cldAddr;
	ACE_INET_Addr m_sldAddr;
	Acceptor* m_acceptor;
};

ACE_FACTORY_DECLARE(ACE_Local_Service, Daemon)