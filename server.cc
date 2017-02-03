// Copied from https://github.com/dkawashima/CS-3-Boost-Echo-Static-Server/blob/dkawashi-nginx/
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.h"

void session(socket_ptr sock)
{
  const int max_length = 1024;

  try
  {
    for (;;)
    {
      char data[max_length];
      boost::system::error_code error;
      size_t length = sock->read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
		  
      // simple http response
      // TODO: make more sophisticated http request handles (for later project).
      const char* httpResponseHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
      boost::asio::write(*sock, boost::asio::buffer(httpResponseHeader, strlen(httpResponseHeader)));

      boost::asio::write(*sock, boost::asio::buffer(data, length));
      
      return;;
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, short port)
{
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
    std::thread t(session, std::move(sock));
    t.detach();
  }
}

/* copied from yichi example lines 3-14 */
int getPort(const NginxConfig &config) { // Gets port from config_file
  for (const auto& statement : config.statements_) {
    bool kl = true;

    //DEBUG: print statement
    std::cout << "DEBUG: statement\n";
    for (const std::string& token : statement->tokens_) {

      // DEBUG: print all tokens
      std::cout << "DEBUG:" << token << std::endl;
      if (!kl) {
        try { return stoi(token); } catch (...) {}
      }
      kl = (token != "listen");
    }
  }
  return -1;
}