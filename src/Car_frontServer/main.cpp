#include "EPollServer.h"
#include <stdlib.h>
#include <stdio.h>
//�����Դ�����Ϣ�������׼����
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		_exit(EXIT_FAILURE);\
	}while (0);

int msgid_back;//������Ϣ����id
ShmFifo shm_fifo2(22, 10, 1024);
//�߳����к���
void* pthread_fun(void* arg)
{
	while (1)
	{
		MYBUF* msg = (MYBUF*)malloc(sizeof(MYBUF));
		//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
		int ret = msgrcv(msgid_back, msg, sizeof(MYBUF) - sizeof(long), 2, 0);
		if (ret == -1)
		{
			perror("msgrcv");
		}
		//�ӹ����ڴ������
		char rec_buffer[1024] = { 0 };
		memcpy(rec_buffer, shm_fifo2.shm_read(), sizeof(rec_buffer));

		PACK_HEAD* head = (PACK_HEAD*)malloc(sizeof(PACK_HEAD));
		memcpy(head, rec_buffer, sizeof(PACK_HEAD));
		int fd_temp = 0;
		//��־��ӡ
		PrintLog::getInstance()->send_packCounts++;
		PrintLog::getInstance()->print_log();
		if (head->bodyType == REGIST_TYPE)//ע��
		{

			//����ע�����
			char body_buffer[sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_RL_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD)+sizeof(PACK_RL_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer+ sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == LOGIN_TYPE)//��¼
		{
			char body_buffer[sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_RL_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == CAR_GETIN)//�볡
		{
			char body_buffer[sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_RL_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_RL_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == CAR_GETOUT)//����
		{
			char body_buffer[sizeof(PACK_EXIT_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_EXIT_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_EXIT_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_EXIT_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == CAR_MSG_TYPE)//������Ϣ��
		{
			char body_buffer[sizeof(PACK_ALLCARMSG_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_ALLCARMSG_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_ALLCARMSG_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_ALLCARMSG_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));
		}
		else if (head->bodyType == VIDEO_OPEN)//��Ƶ�򿪰�
		{
			char body_buffer[sizeof(PACK_VIDEO_BACK)] = { 0 };
			memcpy(body_buffer, rec_buffer + sizeof(PACK_HEAD), sizeof(PACK_VIDEO_BACK));
			memcpy(&fd_temp, rec_buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO_BACK), sizeof(int));
			char total_buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO_BACK)] = { 0 };
			memcpy(total_buffer, head, sizeof(PACK_HEAD));
			memcpy(total_buffer + sizeof(PACK_HEAD), body_buffer, sizeof(body_buffer));
			int res = write(fd_temp, total_buffer, sizeof(total_buffer));

		}
		//cout << "fd_temp=" << fd_temp << endl;
		free(msg); //msg��ָ���ڴ汻�ͷţ�����msg��ָ�ĵ�ַ��Ȼ����
		msg = nullptr;
		free(head);
		head = nullptr;
	}
}

int main()
{
	//����һ��������Ϣ����--���߳����ȥ�����ڴ�ȡ������
	msgid_back = msgget(12345, IPC_CREAT | IPC_EXCL | 0666);
	if (msgid_back == -1) //����ʧ�� ��Ϣ�����Ѿ�����
	{
		msgid_back = msgget(12345, 0);//��һ����Ϣ����
	}
	/*
	//��ȡ��ǰϵͳʱ��
	time_t nowTime;
	nowTime = time(NULL);
	struct tm* sysTime = localtime(&nowTime);
	char nowtime_buf[50] = { 0 };
	sprintf(nowtime_buf, "../../../%d-%d-%d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);

	FILE* fp = fopen(nowtime_buf,"wb+");
	*/

	//����ʱע�����-lpthread�������Ե��þ�̬���ӿ⡣��Ϊpthread����Linuxϵͳ��Ĭ�Ͽ�
	pthread_t pthread_id; //�����߳�
	pthread_create(&pthread_id, NULL, pthread_fun, NULL); //����һ���߳�

	EPollServer* epoll_server = new EPollServer();
	epoll_server->Start();
	return 0;
}
