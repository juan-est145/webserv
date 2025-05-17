/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/17 11:25:19 by juestrel         ###   ########.fr       */
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

	void Server::processClientConn(int eventListIndex)
	{
		const struct epoll_event *eventList = Cluster::cluster->getEventList();
		if (eventList[eventListIndex].events & EPOLLIN)
			this->readSocket(eventList[eventListIndex]);
		else
			this->writeOperations(eventList[eventListIndex]);
	}

	void Server::readSocket(const struct epoll_event &eventList)
	{
		// TO DO: Possibly will need to add a timer to listening to timeout connection
		char buffer[1024];
		std::size_t bodySize;
		std::size_t expectedSize;
		std::cout << "Reading from client " << eventList.data.fd << std::endl;
		memset(buffer, '\0', sizeof(buffer));
		ssize_t bufRead = recv(eventList.data.fd, buffer, sizeof(buffer) - 1, 0);
		if (bufRead <= 0)
		{
			AuxFunc::handleRecvError(Cluster::cluster->getEvent(), eventList, bufRead, Cluster::cluster->getEpollFd());
			Cluster::getInstance()->deleteAcceptSocket(eventList.data.fd);
			return;
		}
		buffer[bufRead] = '\0';
		Request *req = this->_clientPool.find(eventList.data.fd) == this->_clientPool.end() ? new Request(eventList.data.fd) : this->_clientPool[eventList.data.fd];
		req->readReq(buffer, bufRead);
		bodySize = req->getReqBody().size();
		// TO DO: findExpectedSize must be able to work with chunked requests, where there is no Content-Length header
		expectedSize = this->findExpectedSize(req);
		if (bufRead == sizeof(buffer) - 1 || bodySize < expectedSize)
		{
			while (bodySize < expectedSize)
			{
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
		this->_clientPool[eventList.data.fd] = req;
		req->handleReq(this->_configurations);
		if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_MOD, EPOLLOUT, eventList.data.fd, Cluster::cluster->getEvent()))
			throw Webserv::Server::ServerException();
	}

	void Server::writeOperations(const struct epoll_event &eventList)
	{
		std::cout << "Time to write to the client " << eventList.data.fd << std::endl;
		const Request *req = this->_clientPool[eventList.data.fd];
		// *----CAMBIO----//
		HttpResponse Hresp;
		std::string response;
		Director director;
		ConcreteBuilder *builder = new ConcreteBuilder(&Hresp); 
		director.SetBuilder(builder);

		director.BuildDefaultResponse(req);
		response = Hresp.returnResponse(req);
	
		if (send(eventList.data.fd, response.c_str(), response.size(), 0) == -1)
			Webserv::Logger::errorLog(errno, strerror, false);
		delete builder;
		// *----------//

		// The lines below are comented because we now don't close the connection on server. Later on we might need to change this
		// if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_DEL, EPOLLOUT, eventList.data.fd, Cluster::cluster->getEvent()))
		// 	throw Server::ServerException();
		if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_MOD, EPOLLIN, eventList.data.fd, Cluster::cluster->getEvent()))
			throw Webserv::Server::ServerException();
		delete this->_clientPool[eventList.data.fd];
		this->_clientPool.erase(eventList.data.fd);
	}

	std::size_t Server::findExpectedSize(Request *req) const
	{
		const Request::T_reqHeadIter conLenKey = req->getReqHeader().find("Content-Length");
		std::size_t expectedSize = conLenKey == req->getReqHeader().end() ? 0 : std::atol(conLenKey->second.c_str());
		return (expectedSize);
	}

	const char *Server::ServerException::what(void) const throw()
	{
		return ("The server found a problem and must stop now");
	}

	Server::~Server() {}
}
