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
			if (line.substr(0, pos).size() > 0)
				headers.push(line.substr(0, pos));
			line.erase(0, pos + del.length());
			pos = line.find(del);
		}
		// Need to check for valid http method header here for later
		this->_reqHeader["GET"] = headers.front();
		headers.pop();
		while (headers.size() > 0)
		{
			std::string temp = headers.front();
			headers.pop();
			pos = temp.find(":");
			if (pos == std::string::npos)
				exit(EXIT_FAILURE); // Temp solution
			if (this->_reqHeader.find(temp.substr(0, pos)) != this->_reqHeader.end())
			{
				std::cout << "Repeated header, handle later" << std::endl;
				exit(EXIT_FAILURE);
			}
			this->_reqHeader[temp.substr(0, pos)] = temp.substr(pos + 2);
		}
	}
	Request::~Request() {}
}