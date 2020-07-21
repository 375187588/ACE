#include "Common.h"
#include "AsynchClientDaemon.h"
#include "AsynchConnector.h" 

ACE_FACTORY_DEFINE(ACE_Local_Service, AsynchClientDaemon)
ACE_STATIC_SVC_DEFINE(CLD_Descriptor, ACE_TEXT("AsynchClientDaemon"), \
	ACE_SVC_OBJ_T, &ACE_SVC_NAME(AsynchClientDaemon), \
	ACE_Service_Type::DELETE_OBJ | ACE_Service_Type::DELETE_THIS, 0)
	ACE_STATIC_SVC_REQUIRE(CLD_Descriptor)

	AsynchClientDaemon::AsynchClientDaemon(void)
{
}

AsynchClientDaemon::~AsynchClientDaemon(void)
{
}

#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 260

int AsynchClientDaemon::init(int argc, ACE_TCHAR *argv[])
{
	u_short cld_port = ACE_DEFAULT_SERVICE_PORT;
	u_short sld_port = ACE_DEFAULT_LOGGING_SERVER_PORT;
	ACE_TCHAR sld_host[MAXHOSTNAMELEN] = { 0 };
	ACE_OS_String::strcpy(sld_host, ACE_LOCALHOST);

	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("p:r:s:"), 0);
	get_opt.long_option(ACE_TEXT("client_port"), 'p', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_port"), 'r', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_name"), 's', ACE_Get_Opt::ARG_REQUIRED);

	for (int c; (c = get_opt()) != -1;)
	{
		switch (c)
		{
		case 'p':
			cld_port = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 'r':
			sld_port = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 's':
			ACE_OS_String::strsncpy(sld_host, get_opt.opt_arg(), MAXHOSTNAMELEN);
			break;
		}
	}

	cld_addr_.set(cld_port);
	sld_addr_.set(sld_port, sld_host);
	return activate();
}

int AsynchClientDaemon::svc()
{
	printf("enter svc\n");
	if (acceptor_.open(cld_addr_) == -1)
		return -1;

	printf("acceptor open success\n");
	if (CLD_CONNECTOR::instance()->connect(sld_addr_) == 0)
	{
		printf("connector open success\n");
		ACE_Proactor::instance()->proactor_run_event_loop();
	}

	printf("exit event loop\n");
	acceptor_.close();
	CLD_CONNECTOR::close();
	OUTPUT_HANDLER::close();
	return 0;
}

int AsynchClientDaemon::fini()
{
	ACE_Proactor::instance()->proactor_end_event_loop();
	wait();
	return 0;
}

int AsynchClientDaemon::info(ACE_TCHAR **bufferp, size_t length) const
{
	std::cout << *bufferp << std::endl;
	return -1;
}
