#ifndef CSOCKETBASE_H
#define CSOCKETBASE_H

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <vector>
#include <pthread.h>
#include <sys/epoll.h>
using namespace std;
//������
class CSocketBase
{
public:
	CSocketBase()
	{
		//printf("CSocketBase create!!!\n");
	}
	~CSocketBase()
	{
		//printf("CSocketBase destroy!!!\n");
	}
	int getSocketFd()
	{
		return this->socket_fd;
	}
protected:
	int socket_fd;//�׽����ļ�������
	virtual void Start() = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;
};


#endif 