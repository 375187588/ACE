#pragma once

const int MIN_BUF_SIZE = 512;
const int MAX_BUF_SIZE = 4096;
const int HALF_BUF_SIZE = 4096;
const int CMD_HEADER_SIZE = 8;
const int INIT_RETRYDELAY = 3;
const int MAX_RETRYDELAY = 60;
const bool OPEN_SYS_LOG = false;//是否打开系统日志

//系统主机名最大长度
#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 260
#undef ACE_DEFAULT_SERVICE_PORT
#define ACE_DEFAULT_SERVICE_PORT 2525
#undef ACE_DEFAULT_LOGGING_SERVER_PORT
#define ACE_DEFAULT_LOGGING_SERVER_PORT 5252
#undef ACE_NTRACE
#define ACE_NTRACE 1	//关闭日志
#undef ACE_NLOGGING		//关闭日志