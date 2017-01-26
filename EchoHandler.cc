
#include "request_handler.h"

bool EchoHandler::Init(const std::map<std::string, std::string>& config_map) {
return 0;
//Does Nothing if echo.
};



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
bool EchoHandler::HandleRequest(const HttpRequest& request, HttpResponse* response){
	
	response->status_code_ = "200";
    response->http_version_ = "1.0";
    response->reason_phrase_ = "OK";
    response->body_ = request.raw_request_;
    
    std::pair<std::string, std::string> pair("Content-Length", boost::lexical_cast<std::string>(response->body_.size()));
    std::pair<std::string, std::string> pair1("Content-Type", "text/plain");
    response->headers_.push_back(pair);
    response->headers_.push_back(pair1);

    return 0;
};
