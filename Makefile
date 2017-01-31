# !!! USE TABS NOT SPACES !!!

GTEST_DIR=googletest/googletest
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
	rm -f webserver config_parser config_parser_test server_tests *.o *.a *~

test:
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread server_main_test.cc config_parser.cc EchoHandler.cc HttpResponse.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_tests -lboost_system -lpthread
