#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include "MemoryAlloc.h"
#include "MemoryMgr.hpp"
#include "Constants.h"
#include "ace/Message_Queue.h"
#include "ace/Asynch_IO.h"
#include "ace/OS.h"
#include "ace/Proactor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/os_ns_time.h" 
#include "ace/Time_value.h" 
#include "ace/cdr_stream.h" 
#include "ace/SOCK_Stream.h" 
#include "ace/Condition_Thread_Mutex.h" 
#include "ace/Thread_Mutex.h" 
#include "ace/Message_Queue_T.h" 
#include "ace/Thread_Manager.h" 
#include "ace/Service_Object.h" 
#include "ace/Get_Opt.h" 
#include "ace/os_ns_string.h" 
#include "ace/signal.h" 
#include "ace/Service_Config.h" 
#include "ace/Svc_Handler.h" 
#include "ace/Log_Record.h" 
#include "ace/Acceptor.h" 
#include "ace/Connector.h" 
#include "ace/Asynch_Acceptor.h" 
#include "ace/Asynch_Connector.h" 
#include "ace/Service_Config.h" 

#include "Logger.h"

using ReadStream = ACE_Asynch_Read_Stream::Result;
using WriteStream = ACE_Asynch_Write_Stream::Result;
