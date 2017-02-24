#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

#include "Constants.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>


class Request {
 public:
  static unique_ptr<Request> Parse(const std::string& raw_request);

  std::string raw_request() const;
  std::string method() const;
  std::string uri() const;
  std::string version() const;

  using Headers = std::vector<std::pair<std::string, std::string>>;
  Headers headers() const;

  std::string body() const;

  private:
  	std::string m_raw_request; //
  	std::string m_method;  //
  	std::string m_uri;   //
  	std::string m_version; //
  	std::vector<std::pair<std::string, std::string>> m_headers; 
  	std::string m_body;

};

// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(RESPONSE_200);
//   r.SetBody(...);
//   return r.ToString();
//
class Response {
 public:
  enum ResponseCode {
    // Define your HTTP response codes here.
    OK = 200,
    NOT_FOUND = 404,
    NOT_IMPLEMENTED = 501
  };
  
  void SetStatus(const ResponseCode response_code);
  void AddHeader(const std::string& header_name, const std::string& header_value);
  void SetBody(const std::string& body);
  
  std::string ToString();

 private:
 	ResponseCode m_code;
 	std::vector<std::pair<std::string, std::string>> m_headers;
 	std::string m_body;
 	// add a new private member, easier to deal with in ToString()
 	std::string m_first_line;

};

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class RequestHandler {
 public:
  enum Status {
    OK = 0;

    // Define your status codes here.
  };
  
  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500.
  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;
};


#endif 