#include "../include/Server.hpp"

namespace Webserv
{
	Server::Server(void) : _port(8080)
	{
		this->_listenFd = 0;
		this->_sizeAddress = sizeof(this->_address);
	}

	Server::Server(const unsigned short int port) : _port(port)
	{
		this->_listenFd = 0;
		this->_sizeAddress = sizeof(this->_address);
	}

	Server::Server(const Server &copy) : _port(copy._port)
	{
		*this = copy;
	}

	Server &Server::operator=(const Server &assign)
	{
		if (&assign != this)
		{
			this->_address = assign._address;
			this->_sizeAddress = assign._sizeAddress;
		}
		return (*this);
	}

	void Server::initServer(void)
	{
		// TO DO: Make sure to use SO_REUSEADDR in setsockopt to avoid problem with not binding
		this->_listenFd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_listenFd < 0)
			exit(EXIT_FAILURE);
		this->_address.sin_family = AF_INET;
		this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
		this->_address.sin_port = htons(this->_port);
		memset(this->_address.sin_zero, '\0', sizeof(this->_address.sin_zero));
		if (bind(this->_listenFd, (sockaddr *)&(this->_address), this->_sizeAddress) < 0)
		{
			std::cout << "Failed bind " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		if (listen(this->_listenFd, 20) < 0)
			exit(EXIT_FAILURE);
		this->listenConnection();
		// TO DO: Refactor exit method to log errors first. Create a specific method for that
	}

	void Server::listenConnection(void)
	{
		while (1)
		{
			int newSocket = accept(this->_listenFd, (sockaddr *)&(this->_address), (socklen_t *)&(this->_sizeAddress));
			if (newSocket < 0)
				exit(EXIT_FAILURE);
			std::string response = "Hola caracola\n";
			int writeLen = write(newSocket, response.c_str(), response.size());
			if (writeLen > 0)
			{
				close(newSocket);
				break;
			}
			close(newSocket);
		}
	}

	Server::~Server()
	{
		if (close(this->_listenFd) < 0)
			std::cout << "Something got fucked xd" << std::endl;
	}
}

