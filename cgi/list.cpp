#include "../common/common.h"

int main(int argc, char *argv[])
{
    try {
        cgicc::Cgicc cgi;
        char buffer_send[MAXLINE]="\0", buffer_recv[MAXLINE]="\0";
        std::string name = VpCommon::getCookie(cgi, "name");
        std::string type = VpCommon::getCookie(cgi, "type");
        std::string token = VpCommon::getCookie(cgi, "token");
        std::string id = VpCommon::getCookie(cgi, "id");
        
        if (type!="" && name!="" && token!="" && id!="") {
            std::string cmd = "list|" + type + '|' + id + '|' + token;
            VpCommon::sendRecv(cmd.c_str(), cmd.size()+10, buffer_recv, MAXLINE);
            std::cout << "Content-Type:application/json" << "\r\n\r\n";
            std::cout << buffer_recv << "\r\n";
        } else {
            std::cout << cgicc::HTTPHTMLHeader();
            std::cout << "login " << buffer_recv << "\r\n";
            std::cout << "<script type=\"text/javascript\"> \
                    alert(\"非法！将定向到登录页。\");\
                    location.href='/txproj_index.html';\
                    </script>" << "\r\n";
        }
    } 
    catch(std::exception& e) {

    }
    return 0;
}


