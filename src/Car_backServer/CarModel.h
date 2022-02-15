#ifndef  CARMODEL_H
#define CARMODEL_H
#include "SingalDb.h"
#include <string>
#include <iostream>
using namespace std;
#include <list>
#include<vector>
class CarModel
{
public:
    //3.������̬�������ṩ����������ݿⵥ���Ľӿڣ� ����ʵ�������󼴿ɵ���
    static CarModel* getInstance();
    //�������ṩ�ⲿ����
    //��̬������Ҫ����������ʼ���
    int insertCarMsg(string car_num, string in_time, string local,string path);
    list<string> select_intime(string car_num);
    int deletecar();
    int updateCarmsg(string car_number, string out_time, int money);
    int totalCar();//��ѯͣ�������ж��ٳ�
    list<string> select_carmsg(string car_number, int page);
    list<string> select_allcarmsg(string car_number,string in_time,string out_time, int page);
    list<string> select_carmsg_time(string in_time, string out_time, int page);
    list<string> select_carmsg_intime(string in_time,int page);
    list<string> select_carmsg_outtime(string out_time, int page);
    int selectVideoFrame(string name);//��ѯ֡��
    int updateVideoFrame(string name,int count);//�޸�֡��
    int insertNewVideo(string name, int totalframe);//�����µ���Ƶ
private:
    CarModel();//1.���캯��˽�л�  �������ⴴ������
    static CarModel* cardb;//2.��̬˽�ж������ݿⵥ��
    //˽�У��������ⲿ���ⴴ������
    //��̬������Ҫ����������ʼ���
};
#endif // ! CARMODEL_H

