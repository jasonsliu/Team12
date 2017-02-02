# !!! USE TABS NOT SPACES !!!

GTEST_DIR=googletest/googletest
# CFLAGS = ...
TEST_CLASSES =   server_main_test.cc \
				 config_parser_test.cc \
				 integration_test.py 

webserver:
# use -pthread to enable multithreading.
# need to link -lboost_system last.
	g++ -std=c++11 server_main.cc config_parser.cc -o webserver -pthread -lboost_system

config_parser_test:
	./build_nginx_config_parser.sh
	./build_nginx_config_parser_tests.sh

test:
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread server_main_test.cc config_parser.cc EchoHandler.cc HttpResponse.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_tests -lboost_system -lpthread

test-coverage: 
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread server_main_test.cc config_parser.cc EchoHandler.cc HttpResponse.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -fprofile-arcs -ftest-coverage -o server_tests -lboost_system -lpthread
	./server_tests gcov -r server_main_test.cc

clean:
# use -f to ignore non-existent files
	rm -rf webserver config_parser config_parser_test server_tests *.o *.a *~ *.gcov *.gcda *.gcno 

