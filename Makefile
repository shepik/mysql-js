all:
	g++ -O0 -ggdb -shared -o udf_example.so udf_example.cpp -I/usr/include/mysql -L/data/home/v8-3.2.10.15 -lv8

install:
	cp udf_example.so /usr/lib/mysql/plugin/

test:
	mysql test_udf < test.sql
	mysql test_udf < test2.sql

restart:
	/etc/init.d/mysql restart

uninstall:
	mysql test_udf -e 'DROP FUNCTION IF EXISTS execute_js;'
	rm /usr/lib/mysql/plugin/udf_example.so

#gdb --args ./mysqld --defaults-file=my.ini --verbose --console -u root