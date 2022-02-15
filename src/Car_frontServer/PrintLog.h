#ifndef PRINTLOG
#define PRINTLOG
#include <iostream>
using namespace std;
#include <fstream>
class PrintLog
{
public:
    //3.������̬�������ṩ����������ݿⵥ���Ľӿڣ� ����ʵ�������󼴿ɵ���
    static PrintLog* getInstance();
    void print_log();
    void write_login_log(char* buffer, char* fun, int size, char* number, char* name, char* pwd);//д��¼ע�ᵽ�ļ�
    void write_carin_log(char* buffer, char* fun, int size, char* number, char* car_num, int fd,char *in_time);//д�볡�����ļ�
    void write_carout_log(char* buffer, char* fun, int size, char* number, char * car_num, int fd, char *out_time );//д���������ļ�
    void write_carmsg_log(char* buffer, char* fun, int size, char* number, char* car_num, char* in_time, char* out_time);//��ѯ
    void write_video_log(char* buffer, char* fun, int size, char* number, char* video_num, int framecount);//��Ƶ��Ϣ

    int effec_conCounts=0;//��Ч������
    int rec_packCounts=0;//�������ݰ���
    int send_packCounts=0;//�������ݰ���
    int userCounts=0;//�û���¼��
    int get_channelCounts=0;//��ȡ��ƵƵ����
    int get_channel_typeCounts=0;//��ȡ��Ƶ������
    int get_video_local=0;//��ȡ��Ƶ����
    int get_video_list=0;//��ȡ��Ƶ�б�
    int play_counts=0;//��ȡ�ϴ���������
    int play_time=0;//��ȡ�ϴ�����ʱ��
private:
    PrintLog();//1.���캯��˽�л�
    static PrintLog* logdb;//2.��̬˽�ж������ݿⵥ��
};
#endif PRINTLOG

