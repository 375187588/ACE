// ACEClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Client.h"

/*
int main(int argc, char *argv[])
{
	Client client(ACE_OS::atoi("6789"));
	client.connect_to_server();
	client.send_to_server();
    std::cout << "Hello World!\n";
	getchar();
	return 0;
}
*/

#include "Client.h"
using namespace ACE_Client;

int main(int argc, char *argv[])
{
	Client client("localhost"); //服务器的IP地址或者服务器名称

	for (int i = 0; i < 555; i++)
	{
		const char *task1 = "How are you How are you how how?"; //第1个task的数据
		size_t task1_len = strlen(task1)+1; //第1个task的数据长度
		char *task1_t; //无需修改
		ACE_NEW_RETURN(task1_t, char[task1_len + 4], -1); //无需修改
		client.put_task(task1_t, (char*)task1, task1_len); //无需修改

		const char *task2 = "How are you Yes,I'm fine."; //第2个task的数据
		size_t task2_len = strlen(task2)+1; //第2个task的数据长度
		char *task2_t; //无需修改
		ACE_NEW_RETURN(task2_t, char[task2_len + 4], -1); //无需修改
		client.put_task(task2_t, (char*)task2, task2_len); //无需修改

		client.send_tasks(); //将上面的task全部发到服务器

		delete[] task1_t; //释放task1的内存
		delete[] task2_t; //释放task2的内存
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
