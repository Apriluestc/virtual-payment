
PROJECT_PATH		:= /home/shiny/repository/vp/

CGI_PATH			:= /var/www/cgi-bin/

STATICHTML_PATH		:= /var/www/html/

TEST_PATH			:= $(PROJECT_PATH)test/

TEST_BIN			:= $(PROJECT_PATH)test/bin/

BIN_PATH			:= $(PROJECT_PATH)bin/

LD_FLAGS			:= -lcgicc -lpthread

CC					:= g++

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

clean :
	rm -rf $(BIN_PATH) $(TEST_BIN)

ln : $(PROJECT_PATH)html/txproj_index.html $(PROJECT_PATH)html/txproj_buyer.html $(PROJECT_PATH)html/txproj_seller.html
	sudo ln -s $(PROJECT_PATH)html/bootstrap-3.2.0-dist				$(STATICHTML_PATH)
	sudo ln -s $(PROJECT_PATH)html/txproj_buyer.html				$(STATICHTML_PATH)
	sudo ln -s $(PROJECT_PATH)html/txproj_index.html				$(STATICHTML_PATH)
	sudo ln -s $(PROJECT_PATH)html/txproj_seller.html				$(STATICHTML_PATH)
	sudo ln -s $(BIN_PATH)txproj_pay								$(CGI_PATH)
	sudo ln -s $(BIN_PATH)txproj_login								$(CGI_PATH)
	sudo ln -s $(BIN_PATH)txproj_list								$(CGI_PATH)
	sudo ln -s $(BIN_PATH)txproj_refund								$(CGI_PATH)
	sudo ln -s $(BIN_PATH)server									$(CGI_PATH)

run :
	./bin/server &

sql :
	mysql -uroot -p public < init.sql

backup :
	mysqldump -uroot -p public > dump.sql
