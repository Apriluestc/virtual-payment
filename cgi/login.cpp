/*
 * ======================================================================
 * 
 *          fileName:           login.cpp
 *          description:        实现了用户登录
 *                          即卖家登录和买家登录根据 type 进行判断是买家
 *                      还是卖家 && 并且实际登录时拒绝“明文传输”
 *
 *          登录实现:
 *              cookie:买房通过输入账号密码进行用户登录，后台需要验证，
 *              将生成的 cookie 返回给浏览器
 *              
 *              通常用户登录的时候，服务器根据用户名和密码在服务器数据库中
 *              校验该用户是否正确，校验正确后则可以根据用户 id 和 timeStamp
 *              属性加密生成一个 token，并返回给浏览器
 *
 *          密文密码:具体实现
 *             我们用户进行登录，服务端根据账号和密码在服务器数据库中校验该用户是否正确
 *             校验正确后则可以根据用户的 id 和 timeStamp 等属性加密生成一个 token 即
 *             本项目，数据库表中的 token 字段，并返回给浏览器
 *
 *             这样浏览器就会得到一个 cookie，只要这个会话没有关闭，所有访问服务器的请求
 *             都会带上这个 cookie
 *             这便达到了加密的效果
 *
 *          这种情况下，cookie 被人窃取，则会有泄露信息的危险
 *              1、通过设置 httpOnly 属性，这样 cookie 只在 http 中传输，而不会被脚本窃取
 *              但是网络拦截 http 请求还是会得到 cookie
 *              2、在 cookie 中加入校验信息，这个校验信息与用户的使用环境有关，比如 ip 地址
 *              计算机物理地址，在服务器端校验的时候，如果校验发生了变化，则需重新登录
 *          
 *          参考:https://www.cnblogs.com/fishisnow/p/6812210.html
 *
 *          qq:                 1003625407
 *          email:              13669186256@163.com
 *          
 * ======================================================================
 * */

#include "../common/common.h"

int main(int argc, char* argv[])
{
    try {
        cgicc::Cgicc cgi;
        char buffer_send[MAXLINE] = "\0";
        char buffer_recv[MAXLINE] = "\0";
        
        /*
         * name:登录名
         * type:登录类型 BUYER or SELLER
         * password:对应用户类型的登录密码
         * */

        cgicc::const_form_iterator name = cgi.getElement("name");
        cgicc::const_form_iterator type = cgi.getElement("type");
        cgicc::const_form_iterator password = cgi.getElement("password");
        cgicc::const_form_iterator end = cgi.getElements().end();

        /*
         * 生成 token
         * */

        std::string token;
        std::string cmd;

        /*
         * cmd:接收服务端中间处理组件的登录命令:login type name password
         * 买方通过输入账号、密码提交表单登录，后台需要验证，将生成的 token 放入数据库
         * 并以 cookie 进行返回
         * 
         * 其中，cookie 是一种服务器发送给浏览器的一组数据，用于浏览器跟踪用户，
         * 并访问服务器时保持登录状态等功能
         *
         * 其中将个人信息保存在数据库的买方用户表
         * */

        if (type != end && name != end && password != end) {
            cmd = "login|" + type->getValue() + '|' + name->getValue() + '|' + password->getValue();
            VpCommon::sendRecv(cmd.c_str(), cmd.size() + 10, buffer_recv, MAXLINE);
            int err = -1;
            char note[32] = "\0";
            char strid[32] = "\0";
            sscanf(buffer_recv, "error:%d, note:%[^|]%s", &err, strid, note);

            /*
             * err 等于 0，成功登录
             * 接着进行重定向
             *
             * 具体按照 type 字段来判断重定向到目标页面是“买家页面”还是“卖家页面”
             * 
             * 进一步说明：
             *      type 字段属性值为 2 ----> 买家页面，
             *      即 txproj_seller.html
             *      
             *      type 字段属性值为 1 ----> 卖家页面，
             *      即 txproj_buyer.html            
             * */
            
            if (err == 0) {
                if (type->getValue() == "2") {  //buyer.html
                    
                    /*
                     * cgicc 库相关操作
                     * */

                    std::cout << cgicc::HTTPRedirectHeader("/txproj_buyer.html", 1)
                        .setCookie(cgicc::HTTPCookie("name", name->getValue()))
                        .setCookie(cgicc::HTTPCookie("token", note))
                        .setCookie(cgicc::HTTPCookie("id", strid))
                        .setCookie(cgicc::HTTPCookie("type", type->getValue()));
                    
                    return 0;
                } else if (type->getValue() == "1") {   //seller.html
                    
                    std::cout << cgicc::HTTPRedirectHeader("/txproj_seller.html", 1)
                        .setCookie(cgicc::HTTPCookie("name", name->getValue()))
                        .setCookie(cgicc::HTTPCookie("token", note))
                        .setCookie(cgicc::HTTPCookie("id", strid))
                        .setCookie(cgicc::HTTPCookie("type", type->getValue()));
                    

                    return 0;
                }
            }
        }
        
        /*
         * 这里已经满足登录条件
         * 并且 isLogin 返回值非 0
         *
         * 在这里提交表单记录
         * */

        if (type == end && name == end && password == end && VpCommon::isLogin(cgi)) {
            std::string _type = VpCommon::getCookie(cgi, "type");

            if (_type == "2") {
                std::cout << cgicc::HTTPRedirectHeader("/txproj_buyer.html", 1);
                return 0;
            } else if (_type == "1") {
                std::cout << cgicc::HTTPRedirectHeader("/txproj_seller.html", 1);
                return 0;
            }
        }
        
        std::cout << cgicc::HTTPHTMLHeader();
        
        std::cout << "<script type=\"text/javascript\"> \
            alert(\"login" << buffer_recv << "\"); \
            location.href='/txproj_index.html';\
            </script>" << "\r\n";
    
    } catch(std::exception& e) {

    }
    return 0;
}
