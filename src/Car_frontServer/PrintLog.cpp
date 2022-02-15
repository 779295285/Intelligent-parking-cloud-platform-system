#include "PrintLog.h"
PrintLog *PrintLog::logdb;
PrintLog::PrintLog()
{

}
PrintLog* PrintLog::getInstance()
{
    if (PrintLog::logdb == nullptr)
    {
        PrintLog::logdb = new PrintLog();
    }
    return PrintLog::logdb;
}
void PrintLog::print_log()
{
    printf("�������ݣ�\n");
    cout << "����Ϊʵʱ��־" << endl;
    cout << "��Ч��������" << this->effec_conCounts << endl;
    cout << "�������ݰ���" << this->rec_packCounts << endl;
    cout << "�������ݰ���" << this->send_packCounts << endl;
    cout << "�û���¼����" << this->userCounts << endl;
    //cout << "��ȡ��ƵƵ����" << this->get_channelCounts << endl;
    //cout << "��ȡ��Ƶ���ࣺ" << this->get_channel_typeCounts << endl;
    //cout << "��ȡ��Ƶ������" << this->get_video_local << endl;
    //cout << "��ȡ��Ƶ�б�" << this->get_video_list << endl;
    cout << "�ϴ��������ݣ�" << this->play_counts << endl;
    //cout << "�ϴ�����ʱ����" << this->play_time << endl;
    cout << "--------------------------------------------------------------------" << endl;
}
void  PrintLog::write_login_log(char* buffer, char* fun, int size, char* number, char* name, char* pwd)
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    cout << nowtime_buf << endl;
    ofstream out(nowtime_buf, ofstream::out | ofstream::in| ofstream::app);
    out << "ʱ�䣺" << buffer << endl;
    out << "���ܣ�" << fun << endl;
    out << "���ݰ���С��" << size << endl;
    out << "���ݰ���ˮ�ţ�" << number << endl;
    out << "�û�����" << name << endl;
    out << "���룺" << pwd << endl;
    out << endl;
}
void  PrintLog::write_carin_log(char* buffer, char* fun, int size, char* number, char* car_num,int fd,char *in_time)
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "ʱ�䣺" << buffer << endl;
    out << "���ܣ�" << fun << endl;
    out << "���ݰ���С��" << size << endl;
    out << "���ݰ���ˮ�ţ�" << number << endl;
    out << "���ƺţ�" << car_num << endl;
    out<< "�볡ʱ�䣺" << in_time << endl;
    out << "�׽���fd=:" << fd << endl;
    out << endl;
}
void PrintLog::write_carout_log(char* buffer, char* fun, int size, char *number,char* car_num, int fd, char* out_time)
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "ʱ�䣺" << buffer << endl;
    out << "���ܣ�" << fun << endl;
    out << "���ݰ���С��" << size << endl;
    out << "���ݰ���ˮ�ţ�" << number << endl;
    out << "���ƺţ�" << car_num << endl;
    out << "����ʱ�䣺" << out_time << endl;
    out << "�׽���fd=:" << fd << endl;
    out << endl;
}
void PrintLog::write_carmsg_log(char* buffer, char* fun, int size, char* number, char* car_num, char* in_time, char* out_time)//��ѯ
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "ʱ�䣺" << buffer << endl;
    out << "���ܣ�" << fun << endl;
    out << "���ݰ���С��" << size << endl;
    out << "���ݰ���ˮ�ţ�" << number << endl;
    out << "���ƺţ�" << car_num << endl;
    out << "�볡ʱ�䣺" << in_time<< endl;
    out << "����ʱ��:" << out_time << endl;
    out << endl;
}
void PrintLog::write_video_log(char* buffer, char* fun, int size, char* number, char* video_num, int framecount)//��Ƶ��Ϣ
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "ʱ�䣺" << buffer << endl;
    out << "���ܣ�" << fun << endl;
    out << "���ݰ���С��" << size << endl;
    out << "���ݰ���ˮ�ţ�" << number << endl;
    out << "��Ƶ���ƣ�" << video_num << endl;
    out << "��Ƶ��֡����" << framecount << endl;
    out << endl;
}