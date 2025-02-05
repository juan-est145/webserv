#include "../include/Request.hpp"

namespace Webserv
{
	Request::Request(void) {}

	Request::Request(const Request &copy)
	{
		(void)copy;
	}

	Request &Request::operator=(const Request &assign)
	{
		(void)assign;
		return *this;
	}

	Request::~Request() {}
}

// Constructors
