#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <exception>
#include "ConfigFile.hpp"
#include "ConfigServer.hpp"

namespace Webserv
{
	//class ConfigServer;

	class ConfigParser
	{
	private:
		std::vector<ConfigServer> _servers;
		std::vector<std::string> _serversConfig;
		size_t _nServers;
		/**
		 * Removes the comments from '#' to '\n'
		 */
		void removeComments(std::string &content);
		/**
		 * Removes spaces from the start, end and in the
		 * content if there's more than one space
		 */
		void removeSpaces(std::string &content);
		/**
		 * Initializes vector of strings (_server_config)
		 * @throw exception if substring
		 * "server" isn't found in content
		 */
		void initServersConfig(std::string &content);
		/**
		 * Finds the first character of the server
		 * @return index of the 1st char if found, -1 if not found.
		 */
		size_t findStartServer(size_t start, std::string &content);
		/**
		 * Finds the last character of the server
		 * @return index of the last char if found, -1 if not found.
		 */
		size_t findEndServer(size_t start, std::string &content);
		/**
		 * Splits a string into a vector of strings separated by
		 * a delimitator
		 * @return a vector of strings
		 */
		std::vector<std::string> splitParameters(std::string line, std::string del);
		/**
		 * Compare str1 and str2 until pos
		 * @return 0 if equal, anything if not
		 */
		int strnCompare(std::string str1, std::string str2, size_t pos);

		/**
		 * Creates a servers and fills values with config
		 */
		void createServer(std::string &config, ConfigServer &server);

		/**
		 * Checks if there are repeated parameters
		 * @throw if repeated parameter, throws an exception
		 */
		void checkServers();

	public:
		ConfigParser();

		/**
		 * Checks if the file is valid, saves its content,
		 * cleans it up, splits the string into servers
		 * and saves its content into this class.
		 * @throw error if anything fails
		 * @return 0 if it doesn't throw anything
		 */
		void initConfigParser(const std::string &config_file);

		/**
		 * @return a vector of servers
		 */
		const std::vector<ConfigServer> &getServers();

		class ErrorException : public std::exception
		{
		private:
			std::string _message;

		public:
			ErrorException()
			{
				_message = "CONFIG PARSER ERROR: unknown";
			}
			ErrorException(std::string message) throw()
			{
				_message = "CONFIG PARSER ERROR: " + message;
			}
			virtual const char *what() const throw()
			{
				return (_message.c_str());
			}
			virtual ~ErrorException() throw()
			{
			}
		};
		~ConfigParser();
	};
}

#endif
