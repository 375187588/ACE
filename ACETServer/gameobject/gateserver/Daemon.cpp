#include "Common.h"
#include "Daemon.h"
#include "Connector.h" 

ACE_FACTORY_DEFINE(ACE_Local_Service, Daemon)
ACE_STATIC_SVC_DEFINE(CLD_Descriptor, ACE_TEXT("Daemon"), \
	ACE_SVC_OBJ_T, &ACE_SVC_NAME(Daemon), \
	ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
ACE_STATIC_SVC_REQUIRE(CLD_Descriptor)

Daemon::Daemon(void):
	m_acceptor(new Acceptor)
{
}

Daemon::~Daemon(void)
{
	outLog("Daemon::~Daemon(void)\n");
	if (m_acceptor)
	{
		delete m_acceptor;
		m_acceptor = nullptr;
	}
}

int Daemon::init(int argc, ACE_TCHAR *argv[])
{
	u_short uCldPort = ACE_DEFAULT_SERVICE_PORT;
	u_short uSldPort = ACE_DEFAULT_LOGGING_SERVER_PORT;
	ACE_TCHAR szSldHost[MAXHOSTNAMELEN] = { 0 };
	ACE_OS_String::strcpy(szSldHost, ACE_LOCALHOST);

	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("p:r:s:"), 0);
	get_opt.long_option(ACE_TEXT("client_port"), 'p', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_port"), 'r', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_name"), 's', ACE_Get_Opt::ARG_REQUIRED);

	int nOpt = 0;
	while((nOpt = get_opt()) != -1)
	{
		switch (nOpt)
		{
		case 'p':
			uCldPort = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 'r':
			uSldPort = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 's':
			ACE_OS_String::strsncpy(szSldHost, get_opt.opt_arg(), MAXHOSTNAMELEN);
			break;
		}
	}
	m_cldAddr.set(uCldPort);
	m_sldAddr.set(uSldPort, szSldHost);

	return activate();
}

int Daemon::svc()
{
	outLog("enter svc\n");
	if (m_acceptor->open(m_cldAddr) == -1)
		return -1;

	outLog("acceptor open success\n");
	if (CLD_CONNECTOR::instance()->connect(m_sldAddr) == 0)
	{
		outLog("connector open success\n");
		ACE_Proactor::instance()->proactor_run_event_loop();
	}

	outLog("exit event loop\n");
	m_acceptor->close();
	CLD_CONNECTOR::close();
	OUTPUT_HANDLER::close();
	return 0;
}

int Daemon::fini()
{
	outLog("Daemon::fini()\n");
	ACE_Proactor::instance()->proactor_end_event_loop();
	ACE_Proactor::instance()->close_singleton();
	wait();
	return 0;
}

int Daemon::info(ACE_TCHAR **bufferp, size_t length) const
{
	outLog("Daemon::info():%s\n", *bufferp);
	return -1;
}
