#ifndef DEFINE_FILE_H
#define DEFINE_FILE_H

#define MAX_SIZE 1024//��󳤶�

#define ERROR_NUMBER -1//������Ϣ
#define REGIST_TYPE 1 //ע���
#define LOGIN_TYPE 2 //��¼��
#define PHOTO_TYPE 3//ͼƬ��
#define CAR_GETIN 4//�볡��
#define CAR_GETOUT 5//������
#define HEART_TYPE 6//������
#define CAR_MSG_TYPE 7//������Ϣ��
#define VIDEO_TYPE 8//��Ƶ��¼��
#define VIDEO_OPEN 9//��Ƶ��
#define VIDEO_CLOSE 10//��Ƶ�ر�
#define OFFLINE_TYPE 11//���߰�
typedef struct mymsgbuf {
	long mtype;
	char mtext[12];
}MYBUF;//��Ϣ���нṹ��
typedef struct  {
	int fd;
	int time;
}PACK_HEART;//�������ṹ��
typedef struct
{
	int bodyType;//��������
	int bodySize;//�����С
	int crc_code;//crcУ����
	char serialNum[36];//��ˮ��
}PACK_HEAD;//��ͷ
typedef struct
{
	char name[24];
	char pwd[16];
}PACK_REGIST_LOGIN;//��¼ע�����
typedef struct
{
	int result;//0�ɹ� 1�û������� 2�������
}PACK_RL_BACK;//��������
typedef struct
{
	int number;//���
	char car_num[18];//����
	char now_time[36];//ʱ��
	char location[12];//λ��
	char photo_path[100];//ͼƬ·��
}PACK_ENTER;//�볡��

typedef struct
{
	char filename[40];//�ļ���
	int realSize;//��ʵ���ַ�����С
	int num;//��ʾ�ǵڼ�����
	int sum;//�ܹ��м�����
	char context[MAX_SIZE];//�����洢���ݵ�����
}PACK_PHOTO;
typedef struct
{
	int result;//1��ʾ�յ��� ��������0��ʾû�յ�
}PACK_PHOTO_BACK;
typedef struct
{
	char car_number[18];//����
	char out_time[36];//����ʱ��
}PACK_EXIT;

typedef struct
{
	char in_time[36];//�볡ʱ��
	int vip;//�Ƿ�ΪVIP
	char total_time[64];//ͣ��ʱ��
	int money;//���
}PACK_EXIT_BACK;

typedef struct
{
	char car_number[18];//����
	char in_time[36];//�볡ʱ��
	char out_time[36];//����ʱ��
	int page;//�ڼ�ҳ
}PACK_CARMSG;
typedef struct
{
	char car_number[18];//����
	char in_time[36];//�볡ʱ��
	char out_time[36];//����ʱ��
	int money;//ͣ�����
}PACK_CARMSG_BACK;
typedef struct
{
	int realCount;//��ʵ��������
	int totalCount;//������
	PACK_CARMSG_BACK arr[4];//�ĸ�����ṹ��
}PACK_ALLCARMSG_BACK;
typedef struct
{
	char video_name[24];//��Ƶ����
	int frameCount;//��¼֡��
	int totalFrameCount;//��֡��
}PACK_VIDEO;
typedef struct
{
	int frameCount;//��¼֡��
}PACK_VIDEO_BACK;

#endif // !DEFINE_FILE_H
