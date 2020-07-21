// ACELogClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Common.h"
#include "ace/OS.h" 
#include "ace/CDR_Stream.h" 
#include "ace/INET_Addr.h" 
#include "ace/SOCK_Connector.h" 
#include "ace/SOCK_Stream.h" 
#include "ace/Log_Msg.h" 
#include "Log_Record.h" 
#include "ace/streams.h" 
#include <string> 

class Logging_Client
{
public:
	int send(const Logging_Record &log_record);
	ACE_SOCK_Stream& peer() { return logging_peer_; }
	~Logging_Client() { logging_peer_.close(); }

private:
	ACE_SOCK_Stream logging_peer_;
};

int Logging_Client::send(const Logging_Record &log_record)
{
	const size_t max_payload_size =
		4 // type
		+ 4 // pid
		+ 8 // timestamp
		+ 4 // msg len
		+ Logging_Record::MAXLOGMSGLEN // data
		+ ACE_CDR::MAX_ALIGNMENT; // padding

	ACE_OutputCDR payload(max_payload_size);
	payload << log_record;
	ACE_CDR::ULong length = payload.total_length();

	ACE_OutputCDR header(ACE_CDR::MAX_ALIGNMENT + 8);
	header << ACE_OutputCDR::from_boolean(ACE_CDR_BYTE_ORDER);
	header << ACE_CDR::ULong(length);

	iovec iov[2] = { 0 };
	iov[0].iov_base = header.begin()->rd_ptr();
	iov[0].iov_len = 8;
	iov[1].iov_base = payload.begin()->rd_ptr();
	iov[1].iov_len = length;

	return logging_peer_.sendv_n(iov, 2);
}

int main(int argc, char* argv[])
{
	u_short logger_port =
		argc > 1 ? atoi(argv[1]) : 0;
	const char *logger_host =
		argc > 2 ? argv[2] : ACE_DEFAULT_SERVER_HOST;

	int result = 0;
	ACE_INET_Addr server_addr;
	if (logger_port != 0)
		result = server_addr.set(logger_port, logger_host);
	else
		result = server_addr.set("ace_logger", logger_host);

	if (result == -1)
		ACE_ERROR_RETURN((LM_ERROR,
			"lookup %s, %p\n",
			logger_port == 0 ? "ace_logger" : argv[1],
			logger_host), 1);

	ACE_SOCK_Connector connector;
	Logging_Client logging_client;
	if (connector.connect(logging_client.peer(), server_addr) < 0)
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "connect()"), 1);

	cin.width(Logging_Record::MAXLOGMSGLEN);
	for (;;) {
		std::string user_input;
		std::getline(cin, user_input, '\n');
		if (!cin || cin.eof())
			break;

		ACE_Time_Value now(ACE_OS::gettimeofday());
		Logging_Record log_record(LM_INFO, ACE_OS::getpid(), now);
		log_record.msg_data(user_input.c_str());
		if (logging_client.send(log_record) == -1)
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "logging_client.send()"), 1);
		else
			ACE_DEBUG((LM_DEBUG, "send: %s\n", log_record.msg_data()));
	}

	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
