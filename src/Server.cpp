#include "../include/Server.hpp"

namespace Webserv
{
	Server::Server(void) : _host("127.0.0.1"), _port("8080")
	{
		this->_listenFd = 0;
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		// TO DO. Throw exception later on
		if (getaddrinfo(NULL, this->_port.c_str(), &hints, &this->_address) != 0)
			exit(1);
	}

	Server::Server(const std::string &host, const std::string &port) : _host(host), _port(port)
	{
		this->_listenFd = 0;
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		// TO DO. Throw exception later on
		if (getaddrinfo(this->_host.c_str(), this->_port.c_str(), &hints, &this->_address) != 0)
			exit(1);
	}

	Server::Server(const Server &copy) : _port(copy._port)
	{
		*this = copy;
	}

	Server &Server::operator=(const Server &assign)
	{
		if (&assign != this)
			this->_address = assign._address;
		return (*this);
	}

	void Server::initServer(void)
	{
		int optVal = 1;
		(void)optVal;
		// TO DO: Make sure to use SO_REUSEADDR in setsockopt to avoid problem with not binding. Might need to also use SO_REUSEPORT
		this->_listenFd = socket(this->_address->ai_family, this->_address->ai_socktype, this->_address->ai_protocol);
		if (this->_listenFd < 0)
			exit(EXIT_FAILURE);
		if (setsockopt(this->_listenFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) == -1)
			exit(EXIT_FAILURE);
		if (bind(this->_listenFd, this->_address->ai_addr, this->_address->ai_addrlen) < 0)
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
		struct sockaddr_storage clientAddr;
		socklen_t	addrSize = sizeof(clientAddr);
		while (1)
		{
			int newSocket = accept(this->_listenFd, (sockaddr *)&clientAddr, &addrSize);
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
		freeaddrinfo(this->_address);
	}
}
