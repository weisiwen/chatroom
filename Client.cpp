#include <iostream>
#include "Client.h"

using namespace  std;

Client::Client()
{
	//��ʼ��Ҫ���ӵķ�������ַ�Ͷ˿�
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	//��ʼ��socket
	sock  = 0;
	// ��ʼ�����̺�
	pid = 0;
	//�ͻ���״̬
	isClientwork = true;
	//epool fd
	epfd = 0;
}

//���ӷ�����
void Client::Connect()
{
	cout << "Connect Server: " << SERVER_IP << " : " << SERVER_PORT << endl;
	
	//����socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("sock error");
		exit(-1);
	}
	
	//���ӷ����
	if(connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("connect error");
		exit(-1);
	}
	
	
}