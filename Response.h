#ifndef RESPONSE
#define RESPONSE

#include "Constants.h"
#include "Request.h"
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;


void handle_request(socket_ptr sock, char* data, size_t length);


class Response{
public:
	virtual void generate_response_msg()=0;
	virtual void send(socket_ptr sock) = 0;
	void set_response_msg(std::string response_msg){m_response_msg = response_msg;}
	void set_status(Http_status status) {m_status = status;}
	std::string get_response_msg(){return m_response_msg;}
	Http_status get_status(){return m_status;}
private:
	//m_response_msg will only contain headers for the Static file response
	std::string m_response_msg;
	Http_status m_status;
};

class EchoResponse : public Response{
public:
	EchoResponse(char * r);
	virtual void generate_response_msg();
	virtual void send(socket_ptr sock);

private:
	std::string m_req_msg;
};


class StaticResponse : public Response {
public:
    StaticResponse(std::string file_name);
    virtual void generate_response_msg();
    virtual void send(socket_ptr sock);

private:
	std::string m_file_name ;

};


#endif