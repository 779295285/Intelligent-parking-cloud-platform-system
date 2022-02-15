#include "UserModel.h"
UserModel* UserModel::userdb;
UserModel::UserModel()
{

}
UserModel* UserModel::getInstance()
{
    if (UserModel::userdb == nullptr)
    {
        UserModel::userdb = new UserModel();
    }
    return UserModel::userdb;
}
int UserModel::insertUser(string name, string pwd)
{
	//sql���ƴ��
	char sql[256];
	sprintf(sql, "insert into tbl_user values(null,'%s','%s',0)", name.c_str(),pwd.c_str());
	//����׼��
	char** qres;
	int row, col;
	//�����ݿ�
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		return 0;
	}
	else {
		return  -1;
	}
}
int UserModel::searchUser(string name, string pwd)
{
	//sql���ƴ��
	char sql[256];
	sprintf(sql, "select * from tbl_user where user_name='%s' and user_pwd='%s'", name.c_str(), pwd.c_str());
	//����׼��
	char** qres;
	int row, col;
	//�����ݿ�
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row > 0)
		{
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		return  -1;
	}
}
int UserModel::changeState(int state,string name)//�޸ĵ�¼״̬����״̬
{
	//sql���ƴ��
	char sql[256];
	sprintf(sql, "update tbl_user set state=%d where user_name ='%s'", state, name.c_str());
	//����׼��
	char** qres;
	int row, col;
	//�����ݿ�
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		return 0;
	}
	else {
		return  -1;
	}
}