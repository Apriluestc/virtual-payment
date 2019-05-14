/*
 * ========================================================================
 * 
 *          fileName:           common.h
 *          description:        项目共用头文件，其中封装了 CS 通信的 C 端
 *          
 *          对外接口:
 *              int errHandle(const char* str);
 *              void sendRecv(const char* sendBuffer, const int lenSend, char* recvBuffer, const int lenRecv);
 *              const string& getCookie(Cgicc& cgi, const string& name);
 *              int isLogin(Cgicc& cgi);
 *
 *          相关说明:
 *              buffer_send:    发送缓冲区 buffer
 *              buffer_recv:    接收缓冲区 buffer
 *
 *              MAXLINE:        缓冲区 buffer 大小
 *
 *              errHandle():    错误处理
 *              sendRecv():     CS 通信收发数据，由 sendto() 和 recvfrom() 封装
 *              getCookie():    获取 cookie
 *              isLogin():      是否登录，即判断用户状态，这里的用户指买家 & 卖家
 *
 *              其中类 VpCommon 继承基类 noncopyable 仅仅是防止拷贝，减少不必要的开销
 *
 *              DEFAULTPORT:    默认绑定端口 8000，如果需要修改即可
 *              
 *          qq:                 1003625407
 *
 * ========================================================================
 * */

#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPHeader.h>

#include <boost/noncopyable.hpp>

const int MAXLINE =     4096;
const int DEFAULTPORT = 8000;

enum UserType {SELLER = 1, BUYER = 2};

class VpCommon : boost::noncopyable {
private:
    VpCommon() = delete;
    virtual ~VpCommon() = delete;
    VpCommon(const VpCommon& rhx) = delete;
    VpCommon& operator=(const VpCommon& rhx) = delete;
public:

    /*
     * static int errHandle(const char* strs);
     *
     * 该方法只进行错误信息提示，没有进行处理
     *
     * 除此之外可用，系统调用：
     *              
     *              perror()
     *              代替
     * 处理失败，返回 -1
     * */
    
    static int errHandle(const char* err) {
        printf("%s error: %s [%d]\n", err, strerror(errno), errno);
        exit(-1);
    }

    /*
     * static void sendRecv(const char* buffer_send, const int len_send, char* buffer_recv, const int len_recv);
     * 
     * 该方法的作用是 CS 之间进行数据信息传输，具体将
     * sendto() 和 recvfrom() 进行了封装
     *
     * 期间任意环节都进行了错误处理
     *
     * */
    
    static void sendRecv(const char* buffer_send, const int len_send, char* buffer_recv, const int len_recv) {
        int port = DEFAULTPORT;
        char ip[16] = "localhost";
 
        /*
         * 这里的 socket 函数功能是创建套接字
         * 其函数原型为：int socket(int domain, int type, int protocol)
         * 其中，三个参数的意义分别是
         *      
         *      domain:协议族，常用的协议族有：AF_INET、AF_INET6，即 ipv4 和 ipv6
         *      type:指定 socket 类型，常用的 socket 类型有：SOCK_STREAM、SOCK_DGRAM
         *      protocol:指定协议，常用的协议：IPPROTO_TCP、IPPTOTO_UDP
         * 
         * 函数返回值：失败时返回 -1
         * */
        
        int sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock == -1) {
            errHandle("created");
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        /*
         * 这里的 inet_pton 地址转换函数，且能够处理 ipv4 和 ipv6
         * 其中 inet_pton:将点分十进制转换整数
         * 而 inet_ntop 功能则相反
         * */
        
        if (inet_pton(AF_INET, ip, &addr.sin_addr) < 0) {
            errHandle("inet_pton");
        }

        /*
         * 这里的 connect 用于客户端与服务端建立 TCP 连接
         * 其函数原型：int connect(int sock, const struct sockaddr* addr, socklen_t addrlen)
         * 其中，三个参数分别为
         *
         *      sock:由 socket 函数创建的返回值
         *      addr:为服务端 socket 地址
         *      addrlen:为socket 地址的长度
         *
         * 返回值：连接失败时，返回值小于 1
         * */
        
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            errHandle("connect");
        }

        /*
         * 这里的 send 函数功能是一个系统调用函数，用来发送消息到套接字当中和 sendto，功能类似
         * 该项目中采用 send 函数
         * 其函数原型：ssize_t send(int sock, const void* buffer, size_t len, int flags)
         * 其中参数的意义为：
         * 
         *      sock:接收信息的套接字的文件描述符
         *      buffer:要发送的消息
         *      len:要发送的字节数
         *      flags:用来表示下列标志中的一个或者多个
         *          MSG_CONFIRM:用来告诉链路层
         *      当 flags 为 0 时，send 作用与 write 一样
         *
         * 函数返回值：在网络中传递错误对 send 来说是无法检测的，能检测到某些发送错，则会返回 -1，否则返回发送的字节数
         * */
        
        if (send(sock, buffer_send, len_send, 0) < 0) {
            errHandle("send");
        }

        /*
         * 这里的 recv 函数功能是从 TCP 连接的另一端接收数据
         * 其中函数原型：ssize_t recv(int sock, char* buffer, int len, int flags)
         * 其中参数意义为：
         *      sock:指接收端套接字描述符
         *      buffer:指明缓冲区，用来存放接收到的数据
         *      len:指明 buffer 的长度
         *
         * 函数返回值：如果返回值大于 0，则返回值表示接收数据的大小
         * 如果返回值等于 0，则表示对端关闭了 套接字
         * */
        
        int len = recv(sock, buffer_recv, len_recv, 0);

        if (len < 0) {
            errHandle("recv");
        }

        buffer_recv[len_recv] = '\0';

        //关闭套接字
        close(sock);
    }

    //函数对象提供比较方法
    class CkNameCompare : public std::unary_function<cgicc::HTTPCookie, bool> {
    public:
        inline explicit CkNameCompare(const std::string& name)
            :_name(name)
        {}

        inline bool operator() (const argument_type& arg) const {
            return arg.getName() == _name;
        }
    private:
        std::string _name;
    };

    /*
     * static const stding getCookie(cgicc::Cgicc& cgi, const string& name );
     *
     * 该方法用来获取 cookie 信息
     * */

    static const std::string getCookie(cgicc::Cgicc& cgi, const std::string& name) {
        const cgicc::CgiEnvironment& env = cgi.getEnvironment();

        std::vector<cgicc::HTTPCookie>::const_iterator begin = env.getCookieList().begin();
        std::vector<cgicc::HTTPCookie>::const_iterator end = env.getCookieList().end();
        std::vector<cgicc::HTTPCookie>::const_iterator it;
    
        it = std::find_if(begin, end, CkNameCompare(name));

        if (it == end) {
            return "";
        }
        return it->getValue();    
    } 

    /*
     * static int isLogin(cgicc::Cgicc& cgi);
     * 
     * 该方法通过 cgi 实现 cookie 功能以记录用户登录状态
     * 其返回一个整型
     *
     * 判断用户在线状态，记录用户状态
     * */

    static int isLogin(cgicc::Cgicc& cgi) {
        char buffer_recv[MAXLINE] = "\0";

        if (cgi.getEnvironment().getCookies().empty()) {
            return 0;
        }

        std::string type = getCookie(cgi, "type");
        std::string name = getCookie(cgi, "name");
        std::string token = getCookie(cgi, "token");

        if (type.empty() || name.empty() || token.empty()) {
            return 0;
        }

        std::string cmd = "token|" + type + '|' + name + '|' + token;
        sendRecv(cmd.c_str(), cmd.size() + 10, buffer_recv, MAXLINE);
        int err = -1;
        char note[32] = "\0";

        sscanf(buffer_recv, "error:%d, note:%s", &err, note);
        return err == 0;
    }
};

#endif
