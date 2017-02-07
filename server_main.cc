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


int getPort(const NginxConfig &config) { // Gets port from config_file
  for (const auto& statement : config.statements_) {
    bool kl = true;

    for (const std::string& token : statement->tokens_) {
   //   std::cout << ">>>>>>>>>>>DEBUG" << token << std::endl;
      if (token == "server"){
        return getPort(*statement->child_block_.get());
      }
      
   //   std::cout << ">>>>>>>>>>>DEBUG" << kl << std::endl;
      if (!kl) {
        try { return stoi(token); } catch (...) {}
      }
      kl = (token != "listen");
    }
  }
  return -1;
}

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
      std::cerr  << "Parse config file failed\n" ;
      return -1;
    }
    int port = getPort(config);

    Server server(port);
    boost::asio::io_service io;

    server.run_server(io);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}