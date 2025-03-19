#include "ConfigServer.hpp"


namespace	Webserv{

	ConfigServer::ConfigServer()
	{
		this->_port = 0;
		this->_host = 0;
		this->_server_name = "";
		this->_root = "";
		this->_client_max_body_size = MAX_CONTENT_LENGTH;
		this->_index = "";
		this->_listen_fd = 0;
		this->_autoindex = false;
		this->initErrorPages();
	}

	ConfigServer::~ConfigServer()
	{

	}

	ConfigServer::ConfigServer(const ConfigServer &copy)
	{
		if (this != &copy)
		{
			this->_server_name = copy._server_name;
			this->_root = copy._root;
			this->_host = copy._host;
			this->_port = copy._port;
			this->_client_max_body_size = copy._client_max_body_size;
			this->_index = copy._index;
			this->_error_pages = copy._error_pages;
			this->_locations = copy._locations;
			this->_listen_fd = copy._listen_fd;
			this->_autoindex = copy._autoindex;
			this->_server_address = copy._server_address;
		}
		return ;
	}

	ConfigServer &ConfigServer::operator=(const ConfigServer &copy)
	{
		if (this != &copy)
		{
			this->_server_name = copy._server_name;
			this->_root = copy._root;
			this->_host = copy._host;
			this->_port = copy._port;
			this->_client_max_body_size = copy._client_max_body_size;
			this->_index = copy._index;
			this->_error_pages = copy._error_pages;
			this->_locations = copy._locations;
			this->_listen_fd = copy._listen_fd;
			this->_autoindex = copy._autoindex;
			this->_server_address = copy._server_address;
		}
		return (*this);
	}

	void	ConfigServer::initErrorPages(void)
	{
		_error_pages[301] = "";
		_error_pages[302] = "";
		_error_pages[400] = "";
		_error_pages[401] = "";
		_error_pages[402] = "";
		_error_pages[403] = "";
		_error_pages[404] = "";
		_error_pages[405] = "";
		_error_pages[406] = "";
		_error_pages[500] = "";
		_error_pages[501] = "";
		_error_pages[502] = "";
		_error_pages[503] = "";
		_error_pages[504] = "";
		_error_pages[505] = "";
	}

	void	ConfigServer::setServerName(std::string server_name)
	{
		checkToken(server_name);
		this->_server_name = server_name;
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

	void	ConfigServer::setFd(int fd)
	{
		this->_listen_fd = fd;
	}

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
			if (statusCodeString)
		}
	}

	void	ConfigServer::setLocation(std::string nameLocation,
		std::vector<std::string> parameter)
	{

	}
}
