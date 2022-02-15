#include "EPollServer.h"
//����һ�������ڴ�
ShmFifo shm_fifo1(11 ,10, 1024);
map<int, PACK_HEART> EPollServer::socket_map;
pthread_mutex_t EPollServer::mutex;
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		_exit(EXIT_FAILURE);\
	}while (0);

MyTask::MyTask(int fd, int msgid)
{
	this->fd = fd;
	this->msgid = msgid;
}
void* DoHeart(void* arg)
{
	EPollServer* temp = (EPollServer*)arg;
	while (true)
	{
		sleep(1);
		map<int,PACK_HEART>::iterator it;
		for (it = EPollServer::socket_map.begin(); it != EPollServer::socket_map.end(); it++)
		{
			(*it).second.time--;
			//cout << "time=" << (*it).second.time << endl;
			if ((*it).second.time == 0)
			{
				//cout << "closehere" << endl;
				pthread_mutex_lock(&EPollServer::mutex);//��ס
				close((*it).second.fd);
				EPollServer::socket_map.erase(it);
				pthread_mutex_unlock(&EPollServer::mutex);//��ס
			}
		}
	}
}
void MyTask::Run()
{
	//��ȡ��ǰϵͳʱ��
	time_t nowTime;
	nowTime = time(NULL);
	struct tm* sysTime = localtime(&nowTime);
	char nowtime_buf[50] = { 0 };
	char nowtime_buffer[50] = { 0 };
	sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
	sprintf(nowtime_buffer, "%d-%02d-%02d/%02d:%02d:%02d", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday,
														   sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);

	//�пͻ��˷������ݹ���
	MYBUF msg;
	msg.mtype = 1;
	PACK_HEAD head;
	bzero(&head, sizeof(PACK_HEAD));
	int ret = read(this->fd, &head, sizeof(head));
	//���з�������
	if (ret > 0)
	{
		PrintLog::getInstance()->rec_packCounts++;
		if (head.bodyType == REGIST_TYPE)//ע��
		{

			PACK_REGIST_LOGIN regist_login;
			int res = read(this->fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
			PrintLog::getInstance()->write_login_log(nowtime_buffer, "ע��", 100, "123", regist_login.name, regist_login.pwd);

			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				//����Ϣ���з���Ϣ
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}
		}
		else if (head.bodyType == LOGIN_TYPE)//��¼
		{
			PrintLog::getInstance()->userCounts++;
			PACK_REGIST_LOGIN regist_login;
			int res = read(this->fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
			PrintLog::getInstance()->write_login_log(nowtime_buffer,"��¼",100,"123",regist_login.name,regist_login.pwd);

			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), &this->fd, sizeof(int));
			if (res > 0)
			{
				cout << "msg=" << head.bodyType << endl;
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}
		}
		else if (head.bodyType == CAR_GETIN)//�볡��
		{
			PACK_ENTER car_enter;
			int res = read(this->fd, &car_enter, sizeof(PACK_ENTER));
			PrintLog::getInstance()->write_carin_log(nowtime_buffer, "�볡", 100, "123", car_enter.car_num,this->fd,car_enter.now_time);
			cout << "car1=" << car_enter.number << endl;
			cout << "car2=" << car_enter.car_num << endl;
			cout << "car3=" << car_enter.location << endl;
			cout << "car4=" << car_enter.now_time << endl;
			cout << "car5=" << car_enter.photo_path << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_ENTER) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &car_enter, sizeof(PACK_ENTER));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_ENTER), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}

		}
		else if (head.bodyType == CAR_GETOUT)//������
		{
			PACK_EXIT car_exit;
			int res = read(this->fd, &car_exit, sizeof(PACK_EXIT));
			PrintLog::getInstance()->write_carout_log(nowtime_buffer, "����", 100, "123", car_exit.car_number, this->fd, car_exit.out_time);
			//cout << "car1=" << car_exit.car_number << endl;
			//cout << "car2=" << car_exit.out_time << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_EXIT) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &car_exit, sizeof(PACK_EXIT));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_EXIT), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}

		}
		else if (head.bodyType == HEART_TYPE)//������
		{
			map<int,PACK_HEART>::iterator it=EPollServer::socket_map.find(this->fd);
			if (it!= EPollServer::socket_map.end())
			{
				pthread_mutex_lock(&EPollServer::mutex);//��ס
				(*it).second.time = 128;
				cout << "hearthere" << endl;
				pthread_mutex_unlock(&EPollServer::mutex);//����
			}
		}
		else if (head.bodyType == PHOTO_TYPE)//ͼƬ��
		{
			PACK_PHOTO photo_pack;
			PACK_PHOTO_BACK photo_result;
			int res = read(this->fd, &photo_pack, sizeof(PACK_PHOTO));
			char fileName[24] = { 0 };
			sprintf(fileName, "./photo/%s", photo_pack.filename);
			cout << photo_pack.filename << endl;
			FILE* fp;                //����һ���ļ�ָ��
			fp = fopen(fileName,"a+");   //�Զ�д��ʽ�򿪻���һ���������ļ����������д��
			fwrite(photo_pack.context, photo_pack.realSize,1, fp);
			fclose(fp);
			photo_result.result = 1;
			char sendbuffer[sizeof(PACK_HEAD) + sizeof(PACK_PHOTO_BACK)] = { 0 };
			memcpy(sendbuffer, &head, sizeof(PACK_HEAD));
			memcpy(sendbuffer + sizeof(PACK_HEAD), &photo_result, sizeof(PACK_PHOTO_BACK));
			write(this->fd, sendbuffer, sizeof(sendbuffer));
		}
		else if (head.bodyType == CAR_MSG_TYPE)//������Ϣ��
		{
			PACK_CARMSG car_msg;
			int res = read(this->fd, &car_msg, sizeof(PACK_CARMSG));
			PrintLog::getInstance()->write_carmsg_log(nowtime_buffer,"��ѯ",100,"123", car_msg.car_number,car_msg.in_time,car_msg.out_time);
			//cout << "car1=" << car_msg.car_number << endl;
			//cout << "car2=" << car_msg.page << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_CARMSG) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &car_msg, sizeof(PACK_CARMSG));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_CARMSG), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}
		}
		else if (head.bodyType == VIDEO_TYPE)//��Ƶ��Ϣ��
		{
			PACK_VIDEO video;
			int res = read(this->fd, &video, sizeof(PACK_VIDEO));
			PrintLog::getInstance()->write_video_log(nowtime_buffer, "�����Ƶ", 100, "123", video.video_name, video.totalFrameCount);
			//cout << "car1=" << video.video_name << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &video, sizeof(PACK_VIDEO));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), &this->fd, sizeof(int));
			if (res > 0)
			{
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}
		}
		else if (head.bodyType == VIDEO_OPEN)//��Ƶ�򿪰�
		{
			PrintLog::getInstance()->play_counts++;

			PACK_VIDEO video;
			int res = read(this->fd, &video, sizeof(PACK_VIDEO));
			
			//cout << "car1=" << video.video_name << endl;
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &video, sizeof(PACK_VIDEO));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), &this->fd, sizeof(int));
			if (res > 0)
			{
				//cout << "msg=" << head.bodyType << endl;
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}
		}
		else if (head.bodyType == VIDEO_CLOSE)//��Ƶ�رհ�
		{

			PACK_VIDEO video_close;
			int res = read(this->fd, &video_close, sizeof(PACK_VIDEO));
			char buffer[sizeof(PACK_HEAD) + sizeof(PACK_VIDEO) + sizeof(int)] = { 0 };
			//������ͷ
			memcpy(buffer, &head, sizeof(PACK_HEAD));
			//��������
			memcpy(buffer + sizeof(PACK_HEAD), &video_close, sizeof(PACK_VIDEO));
			//����fd
			memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_VIDEO), &this->fd, sizeof(int));
			if (res > 0)
			{
				//cout << "msg=" << head.bodyType << endl;
				shm_fifo1.shm_write(buffer);
				int res = msgsnd(msgid, &msg, sizeof(MYBUF) - sizeof(long), 0);//��Ҫ��ȥ��Ϣ����sizeof(long)�����δ���Խ��
			}
		}
		PrintLog::getInstance()->print_log();
	}
	else
	{
		//�ͻ��˶Ͽ�����
		cout << "client error close" << endl;
		//EPollServer::Epoll_Del(this->fd, EPOLLIN);
		close(this->fd);
		return;
	}
}
EPollServer::EPollServer()
{
	pool = new ThreadPool(20);
	//����һ����Ϣ����
	msgid = msgget(1234, IPC_CREAT | IPC_EXCL | 0666);
	if (msgid == -1)//����ʧ�� ��Ϣ�����Ѿ�����
	{
		msgid = msgget(1234, 0);//��һ����Ϣ����
	}
	pthread_mutex_init(&mutex, NULL);
	
}

EPollServer::~EPollServer()
{
}

void EPollServer::Start()
{
	this->epoll_fd = epoll_create(10);//����epoll����,�� Linux �ں� 2.6.8 �汾�Ժ���������Ǳ����Ե�,������������
	if (this->epoll_fd < 0)
	{
		ERR_EXIT("epoll error:");
		exit(0);
	}
	this->Run();
}
void EPollServer::Run()
{
	//����TCP����
	this->server_socket = new TCPSocketServer();
	this->server_socket->Start();

	//���������߳�
	pthread_create(&pthread_id, NULL, DoHeart, this);
	cout << "FrontServer open!!!!" << endl;
	//������׽��ֺͼ����¼�����ϵͳ
	this->Epoll_Add(this->server_socket->getSocketFd(), EPOLLIN); //�ѷ���������׽��ִ���epoll�ڲ�

	while (true)
	{
		//��ִ��epoll_event������ʱ�����������������ֱ��ĳ���׽��������ݲŻ������
		//������������ͬһʱ�����������ٸ��׽��֣�һ��Ϊǰ�洴���������С��
		//���ĸ��������û���׽��ֵ�ʱ��һֱ������Ϊ-1
		//epoll_wait�ķ���ֵ�����ĳһʱ���ж���׽��������ݣ�epoll_wait�ķ���ֵ���Ǽ�¼�����¼�����
		//���� epoll_wait() �������ں�ȥ���������¼��������������¼��ŵ������б��в����أ�ͨ�����ص��¼���������һ�����¼�����
		int size = sizeof(epoll_events) / sizeof(epoll_events[0]);
		epoll_wait_return = epoll_wait(this->epoll_fd, this->epoll_events, size, -1);
		
		for (int i = 0; i < this->epoll_wait_return; i++)
		{
			//��ʱ���ж��Ƿ��Ǽ���sockfd�׽��ֵ�������������Ǿͽ�������
			if (epoll_events[i].data.fd == this->server_socket->getSocketFd())
			{
				//�ͻ�������
				//����˵��������sock���¼�������˵�����µĿͻ�������
				//accept���� һ���µ��ļ�������������ͨ�ŵģ�read write��  con_fd���ǿͻ���
				//���������������Ϊ�� ��ʾ�Կͻ��˵�ip��ַ�Ͷ˿ڲ�����
				//accept������һ�������Ǽ������ļ����������ڶ�����������������ͻ����׽��ֶ�Ӧ�ġ��ط����ͻ���IP�Ͷ˿���Ϣ�ȣ���������������ռ�ش�С�������ݲ���ð��NULL
				int con_fd = accept(this->server_socket->getSocketFd(), NULL, NULL);
				cout << "client connect!!" << endl;
				
				PACK_HEART heart;
				heart.fd = con_fd;
				heart.time = 128;
				pthread_mutex_lock(&mutex);//��ס
				
				this->Epoll_Add(con_fd, EPOLLIN); 
				socket_map.insert(pair<int, PACK_HEART>(con_fd, heart));

				pthread_mutex_unlock(&mutex);//����
				//��־��ӡ
				PrintLog::getInstance()->effec_conCounts++;
				PrintLog::getInstance()->print_log();
			}
			//ͨѶ
			else if (epoll_events[i].events && EPOLLIN)
			{
				//this->epoll_recv(epoll_events[i].data.fd, this->msgid);
				MyTask* task = new MyTask(epoll_events[i].data.fd, this->msgid);
				pool->addTask(task);
				cout << "msg!!!" << endl;
				PrintLog::getInstance()->rec_packCounts++;
				PrintLog::getInstance()->print_log();
				/*
				PrintLog::getInstance()->rec_packCounts++;
				PrintLog::getInstance()->print_log();
				//�пͻ��˷������ݹ���
				PACK_HEAD head;
				int ret = read(epoll_events[i].data.fd, &head, sizeof(head));
				//���з�������
				if (ret > 0)
				{
					if (head.bodyType == REGIST_TYPE)//ע��
					{
						PACK_REGIST_LOGIN regist_login;
						int res = read(epoll_events[i].data.fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
						char head_buffer[sizeof(PACK_HEAD)] = { 0 };
						memcpy(head_buffer, &head, sizeof(PACK_HEAD));

						char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN)+sizeof(int)] = { 0 };
						//������ͷ
						memcpy(buffer, &head, sizeof(PACK_HEAD));
						//��������
						memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
						//����fd
						memcpy(buffer + sizeof(PACK_HEAD)+sizeof(PACK_REGIST_LOGIN), &epoll_events[i].data.fd, sizeof(int));
						if (res > 0)
						{
							cout << "msg=" << head.bodyType << endl;
							shm_fifo1.shm_write(buffer);
							this->epoll_recv(epoll_events[i].data.fd,buffer);
						}
					}
					else if (head.bodyType == LOGIN_TYPE)//��¼
					{
						PACK_REGIST_LOGIN regist_login;
						int res = read(epoll_events[i].data.fd, &regist_login, sizeof(PACK_REGIST_LOGIN));
						char head_buffer[sizeof(PACK_HEAD)] = { 0 };
						memcpy(head_buffer, &head, sizeof(PACK_HEAD));

						char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN) + sizeof(int)] = { 0 };
						//������ͷ
						memcpy(buffer, &head, sizeof(PACK_HEAD));
						//��������
						memcpy(buffer + sizeof(PACK_HEAD), &regist_login, sizeof(PACK_REGIST_LOGIN));
						//����fd
						memcpy(buffer + sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN), &epoll_events[i].data.fd, sizeof(int));
						if (res > 0)
						{
							cout << "msg=" << head.bodyType << endl;
							shm_fifo1.shm_write(buffer);
							this->epoll_recv(epoll_events[i].data.fd,buffer);
						}
					}
				}

				else
				{
					//�ͻ��˶Ͽ�����
					cout << "client error close" << endl;
					close(epoll_events[i].data.fd);
					break;
				}
				*/
			}
			
		}
	}
}
void EPollServer::Stop()
{

}
void EPollServer::epoll_recv(int fd, int msgid)
{
	MyTask* task;
	task = new MyTask(fd, msgid);
	pool->addTask(task);
	PrintLog::getInstance()->rec_packCounts++;
	PrintLog::getInstance()->print_log();
}

bool EPollServer::Epoll_Add(int fd, int event_type)
{
	//accept�����������µ��׽��֣�Ҫ������µ��׽��ִ����ں�������
	this->event.data.fd = fd; //��¼�ļ�������
	this->event.events = event_type | EPOLLET;  //������׽����¼����¼�
	//epoll_ctl�����������ǹ�������ʵ���ϵĽڵ㣬���Խ�����ӡ�ɾ�����޸Ĳ���
	int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);//�ѷ���������׽��ִ���epoll�ڲ�
	if (res < 0)
	{
		ERR_EXIT("epoll_ctl");
		return false;
	}
	return true;
}
bool EPollServer::Epoll_Del(int fd, int event_type)
{
	this->event.data.fd = fd;
	this->event.events = event_type; //������׽����¼�����Ȥ
	int res = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);  //�ѷ���������׽��ִ�epoll�ڲ�ɾ��
	if (res < 0)
	{
		ERR_EXIT("epoll_ctl");
		return false;
	}
	return true;
}