/*
 * ========================================================================
 * 
 *          fileName:           server.cpp
 *          description:        服务端程序，作为服务端调用 cgi 层处理请求
 *
 *          DEFAULTPORT:        默认绑定 8000 端口
 *
 *          本项目中用:
 *              const 常量代替，define 宏常量
 *              
 *              且封装的具体类都继承了 boost::noncopyable 禁止拷贝
 *              
 *              并且设置类中某些方法为虚函数，以便代码复用，作为其他类的基类
 *
 *          qq:                 1003625407
 *          email:              13669186256@163.com
 *          
 * ========================================================================
 * */

#include "../common/common.h"
#include "../include/DatabaseMysql.h"
#include "../include/MiddleHandle.h"


int main(int argc, char* argv[])
{
    DatabaseMysql::initMysql();

    int port = DEFAULTPORT;

    if (argc >= 2) {
        sscanf(argv[1], "%d", &port);
    }

    int sock_s = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_s == -1) {
        VpCommon::errHandle("create");
    }

    struct sockaddr_in addr_s, addr_c;
    memset(&addr_s, 0, sizeof(struct sockaddr_in));
    memset(&addr_c, 0, sizeof(struct sockaddr_in));
    addr_s.sin_family = AF_INET;
    addr_s.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_s.sin_port = htons(port);

    if (bind(sock_s, (struct sockaddr*)&addr_s, sizeof(addr_s)) < 0) {
        VpCommon::errHandle("bind");
    }

    if (listen(sock_s, 10000) == -1) {
        VpCommon::errHandle("listen");
    }

    printf("listen at port %d ...\n", port);

    while (1) {
        socklen_t addrlen;
        (void)addrlen;
        int sock_c = accept(sock_s, (struct sockaddr*)NULL, NULL);
        printf("new session [socket:%d]\n", sock_c);
        char buffer_recv[MAXLINE] = "\0";
        char buffer_send[MAXLINE] = "\0";
        int len = 0;

        while (1) {
            len = recv(sock_c, buffer_recv, MAXLINE, 0);

            if (len < 0) {
                VpCommon::errHandle("recv");
            }

            if (len == 0 || buffer_recv[0] == '\0') {
                break;
            }

            buffer_recv[MAXLINE - 1] = '\0';

            printf("recv: %s [butes:%d]\n", buffer_recv, len);
            if (0 == strcmp(buffer_recv, "exit")) {
                break;
            }

            MiddleHandler::middleHandle(buffer_recv, buffer_send, MAXLINE);
            buffer_send[MAXLINE - 1] = '\0';

            if (send(sock_c, buffer_send, MAXLINE, 0) < 0) {
                VpCommon::errHandle("send");
            }
        }
        close(sock_c);
    }
    return 0;
}
