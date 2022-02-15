#ifndef USERMODEL_H
#define USERMODEL_H
#include <iostream>
using namespace std;
#include <string>
#include <list>
#include"SingalDb.h"
class UserModel
{
public:
    //3.������̬�������ṩ����������ݿⵥ���Ľӿڣ� ����ʵ�������󼴿ɵ���
    static UserModel* getInstance();
    int insertUser(string name, string pwd);//�����û�
    int searchUser(string name, string pwd);//�����û�
    int changeState(int state,string name);//�޸ĵ�¼״̬����״̬
    
private:
    UserModel();//1.���캯��˽�л�
    static UserModel* userdb;//2.��̬˽�ж������ݿⵥ��
};
#endif // USERMODEL_H