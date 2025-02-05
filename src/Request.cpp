#include "../include/Request.hpp"

// Constructors
Request::Request()
{
}

Request::Request(const Request &copy)
{
	(void) copy;
}


// Destructor
Request::~Request()
{
}


// Operators
Request & Request::operator=(const Request &assign)
{
	(void) assign;
	return *this;
}

