#include "ThreadPool.h"
ThreadPool::ThreadPool(int maxCount)
{
	pthread_t pthread_id;
	this->maxCount = maxCount;
	pthread_mutex_init(&this->mutex, NULL);
	pthread_cond_init(&this->cond, NULL);
	this->isQuit = false;
	//�̳߳ؿ���һ��ʼ�ʹ���N���߳�
	for (int i = 0; i < this->maxCount; i++)
	{
		pthread_create(&pthread_id, NULL, ThreadPool::Routinue, this);
		pthread_vector.push_back(pthread_id);
	}
}
ThreadPool::~ThreadPool()
{

}
void ThreadPool::addTask(TaskBase* pTask)
{
	pthread_mutex_lock(&mutex);

	this->taskList.push_back(pTask);
	pthread_cond_signal(&cond);

	pthread_mutex_unlock(&mutex);
}
void* ThreadPool::Routinue(void* pData)//�̳߳��߳����庯��
{
	TaskBase* pTask;
	ThreadPool* pool = (ThreadPool*)pData;
	//cout << "create a new thread" << endl;
	while (!(pool->isQuit))
	{
		pthread_mutex_lock(&pool->mutex);

		//��������б��Ƿ�Ϊ��
		if (pool->taskList.empty())
		{
			//cout << "thread wait..." << endl;
			pthread_cond_wait(&pool->cond, &pool->mutex);

			pthread_mutex_unlock(&pool->mutex);
		}
		else//˵�����в�Ϊ�� ȡ����
		{
			pTask = pool->taskList.front();//ȡ����һ������
			pool->taskList.pop_front();//ɾ����һ������

			pthread_mutex_unlock(&pool->mutex);
			//���������б������
			pTask->Run();
		}
	}
}