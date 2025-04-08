#include "ConfigServer.hpp"


namespace	Webserv{

	ConfigServer::ConfigServer()
	{
		this->_port = 0;
		this->_host = 0;
		this->_serverName = "";
		this->_root = "";
		this->_client_max_body_size = MAX_CONTENT_LENGTH;
		this->_index = "";
		this->_autoindex = false;
		this->initErrorPages();
	}

	ConfigServer::ConfigServer(const ConfigServer &copy)
	{
		if (this != &copy)
		{
			this->_serverName = copy._serverName;
			this->_root = copy._root;
			this->_host = copy._host;
			this->_port = copy._port;
			this->_client_max_body_size = copy._client_max_body_size;
			this->_index = copy._index;
			this->_errorPages = copy._errorPages;
			this->_locations = copy._locations;
			//this->_listen_fd = copy._listen_fd;
			this->_autoindex = copy._autoindex;
			//this->_serverName = copy._server_address;
		}
		return ;
	}

	ConfigServer &ConfigServer::operator=(const ConfigServer &copy)
	{
		if (this != &copy)
		{
			this->_serverName= copy._serverName;
			this->_root = copy._root;
			this->_host = copy._host;
			this->_port = copy._port;
			this->_client_max_body_size = copy._client_max_body_size;
			this->_index = copy._index;
			this->_errorPages = copy._errorPages;
			this->_locations = copy._locations;
			//this->_listen_fd = copy._listen_fd;
			this->_autoindex = copy._autoindex;
			//this->_server_address = copy._server_address;
		}
		return (*this);
	}

	void	ConfigServer::initErrorPages(void)
	{
		this->_errorPages[301] = "";
		this->_errorPages[302] = "";
		this->_errorPages[400] = "";
		this->_errorPages[401] = "";
		this->_errorPages[402] = "";
		this->_errorPages[403] = "";
		this->_errorPages[404] = "";
		this->_errorPages[405] = "";
		this->_errorPages[406] = "";
		this->_errorPages[500] = "";
		this->	_errorPages[501] = "";
		this->_errorPages[502] = "";
		this->_errorPages[503] = "";
		this->_errorPages[504] = "";
		this->_errorPages[505] = "";
	}

	void	ConfigServer::setServerName(std::string serverName)
	{
		checkToken(serverName);
		this->_serverName = serverName;
	}

	void	ConfigServer::setHost(std::string parameter)
	{
		checkToken(parameter);
		if (parameter == "localhost")
			parameter = "127.0.0.1";
		if (!isValidHost(parameter))
			throw ErrorException("Wrong syntax: host");
		this->_host = inet_addr(parameter.data());
	}

	void	ConfigServer::setRoot(std::string root)
	{
		checkToken(root);
		if (ConfigFile::getPathType(root) ==
			ConfigFile::PATH_OTHER)
		{
			this->_root = root;
			return ;
		}

		char dir[1024];
		getcwd(dir, 1024);
		std::string full_root = dir + root;
		if (ConfigFile::getPathType(full_root) !=
			ConfigFile::PATH_OTHER)
			throw ErrorException("Wrong syntax: root");
		this->_root = full_root;
	}

	// void	ConfigServer::setFd(int fd)
	// {
	// 	this->_listen_fd = fd;
	// }

	void	ConfigServer::setPort(std::string parameter)
	{
		unsigned int	port;

		port = 0;
		checkToken(parameter);
		for (size_t i = 0; i < parameter.length(); i++)
		{
			if (!std::isdigit(parameter[i]))
				throw ErrorException("Wrong syntax: port");
		}
		port = stoi(parameter);
		if (port < 1 || port > 65636)
			throw ErrorException("Wrong syntax: port");
		this->_port = (uint16_t) port;
	}

	void	ConfigServer::setClientMaxBodySize
		(std::string parameter)
	{
		unsigned long int	body_size;

		body_size = 0;
		checkToken(parameter);
		for (size_t i = 0; i < parameter.length(); i++)
		{
			if (!std::isdigit(parameter[i]))
				throw ErrorException
					("Wrong syntax: client_max_body_size");
		}
		body_size = stoi(parameter);
		if (!body_size)
			throw ErrorException
				("Wrong syntax: client_max_body_size");
		this->_client_max_body_size = body_size;
	}

	void	ConfigServer::setIndex(std::string index)
	{
		checkToken(index);
		this->_index = index;
	}

	void	ConfigServer::setAutoindex(std::string autoindex)
	{
		checkToken(autoindex);
		if (autoindex != "on" && autoindex != "off")
			throw ErrorException("Wrong syntax: autoindex");
		this->_autoindex = (autoindex == "on" ? true : false);
	}

	std::string statusCodeString(short statusCode)
	{
		switch (statusCode)
		{
			case 100:
				return "Continue";
			case 101:
				return "Switching Protocol";
			case 200:
				return "OK";
			case 201:
				return "Created";
			case 202:
				return "Accepted";
			case 203:
				return "Non-Authoritative Information";
			case 204:
				return "No Content";
			case 205:
				return "Reset Content";
			case 206:
				return "Partial Content";
			case 300:
				return "Multiple Choice";
			case 301:
				return "Moved Permanently";
			case 302:
				return "Moved Temporarily";
			case 303:
				return "See Other";
			case 304:
				return "Not Modified";
			case 307:
				return "Temporary Redirect";
			case 308:
				return "Permanent Redirect";
			case 400:
				return "Bad Request";
			case 401:
				return "Unauthorized";
			case 403:
				return "Forbidden";
			case 404:
				return "Not Found";
			case 405:
				return "Method Not Allowed";
			case 406:
				return "Not Acceptable";
			case 407:
				return "Proxy Authentication Required";
			case 408:
				return "Request Timeout";
			case 409:
				return "Conflict";
			case 410:
				return "Gone";
			case 411:
				return "Length Required";
			case 412:
				return "Precondition Failed";
			case 413:
				return "Payload Too Large";
			case 414:
				return "URI Too Long";
			case 415:
				return "Unsupported Media Type";
			case 416:
				return "Requested Range Not Satisfiable";
			case 417:
				return "Expectation Failed";
			case 418:
				return "I'm a teapot";
			case 421:
				return "Misdirected Request";
			case 425:
				return "Too Early";
			case 426:
				return "Upgrade Required";
			case 428:
				return "Precondition Required";
			case 429:
				return "Too Many Requests";
			case 431:
				return "Request Header Fields Too Large";
			case 451:
				return "Unavailable for Legal Reasons";
			case 500:
				return "Internal Server Error";
			case 501:
				return "Not Implemented";
			case 502:
				return "Bad Gateway";
			case 503:
				return "Service Unavailable";
			case 504:
				return "Gateway Timeout";
			case 505:
				return "HTTP Version Not Supported";
			case 506:
				return "Variant Also Negotiates";
			case 507:
				return "Insufficient Storage";
			case 510:
				return "Not Extended";
			case 511:
				return "Network Authentication Required";
			default:
				return "Undefined";
		}
	}

	void	ConfigServer::setErrorPages(
		std::vector<std::string> &parameter)
	{
		if (parameter.empty())
			return ;
		if (parameter.size() % 2 == 1)
			throw ErrorException
				("Error_page initialization failed");
		for (size_t i = 0; i < parameter.size() - 1; i++)
		{
			for (size_t j = 0; j < parameter[i].size(); j++)
			{
				if (!std::isdigit(parameter[i][j]))
					throw ErrorException("Error code is invalid");
			}
			if (parameter[i].size() != 3)
				throw ErrorException("Error code is invalid");
			short	code_error = stoi(parameter[i]);
			if (statusCodeString(code_error) == "Undefined"
					|| code_error < 400)
				throw ErrorException("Incorrect error code: "
					+ parameter[i]);
			i++;
			std::string path = parameter[i];
			checkToken(path);
			if (ConfigFile::getPathType(path)
				!= ConfigFile::PATH_OTHER)
			{
				if (ConfigFile::getPathType(this->_root + path)
					!= ConfigFile::PATH_FOLDER)
					throw ErrorException
						("Incorrect path for error page file: "
						+ this->_root + path);
				if (ConfigFile::fileOk
						(this->_root + path) == -1
					|| ConfigFile::fileOk
						(this->_root + path) == -1)
					throw ErrorException
						("Error page file: "
						+ this->_root + path
						+ " is not accesible");
			}
			std::map<short, std::string>::iterator it =
				this->_error_pages.find(code_error);
			if (it != _error_pages.end())
				this->_error_pages[code_error] = path;
			else
				this->_error_pages.insert
					(std::make_pair(code_error, path));
		}
	}

	void	ConfigServer::setLocation(std::string nameLocation,
		std::vector<std::string> parameter)
	{
		//TO DO
	}

	bool	ConfigServer::isValidHost(std::string host) const
	{
		struct	sockaddr_in	sockaddr;

		return (inet_pton(AF_INET, host.c_str(),
			&(sockaddr.sin_addr)) ? true : false);
	}

	bool	ConfigServer::isValidErrorPages()
	{
		std::map<short, std::string>::const_iterator	it;
		for (it = this->_error_pages.begin();
			it != this->_error_pages.end(); it++)
		{
			if (it->first < 100 || it->first > 599)
				return (false);
			if (ConfigFile::fileOk
				(getRoot() + it->second) < 0)
				return (false);
		}
		return (true);
	}

	int		ConfigServer::isValidLocation
		(Location &location)
	{
		//TO DO
	}

	int		ConfigServer::getFd()
	{
		return (this->_listen_fd);
	}

	const std::string	&ConfigServer::getServerName() const
	{
		return (this->_server_name);
	}

	const uint16_t	&ConfigServer::getPort() const
	{
		return (this->_port);
	}

	const in_addr_t	&ConfigServer::getHost() const
	{
		return (this->_host);
	}

	const size_t	&ConfigServer::getClientMaxBodySize() const
	{
		return (this->_client_max_body_size);
	}

	const std::vector<Location>
		&ConfigServer::getLocations() const
	{
		return (this->_locations);
	}

	const std::string	&ConfigServer::getRoot() const
	{
		return (this->_root);
	}

	const std::map<short, std::string>
		&ConfigServer::getErrorPages() const
	{
		return (this->_error_pages);
	}

	const std::string	&ConfigServer::getIndex() const
	{
		return (this->_index);
	}

	const bool	&ConfigServer::getAutoindex() const
	{
		return (this->_autoindex);
	}

	const std::string	&ConfigServer::getPathErrorPage
		(short key) const
	{
		std::map<short, std::string>::const_iterator
			it = this->_error_pages.find(key);
		if (it == this->_error_pages.end())
			throw ErrorException("Error_page does not exist");
		return (it->second);
	}

	const std::vector<Location>::const_iterator
		ConfigServer::getLocationKey(std::string key) const
	{
		std::vector<Location>::const_iterator	it;
		for (it = this->_locations.begin();
			it != this->_locations.end(); it++)
		{
			if (it->getPath() == key)
				return (it);
		}
		throw ErrorException("Error: path to location not found");
	}

	void ConfigServer::checkToken
		(std::string &parameter)
	{
		size_t	pos = parameter.rfind(';');
		if (pos != parameter.size() - 1)
			throw ErrorException("Token is invalid");
		parameter.erase(pos);
	}

	bool ConfigServer::checkLocations()
	{
		std::vector<Location>::const_iterator it1;
		std::vector<Location>::const_iterator it2;
		if (this->_locations.size() < 2)
			return (false);
		for (it1 = this->_locations.begin();
			it1 != this->_locations.end() - 1; it1++) {
			for (it2 = it1 + 1;
				it2 != this->_locations.end(); it2++) {
				if (it1->getPath() == it2->getPath())
					return (true);
			}
		}
		return (false);
	}

	ConfigServer::~ConfigServer() {}

}
