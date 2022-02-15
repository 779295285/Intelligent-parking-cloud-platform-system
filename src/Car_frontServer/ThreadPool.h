#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <iostream>
using namespace std;
//�������Ǵ����ࡣҪ�̳�����࣬��run������ʹ��
class TaskBase
{
public:
	virtual void Run() = 0;
private:
	void* data;
	int fd;
};

class ThreadPool
{
public:
	ThreadPool(int maxCount = 20);
	~ThreadPool();
	void addTask(TaskBase* pTask);
	static void* Routinue(void* pData);//�̳߳��߳����庯��
	//��̬��ԭ��pthread_ create��C���Ժ��������ĵ������������̺߳�����
	//��������ķ����������Դ�thisָ��
private:
	list <TaskBase*> taskList;
	vector<pthread_t> pthread_vector;
	pthread_mutex_t mutex;
	pthread_mutex_t listmutex;
	pthread_cond_t cond;
	int maxCount;
	bool isQuit;

};

#endif // !THREADPOOL_H


