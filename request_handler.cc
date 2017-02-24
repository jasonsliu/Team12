#include "request_handler.h"

std::unique_ptr<Request> Request::Parse(const std::string& raw_request){
	
	std::unique_ptr<Request> return_val(new Request());

	std::cout<<"start parse_request"<<std::endl;
	std::istringstream req_stream(raw_request);
	std::string req_first_line;


	//deal with the first line from the requst message 
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

    return_val->m_uri = url;
    return_val->m_method = method;
    return_val->m_version = http_ver;
    return_val->m_raw_request = raw_request;

    // now deal with the headers 
    std::string line;
    std::string field;
	std::string value;
    while(std::getline(req_stream, line)&& line != "\r")
    {
    	std::size_t index = line.find_first_of(":");
    	if(index == std::string::npos)
    	{
    		std::cout << "can not parse request line : "<<line<< std::endl;
    	}
    	else
    	{
    		field = line.substr(0, index);
    		value = line.substr(index+2);
    		// delete the \r at the end
    		value = value.substr(0, value.length()-1);
    	}

    	std::cout << "push the header:  " <<field << " : " <<value << std::endl;
    	return_val->m_headers.push_back(std::make_pair(field, value));

    }


    // deal with the body of the request 
    while(std::getline(req_stream, line))
    {
    	return_val->m_body += line;
    }
    std::cout << "the body of the request is : "<< std::endl<< return_val->m_body <<std::endl;

    return return_val;

}

std::string Request::raw_request() const {
    return m_raw_request;
}

std::string Request::method() const {
    return m_method;
}

std::string Request::uri() const {
    return m_uri;
}

std::string Request::version() const {
    return m_version;
}

Request::Headers Request::headers() const {
    return m_headers;
}

std::string Request::body() const {
    return m_body;
}


// =================  RESPONSE ===================================


void Response::SetStatus(const ResponseCode response_code)
{
	m_code = response_code;
	if(response_code == OK)
	{
		m_first_line = "HTTP/1.0 200 OK"
	}
	else if(response_code == NOT_FOUND)
	{
		m_first_line = "HTTP/1.0 400 Not Found"

	}
	else if(response_code== NOT_IMPLEMENTED)
	{
		m_first_line = "HTTP/1.0 501 Not Implemented"
	}
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value) 
{
	m_headers.push_back(std::make_pair(header_name, header_value));
}

void Response::SetBody(const std::string& body) 
{
	m_body = body; 
}

std::string Response::ToString()
{
	// deal with the first line of the response
	std::string ret_val = m_first_line;

	for(int n= 0 ;n<m_headers.size(); n++)
	{
		std::pair<std::string, std::string> header_pair = m_headers[n];
		ret_val += header_pair.first;
		ret_val += ": ";
		ret_val +=  header_pair.second;
		ret_val += "\r\n"
	}
	ret_val += "\r\n";

	//deal with the body

	ret_val += m_body;

	return ret_val;
}








