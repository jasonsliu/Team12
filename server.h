#ifndef SERVER_H
#define SERVER_H

// Copied from https://github.com/dkawashima/CS-3-Boost-Echo-Static-Server/blob/dkawashi-nginx/
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "config_parser.h"
#include "request_handler.h"
#include <cstdlib>
#include <iostream>
//#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
//#include <boost/thread.hpp>
#include <thread>
#include <utility>
#include <string>


using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;


class Server{
public:
	void run_server();
	Server(const NginxConfig &config);
	std::string find_longest_prefix(std::string uri); 

private:
	unsigned short port;
	std::map <std::string, RequestHandler*> uri2handler;

	void session(socket_ptr sock);
};


#endif