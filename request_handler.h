#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "config_parser.h"
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <thread>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
//#include "request_parser.h"
#include <fstream>
//using Headers = std::vector<std::pair<std::string, std::string>>;
// Above line does not work with older versions of C++
typedef std::vector<std::pair<std::string, std::string>> Headers;

struct HttpRequest {
std::string raw_request_;
std::string method_;
std::string uri_;
std::string version_;
Headers headers_;
std::string body_;
};
class HttpResponse {
public:
std::string http_version_;
std::string status_code_;
std::string reason_phrase_;
Headers headers_;
std::string body_;
std::vector<boost::asio::const_buffer> to_buffers();
};
// Created a server startup, and lasts for the lifetime of the server.
class RequestHandler {
public:

// Initializes the RequestHandler from a map of configuration keys and values.
// Returns true if the RequestHandler was initialized successfully. If the
// return value is false, the object is invalid and should not be used.
//
// The config_map is the keys-value pairs in the config file.
//
// For example, given:
// handler static {
// path /static;
// directory /foo;
// }
// The config_map would have keys "path" and "directory" with values "/static" and "/foo".
virtual bool Init(const std::map<std::string, std::string>& config_map) = 0;
// Handles an HTTP request. Writes the response to the HttpResponse object,
// and returns true if the request was handled successfully. If the return value
// is false, then the HttpResponse object is invalid and should not be used.
//
// Each config block has a special config named "path" that determines
// the path prefix for which the handler will be called.
//
// For example, the config "handler echo { path /echo; }" means that an echo
// handler will be called for all requests whose URI paths start with "/echo".
//
// The dispatch mechanism is implemented in the main server code.
virtual bool HandleRequest(const HttpRequest& request, HttpResponse* response) = 0;
};

namespace mime_types {

/// Convert a file extension into a MIME type.
std::string extension_to_type(const std::string& extension);

} // namespace mime_types

class EchoHandler : public RequestHandler{
public:

bool Init(const std::map<std::string, std::string>& config_map);

bool HandleRequest(const HttpRequest& request, HttpResponse* response);
};

class StaticHandler : public RequestHandler {
public:

std::string directory;

bool Init(const std::map<std::string, std::string>& config_map);

bool HandleRequest(const HttpRequest& request, HttpResponse* response);
bool url_decode(const std::string& in, std::string& out);
};

class Not_Found_Handler : public RequestHandler { 
public:

bool Init(const std::map<std::string, std::string>& config_map);

bool HandleRequest(const HttpRequest& request, HttpResponse* response);
};

class request_parser
{
public:
  /// Construct ready to parse the request method.
  request_parser();

  /// Reset to initial parser state.
  void reset();

  /// Parse some data. The tribool return value is true when a complete request
  /// has been parsed, false if the data is invalid, indeterminate when more
  /// data is required. The InputIterator return value indicates how much of the
  /// input has been consumed.
  template <typename InputIterator>
  boost::tuple<boost::tribool, InputIterator> parse(HttpRequest& req,
      InputIterator begin, InputIterator end)
  {
    while (begin != end)
    {
      boost::tribool result = consume(req, *begin++);
      if (result || !result)
        return boost::make_tuple(result, begin);
    }
    boost::tribool result = boost::indeterminate;
    return boost::make_tuple(result, begin);
  }

private:
  /// Handle the next character of input.
  boost::tribool consume(HttpRequest& req, char input);

  /// Check if a byte is an HTTP character.
  static bool is_char(int c);

  /// Check if a byte is an HTTP control character.
  static bool is_ctl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool is_tspecial(int c);

  /// Check if a byte is a digit.
  static bool is_digit(int c);

  /// The current state of the parser.
  enum state
  {
    method_start,
    method,
    uri_start,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    expecting_newline_2,
    expecting_newline_3
  } state_;
};