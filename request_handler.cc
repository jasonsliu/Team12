#include "request_handler.h"

std::unique_ptr<Request> Request::Parse(const std::string& raw_request){
	std::cout<<"start parse_request"<<std::endl;
	std::istringstream req_stream(raw_request);
	std::string req_first_line;
	//get the first line from the requst message 
    std::getline(req_stream, req_first_line);
	// start to decode first line 
    std::string method = "";
    std::string url = "";
    std::string http_ver = "";
    int space_count = 0;
    for (unsigned i = 0; i < req_first_line.length(); i++)
    {
        if (req_first_line[i] == ' ')
        {
            space_count++;
            continue;
        }
        if (space_count == 0)
        {
            method += toupper(req_first_line[i]);
        }
        else if (space_count == 1)
        {
            url += req_first_line[i];
        }
        else
        {
            http_ver += toupper(req_first_line[i]);
        }
    }

    m_uri = url;
    m_method = method;
    m_version = heep_ver;
    m_raw_request = raw_request;



}