// ACEConnectServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/Mem_Map.h>
#include <ace/Log_Msg.h>
#include <ace/Truncate.h>

char* get_url_pathname(ACE_SOCK_Stream * peer)
{
	static const char* docBase = "./pages";
	char buf[512];
	memset(buf, 0, sizeof(buf));
	if (peer->recv(buf, sizeof(buf)) != -1 &&
		buf[0] != '\0' )
	{
		const char* pos1 = strchr(buf, '  ');
		const char* pos2 = strchr(++pos1, '  ');
		const size_t len = pos2 - pos1;
		const size_t docLen = std::strlen(docBase);
		char* path = new char[len + docLen + 1];
		memcpy(path, docBase, docLen);
		memcpy(path + docLen, pos1, len);
		path[len + docLen] = '\0';

		return path;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	//ACE_Mem_Map mmap;
	//if (mmap.map("F:\\AStudy\\ACE\\ACEConnect\\Debug\\pages\\index.html", static_cast<size_t> (-1), O_RDWR) == -1)
	//	ACE_ERROR_RETURN((LM_ERROR, "%n: %p\n", "mmap"), -1);
	////获取文件的大小
	//printf("length=%d\n", (ACE_Utils::truncate_cast<int>)(mmap.size()));
	////获取文件的内容
	//printf("%s", mmap.addr());
	//getchar();
	//return 0;
	ACE_INET_Addr server_addr;
	ACE_SOCK_Acceptor acceptor;
	ACE_SOCK_Stream peer;
	if (server_addr.set(8868) == -1)
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "set()"), 1);
	else
		std::cout << "set 8868 success!" << std::endl;

	if (acceptor.open(server_addr) == -1)
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "open()"), 1);
	else
		std::cout << "acceptor.open success!" << std::endl;

	for (;;)
	{
		if (acceptor.accept(peer) == -1)
		{
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "accept()"), 1);
		}

		peer.disable(ACE_NONBLOCK);
		char* pathname = get_url_pathname(&peer);
		//std::string strPath = "F:\\AStudy\\ACE\\ACEConnect\\Debug\\";
		//strPath += pathname;
		//ACE_Mem_Map Mmapped_file(strPath.c_str());
		//ACE_Mem_Map Mmapped_file("F:\\AStudy\\ACE\\ACEConnect\\Debug\\pages\\index.html");
		ACE_Mem_Map Mmapped_file(pathname);
		if (peer.send_n(Mmapped_file.addr(), Mmapped_file.size()) == -1)
		{
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "send_n()"), 1);
		}
		delete[]pathname;
		peer.close();
	}

	return acceptor.close() == -1?1:0;
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
