#MYSQL_SRC = -I/data/home/mysql-5.1.56/include -I/data/home/mysql-5.1.56/regex -I/data/home/mysql-5.1.56/sql
V8_LIB = -L/data/home/v8-3.2.10.15 -lv8

all:
	g++ -O0 -ggdb -shared -o udf_example.so udf_example.cpp -I/usr/include/mysql $(V8_LIB) --std=c++0x $(MYSQL_SRC)

install:
	cp udf_example.so /usr/lib/mysql/plugin/

test:
	mysql test_udf < test.sql
	mysql test_udf < test2.sql

restart:
	/etc/init.d/mysql restart

uninstall:
	rm /usr/lib/mysql/plugin/udf_example.so || true
	mysql test_udf -e 'DROP FUNCTION IF EXISTS execute_js;' || true

#gdb --args ./mysqld --defaults-file=my.ini --verbose --console -u root
