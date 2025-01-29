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
		socklen_t addrSize = sizeof(clientAddr);
		struct epoll_event event;
		struct epoll_event eventList[20];
		event.events = EPOLLIN | EPOLLOUT;
		int epollFd = epoll_create(NUMBER_EPOLL);
		if (epollFd == -1)
			exit(EXIT_FAILURE);
		event.data.fd = this->_listenFd;
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, this->_listenFd, &event) == -1)
			exit(EXIT_FAILURE);
		while (1)
		{
			int eventCount = epoll_wait(epollFd, eventList, sizeof(eventList), E_WAIT_TIMEOUT);
			if (eventCount == -1)
				exit(EXIT_FAILURE);
			for (int i = 0; i < eventCount; i++)
			{
				if (eventList[i].data.fd == this->_listenFd)
				{
					int newSocket = accept(this->_listenFd, (sockaddr *)&clientAddr, &addrSize);
					if (newSocket < 0)
						exit(EXIT_FAILURE);
					if (epoll_ctl(epollFd, EPOLL_CTL_ADD, newSocket, &event) == -1)
						exit(EXIT_FAILURE);
				}
				else
				{
					// TO DO: Read about flags in recv and send
					char buffer[1024];
					ssize_t bufRead = recv(eventList[i].data.fd, buffer, sizeof(buffer), 0);
					if (bufRead == -1)
						exit(EXIT_FAILURE);
					std::string response = "Hola caracola\n";
					send(eventList[i].data.fd, response.c_str(), response.size(), 0);
					close(eventList[i].data.fd);
					epoll_ctl(epollFd, EPOLL_CTL_DEL, eventList[i].data.fd, &event);
				}
			}
		}
		epoll_ctl(epollFd, EPOLL_CTL_DEL, this->_listenFd, &event);
	}

	Server::~Server()
	{
		if (close(this->_listenFd) < 0)
			std::cout << "Something got fucked xd" << std::endl;
		freeaddrinfo(this->_address);
	}
}
