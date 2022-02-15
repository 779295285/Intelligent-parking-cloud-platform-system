#include "SingalDb.h"
#include <iostream>
using namespace std;
SingalDb* SingalDb::mydb;
SingalDb::SingalDb()
{
    this->openDataBase("./carDB");
}
SingalDb* SingalDb::getInstance()
{
    if (SingalDb::mydb == nullptr)
    {
        SingalDb::mydb = new SingalDb();
    }
    return SingalDb::mydb;
}
/**�����ݿ�
 * @brief SingalDb::openDataBase
 * @param dbPath
 */
void SingalDb::openDataBase(const char* dbPath)
{
    int res = sqlite3_open(dbPath, &sqldb);
    if (res != SQLITE_OK)
    {
        cout << sqlite3_errmsg(sqldb) << endl;
        cout << sqlite3_errcode(sqldb) << endl;
    }
}
/**�ر����ݿ�
 * @brief SingalDb::closeDataBase
 */
void SingalDb::closeDataBase()
{
    int res = sqlite3_close(sqldb);
    if (res != SQLITE_OK)
    {
        cout << sqlite3_errmsg(sqldb) << endl;
        cout << sqlite3_errcode(sqldb) << endl;
    }
}
/**���ݿ�������
 * @brief SingalDb::dosql
 * @param sql
 * @param result
 * @param row
 * @param col
 * @return
 */
int SingalDb::dosql(char* sql, char**& result, int& row, int& col)
{
    int res = sqlite3_get_table(sqldb, sql, &result, &row, &col, &errmsg);
    if (res != SQLITE_OK)
    {
        cout << sqlite3_errmsg(sqldb) << endl;
        cout << sqlite3_errcode(sqldb) << endl;
        return sqlite3_errcode(sqldb);
    }
    return 0;
}
