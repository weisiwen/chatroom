#include <iostream>
#include "Client.h"

using namespace  std;

Client::Client()
{
	//初始化要连接的服务器地址和端口
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	//初始化socket
	sock  = 0;
	// 初始化进程号
	pid = 0;
	//客户端状态
	isClientwork = true;
	//epool fd
	epfd = 0;
}

//连接服务器
void Client::Connect()
{
	cout << "Connect Server: " << SERVER_IP << " : " << SERVER_PORT << endl;
	
	//创建socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("sock error");
		exit(-1);
	}
	
	//连接服务端
	if(connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("connect error");
		exit(-1);
	}
	
	
}