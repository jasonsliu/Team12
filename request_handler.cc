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



//========= HANDLERSS ===========================================


virtual RequestHandler::Status Handler_Echo::HandleRequest(const Request& req, Response* res){
	res->SetStatus(res->OK);
	res->AddHeader("Content-type", "text/plain")
	res->AddHeader("Content-length", std::to_string((int)req->raw_request().length()));
	res->SetBody(req->raw_request());
	return OK;
}


virtual RequestHandler::Status Handler_404::HandleRequest(const Request& req, Response* res){
	res->SetStatus(res->NOT_FOUND);
	res->AddHeader("Content-type", "text/html")
	res->AddHeader("Content-length", "79");
	res->SetBody("<html><body><h1>404 Can not file what you are looking for :(</h1></body></html>");
	return OK;
}


virtual RequestHandler::Status Handler_500::HandleRequest(const Request& req, Response* res){
	res->SetStatus(res->INTERNAL_SERVER_ERROR);
	res->AddHeader("Content-type", "text/html")
	res->AddHeader("Content-length", "62");
	res->SetBody("<html><body><h1>500 Internal Server Error!(</h1></body></html>");
	return OK;
}


virtual RequestHandler::Status Handler_Static::Init(const std::string& uri_prefix, const NginxConfig& config)
{
	this->uri = uri_prefix;
	const std::vector<std::string> tokens = config->tokens_;
	for (const auto& statement : config.statements_) {
		const std::vector<std::string> tokens = statement->tokens_;
		if (tokens[0] == "root"){
			if (tokens.size() >= 2){ 
				this->rootDir = torkens[1]
			}
		}
	}
}


virtual RequestHandler::Status Handler_Static::HandleRequest(const Request& req, Response* res){
	std::string file_name = this->rootDir + req.uriTail()
	int file_fd = open(, O_RDONLY);
	//can not open the file
	if(file_fd == -1)
	{
		return NOT_FOUND;
	}
	struct stat file_info;
	if(fstat(file_fd, &file_info) < 0)
	{
		std::cerr << "Can not retrive file info" << file_name << ".\n";
		return ERROR;
	}

	res->SetStatus(res->OK);

	// get the content type of the file 
	// Get insight from https://github.com/UCLA-CS130/Team-KBBQ/blob/master/HttpResponse.cc
	std::string file_type;
	std::string file_extension;
	size_t pos = file_name.find_last_of('.');
	if (pos == std::string::npos || (pos+1) >= file_name.size())
	{
		res->AddHeader("Content-type", "application/octet-stream");
	}
	else 
	{	
	// retreive the file extension
		file_extension = file_name.substr(pos+1);
		std::transform(file_extension.begin(), file_extension.end(), file_extension.begin(), ::tolower);
		
		if(file_extension=="gif")
			file_type = "image/gif";

		else if(file_extension=="jpeg")
			file_type = "image/jpeg";

		else if(file_extension=="jpg")
			file_type = "image/jpeg";

		else if(file_extension=="htm")
			file_type = "text/html";

		else if(file_extension=="html")
			file_type = "text/html";

		else if(file_extension=="png")
			file_type = "image/png";

		else if(file_extension=="pdf")
			file_type = "application/pdf";

		else
			file_type = "text/plain";
	}

	std::cout<< "file type is: " <<file_type<<std::endl;
	res->AddHeader("Content-type", file_type)
	res->AddHeader("Content-length", std::to_string(file_info.st_size));

	// Read entire file
	std::ifstream ifs(file_name);
  	std::string content( (std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()) );
	res->SetBody(content);
	return OK;
}
