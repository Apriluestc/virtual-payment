/*
 * ======================================================================
 * 
 *          fileName:           MiddleHandler.h
 *          description:        该文件实现了命令式交互
 *                          命令处理组件
 *
 *          对外接口:
 *              
 *              middleHandle();
 *              login();
 *              token();
 *              payOne();
 *              payTwo();
 *              refundOne();
 *              refundTwo();
 *              list();
 *
 *          qq:                 1003625407
 *          email:              13669186256@163.com
 *          
 * ======================================================================
 * */

#ifndef INCLUDE_MIDDLEHANDLE_H
#define INCLUDE_MIDDLEHANDLE_H

#include <iostream>
#include <string>

#include <boost/noncopyable.hpp>

#include "../common/common.h"
#include "DatabaseMysql.h"

class MiddleHandler : boost::noncopyable {
private:
    MiddleHandler() = delete;
    virtual ~MiddleHandler() = delete;
    MiddleHandler(const MiddleHandler& rhx) = delete;
    MiddleHandler& operator=(const MiddleHandler& rhx) = delete;
public:

    /*
     * 命令处理组件
     * */

    static void middleHandle(const char* buffer_recv, char* buffer, int len) {
        if (0 == strncmp(buffer_recv, "token", 5)) {
            token(buffer_recv, buffer, len);
        } else if (0 == strncmp(buffer_recv, "login", 5)) {
            login(buffer_recv, buffer, len);
        } else if (0 == strncmp(buffer_recv, "pay", 3)) {
            payTwo(buffer_recv, buffer, len);
        } else if (0 == strncmp(buffer_recv, "refund", 6)) {
            refundTwo(buffer_recv, buffer, len);
        } else if (0 == strncmp(buffer_recv, "list", 4)) {
            list(buffer_recv, buffer, len);
        } else {
            const char* s = "error:100, note:echo command if unknow";
            strncat(buffer, s, strlen(s));
            strncat(buffer, buffer_recv, MAXLINE - strlen(s));
        }
    }

    /*
     * 使用命令进行通信，命令处理函数
     * 命令:login type name password
     * */

    static void login(const char* recv, char* buffer, int len) {
        (void)len;
        MYSQL* mysqlHanlder = DatabaseMysql::connectMysql();
        
        char name[32];
        char password[32] = "\0";
        char sql[MAXLINE];
        char token[32];
        int type;
        
        sscanf(recv, "login|%d|%[^|]%[^\n]", &type, name, password);

        if (type == BUYER) {
            
            /*
             * type == BUYER
             * 买家登录，进行订单信息查询
             * */

            snprintf(sql, MAXLINE, "select id from txproj_buyer where name='%s'and password='%s'", name, password);
        } else if (type == SELLER) {

            /*
             * type == SELLER
             * 卖家登录进行订单信息查询
             * */

            snprintf(sql, MAXLINE, "select id from txproj_seller where name='%s' and password='%s'", name, password);
        } else {
            snprintf(buffer, MAXLINE, "error:8, note:type error");
            return;
        }

        DatabaseMysql::query(mysqlHanlder, sql);
        MYSQL_RES* result = mysql_use_result(mysqlHanlder);

        MYSQL_ROW* row = (MYSQL_ROW*)mysql_fetch_row(result);
        if (!row) {
            snprintf(buffer, MAXLINE, "error:2, note:name or password wrong");
            return;
        }

        srand(time(0));
        snprintf(token, 32, "%d", rand());
        strncat(token, password, 32 - strlen(token));
        token[31] = '\0';

        int id = 0;
        sscanf((const char*)row[0], "%d", &id);

        DatabaseMysql::releaseAfterSelect(mysqlHanlder, result);

        if (type == BUYER) {
            
            /*
             * 修改
             * */

            snprintf(sql, MAXLINE, "update txproj_buyer set token='%s' where id=%d", token, id);
        } else if (type == SELLER) {
            snprintf(sql, MAXLINE, "update txproj_seller set token='%s' where id=%d", token, id);
        } else {
            snprintf(buffer, MAXLINE, "error:8, note:type error");
            return;
        }

        DatabaseMysql::query(mysqlHanlder, sql);
        DatabaseMysql::exitMysql(mysqlHanlder);
        snprintf(buffer, MAXLINE, "error:0, note:%d|%s", id, token);
    }

    /*
     * 命令:list type name token
     * */

    static void list(const char* recv, char* buffer, int len) {
        MYSQL* mysqlHanlder = DatabaseMysql::connectMysql();
        //char name[32];
        char token[32];
        char sql[MAXLINE];
        int type;
        int id;
        printf("before sscanf: %s\n", recv);
        sscanf(recv, "list|%d|%d|%s", &type, &id, token);
        printf("list|%d|%d|%s", type, id, token);

        if (!DatabaseMysql::checkTokenWithID(mysqlHanlder, type, id, token)) {
            snprintf(buffer, len, "{\"error\":1, \"note\":\"token error}");
            return;
        }
        const char* other;

        if (type == BUYER) {
            snprintf(sql, MAXLINE, "select a.id, b.name, a.amount, a.refund, a.create_time, a.status from txproj_deal as a, txproj_buyer as b where a.buyer_id=b.id and b.id=%d", id);
            other = "seller";
        } else if (type == SELLER) {
            snprintf(sql, MAXLINE, "select a.id, b.name, a.amount, a.refund, a.create_time, a.status from txproj_deal as a, txproj_seller as b where a.seller_id=b.id and b.id=%d", id);
            other = "buyer";
        } else {
            snprintf(buffer, MAXLINE, "{\"error\":8,\"note\":\"type error\"}");
            return;
        }
        int size = DatabaseMysql::query(mysqlHanlder, sql);
        MYSQL_RES* result = mysql_use_result(mysqlHanlder);
        //unsigned int col = mysql_field_count(mysqlHanlder);

        char data[MAXLINE] = "\0";
        char line[MAXLINE] = "\0";
        size_t left = sizeof(data);

        MYSQL_ROW* row = (MYSQL_ROW*)mysql_fetch_row(result);

        while (row) {
            snprintf(line, MAXLINE, 
                     "{\"id\":%s, \"%s\":\"%s\", \"amount\":%s, \"refund\":%s, \"create_time\":\"%s\", \"status\":%s}",
                     (const char*)row[0], other, (const char*)row[1],
                     (const char*)row[2],(const char*)row[3],(const char*)row[4],(const char*)row[5]);
            if (strlen(line) >= left) {
                break;
            }
            strncat(data, line, left);
            left -= strlen(line);
            strncat(data, ",", 1);
            row = (MYSQL_ROW*)mysql_fetch_row(result);
        }
        data[strlen(data)-1] = '\0';
        snprintf(buffer, len, "{\"error\":0,\"type\":%d, \"len\":%d, \"data\":[%s]}", type, size, data);
        DatabaseMysql::exitMysql(mysqlHanlder);
    }
    
    /*
     * 命令:pay deal_id buyer_id seller_id amount
     * */

    static void payTwo(const char* recv, char* buffer, int len) {
        (void)len;
        
        MYSQL* mysqlHandler = DatabaseMysql::connectMysql();
        long long amount;
        int deal_id, buyer_id, seller_id;
        char sql[MAXLINE] = "\0", token[32]="\0", password_pay[32]="\0";
        
        sscanf(recv, "pay|%d|%[^|]|%d|%[^|]", &deal_id, password_pay, &buyer_id, token);
        
        if (!DatabaseMysql::checkTokenWithID(mysqlHandler, BUYER, buyer_id, token)) {
            snprintf(buffer, MAXLINE, "error:1, note:token error");
            return;
        }
        
        snprintf(sql, MAXLINE, "select seller_id, amount, status from txproj_deal where id=%d and buyer_id=%d",
                             deal_id, buyer_id);
        
        DatabaseMysql::query(mysqlHandler, sql);
        MYSQL_RES *result = mysql_use_result(mysqlHandler);
        MYSQL_ROW *row = (MYSQL_ROW *)mysql_fetch_row(result);
        
        if (!row) {
            snprintf(buffer, MAXLINE, "error:3, note: no such deal, maybe arguments wrong");
            return;
        }
        
        int status=0;
        
        sscanf((const char*)row[0], "%d", &seller_id);
        sscanf((const char*)row[1], "%lld", &amount);
        sscanf((const char*)row[2], "%d", &status);
        
        if (status > 1) {
            snprintf(buffer, MAXLINE, "error:4, note: has been paid");
            return;
        }
        
        DatabaseMysql::releaseAfterSelect(mysqlHandler, result);
        
        snprintf(sql, MAXLINE, "insert into txproj_pay (deal_id, buyer_id, seller_id, amount, pay_time) values \
                 (%d, %d, %d, %lld, now());", deal_id, buyer_id, seller_id, amount);
        
        DatabaseMysql::query(mysqlHandler, sql);
        
        snprintf(sql, MAXLINE, "update txproj_deal set status=2 where id=%d", deal_id);
        
        DatabaseMysql::query(mysqlHandler, sql);
        
        snprintf(buffer, MAXLINE, "error:0, note:%lld", amount);
        
        DatabaseMysql::exitMysql(mysqlHandler);
    }
    
    /*
     * 命令:refund dealid buyer seller amount
     * */

    static void refundTwo(const char* recv, char* buffer, int len) {
        (void)len;
        
        MYSQL* mysqlHandler = DatabaseMysql::connectMysql();
        long long amount;
        int deal_id, buyer_id, seller_id;
        char sql[MAXLINE] = "\0";
        char token[32]="\0";
        
        sscanf(recv, "refund|%d|%lld|%d|%s", &deal_id, &amount, &seller_id, token);
        
        if (!DatabaseMysql::checkTokenWithID(mysqlHandler, SELLER, seller_id, token)) {
            snprintf(buffer, MAXLINE, "error:1, note:token error");
            return;
        }
        
        snprintf(sql, MAXLINE, "select buyer_id, amount, refund, status from txproj_deal where id=%d and seller_id=%d", deal_id, seller_id);
        
        DatabaseMysql::query(mysqlHandler, sql);
        MYSQL_RES *result = mysql_use_result(mysqlHandler);
        MYSQL_ROW *row = (MYSQL_ROW *)mysql_fetch_row(result);

        if (!row) {
            snprintf(buffer, MAXLINE, "error:3, note: no such deal, maybe arguments wrong");
            return;
        }
        
        int status=0;
        long long pay=0, refund=0;
        
        sscanf((const char*)row[0], "%d", &buyer_id);
        sscanf((const char*)row[1], "%lld", &pay);
        sscanf((const char*)row[2], "%lld", &refund);
        sscanf((const char*)row[3], "%d", &status);
        
        if (status==1) {
            snprintf(buffer, MAXLINE, "error:7, note:7, note: has not been paid");
            return;
        }
        
        if (status==3) {
            snprintf(buffer, MAXLINE, "error:5, note: has been refund");
            return;
        }
        
        long long left = pay - refund;
        
        if (amount > left) {
            snprintf(buffer, MAXLINE, "error:6, note:max=%lld", left);
            return;
        }
        
        DatabaseMysql::releaseAfterSelect(mysqlHandler, result);
        
        snprintf(sql, MAXLINE, "insert into txproj_refund (deal_id, buyer_id, seller_id, amount, refund_time) values \
                 (%d, %d, %d, %lld, now());", deal_id, buyer_id, seller_id, amount);
        DatabaseMysql::query(mysqlHandler, sql);
        
        snprintf(sql, MAXLINE, "update txproj_deal set status=3, refund=refund+%lld where id=%d", amount, deal_id);
        
        DatabaseMysql::query(mysqlHandler, sql);
        
        snprintf(buffer, MAXLINE, "error:0, note:%lld", amount);
        
        DatabaseMysql::exitMysql(mysqlHandler);
    }
    
    static void token(const char* recv, char* buffer, int len) {
        MYSQL* mysqlHanlder = DatabaseMysql::connectMysql();
        char name[32];
        char token[32];
        int type;
        sscanf(recv, "token|%d|%[^|]|%[^\n]", &type, name, token);
        
        if (DatabaseMysql::checkTokenWithName(mysqlHanlder, type, name, token)) {
            snprintf(buffer, len, "error:0, note:true");
        } else {
            snprintf(buffer, len, "error:1, note:token error");
        }

        DatabaseMysql::exitMysql(mysqlHanlder);
    }
};

#endif
