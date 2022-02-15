#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include<iostream>
#include<malloc.h>
using namespace std;

typedef struct ShmHead
{
    int r_index;
    int w_index;
    int blocks;
    int block_size;
}SHM_HEAD_T;

class ShmFifo
{
public:
    SHM_HEAD_T* shm_start_addr;
    char* shm_data_add;//�����ڴ�ʵ�����ݵ�ַ(�����洢���ݿ�ʼ�ĵ�ַ)
    int shm_id;//�����ڴ�id
    ShmFifo(int key, int blocks, int block_size)
    {
        //����ں��в����ڼ�ֵ��key��ȵĹ����ڴ棬���½�һ����Ϣ���У�������������Ĺ����ڴ��򱨴�
        this->shm_id = shmget(key, blocks * block_size, IPC_CREAT | IPC_EXCL);
        if (this->shm_id == -1) //�Ѿ������������ڴ���
        {
            //����ں��в����ڼ�ֵ��key��ȵĹ����ڴ棬���½�һ�������ڴ棻������������Ĺ����ڴ棬���ش˹����ڴ�ı�ʶ��
            this->shm_id = shmget(key, blocks * block_size, IPC_CREAT);
            //�����ڴ���ʼ��ַ����ͷ���ṹ������
            this->shm_start_addr = (SHM_HEAD_T*)shmat(this->shm_id, 0, 0);
            //�������ʵ�ʵ�ַ
            this->shm_data_add = (char*)(this->shm_start_addr + 1);
        }
        else//��һ�δ�����Ҫ����ͷ��ַ�����ݿ�ʼ��ַ
        {
            //�����ڴ���ʼ��ַ����ͷ���ṹ������
            this->shm_start_addr = (SHM_HEAD_T*)shmat(this->shm_id, 0, 0);
            //�������ʵ�ʵ�ַ
            this->shm_data_add = (char*)(this->shm_start_addr + 1);
            this->shm_start_addr->blocks = blocks;
            this->shm_start_addr->block_size = block_size;
            this->shm_start_addr->w_index = 0;
            this->shm_start_addr->r_index = 0;
            

        }
    }
    char* shm_read()
    {
        char* buf = (char*)malloc(this->shm_start_addr->block_size);
        //�ѹ����ڴ��Ӧ��λ�����ݿ�����buf��
        memcpy(buf, this->shm_data_add + this->shm_start_addr->r_index * this->shm_start_addr->block_size, this->shm_start_addr->block_size);
        this->shm_start_addr->r_index = (this->shm_start_addr->r_index + 1) % this->shm_start_addr->blocks;
        return buf;
    }
    void shm_write(char* buf)
    {
        memcpy(this->shm_data_add + this->shm_start_addr->w_index * this->shm_start_addr->block_size, buf, this->shm_start_addr->block_size);
        //ʵ�ֻ���
        this->shm_start_addr->w_index = (this->shm_start_addr->w_index + 1) % this->shm_start_addr->blocks;
    }
};

