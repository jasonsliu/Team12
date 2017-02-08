// Copied from https://github.com/dkawashima/CS-3-Boost-Echo-Static-Server/blob/dkawashi-nginx/
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.h"
#include "Request.h"
#include "Response.h"
#include "Constants.h"

Server::Server(int p)
{
  port = p;
}

void Server::session(socket_ptr sock)
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
		  
      

      Request req(data);
      //parse the request message
      req.parse_request();

      if(req.get_type()==INVALID_SERVICE)
      {
        const char * response_msg;
        response_msg =  "HTTP/1.0 404 Not Found\r\n"
              "Content-type: text/html\r\n"
              "Content-length: 80\r\n\r\n"
              "<html><body><h1>404 Can not file what you are looking for :(</h1></body></html>";
  
         boost::asio::write(*sock, boost::asio::buffer(response_msg, strlen(response_msg)));
      }
      else if(req.get_type()==ECHO_SERVICE)
      {
        EchoResponse echo_response(data);
        echo_response.generate_response_msg();
        echo_response.send(sock);
      }
      else
      {
         StaticResponse static_response(req.get_file());
         static_response.generate_response_msg();
         static_response.send(sock);
      }      
      return;
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}


void Server::run_server(boost::asio::io_service& io_service)
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
    std::thread t(&Server::session, this, std::move(sock));
    t.detach();
  }
}













