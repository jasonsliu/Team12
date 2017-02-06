# !!! USE TABS NOT SPACES !!!

GTEST_DIR=googletest/googletest
CFLAGS = -std=c++0x -Wall -g

compile_webserver:
# use -pthread to enable multithreading.
# need to link -lboost_system last.
	g++ $(CFLAGS) server_main.cc server.cc config_parser.cc -o webserver -pthread -lboost_system

compile_gtest:
# make googletest, NOT use variable CFLAGS to aviod redundant files
	g++ $(CFLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

compile_parser_test: compile_gtest
# build config_parser main
	g++ config_parser.cc config_parser_main.cc $(CFLAGS) -o config_parser
# build config_parser test
	g++ $(CFLAGS) -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

compile_server_test: compile_gtest
	g++ $(CFLAGS) -isystem ${GTEST_DIR}/include -pthread server_test.cc server.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_tests -lboost_system -lpthread

compile_all_tests: compile_parser_test compile_server_test

compile_all_tests_with_coverage:  CFLAGS += -fprofile-arcs -ftest-coverage
compile_all_tests_with_coverage: compile_all_tests

run_all_tests: compile_all_tests_with_coverage
	./config_parser_test
	./server_tests

run_test_coverage: run_all_tests
# add dependency to run_all_tests only when all tests pass
	gcov -r config_parser.cc
	gcov -r server.cc

run_integration_test: compile_webserver
	python integration_test.py

clean:
# use -f to ignore non-existent files
	rm -rf webserver server_tests config_parser config_parser_test server_tests *.o *.a *~ *.gcov *.gcda *.gcno 

