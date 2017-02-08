#ifndef REQUEST
#define REQUEST

#include "Constants.h"
#include <string>
#include <iostream>
#include <sstream>


class Request 
{
public:
	Request(const char *r);

	std::string get_url(){return m_url;}
	Service_type get_type (){return m_type;}
	std::string get_req_msg(){return m_req_msg;}
	std::string get_file(){return m_file;}
	Http_method get_method(){return m_method;}


	void set_method(std::string method);
	void set_type(Service_type type);
private:
	void parse_request();

	// The actual request message
	std::string m_req_msg;	
	// type can either be echo or file 
	Service_type m_type;
	//  POST or GET ...
	Http_method m_method;

	std::string m_url;

	std::string m_file;
  
   // don not have to implement http version yet
};


#endif 