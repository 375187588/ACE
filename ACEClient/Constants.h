#pragma once

#include "ace/Time_Value.h"

namespace ACE_Client
{
	static const size_t BUFFER_SIZE = 4096; //数据缓冲区大小
	static const size_t SERVER_PORT_NUM = 10101; //服务器的通信端口号
	static const ACE_Time_Value TIME_INTERVAL(0, 0);// 1000000); //两次数据发送之间的时间间隔(0 s + 1000000 us = 1 s)
}
