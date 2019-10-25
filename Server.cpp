#include <iostream>
#include "Server.h"
using namespace std;

//服务端类构造函数
Server::Server()
{
	//初始化服务器地址和端口
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	//初始化socket
	listener = 0;
	
	//epool fd
	epfd = 0;
}

//初始化服务端并启动监听
void Server::Init()
{
	cout << "Init Server..." << endl;
	
	//创建监听socket
	listener = socket(PF_INET, SOCK_STREAM, 0);
	if (listener < 0)
	{
		perror("listener");
		exit(-1);
	}
	
	//绑定地址
	if(bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("bind error");
		exit(-1);
	}
	
	//监听
	int ret = listen(listener, 5);
	if(ret < 0)
	{
		perror("listen error");
		exit(-1);
	}
	
	cout << "Start to listen: " << SERVER_IP << endl;
	
	//在内核中创建事件表
	epfd = epoll_create(EPOLL_SIZE);
	
	if (epfd < 0)
	{
		perror("epfd error");
		exit(-1);
	}
	
	//往事件表里添加监听事件
	addfd(epfd, listener, true);
}

void Server::Close()
{
	//关闭socket
	close(listener);
	
	//关闭epoll监听
	close(epfd);
}

// 发送广播消息给所有客户端
int Server::SendBroadcastMessage(int clientfd)
{
	//buf[BUF_SIZE] 接受新消息
	//message[BUF_SIZE] 保存格式化的消息
	char buf[BUF_SIZE], message[BUF_SIZE];
	bzero(buf, BUF_SIZE);
	bzero(message, BUF_SIZE);
	
	//接收消息
	cout << "read from client (clientID = " << clientfd << ")" << endl;
	int len = recv(clientfd, buf, BUF_SIZE,0);
	
	//如果客户端关闭了连接
	if(len == 0)
	{
		close(clientfd);
		
		//在客户端列表中删除该客户端
		clients_list.remove(clientfd);
		cout << "ClientID = " << clientfd
			<< " closed.\n now there are "
			<< clients_list.size()
			<< " client in the char room"
			<< endl;
	}
	//发送广播消息给所以客户端
	else
	{
		// 判断是否聊天室还有其他客户端
		if(clients_list.size() == 1)
		{
			//发送提示消息
			send(clientfd, CAUTION, strlen(CAUTION), 0);
			return len;
		}
		//格式化发送的消息内容
		sprintf(message, SERVER_MESSAGE, clientfd, buf);
		
		// 遍历客户端列表依次发送消息，需要判断不要给来源客户端发
		list<int>::iterator it;
		for (it = clients_list.begin(); it != clients_list.end(); ++it)
		{
			if (* it != clientfd)
			{
				if (send(*it, message, BUF_SIZE, 0) < 0)
				{
					return -1;
				}
			}
		}
		
	}
	return len;
}

//启动服务端
void Server::Start()
{
	//epoll 事件队列
	static struct epoll_event events[EPOLL_SIZE];
	
	//初始化
	Init();
	
	while (1)
	{
		//epoll_events_count表示就绪事件的数目
		int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
		
		if (epoll_events_count < 0)
		{
			perror("epoll failure");
			break;
		}
		
		cout << "epoll_events_count =\n" << epoll_events_count << endl;
		
		//处理这epoll_events_count个就绪事件
		for(int i = 0 ; i < epoll_events_count ; ++i)
		{
			int sockfd = events[i].data.fd;
			//新用户连接
			if(sockfd == listener)
			{
				struct sockaddr_in_client_address;
				
			}
		}
		
		
	}
}

