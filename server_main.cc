// Copied from https://github.com/dkawashima/CS-3-Boost-Echo-Static-Server/blob/dkawashi-nginx/
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "config_parser.h"
#include "server.h"
#include <cstdlib>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
  using namespace std; 
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: webserver <config_file>\n";      
      return 1;
    }

    /* 
      copied from yichi server.main lines 17 to 21 
      parse server configuration
    */
    NginxConfigParser config_parser;
    NginxConfig config;
    if (!config_parser.Parse(argv[1], &config)) {
      return -1;
    }

    int port_ = getPort(config);
    boost::asio::io_service io;
    server(io, port_);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}