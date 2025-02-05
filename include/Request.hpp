#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>

namespace Webserv
{
	class Request
	{
	private:
		std::map<std::string, std::string> reqHeader;

	public:
		Request(void);
		Request(const Request &copy);
		Request &operator=(const Request &assign);
		~Request();
	};
}

#endif