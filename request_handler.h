#ifndef REQUEST
#define REQUEST

#include "Constants.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>

//TODO: HTTP version

class Request 
{
public:
	// Request(const char *r, 
	// 	std::map<std::string, Service_type> str2service, 
	// 	std::map<std::string, std::string> str2staticBaseDir);

	// std::string get_url(){return m_url;}
	// Service_type get_service_type(){return m_type;}
	// std::string get_req_msg(){return m_req_msg;}
	// std::string get_static_file_path(){return m_file;}
	// Http_method get_method(){return m_method;}
	// void parse_request();
	// void set_method(std::string method);
	// void set_type(Service_type type){ m_type = type; }

	static unique_ptr<Request> Parse(const std::string& raw_request);
	std::string raw_request() const;
  	std::string method() const;
  	std::string uri() const;
 	std::string version() const;
 	
 	using Headers = std::vector<std::pair<std::string, std::string>>;
  	Headers headers() const;
  	std::string body() const;


private:
	// std::string m_req_msg;// The entire request message
	// Service_type m_type; // echo or static 
	// Http_method m_method; // GET, POST etc.
	// std::string m_url;
	// std::string m_file;
	// std::map <std::string, Service_type> m_str2service;
 //  	std::map <std::string, std::string> m_str2staticBaseDir;
	
};


#endif 