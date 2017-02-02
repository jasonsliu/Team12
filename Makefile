# !!! USE TABS NOT SPACES !!!

GTEST_DIR=googletest/googletest
CFLAGS = -std=c++0x

webserver:
# use -pthread to enable multithreading.
# need to link -lboost_system last.
	g++ $(CFLAGS) server_main.cc config_parser.cc -o webserver -pthread -lboost_system

test_make_gtest:
	g++ $(CFLAGS) ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

config_parser_test:
	./build_nginx_config_parser.sh
	./build_nginx_config_parser_tests.sh

test: config_parser_test
test: CFLAGS += -isystem
test:
	g++ $(CFLAGS) ${GTEST_DIR}/include -pthread server_main_test.cc server_main.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_tests -lboost_system -lpthread

test-coverage: CFLAGS += -fprofile-arcs -ftest-coverage
test-coverage: test
test-coverage:
	./server_tests gcov -r server_main_test.cc

clean:
# use -f to ignore non-existent files
	rm -rf webserver config_parser config_parser_test server_tests *.o *.a *~ *.gcov *.gcda *.gcno 

