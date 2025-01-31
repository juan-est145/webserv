#include "../include/Server.hpp"

extern bool g_stop;

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
		if (int errorCode = getaddrinfo(NULL, this->_port.c_str(), &hints, &this->_address) != 0)
		{
			Webserv::Logger::errorLog(errorCode, gai_strerror, false);
			throw Server::ServerException();
		}
	}

	Server::Server(const std::string &host, const std::string &port) : _host(host), _port(port)
	{
		this->_listenFd = 0;
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		if (int errorCode = getaddrinfo(this->_host.c_str(), this->_port.c_str(), &hints, &this->_address) != 0)
		{
			Webserv::Logger::errorLog(errorCode, gai_strerror, false);
			throw Server::ServerException();
		}
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
		this->_listenFd = socket(this->_address->ai_family, this->_address->ai_socktype, this->_address->ai_protocol);
		if (this->_listenFd < 0)
		{
			freeaddrinfo(this->_address);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		if (setsockopt(this->_listenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optVal, sizeof(optVal)) == -1)
		{
			freeaddrinfo(this->_address);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		if (bind(this->_listenFd, this->_address->ai_addr, this->_address->ai_addrlen) < 0)
		{
			freeaddrinfo(this->_address);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		freeaddrinfo(this->_address);
		if (listen(this->_listenFd, 20) < 0)
		{
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		this->listenConnection();
	}

	void Server::listenConnection(void) const
	{
		struct epoll_event event;
		// TO DO: Later on, try make eventList a buffer in HEAP and multiply a base value
		// by how many sockets we are going to be listening to
		struct epoll_event eventList[50];
		event.events = EPOLLIN;
		int epollFd = epoll_create(NUMBER_EPOLL);
		if (epollFd == -1)
		{
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		event.data.fd = this->_listenFd;
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, this->_listenFd, &event) == -1)
		{
			close(epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		while (!g_stop)
		{
			int eventCount = epoll_wait(epollFd, eventList, sizeof(eventList), E_WAIT_TIMEOUT);
			if (eventCount == -1)
			{
				if (g_stop)
					break;
				std::cout << "Failed here" << std::endl;
				close(epollFd);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Server::ServerException();
			}
			for (int i = 0; i < eventCount; i++)
			{
				if (eventList[i].data.fd == this->_listenFd)
					this->addConnectionToQueue(epollFd, event);
				else
					this->processClientConn(epollFd, eventList[i], event);
			}
		}
		if (epoll_ctl(epollFd, EPOLL_CTL_DEL, this->_listenFd, &event) == -1)
		{
			close(epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		close(epollFd);
	}

	void Server::addConnectionToQueue(int epollFd, struct epoll_event &event) const
	{
		struct sockaddr_storage clientAddr;
		socklen_t addrSize = sizeof(clientAddr);
		std::cout << "We have a connection" << std::endl;
		int newSocket = accept(this->_listenFd, (sockaddr *)&clientAddr, &addrSize);
		if (newSocket < 0)
		{
			close(epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		event.events = EPOLLIN;
		event.data.fd = newSocket;
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, newSocket, &event) == -1)
		{
			close(epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
	}

	void Server::processClientConn(int epollFd, struct epoll_event &eventList, struct epoll_event &eventConf) const
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
				{
					close(epollFd);
					Webserv::Logger::errorLog(errno, strerror, false);
					throw Server::ServerException();
				}
				if (epoll_ctl(epollFd, EPOLL_CTL_DEL, eventList.data.fd, NULL) == -1)
				{
					close(epollFd);
					Webserv::Logger::errorLog(errno, strerror, false);
					throw Server::ServerException();
				}
				close(eventList.data.fd);
			}
			else
			{
				eventConf.events = EPOLLOUT;
				eventConf.data.fd = eventList.data.fd;
				if (epoll_ctl(epollFd, EPOLL_CTL_MOD, eventList.data.fd, &eventConf) == -1)
				{
					close(epollFd);
					Webserv::Logger::errorLog(errno, strerror, false);
					throw Server::ServerException();
				}
			}
		}
		else
		{
			// Aquí es donde se procesa la respuesta Miguel. Prueba aquí si quieres a mandar una respuesta GET HTTP-1.1 para verla en el navegador
			// Si intentas hacer eso, crea una clase aparte y en otra rama para evitar problemas y conflictos.
			std::cout << "Time to write to the client" << std::endl;
			std::string response = "Hola caracola\n";
			if (send(eventList.data.fd, response.c_str(), response.size(), 0) == -1)
				Webserv::Logger::errorLog(errno, strerror, false);
			if (epoll_ctl(epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
			{
				close(epollFd);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Server::ServerException();
			}
			close(eventList.data.fd);
		}
	}

	const char *Server::ServerException::what(void) const throw()
	{
		return ("The server found a problem and must stop now");
	}

	Server::~Server()
	{
		if (close(this->_listenFd) < 0)
			Webserv::Logger::errorLog(errno, strerror, true);
	}
}
