# !!! USE TABS NOT SPACES !!!

GTEST_DIR=googletest/googletest
CFLAGS = -std=c++0x -Wall -g

compile_webserver:
# use -pthread to enable multithreading.
# need to link -lboost_system last.
	g++ $(CFLAGS) server_main.cc server.cc config_parser.cc -o webserver -pthread -lboost_system

compile_gtest:
# make googletest 
	g++ $(CFLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

compile_parser_test: compile_gtest
# build config_parser main
	g++ config_parser.cc config_parser_main.cc $(CFLAGS) -o config_parser
# build config_parser test
	g++ $(CFLAGS) -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

compile_server_test: compile_gtest
	g++ $(CFLAGS) -isystem ${GTEST_DIR}/include -pthread server_test.cc server.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_tests -lboost_system -lpthread

run_all_tests: compile_parser_test compile_server_test
	./config_parser_test
	./server_tests

run_test_coverage: CFLAGS += -fprofile-arcs -ftest-coverage
run_test_coverage: compile_parser_test compile_server_test
	./config_parser_test gcov -r config_parser.cc
	./server_tests gcov -r server.cc

clean:
# use -f to ignore non-existent files
	rm -rf webserver server_tests config_parser config_parser_test server_tests *.o *.a *~ *.gcov *.gcda *.gcno 

