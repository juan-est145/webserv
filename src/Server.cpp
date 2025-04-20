/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/20 14:29:46 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

namespace Webserv
{
	Server::Server(void) : _configurations()
	{
		this->_listenFd = -1;
	}

	Server::Server(const std::vector<ConfigServer> &configurations, int listenFd) : _configurations(configurations)
	{
		this->_listenFd = listenFd;
	}

	Server::Server(const Server &copy) : _configurations(copy._configurations)
	{
		*this = copy;
	}

	Server &Server::operator=(const Server &assign)
	{
		if (&assign != this)
		{
			this->_listenFd = assign._listenFd;
			this->_clientPool = assign._clientPool;
		}
		return (*this);
	}

	void Server::processClientConn(int socketFd, int eventListIndex)
	{
		const struct epoll_event *eventList = Cluster::cluster->getEventList();
		if (eventList[eventListIndex].events & EPOLLIN)
			this->readOperations(socketFd, eventList[eventListIndex]);
		else
			this->writeOperations(eventList[eventListIndex]);
	}

	void Server::readOperations(int socketFd, const struct epoll_event &eventList)
	{
		struct stat statbuf;
		if (fstat(socketFd, &statbuf) == -1)
		{
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		if (S_ISSOCK(statbuf.st_mode))
			this->readSocket(eventList);
		// Later on, this could be used for CGI
		// else
		// 	this->readFile(eventList, eventConf);
	}

	void Server::readSocket(const struct epoll_event &eventList)
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
			AuxFunc::handleRecvError(Cluster::cluster->getEvent(), eventList, bufRead, Cluster::cluster->getEpollFd());
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
						AuxFunc::handleRecvError(Cluster::cluster->getEvent(), eventList, bufRead, Cluster::cluster->getEpollFd());
						return;
					}
					buffer[bufRead] = '\0';
					std::string body(buffer, bufRead);
					bodySize = req->setReqBody(body);
				}
			}
		}
		req->handleReq(this->_configurations);
		this->_clientPool[eventList.data.fd] = req;
		if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_MOD, EPOLLOUT, eventList.data.fd, Cluster::cluster->getEvent()))
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

	void Server::writeOperations(const struct epoll_event &eventList)
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
			director.BuildOkResponse(req->getResourceSize());
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
		if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_DEL, EPOLLOUT, eventList.data.fd, Cluster::cluster->getEvent()))
			throw Server::ServerException();
		delete this->_clientPool[eventList.data.fd];
		this->_clientPool.erase(eventList.data.fd);
		close(eventList.data.fd);
	}

	const char *Server::ServerException::what(void) const throw()
	{
		return ("The server found a problem and must stop now");
	}

	Server::~Server() {}
}
