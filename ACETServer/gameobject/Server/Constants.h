#pragma once

const int MIN_BUF_SIZE = 512;
const int MAX_BUF_SIZE = 4096;
const int HALF_BUF_SIZE = 4096;
const int CMD_HEADER_SIZE = 8;
const int INIT_RETRYDELAY = 3;
const int MAX_RETRYDELAY = 60;
const bool OPEN_SYS_LOG = false;//�Ƿ��ϵͳ��־

//ϵͳ��������󳤶�
#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 260
#undef ACE_DEFAULT_SERVICE_PORT
#define ACE_DEFAULT_SERVICE_PORT 2525
#undef ACE_DEFAULT_LOGGING_SERVER_PORT
#define ACE_DEFAULT_LOGGING_SERVER_PORT 5252
#undef ACE_NTRACE
#define ACE_NTRACE 1	//�ر���־
#undef ACE_NLOGGING		//�ر���־