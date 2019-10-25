#include <iostream>
#include "Server.h"
using namespace std;

//������๹�캯��
Server::Server()
{
	//��ʼ����������ַ�Ͷ˿�
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	//��ʼ��socket
	listener = 0;
	
	//epool fd
	epfd = 0;
}

//��ʼ������˲���������
void Server::Init()
{
	cout << "Init Server..." << endl;
	
	//��������socket
	listener = socket(PF_INET, SOCK_STREAM, 0);
	if (listener < 0)
	{
		perror("listener");
		exit(-1);
	}
	
	//�󶨵�ַ
	if(bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("bind error");
		exit(-1);
	}
	
	//����
	int ret = listen(listener, 5);
	if(ret < 0)
	{
		perror("listen error");
		exit(-1);
	}
	
	cout << "Start to listen: " << SERVER_IP << endl;
	
	//���ں��д����¼���
	epfd = epoll_create(EPOLL_SIZE);
	
	if (epfd < 0)
	{
		perror("epfd error");
		exit(-1);
	}
	
	//���¼�������Ӽ����¼�
	addfd(epfd, listener, true);
}

void Server::Close()
{
	//�ر�socket
	close(listener);
	
	//�ر�epoll����
	close(epfd);
}

// ���͹㲥��Ϣ�����пͻ���
int Server::SendBroadcastMessage(int clientfd)
{
	//buf[BUF_SIZE] ��������Ϣ
	//message[BUF_SIZE] �����ʽ������Ϣ
	char buf[BUF_SIZE], message[BUF_SIZE];
	bzero(buf, BUF_SIZE);
	bzero(message, BUF_SIZE);
	
	//������Ϣ
	cout << "read from client (clientID = " << clientfd << ")" << endl;
	int len = recv(clientfd, buf, BUF_SIZE,0);
	
	//����ͻ��˹ر�������
	if(len == 0)
	{
		close(clientfd);
		
		//�ڿͻ����б���ɾ���ÿͻ���
		clients_list.remove(clientfd);
		cout << "ClientID = " << clientfd
			<< " closed.\n now there are "
			<< clients_list.size()
			<< " client in the char room"
			<< endl;
	}
	//���͹㲥��Ϣ�����Կͻ���
	else
	{
		// �ж��Ƿ������һ��������ͻ���
		if(clients_list.size() == 1)
		{
			//������ʾ��Ϣ
			send(clientfd, CAUTION, strlen(CAUTION), 0);
			return len;
		}
		//��ʽ�����͵���Ϣ����
		sprintf(message, SERVER_MESSAGE, clientfd, buf);
		
		// �����ͻ����б����η�����Ϣ����Ҫ�жϲ�Ҫ����Դ�ͻ��˷�
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

//���������
void Server::Start()
{
	//epoll �¼�����
	static struct epoll_event events[EPOLL_SIZE];
	
	//��ʼ��
	Init();
	
	while (1)
	{
		//epoll_events_count��ʾ�����¼�����Ŀ
		int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
		
		if (epoll_events_count < 0)
		{
			perror("epoll failure");
			break;
		}
		
		cout << "epoll_events_count =\n" << epoll_events_count << endl;
		
		//������epoll_events_count�������¼�
		for(int i = 0 ; i < epoll_events_count ; ++i)
		{
			int sockfd = events[i].data.fd;
			//���û�����
			if(sockfd == listener)
			{
				struct sockaddr_in_client_address;
				
			}
		}
		
		
	}
}

