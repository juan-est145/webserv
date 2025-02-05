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
	// TO DO: consider creating attributes to HTTP method with an enum and the response code to use later on with Response class
	private:
		std::map<std::string, std::string> _reqHeader;

	public:
		Request(void);
		Request(const Request &copy);
		Request &operator=(const Request &assign);
		void processReq(const char *buffer);
		const std::map<std::string, std::string> &getReqHeader(void) const;
		~Request();
	};
}

#endif