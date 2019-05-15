#项目所在路径
PROJECT_PATH		:= /home/shiny/repository/vp/

#cgi 程序路径，自己可在 httpd.conf 中配置 cgi-bin 目录
CGI_PATH			:= /var/www/cgi-bin/

#apache html 目录，用户自定义配置
STATICHTML_PATH		:= /var/www/html/

#项目测试文件所在目录
TEST_PATH			:= $(PROJECT_PATH)test/

#项目测试文件，可执行文件所在目录
TEST_BIN			:= $(PROJECT_PATH)test/bin/

#可执行应用程序目录
BIN_PATH			:= $(PROJECT_PATH)bin/

PERFORMANCE_PATH	:= $(PROJECT_PATH)performance/
#参数选项
LD_FLAGS			:= -lcgicc -lpthread

#编译选项
CC					:= g++

C					:= gcc

#all cgi-bin are being
all : $(BIN_PATH)txproj_pay $(BIN_PATH)txproj_login $(BIN_PATH)txproj_refund $(BIN_PATH)txproj_list $(BIN_PATH)server $(TEST_BIN)mysqlTest

$(BIN_PATH)txproj_pay : $(PROJECT_PATH)cgi/pay.cpp
	[ -e $(BIN_PATH) ] || mkdir $(BIN_PATH)
	$(CC) -g $^ -o $@ -lcgicc -std=c++11

$(BIN_PATH)txproj_login : $(PROJECT_PATH)cgi/login.cpp
	$(CC) -g $^ -o $@ -lcgicc -std=c++11

$(BIN_PATH)txproj_refund : $(PROJECT_PATH)cgi/refund.cpp
	$(CC) -g $^ -o $@ -lcgicc -std=c++11

$(BIN_PATH)txproj_list : $(PROJECT_PATH)cgi/list.cpp
	$(CC) -g $^ -o $@ -lcgicc -std=c++11

$(BIN_PATH)server : $(PROJECT_PATH)server/server.cpp
	$(CC) -g $^ -o $@ -lmysqlclient -std=c++11

$(TEST_BIN)mysqlTest : $(TEST_PATH)mysql.cpp
	[ -e $(TEST_BIN) ] || mkdir $(TEST_BIN)
	$(CC) -g $^ -o $@ -lmysqlclient -std=c++11

.PHONY :

#清除生成 cgi-bin 可执行程序
clean :
	rm -rf $(BIN_PATH) $(TEST_BIN)

#连接目录至 apache 配置项目录
ln : $(PROJECT_PATH)html/txproj_index.html $(PROJECT_PATH)html/txproj_buyer.html $(PROJECT_PATH)html/txproj_seller.html
	sudo ln -s $(PROJECT_PATH)html/bootstrap-3.2.0-dist		$(STATICHTML_PATH)
	sudo ln -s $(PROJECT_PATH)html/txproj_buyer.html		$(STATICHTML_PATH)
	sudo ln -s $(PROJECT_PATH)html/txproj_index.html		$(STATICHTML_PATH)
	sudo ln -s $(PROJECT_PATH)html/txproj_seller.html		$(STATICHTML_PATH)
	sudo ln -s $(BIN_PATH)txproj_pay						$(CGI_PATH)
	sudo ln -s $(BIN_PATH)txproj_login						$(CGI_PATH)
	sudo ln -s $(BIN_PATH)txproj_list						$(CGI_PATH)
	sudo ln -s $(BIN_PATH)txproj_refund						$(CGI_PATH)
	sudo ln -s $(BIN_PATH)server							$(CGI_PATH)

#不到万不得已不可用
cp : $(PROJECT_PATH)html/txproj_index.html $(PROJECT_PATH)html/txproj_buyer.html $(PROJECT_PATH)html/txproj_seller.html
	sudo cp -r $(PROJECT_PATH)html/bootstrap-3.2.0-dist		$(STATICHTML_PATH)
	sudo cp $(PROJECT_PATH)html/txproj_buyer.html 			$(STATICHTML_PATH)
	sudo cp $(PROJECT_PATH)html/txproj_index.html			$(STATICHTML_PATH)
	sudo cp $(PROJECT_PATH)html/txproj_seller.html 			$(STATICHTML_PATH)
	sudo cp $(BIN_PATH)txproj_pay 							$(CGI_PATH)
	sudo cp $(BIN_PATH)txproj_login							$(CGI_PATH)
	sudo cp $(BIN_PATH)txproj_list							$(CGI_PATH)
	sudo cp $(BIN_PATH)txproj_refund 						$(CGI_PATH)
	sudo cp $(BIN_PATH)server								$(CGI_PATH)

#运行服务
run :
	./bin/server &

#初始化 mysql 脚本
#ps:mysql 密码写在命令行时，不安全
sql :
	mysql -uroot -p public < init.sql

#备份 public 数据库
backup :
	mysqldump -uroot -p public > dump.sql

#网站测压
ab :
	[ -e $(PERFORMANCE_PATH)ab/ ] || mkdir $(PERFORMANCE_PATH)ab/
	ab -c 50 -n 500 http://39.107.70.253/txproj_index.html > $(PERFORMANCE_PATH)ab/index.dat
	ab -c 50 -n 500 http://39.107.70.253/txproj_buyer.html > $(PERFORMANCE_PATH)ab/buyer.dat
	ab -c 50 -n 500 http://39.107.70.253/txproj_seller.html > $(PERFORMANCE_PATH)ab/seller.dat

webbench :
	[ -e $(PERFORMANCE_PATH)webbench/ ] || mkdir $(PERFORMANCE_PATH)webbench/
	webbench -c 500 -t 10 http://39.107.70.253/txproj_index.html > $(PERFORMANCE_PATH)webbench/index.dat
	webbench -c 500 -t 10 http://39.107.70.253/txproj_buyer.html > $(PERFORMANCE_PATH)webbench/buyer.dat
	webbench -c 500 -t 10 http://39.107.70.253/txproj_seller.html > $(PERFORMANCE_PATH)webbench/seller.dat

cleandat :
	rm $(PERFORMANCE_PATH)ab/*.dat rm $(PERFORMANCE_PATH)webbench/*.dat
