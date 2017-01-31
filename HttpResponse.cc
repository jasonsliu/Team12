#include "request_handler.h"


namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

}
// Converts response object to valid buffer for return
std::vector<boost::asio::const_buffer> HttpResponse::to_buffers()
{
  std::vector<boost::asio::const_buffer> buffers;
  std::string status = "HTTP/" + http_version_ + " " + status_code_ + " " + reason_phrase_ + "\r\n";
  buffers.push_back(boost::asio::buffer(status));
  for (std::size_t i = 0; i < headers_.size(); ++i)
  {
    std::pair<std::string, std::string> h = headers_[i];
    buffers.push_back(boost::asio::buffer(h.first));
    buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
    buffers.push_back(boost::asio::buffer(h.second));
    buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  }
  buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  buffers.push_back(boost::asio::buffer(body_));
  return buffers;
}