#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

namespace Webserv
{
	class Logger
	{
	private:
		Logger(void);
		Logger(Logger &toCopy);
		Logger &operator=(const Logger &assign);
		~Logger();

	public:
		static void errorLog(const int errorCode, const char*(&f)(int), const bool fatal);
		static void errorLog(const int errorCode, char*(&f)(int), const bool fatal);
	};

}
#endif