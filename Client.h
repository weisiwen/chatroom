#pragma once

#include <string>
#include "Common.h"

using namespace std;

class Client
{
	Client();
	void Connect();
	void Close();
	void Start();
	
private:
	int sock;
	int pid;
	int epfd;
	int pipe_fd[2];
	bool isClientwork;
	char message[BUF_SIZE,];
	struct sockaddr_in serverAdr;
	
};
