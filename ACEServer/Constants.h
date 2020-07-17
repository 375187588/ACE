#pragma once
namespace ACE_Server
{
	static const size_t SERVER_THREAD_POOL_SIZE = 5; //进行数据接收的线程池大小
	static const size_t TASK_THREAD_POOL_SIZE = 5; //进行数据处理的线程池大小
	static const size_t BUFFER_SIZE = 4096; //数据缓冲区大小
	static const size_t SERVER_PORT_NUM = 10101; //服务器的通信端口号
}