#include "logging.h"


// Global static pointer used to ensure a single instance of the class.
Logger* Logger::m_pInstance = NULL; 


/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/
Logger* Logger::Instance()
{
	if (!m_pInstance)   // Only allow one instance of class to be generated.
		m_pInstance = new Logger;
   return m_pInstance;
}


std::string Logger::get_statusPage(){
	std::string response = "<html><body><h1>Status Page</h1>";
	response += "Number of requests received: " + std::to_string(numOfReq) + "<br>";
	response += "Supported URL handlers: <br>";
	for (auto& kv : uri2handlerName) {
		response += kv.first + " => " + kv.second + "<br>";
	}
	response += "</body></html>";
	return response;
}