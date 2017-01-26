# !!! USE TABS NOT SPACES !!!

# GTEST_DIR = ...
# CFLAGS = ...

webserver:
# use -pthread to enable multithreading.
# need to link -lboost_system last.
	g++ -std=c++11 server_main.cc config_parser.cc -o webserver -pthread -lboost_system

config_parser_test:
	./build_nginx_config_parser.sh
	./build_nginx_config_parser_tests.sh

clean:
# use -f to ignore non-existent files
	rm -f webserver config_parser config_parser_test *.o *.a *~