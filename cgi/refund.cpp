#include "../common/common.h"

int main(int argc, char* argv[])
{
    try {
        cgicc::Cgicc cgi;
        char buffer_send[MAXLINE] = "\0";
        char buffer_recv[MAXLINE] = "\0";

        std::string name = VpCommon::getCookie(cgi, "name");
        std::string type = VpCommon::getCookie(cgi, "type");
        std::string token = VpCommon::getCookie(cgi, "token");
        std::string id = VpCommon::getCookie(cgi, "id");

        cgicc::const_form_iterator amount = cgi.getElement("amount");
        cgicc::const_form_iterator deal = cgi.getElement("deal");
        cgicc::const_form_iterator end = cgi.getElements().end();

        if (type != "" && name != "" && token != "" && id != "" && amount != end && deal != end) {
            std::string cmd = "refund|" + deal->getValue() + '|' + amount->getValue() + '|' + id + '|' + token;
            VpCommon::sendRecv(cmd.c_str(), cmd.size() + 10, buffer_recv, MAXLINE);
            int err = -1;
            char note[32] = "\0";
            char strid[32] = "\0";
            sscanf(buffer_recv, "error:%d, note:%[^|]|%s", &err, strid, note);

            if (err == 0) {
                std::cout << "Content-Type:application/json" << "\r\n\r\n";
                std::cout << "{\"error\":0}" << "\r\n";
                return 0;
            } else {
                std::cout << std::cout << "Content-Type:application/json" << "\r\n\r\n";
                std::cout << "{\"error\":" << err << ",\"note\":\"" << note <<"\"}" << "\r\n";
                return 0;
            }
        }
        std::cout << cgicc::HTTPHTMLHeader();
        std::cout << "<script type=\"text/javascript\"> \
            alert(\"login " << buffer_recv << "\");\
            location.href='/txproj_index.html';\
            </script>" << "\r\n";
    } catch(std::exception& e) {}
    return 0;
}
