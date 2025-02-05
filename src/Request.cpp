#include "../include/Request.hpp"

namespace Webserv
{
	Request::Request(void) {}

	Request::Request(const Request &copy)
	{
		*this = copy;
	}

	Request &Request::operator=(const Request &assign)
	{
		if (this != &assign)
			this->_reqHeader = assign._reqHeader;
		return *this;
	}

	void Request::processReq(const char *buffer)
	{	
		std::string line(buffer);
		std::string del = "\r\n";
		std::queue<std::string> headers;
		std::size_t pos = line.find(del);
		while (pos != std::string::npos)
		{
			headers.push(line.substr(0, pos));
			line.erase(0, pos + del.length());
			pos = line.find(del);
		}
	}
	Request::~Request() {}
}