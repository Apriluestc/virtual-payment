/*
 * ========================================================================
 * 
 *          fileName:           DatabaseMysql.h
 *          description:        该文件对数据库进行了简单的封装，包括:
 *                          数据库的连接
 *                          数据库的退出
 *                          数据表的按字段查询
 *
 *          对外接口:
 *
 *              connectMysql();         连接数据库
 *              initMysql();            调用数据库初始化脚本 init.sql 进行初始化
 *              exitMysql();            数据库的退出
 *              query();                对 mysql_query() 的简单封装，进行查找
 *              checkTokenWithID();     根据 id 进行查找数据库
 *              checkTokenWithName();   根据 name 进行查找数据库
 *              releaseAfterSelect();   释放查找结果
 *
 *          qq:                 1003625407
 *          email:              13669186256@163.com
 *          
 * ========================================================================
 * */

#ifndef INCLUDE_DATABASEMYSQL_H
#define INCLUDE_DATABASEMYSQL_H

#include <stdio.h>

#include <iostream>
#include <string>
#include <mysql/mysql.h>

#include <boost/noncopyable.hpp>

#include "../common/common.h"

/*
 * 虚函数：C++ 虚函数的主要作用是“运行时多态”，父类提供虚函数的实现，子类提供默认函数的实现
 * 除此之外，子类可以重写父类的虚函数实现子类的特殊化
 * 
 * 纯虚函数：C++ 中包含纯虚函数的类称为抽象类，抽象类不能 new 出对象，
 * 只有实现了这个纯虚函数的子类，才能 new 出对象
 *
 * C++ 中的纯虚函数更像是只提供声明，没有实现，是对子类的约束，这称为接口继承
 * */

class DatabaseMysql : boost::noncopyable {
private:
    DatabaseMysql() = delete;
    virtual ~DatabaseMysql() = delete;
    DatabaseMysql(const DatabaseMysql& rhx) = delete;
    DatabaseMysql& operator=(const DatabaseMysql& rhx) = delete;
public:

    /*
     * 此函数用于 MySQL 数据库的连接，其中参数说明：
     *      root:用户名
     *      windows:密码
     *      virtualpayment:数据库名
     * */

    static MYSQL* connectMysql() {
        MYSQL* mysqlHandler = mysql_init(NULL);
        mysql_real_connect(mysqlHandler,"localhost", "root", "windows", "virtualpayment", 0, NULL, 0);
        return mysqlHandler;
    }
    
    static bool initIalize(const std::string& host, const std::string& user, const std::string& password, const std::string& databaseName);

    static void initMysql() {

    }
    
    //这个函数用于关闭数据库
    static void exitMysql(MYSQL* mysqlHandler) {
        mysql_close(mysqlHandler);
    }
    
    /*
     * 此方法对 MySQL 中的 query 方法进行了封装
     * 用于错误处理
     * */

    static int query(MYSQL* mysqlHandler, const char* sql) {
        int err = mysql_query(mysqlHandler, sql);

        if (err == 0) {
            return err;
        }
        printf("'%s' sql error: %s [%d]\n", sql, mysql_error(mysqlHandler), mysql_errno(mysqlHandler));
        exit(-1);
    }
    
    static int query(MYSQL* mysqlHandler, std::string& sql) {
        return query(mysqlHandler, sql.c_str());
    }
    
    /*
     * checkTokenWithID: 此方法提供了按照 id 进行查找数据表的接口
     *
     * 其中查询到结果保存在 ret 中
     * */

    static int checkTokenWithID(MYSQL* mysqlHandler, int type, int id, char* token) {
        char sql[MAXLINE] = "\0";

        if (type == BUYER) {
            snprintf(sql, MAXLINE, "select token from txproj_buyer where id=%d", id);
        } else if (type == SELLER) {
            snprintf(sql, MAXLINE, "select token from txproj_seller where id=%d", id);
        } else {
            return 0;
        }

        query(mysqlHandler, sql);

        MYSQL_RES* result = mysql_use_result(mysqlHandler);

        if (!result) {
            VpCommon::errHandle("mysql result null");
        }

        MYSQL_ROW* row = (MYSQL_ROW*)mysql_fetch_row(result);
        int ret = (int)(row && 0 == strcmp((const char*)row[0], token));

        releaseAfterSelect(mysqlHandler, result);
        return ret;
    }
    
    /*
     * 这个函数 checkTokenWithName: 提供了按照 name 进行查找数据表
     * 然后将查询的额结果作为返回值
     * */

    static int checkTokenWithName(MYSQL* mysqlHandler, int type, char* name, char* token) {
        char sql[MAXLINE] = "\0";

        if (type == BUYER) {
            snprintf(sql, MAXLINE, "select token from txproj_buyer where name='%s'", name);
        } else if (type == SELLER) {
            snprintf(sql, MAXLINE, "select token from txproj_seller where name='%s'", name);
        } else {
            return 0;
        }

        query(mysqlHandler, sql);
        MYSQL_RES* result = mysql_use_result(mysqlHandler);

        if (!result) {
            VpCommon::errHandle("mysql result is null");
        }

        MYSQL_ROW* row = (MYSQL_ROW*)mysql_fetch_row(result);
        int ret = (int)(row && 0 == strcmp((const char*)row[0], token));

        releaseAfterSelect(mysqlHandler, result);

        return ret;
    }
    
    /*
     * 此方法功能，释放当前查询结果集，避免影响下次查询结果
     *
     * 接口:    releaseAfterSelect(mysqlHandler, result)
     * */

    static void releaseAfterSelect(MYSQL* mysqlHandler, MYSQL_RES* result) {
        mysql_free_result(result);

        while (!mysql_next_result(mysqlHandler)) {
            result = mysql_store_result(mysqlHandler);
            mysql_free_result(result);
        }
    }
};

#endif
