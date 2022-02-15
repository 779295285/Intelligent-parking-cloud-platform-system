#include "TCPSocketServer.h"
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		_exit(EXIT_FAILURE);\
	}while (0);

void TCPSocketServer::Start()
{
	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->socket_fd < 0)
	{
		ERR_EXIT("socket_fd:");
	}
	this->Run();
}

void TCPSocketServer::Run()
{
	//���󶨺ͼ���ʵ��
	//IP��ַҲ�� v4 v6���֣�����Ҫ�ѵ�ַ��װ����
	this->my_ip_port = new CAddress();
	this->my_ip_port->setPort(SERVER_PORT);//8090
	this->my_ip_port->setIp("192.168.142.136");
	//���addr����ṹ��
	this->s_addr.sin_family = AF_INET;//Э�����  ipv4
	this->s_addr.sin_port = htons(this->my_ip_port->getPort());//�˿ڣ������ֽ���   �����ֽ���
	this->s_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY����������ip �����ĵ���ip�� 192.168.52.128 ��ô��������ip����192.168.52.128
	//this->s_addr.sin_addr.s_addr =inet_addr("116.62.144.231");
	//inet_aton(this->my_ip_port->getIp(), &s_addr.sin_addr);

	//����˿��ظ�ʹ�ã��˿�����ʹ�ã����õȴ�
	int on = 1;
	int ret = setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (ret < 0)
	{
		ERR_EXIT("setsockopt");
	}
		
	//������ip�˿�
	ret = bind(this->socket_fd, (struct sockaddr*)&this->s_addr, sizeof(struct sockaddr_in));
	if (ret < 0)
	{
		ERR_EXIT("bind");
	}
		
	//��������
	ret = listen(this->socket_fd, SOMAXCONN);
	if (ret < 0)
	{
		ERR_EXIT("listen");
	}
		
}
void TCPSocketServer::Stop()
{

}

TCPSocketServer::TCPSocketServer()
{

}

TCPSocketServer::~TCPSocketServer()
{

}