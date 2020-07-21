#include "Common.h" 
#include "UserEventHandler.h"

#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 760

ACE_HANDLE UserEventHandler::get_handle() const
{
	UserHandler &h = const_cast<UserHandler&>(logging_handler_);
	return h.peer().get_handle();
}

int UserEventHandler::open()
{
	static const char LOGFILE_SUFFIX[] = ".log";
	char filename[MAXHOSTNAMELEN + sizeof(LOGFILE_SUFFIX)] = { 0 };
	ACE_INET_Addr logging_peer_addr;

	logging_handler_.peer().get_remote_addr(logging_peer_addr);
	logging_peer_addr.get_host_name(filename, MAXHOSTNAMELEN);
	ACE_OS::strcat(filename, LOGFILE_SUFFIX);

	ACE_FILE_Connector connector;
	connector.connect(log_file_,
		ACE_FILE_Addr(filename),
		0,
		ACE_Addr::sap_any,
		0,
		O_RDWR | O_CREAT | O_APPEND,
		ACE_DEFAULT_FILE_PERMS);

	return reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
}

int UserEventHandler::handle_input(ACE_HANDLE)
{
	return logging_handler_.log_record();
}

int UserEventHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
	logging_handler_.close();
	log_file_.close();
	delete this;
	return 0;
}