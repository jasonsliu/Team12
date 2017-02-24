q#include "request_handler.h"


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
