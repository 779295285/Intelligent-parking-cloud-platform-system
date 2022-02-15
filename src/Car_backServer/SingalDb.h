#ifndef SINGALDB_H
#define SINGALDB_H
#include "sqlite3.h"

#include <string>
#include <stdio.h>
class SingalDb
{
public:
    //3.������̬�������ṩ����������ݿⵥ���Ľӿڣ� ����ʵ�������󼴿ɵ���
    static SingalDb* getInstance();
    //�������ṩ�ⲿ����
    //��̬������Ҫ����������ʼ���
    void openDataBase(const char* daPath);
    void closeDataBase();
    int dosql(char* sql, char**& result, int& row, int& col);
private:
    SingalDb();//1.���캯��˽�л�  �������ⴴ������
    static SingalDb* mydb;//2.��̬˽�ж������ݿⵥ��
    //˽�У��������ⲿ���ⴴ������
    //��̬������Ҫ����������ʼ���
    sqlite3* sqldb;
    char* dbPath;
    char* errmsg;
};

#endif // SINGALDB_H