// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.h"
#include "Response.h"

//TODO: Logging

struct server_parameters{
  int port = -1;
  std::map <std::string, Service_type> str2service;
  std::map <std::string, std::string> str2staticBaseDir;
};


server_parameters get_server_parameters(const NginxConfig &config) {
  server_parameters sp;

  for (const auto& statement : config.statements_) {
    const std::vector<std::string> tokens = statement->tokens_;

      if( tokens[0] == "server" ){
        return get_server_parameters(*statement->child_block_.get());
      }
      else if ( tokens[0] == "listen" ){
        if (tokens.size() >= 2){ 
          sp.port = stoi(tokens[1]); 
        } 
        else{ 
          std::cerr << "CONFIG ERROR: PORT " << tokens.size() << std::endl;
          sp.port = -1; 
        }        
      }   
      else if ( tokens[0] == "echo_service" ) {
        if (tokens.size() >= 2){
          sp.str2service[tokens[1]] = ECHO_SERVICE;
        }
        else{
          std::cerr << "CONFIG ERROR: ECHO SERVICE " << tokens.size() << std::endl;
        }
      }
      else if ( tokens[0] == "static_service" ) {
        if (tokens.size() >= 3){
          sp.str2service[tokens[1]] = STATIC_SERVICE;
          std::string dir = tokens[2];
          if ( dir[dir.length() - 1] != '/' ){ dir += '/'; }
          sp.str2staticBaseDir[tokens[1]] = dir;
        }
        else{
          std::cerr << "CONFIG ERROR: STATIC SERVICE " << tokens.size() << std::endl;
        }
      }
  }

  return sp;
}


Server::Server(const NginxConfig &config)
{
  server_parameters sp = get_server_parameters(config);

  this->port = sp.port;
  this->str2service = sp.str2service;
  this->str2staticBaseDir = sp.str2staticBaseDir;
}


void Server::session(socket_ptr sock)
{
  const int max_length = 1024;

  try
  {
    // TODO: implement timeout for session
    for (;;)
    {
      char data[max_length];
      boost::system::error_code error;
      size_t length = sock->read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
      
      Request req(data, this->str2service, this->str2staticBaseDir);
      handle_request(sock, req, length);
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}


void Server::run_server()
{
  boost::asio::io_service io_service;
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));

  // Reuse port (server useually )
  // TODO: Implement catch signal to cleanly shut down server
  // http://stackoverflow.com/questions/6364681/how-to-handle-control-c-in-a-boost-tcp-udp-server
  a.set_option(tcp::acceptor::reuse_address(true));

  std::cout << "Listening on port " << port << "\n";
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    
    // use std thread for non-blocking.
    std::thread t(&Server::session, this, std::move(sock));
    t.detach();
  }
}