#include "Response.h"
#include <boost/asio.hpp>


void Response::send(socket_ptr sock)
{
	const char * response_msg = m_response_msg.c_str();
	boost::asio::write(*sock, boost::asio::buffer(response_msg, strlen(response_msg)));
}


EchoResponse::EchoResponse(char * r)
{
	std::string str(r);
	m_req_msg = str;
	set_status(OK);
}


void EchoResponse::generate_response_msg()
{
	set_response_msg("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
	set_response_msg(get_response_msg()+m_req_msg);
	return;  
}



