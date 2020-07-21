#pragma once
#include "AsynchAcceptor.h" 

class AsynchClientDaemon : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	AsynchClientDaemon(void);
	virtual ~AsynchClientDaemon(void);

	virtual int init(int argc, ACE_TCHAR *argv[]);
	virtual int fini();
	virtual int info(ACE_TCHAR **bufferp, size_t length = 0) const;
	virtual int svc();

protected:
	ACE_INET_Addr cld_addr_;
	ACE_INET_Addr sld_addr_;
	AsynchAcceptor acceptor_;
};

ACE_FACTORY_DECLARE(ACE_Local_Service, AsynchClientDaemon)