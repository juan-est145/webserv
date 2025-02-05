#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include "Logger.hpp"


namespace Webserv
{
	class Request
	{
	private:
		std::map<std::string, std::string> _reqHeader;

	public:
		Request(void);
		Request(const Request &copy);
		Request &operator=(const Request &assign);
		void processReq(const char *buffer);
		~Request();
	};
}

#endif