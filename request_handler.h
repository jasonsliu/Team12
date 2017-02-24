#ifndef REQUEST
#define REQUEST

#include "Constants.h"
#include "config_parser.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include　<fstream>

//TODO: HTTP version

class Request 
{
public:
	static unique_ptr<Request> Parse(const std::string& raw_request);
	std::string raw_request() const;
  	std::string method() const;
  	std::string uri() const;
 	std::string version() const;
 	
 	using Headers = std::vector<std::pair<std::string, std::string>>;
  	Headers headers() const;
  	std::string body() const;

	//TODO: i.e. /static/foo/bar -> "/static" + "/foo/bar"
	std::string get_uriHead();
	std::string get_uriTail();

private:

};


// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(RESPONSE_200);
//   r.SetBody(...);
//   return r.ToString();
//
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.
class Response {
 public:
  enum ResponseCode {
    //Informational
	CONTINUE=100,
	SWITCHING,

	//Success
	OK=200,
	CREATED,
	ACCEPTED,
	NO_CONTENT=204,

	//Redirection
	MOVED_PERMANENTLY=301,
	MOVED_TEMPORARILY,
	NOT_MODIFIED=304,

	//Client Error
	BAD_REQUEST=400,
	UNAUTHORIZED,
	FORBIDDEN=403,
	NOT_FOUND,
	METHOD_NOT_ALLOWED,
	NOT_ACCEPTABLE,

	//Server Error
	INTERNAL_SERVER_ERROR=500,
	NOT_IMPLEMENTED,
	SERVICE_UNAVAILABLE=503
  };
  
  void SetStatus(const ResponseCode response_code);
  //What is header_name?
  void AddHeader(const std::string& header_name, const std::string& header_value);
  void SetBody(const std::string& body);
  
  std::string ToString();
};


// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class RequestHandler {
 public:
	enum Status {
		OK = 0;
		NOT_FOUND;
		ERROR;
	};
  
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;
  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;
};



// Different types of derived handlers listed below for now
// for convenience
// may put in separate files later

class Handler_Echo: public RequestHandler {
	public:
		virtual Status Init(const std::string& uri_prefix, 
						const NginxConfig& config){
			this->uri = uri_prefix;
		}
		virtual Status HandleRequest(const Request& request,
                               Response* response);

	private:
		std::string uri;
}


class Handler_Static: public RequestHandler {
	public:
		virtual Status Init(const std::string& uri_prefix, 
						const NginxConfig& config);
		virtual Status HandleRequest(const Request& request,
                               Response* response);
		
	private:
		std::string uri;
		std::string rootDir;
}


class Handler_404: public RequestHandler {
	public:
		virtual Status Init(const std::string& uri_prefix, 
						const NginxConfig& config){
			this->uri = uri_prefix;
		}
		virtual Status HandleRequest(const Request& request,
                               Response* response);

	private:
		std::string uri;
}


class Handler_500: public RequestHandler {
	public:
		virtual Status Init(const std::string& uri_prefix, 
						const NginxConfig& config){
			this->uri = uri_prefix;
		}
		virtual Status HandleRequest(const Request& request,
                               Response* response);

	private:
		std::string uri;
}

#endif 