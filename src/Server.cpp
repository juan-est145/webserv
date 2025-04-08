/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/08 18:20:42 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

extern bool g_stop;

namespace Webserv
{
	Server::Server(void) : _host("127.0.0.1"), _port("8080")
	{
		this->_listenFd = -1;
		this->_epollFd = -1;
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
		this->_listenFd = -1;
		this->_epollFd = -1;
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

	void Server::listenConnection(void)
	{
		struct epoll_event event;
		// TO DO: Lat er on, try make eventList a buffer in HEAP and multiply a base value
		// by how many sockets we are going to be listening to
		struct epoll_event eventList[50];
		this->_epollFd = epoll_create(NUMBER_EPOLL);
		if (this->_epollFd == -1)
		{
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_ADD, EPOLLIN, this->_listenFd, event))
			throw Server::ServerException();
		while (!g_stop)
		{
			int eventCount = epoll_wait(this->_epollFd, eventList, sizeof(eventList), E_WAIT_TIMEOUT);
			if (eventCount == -1)
			{
				if (g_stop)
					break;
				std::cout << "Failed here" << std::endl;
				close(this->_epollFd);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Server::ServerException();
			}
			for (int i = 0; i < eventCount; i++)
			{
				if (eventList[i].data.fd == this->_listenFd)
					this->addConnectionToQueue(event);
				else
					this->processClientConn(eventList[i], event);
			}
		}
		if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_DEL, EPOLLIN, this->_listenFd, event))
			throw Server::ServerException();
		close(this->_epollFd);
	}

	void Server::addConnectionToQueue(struct epoll_event &event) const
	{
		struct sockaddr_storage clientAddr;
		socklen_t addrSize = sizeof(clientAddr);
		std::cout << "We have a connection" << std::endl;
		int newSocket = accept(this->_listenFd, (sockaddr *)&clientAddr, &addrSize);
		if (newSocket < 0)
		{
			close(this->_epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_ADD, EPOLLIN, newSocket, event))
			throw Server::ServerException();
	}

	void Server::processClientConn(struct epoll_event &eventList, struct epoll_event &eventConf)
	{
		if (eventList.events & EPOLLIN)
			this->readOperations(eventList, eventConf);
		else
			this->writeOperations(eventList, eventConf);
	}

	void Server::readOperations(struct epoll_event &eventList, struct epoll_event &eventConf)
	{
		struct stat statbuf;
		if (fstat(eventList.data.fd, &statbuf) == -1)
		{
			close(this->_epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		if (S_ISSOCK(statbuf.st_mode))
			this->readSocket(eventList, eventConf);
		// Later on, this could be used for CGI
		// else
		// 	this->readFile(eventList, eventConf);
	}

	void Server::readSocket(struct epoll_event &eventList, struct epoll_event &eventConf)
	{
		// TO DO: Set dynamic buffer size according to body size.
		// TO DO: Possibly will need to add a timer to listening to timeout connection
		char buffer[1024];
		std::cout << "Reading from client " << eventList.data.fd << std::endl;
		// If later bufRead  is less than size of buffer, then we now for a fact that we have read everything.
		// Try to implement later onto to the logic of the program.
		ssize_t bufRead = recv(eventList.data.fd, buffer, sizeof(buffer) - 1, 0);
		if (bufRead <= 0)
		{
			AuxFunc::handleRecvError(eventConf, eventList, bufRead, this->_epollFd);
			return;
		}
		buffer[bufRead] = '\0';
		Request *req = new Request(eventList.data.fd);
		req->readReq(buffer, bufRead);
		// TO DO: Work with transfer encoding chunked later on
		if (req->getMethod() == Request::POST)
		{
			std::size_t bodySize = req->getReqBody().size();
			std::size_t expectedSize;
			const Request::T_reqHeadIter conLenKey = req->getReqHeader().find("Content-Length");
			if (conLenKey == req->getReqHeader().end())
				req->setResCode(400);
			else
			{
				expectedSize = std::atol(conLenKey->second.c_str());
				// TO DO: Later on, we need to check the config for max body size. This is all very messy for now
				while (bodySize < expectedSize)
				{
					// TO DO: In order to avoid lagging behind with 'big download requests', we need
					// to reserve enough space in the body property of request. Should use the value given by conf
					memset(buffer, '\0', sizeof(buffer));
					bufRead = recv(eventList.data.fd, buffer, sizeof(buffer) - 1, 0);
					if (bufRead <= 0)
					{
						AuxFunc::handleRecvError(eventConf, eventList, bufRead, this->_epollFd);
						return;
					}
					buffer[bufRead] = '\0';
					std::string body(buffer, bufRead);
					bodySize = req->setReqBody(body);
				}
			}
		}
		req->handleReq();
		this->_clientPool[eventList.data.fd] = req;
		if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_MOD, EPOLLOUT, eventList.data.fd, eventConf))
			throw Webserv::Server::ServerException();
	}

	// Later on, this might be useful for CGI
	// void Server::readFile(struct epoll_event &eventList, struct epoll_event &eventConf)
	// {
	// 	int htmlFd = eventList.data.fd;
	// 	int socketFd = this->_htmlFdSockPair[htmlFd]->getSocketFd();
	// 	long size = this->_htmlFdSockPair[htmlFd]->getRequest().getResourceData().size;
	// 	char *buffer = new char[size + 1];
	// 	// TO DO. Check value of read. If negative, maybe send a response code of the 500 family?
	// 	read(eventList.data.fd, buffer, size);
	// 	buffer[size] = '\0';
	// 	this->_htmlFdSockPair[htmlFd]->setContent(buffer);
	// 	delete[] buffer;
	// 	if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_DEL, EPOLLIN, eventList.data.fd, eventConf))
	// 		throw Server::ServerException();
	// 	this->_sockFdHtmlPair[socketFd] = this->_htmlFdSockPair[htmlFd];
	// 	this->_htmlFdSockPair.erase(htmlFd);
	// 	close(eventList.data.fd);
	// 	if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_ADD, EPOLLOUT, socketFd, eventConf))
	// 		throw Server::ServerException();
	// }

	void Server::writeOperations(struct epoll_event &eventList, struct epoll_event &eventConf)
	{
		std::cout << "Time to write to the client " << eventList.data.fd << std::endl;
		std::stringstream format;
		const Request *req = this->_clientPool[eventList.data.fd];
		// *----CAMBIO----//
		HttpResponse Hresp;
		std::string response;
		Director director;
		ConcreteBuilder *builder = new ConcreteBuilder(&Hresp);//hay que borrar este objeto
		director.SetBuilder(builder);
		
		if (req->getResCode() == 200)
		{
			director.BuildOkResponse(req->getPath().c_str());
			response = Hresp.Print(req);
			
		}
		else if (req->getResCode() == 201)
		{
			director.BuildOkUploadResponse(req->getResourceSize());
			response = Hresp.Print(req);
		}
		else if (req->getResCode() == 404)
		{
			director.BuildNotFoundResponse();
			response = Hresp.Print(req);
		}
		if (send(eventList.data.fd, response.c_str(), response.size(), 0) == -1)
			Webserv::Logger::errorLog(errno, strerror, false);
		delete builder;
		// *----------//
		if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_DEL, EPOLLOUT, eventList.data.fd, eventConf))
			throw Server::ServerException();
		delete this->_clientPool[eventList.data.fd];
		this->_clientPool.erase(eventList.data.fd);
		close(eventList.data.fd);
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
