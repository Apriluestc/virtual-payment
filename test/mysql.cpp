#include <iostream>

#include "../common/common.h"
#include "../include/DatabaseMysql.h"

using namespace std;
int main()
{
    char sql[32] = "\0";
    int type = 2;
    int id1 = 1002;
    int id2 = 1400;
    int id3 = 1000001;
    char token[32];
    char name1[32] = "simple";
    char name2[32] = "rich";
    char name3[32] = "poor";
    char name4[32] = "jinanyuan";
    DatabaseMysql::initMysql();
    MYSQL* mysqlHandler = DatabaseMysql::connectMysql();
    DatabaseMysql::query(mysqlHandler, sql);
    int res1;
    int res2;
    int res3;
    int s1, s2, s3, s4;
    res1 = DatabaseMysql::checkTokenWithID(mysqlHandler, type, id1, token);
    res2 = DatabaseMysql::checkTokenWithID(mysqlHandler, type, id2, token);
    res3 = DatabaseMysql::checkTokenWithID(mysqlHandler, type, id3, token);
    cout << res1 << endl;
    cout << res2 << endl;
    cout << res3 << endl;
    s1 = DatabaseMysql::checkTokenWithName(mysqlHandler, type, name1, token);
    s2 = DatabaseMysql::checkTokenWithName(mysqlHandler, type, name2, token);
    s3 = DatabaseMysql::checkTokenWithName(mysqlHandler, type, name3, token);
    s4 = DatabaseMysql::checkTokenWithName(mysqlHandler, type, name4, token);
    
    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    cout << s4 << endl;

    DatabaseMysql::exitMysql(mysqlHandler);
    return 0;
}
