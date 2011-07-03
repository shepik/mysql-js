MYSQL_SRC = -I/data/home/mysql-5.1.56/include -I/data/home/mysql-5.1.56/regex -I/data/home/mysql-5.1.56/sql
V8_LIB = -L/data/home/v8-3.2.10.15 -lv8

all:
	g++ -O0 -ggdb -shared -o mysql-js.so mysql-js.cpp -I/usr/include/mysql $(V8_LIB) --std=c++0x $(MYSQL_SRC)

install:
	mysql test_udf -e 'DROP FUNCTION IF EXISTS execute_js'
	cp mysql-js.so /usr/lib/mysql/plugin/
	mysql test_udf -e 'CREATE FUNCTION execute_js RETURNS STRING SONAME "mysql-js.so"'

test:
	mysql test_udf < test.sql

restart:
	/etc/init.d/mysql restart

uninstall:
	mysql test_udf -e 'DROP FUNCTION IF EXISTS execute_js;' || true
	rm /usr/lib/mysql/plugin/mysql-js.so || true

#gdb --args ./mysqld --defaults-file=my.ini --verbose --console -u root
