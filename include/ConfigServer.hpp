#ifndef CONFIGSERVER_HPP
#define CONFIGSERVER_HPP

#ifndef MAX_CONTENT_LENGTH
#define MAX_CONTENT_LENGTH 35000000
#endif

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
#include <vector>
#include <map>
#include "ConfigFile.hpp"
#include "ConfigParser.hpp"
#include "Logger.hpp"

namespace Webserv
{
	static std::string serverParameters[] =
		{
			"server_name",
			"listen",
			"root",
			"index",
			"allow_methods",
			"client_body_buffer_size"
		};

	class Location;

	class ConfigServer
	{
	private:
		uint16_t _port;
		in_addr_t _host;
		std::string _serverName;
		std::string _root;
		std::string _index;
		unsigned long _clientMaxBodySize;
		bool _autoindex;
		std::map<short, std::string> _errorPages;
		std::vector<Location> _locations;

	public:
		// Default constructor
		ConfigServer();
		// Default destructor
		~ConfigServer();
		// Copy constructor
		ConfigServer(const ConfigServer &copy);
		// ConfigServer assignation operator overload
		ConfigServer &operator=(const ConfigServer &copy);

		/**
		 * Initializes all error pages at ""
		 */
		void initErrorPages(void);

		/**
		 * Sets the server name attribute
		 * @throw exception if token is invalid
		 */
		void setServerName(std::string serverName);

		/**
		 * Sets the host attribute
		 * @throw exception if token is invalid or if
		 * the syntax is invalid
		 */
		void setHost(std::string parameter);

		/**
		 * Sets the root attribute
		 * @throw exception if token is invalid or if
		 * the syntax is invalid
		 */
		void setRoot(std::string root);

		// /**
		//  * Sets the fd attribute
		//  */
		// void setFd(int fd);

		/**
		 * Sets the port attribute
		 * @throw exception if token is invalid or if
		 * the syntax is invalid
		 */
		void setPort(std::string parameter);

		/**
		 * Sets the client_max_body_size attribute
		 * @throw exception if token is invalid or if
		 * the syntax is invalid
		 */
		void setClientMaxBodySize(std::string parameter);

		/**
		 * Checks if there's a default error code.
		 * If there is, it overwrites the path to the file
		 * If there's not, it creates a new pair
		 * (error_code, path_to_the_file)
		 * @throw exception if page initialization failed,
		 * if error code is invalid, if path is incorrect
		 * or if the error page file is not accesible
		 */
		void setErrorPages(
			std::vector<std::string> &parameter);

		/**
		 * Sets the index atttribute
		 * @throw exception if token is invalid
		 */
		void setIndex(std::string index);

		/**
		 * Parses and sets locations
		 * @throw exception if any attribute is duplicated
		 * or any token is invalid
		 */
		void setLocation(std::string nameLocation, std::vector<std::string> parameter);

		/**
		 * Sets the autoindex
		 * @throw exception if token is invalid or
		 * the syntax is wrong
		 */
		void setAutoindex(std::string autoindex);

		/**
		 * Checks host
		 * @return true if host is valid, false if invalid
		 */
		bool isValidHost(std::string host) const;

		/**
		 * Valid error pages
		 * @return true if error pages are valid, false if
		 * invalid
		 */
		bool isValidErrorPages();

		/**
		 * Checks parameters of location
		 * @return 0 if ok, 1 if not ok
		 */
		int isValidLocation(Location &location);

		/**
		 * Gets listen fd
		 */
		int getFd();

		/**
		 * Gets server name
		 */
		const std::string &getServerName() const;

		/**
		 * Gets port
		 */
		const uint16_t &getPort() const;

		/**
		 * Gets host
		 */
		const in_addr_t &getHost() const;

		/**
		 * Gets client_max_body_size
		 */
		const size_t &getClientMaxBodySize() const;

		/**
		 * Gets a vector of Locations
		 */
		const std::vector<Location> &getLocations() const;

		/**
		 * Gets root
		 */
		const std::string &getRoot() const;

		/**
		 * Gets error pages
		 */
		const std::map<short, std::string> &getErrorPages() const;

		/**
		 * Gets index
		 */
		const std::string &getIndex() const;

		/**
		 * Gets autoindex
		 */
		const bool &getAutoindex() const;

		/**
		 * Gets path of the error page
		 * @throw exception if error_page doesn't exist
		 */
		const std::string &getPathErrorPage(short key) const;

		/**
		 * Finds location by the name
		 * @throw exception if path to location not found
		 */
		const std::vector<Location>::const_iterator
		getLocationKey(std::string key) const;

		/**
		 * Check if parameter is properly ended
		 * @throw exception if token is invalid
		 */
		static void checkToken(std::string &parameter);

		/**
		 * Checks if there's a duplicate location
		 * @return true if yes, false if not
		 */
		bool checkLocations();

	public:
		class ErrorException : public std::exception
		{
		private:
			std::string _message;

		public:
			ErrorException() throw()
			{
				_message = "SERVER CONFIG ERROR: unknown";
			}
			ErrorException(std::string message) throw()
			{
				_message = "SERVER CONFIG ERROR: " + message;
			}
			virtual const char *what() const throw()
			{
				return (_message.c_str());
			}
			virtual ~ErrorException() throw() {}
		};
	};
};

#endif
