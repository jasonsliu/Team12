#ifndef REQUEST
#define REQUEST

#include "Constants.h"


class Request {
public:
	Request(char *r);
	int decode();
private:
	// type can either be echo or file 
	std::string type;
	// The actual request message
	char *req_msg;
	

};





#endif
