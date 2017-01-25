# !!! USE TABS NOT SPACES !!!

# GTEST_DIR = ...
# CFLAGS = ...

webserver:
# use -pthread to enable multithreading.
# need to link -lboost_system last.
	g++ -std=c++11 main.cc config_parser.cc -o webserver -pthread -lboost_system

clean:
	rm webserver
