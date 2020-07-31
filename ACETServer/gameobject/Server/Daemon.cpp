#include "Daemon.h"
#include "Common.h"
#include "Connector.h" 

ACE_FACTORY_DEFINE(ACE_Local_Service, Daemon)
ACE_STATIC_SVC_DEFINE(CLD_Descriptor, ACE_TEXT("Daemon"), \
	ACE_SVC_OBJ_T, &ACE_SVC_NAME(Daemon), \
	ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(CLD_Descriptor)


Daemon::Daemon(void)
{
	ACE::debug(OPEN_SYS_LOG);
}

Daemon::~Daemon(void)
{
	m_acceptor.close();
	outLog("Daemon::~Daemon(void)\n");
}

int Daemon::init(int argc, ACE_TCHAR *argv[])
{
	ACE::debug(false);
	u_short nCldPort = ACE_DEFAULT_SERVICE_PORT,
		nSldPort = ACE_DEFAULT_LOGGING_SERVER_PORT;

	ACE_TCHAR szHost[MAXHOSTNAMELEN] = { 0 };
	ACE_OS_String::strcpy(szHost, ACE_LOCALHOST);

	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("p:r:s:"), 0);
	get_opt.long_option(ACE_TEXT("client_port"), 'p', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_port"), 'r', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_name"), 's', ACE_Get_Opt::ARG_REQUIRED);

	int opt = 0;
	while((opt = get_opt()) != -1)
	{
		switch (opt)
		{
		case 'p':
			nCldPort = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 'r':
			nSldPort = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 's':
			ACE_OS_String::strsncpy(szHost, get_opt.opt_arg(), MAXHOSTNAMELEN);
			break;
		}
	}

	m_cldAddr.set(nCldPort);
	m_sldAddr.set(nSldPort, szHost);

	outLog("Daemon::init: cld:%d sld:%d,%s\n", nCldPort, nSldPort, szHost);
	return activate();// (THR_NEW_LWP, 3);
}

int Daemon::svc()
{
	outLog("Enter Daemon::svc\n");
	if (m_acceptor.open(m_cldAddr) == -1)
		return -1;

	outLog("Acceptor open success\n");
	outLog("===================Daemon:Server%d\n", m_sldAddr);
	outLog("\nDaemon::svc connector open success\n");
	MutextConnector::instance()->connect(m_sldAddr);
	if (ACE_Proactor::instance())
	{
		ACE_Proactor::instance()->proactor_run_event_loop();		
	}
	outLog("Exit event loop\n");
	
	MutextConnector::close();
	MutexOutputHandle::close();

	return 0;
}
/*
int Daemon::fini()
{
	std::cout << "Daemon::fini" << std::endl;
	ACE_Proactor::instance()->proactor_end_event_loop();
	ACE_Proactor::instance()->close_singleton();
	wait();
	return 0;
}
*/