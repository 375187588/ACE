// ACEConnect.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <ace/INET_Addr.h>
#include <ace/Log_Msg.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>

const short BUFSIZE = 1024;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!\n";
	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;
	ACE_INET_Addr peerAddr;
	if (peerAddr.set(8868, "127.0.0.1") == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "peerAddr.set()"), 1);
		return 1;
	}
	//发起阻塞式的连接
	if (connector.connect(peer, peerAddr) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "connector.connect()"), 1);
		return 1;
	}
	char buf[BUFSIZE];
	iovec iov[3];
	iov[0].iov_base = (char*)"GET ";
	iov[0].iov_len = 4;
	iov[1].iov_base = (char*)"index.html";
	iov[1].iov_len = 11;
	iov[2].iov_base = (char*)" HTTP/1.0\r\n\r\n";
	iov[2].iov_len = 13;
	if (peer.sendv_n(iov, 3) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "sendv_n()"), 1);
	}

	int index = 0;
	memset(buf, 0, sizeof(buf));
	for (ssize_t n; (n = peer.recv(buf, sizeof(buf)) > 0);)
	{
		ACE::write_n(ACE_STDOUT, buf, n);
		index += n;
	}

	std::cout << buf << std::endl << std::endl;
	peer.close();
	std::cout << "end\n" << std::endl;
	getchar();
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
