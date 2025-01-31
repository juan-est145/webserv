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
		struct epoll_event event;
		// Later on, try make eventList a buffer in HEAP and multiply a base value
		// by how many sockets we are going to be listening to
		struct epoll_event eventList[50];
		event.events = EPOLLIN;
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
					this->addConnectionToQueue(epollFd, event);
				else
					this->processClientConn(epollFd, eventList[i], event);
			}
		}
		if (epoll_ctl(epollFd, EPOLL_CTL_DEL, this->_listenFd, &event) == -1)
			exit(EXIT_FAILURE);
		close(epollFd);
	}

	void Server::addConnectionToQueue(int epollFd, struct epoll_event &event)
	{
		struct sockaddr_storage clientAddr;
		socklen_t addrSize = sizeof(clientAddr);
		std::cout << "We have a connection" << std::endl;
		int newSocket = accept(this->_listenFd, (sockaddr *)&clientAddr, &addrSize);
		if (newSocket < 0)
			exit(EXIT_FAILURE);
		event.events = EPOLLIN;
		event.data.fd = newSocket;
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, newSocket, &event) == -1)
			exit(EXIT_FAILURE);
	}

	void Server::processClientConn(int epollFd, struct epoll_event &eventList, struct epoll_event &eventConf)
	{
		if (eventList.events & EPOLLIN)
		{
			// TO DO: Set dynamic buffer size according to body size.
			// TO DO: Possibly will need to add a timer to listening to timeout connection
			char buffer[1024];
			std::cout << "Reading from client" << std::endl;
			ssize_t bufRead = recv(eventList.data.fd, buffer, sizeof(buffer), 0);
			if (bufRead <= 0)
			{
				if (bufRead == -1)
					exit(EXIT_FAILURE);
				if (epoll_ctl(epollFd, EPOLL_CTL_DEL, eventList.data.fd, NULL) == -1)
					exit(EXIT_FAILURE);
				close(eventList.data.fd);
			}
			else
			{
				eventConf.events = EPOLLOUT;
				eventConf.data.fd = eventList.data.fd;
				if (epoll_ctl(epollFd, EPOLL_CTL_MOD, eventList.data.fd, &eventConf) == -1)
					exit(EXIT_FAILURE);
			}
		}
		else
		{
			// Aquí es donde se procesa la respuesta Miguel. Prueba aquí si quieres a mandar una respuesta GET HTTP-1.1 para verla en el navegador
			// Si intentas hacer eso, crea una clase aparte y en otra rama para evitar problemas y conflictos.
			std::cout << "Time to write to the client" << std::endl;
			// Construir una respuesta HTTP/1.1
			std::string response = 
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: 14\r\n"
				"\r\n"
				"Hola caracola\n";
			if (send(eventList.data.fd, response.c_str(), response.size(), 0) == -1)
				exit(EXIT_FAILURE);
			if (epoll_ctl(epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
				exit(EXIT_FAILURE);
			close(eventList.data.fd);
		}
	}

	Server::~Server()
	{
		if (close(this->_listenFd) < 0)
			std::cout << "Something got fucked xd" << std::endl;
		freeaddrinfo(this->_address);
	}
}
