#include "Response.h"
#include "Constants.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <boost/asio.hpp>
#include <algorithm>


//=================================================================


void handle_request(socket_ptr sock, char* data, size_t length){
	Request req(data);

	if(req.get_type()==INVALID_SERVICE)
      {
        const char * response_msg;
        response_msg =  "HTTP/1.0 404 Not Found\r\n"
              "Content-type: text/html\r\n"
              "Content-length: 80\r\n\r\n"
              "<html><body><h1>404 Can not file what you are looking for :(</h1></body></html>";
  
         boost::asio::write(*sock, boost::asio::buffer(response_msg, strlen(response_msg)));
      }
      else if(req.get_type()==ECHO_SERVICE)
      {
        EchoResponse echo_response(data);
        echo_response.generate_response_msg();
        echo_response.send(sock);
      }
      else
      {
         StaticResponse static_response(req.get_file());
         static_response.generate_response_msg();
         static_response.send(sock);
      }
}

//=================================================================


EchoResponse::EchoResponse(char * r)
{
	std::string str(r);
	m_req_msg = str;
	set_status(OK);
}


void EchoResponse::generate_response_msg()
{
	set_response_msg("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
	set_response_msg(get_response_msg()+m_req_msg);
	return;  
}

void EchoResponse::send(socket_ptr sock)
{
	const char * response_msg = get_response_msg().c_str();	
	boost::asio::write(*sock, boost::asio::buffer(response_msg, strlen(response_msg)));
}

//===================================================================


StaticResponse::StaticResponse(std::string file_name)
{
	m_file_name = file_name;
}


void StaticResponse::generate_response_msg()
{
	int file_fd = open(m_file_name.c_str(), O_RDONLY);
	//can not open the file
	if(file_fd == -1)
	{
		set_status(NOT_FOUND);
	}
	struct stat file_info;
	if(fstat(file_fd, &file_info) < 0)
	{
		std::cerr << "Can not retrive file info" << m_file_name << ".\n";
		set_status(NOT_FOUND);
		return; 
	}

	std::string response_headers = "HTTP/1.1 200 OK\r\n";

	// get the content type of the file 
	// Get insight from https://github.com/UCLA-CS130/Team-KBBQ/blob/master/HttpResponse.cc
	std::string file_type;
	std::string file_extension;
	size_t pos = m_file_name.find_last_of('.');
	if (pos == std::string::npos || (pos+1) >= m_file_name.size())
	{
		file_type = "application/octet-stream";
	}
	else 
	{	
	// retreive the file extension
		file_extension = m_file_name.substr(pos+1);
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

	response_headers += "Content-type: ";
	response_headers += file_type;
	response_headers += "\r\n";
	response_headers += "Content-length: ";
	response_headers += std::to_string(file_info.st_size);
	response_headers += "\r\n\r\n";

	close(file_fd);

	set_response_msg(response_headers);
	
	return;
}


void StaticResponse::send(socket_ptr sock)
 {
	const char * response_msg;
	// 404 response
	if(get_status()==NOT_FOUND)
	{
		response_msg =  "HTTP/1.0 404 Not Found\r\n"
   						"Content-type: text/html\r\n"
   						"Content-length: 80\r\n\r\n"
    					"<html><body><h1>404 Can not file what you are looking for :(</h1></body></html>";
	
		boost::asio::write(*sock, boost::asio::buffer(response_msg, strlen(response_msg)));
	}
	// OK 200
	else
	{
		
		response_msg = get_response_msg().c_str();

		// response_msg now only contains headers

		
		 
		// open the file   
		std::ifstream in_stream(m_file_name.c_str(), std::ios::in | std::ios::binary);
		if (!in_stream) 
		{
			response_msg =  "HTTP/1.0 404 Not Found\r\n"
   						"Content-type: text/html\r\n"
   						"Content-length: 80\r\n\r\n"
    					"<html><body><h1>404 Can not file what you are looking for :(</h1></body></html>";
	
			boost::asio::write(*sock, boost::asio::buffer(response_msg, strlen(response_msg)));
	    }

	    //send the headers
		boost::asio::write(*sock, boost::asio::buffer(response_msg, strlen(response_msg)));

		//send the files
		char temp_buffer[1024];
	    while (in_stream.read(temp_buffer, sizeof(temp_buffer)).gcount() > 0) 
	    {
	        boost::asio::write(*sock, boost::asio::buffer(temp_buffer, in_stream.gcount()));
	    }


	}
	
	return;
	
 }