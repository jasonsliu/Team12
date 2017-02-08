#include "Request.h"


Request::Request(const char *r)
{
	std::string str(r);
	m_req_msg = str;
    parse_request();
}


void Request::set_type(Service_type type)
{
    m_type = type;
}


void Request::set_method(std::string method)
{
	if (method.compare("GET") == 0)
    {
        m_method = GET;
    }
    else if (method.compare("POST") == 0)
    {
        m_method = POST;
    }
    else if (method.compare("HEAD") == 0)
    {
        m_method = HEAD;
    }
    else if (method.compare("PUT") == 0)
    {
        m_method = PUT;
    }
    else if (method.compare("TRACE") == 0)
    {
        m_method = TRACE;
    }
    else if (method.compare("CONNECT") == 0)
    {
        m_method = CONNECT;
    }
    else if (method.compare("OPTION") == 0)
    {
        m_method = OPTION;
    }
    else
    {
        m_method = GET;
    }
}


std::string s_toupper(std::string s){
    std::string rs = "";
    for (std::string::size_type i=0; i < s.length(); ++i)
        rs += toupper(s[i]);
    return rs;
}


void Request::parse_request()
{
    std::cout<<"start parse_request"<<std::endl;
    //create a istringstream object corresponding to the requst msg
    std::istringstream req_stream(m_req_msg);
    std::string req_first_line;
    //get the first line from the requst message 
    std::getline(req_stream, req_first_line);
    std::cout<<"debug msg: "<<"first line of request is :"<<req_first_line<<std::endl;


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


    std::cout << "Method is " << method << std::endl;
    std::cout << "Url is " << url << std::endl;
    std::cout << "version is " << http_ver << std::endl;

    m_url = url;   //url can either indicate echo or file request
    set_method(method);

    //need to specify echo or static 
    if(url == "/")
    {
            std::cout<<"invalid type"<<std::endl;
            set_type(INVALID_SERVICE);
    }
    else 
    {
        url.erase(0,1);
        size_t pos = url.find("/");
        std::string type = s_toupper(url.substr(0, pos));
        if(type=="ECHO")
        {
            std::cout << "echo type"<<std::endl; 
            set_type(ECHO_SERVICE);
        }
        else if(type == "STATIC")
        {
            std::cout << "static type"<<std::endl; 
            set_type(STATIC_SERVICE);
            // if does not exist second "/"  example: /STATIC 
            if (pos == std::string::npos)
            {
                m_file = "";
            }
            else 
            {
                m_file = url.substr(pos+1, std::string::npos);
            }
            if(m_file[m_file.length()-1]=='/')
            {
                m_file.erase(m_file.length()-1);
            }
            std::cout<<"FILE NAME IS: "<<m_file<<std::endl;
        }
        else
        {
            std::cout<<"invalid type"<<std::endl;
            set_type(INVALID_SERVICE);
        }
        // if does not exist second "/"  example: /STATIC 

    }



    return ;
}