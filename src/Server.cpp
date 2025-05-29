/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 12:51:06 by juestrel         ###   ########.fr       */
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
		this->deleteExpiredSessions();
		const struct epoll_event *eventList = ICluster::cluster->getEventList();
		const std::map<int, ICluster::SocketData> socketData = ICluster::cluster->getSockets();
		std::map<int, ICluster::SocketData>::const_iterator socketInfo = socketData.find(eventList[eventListIndex].data.fd);
		
		if (eventList[eventListIndex].events & EPOLLIN && socketInfo->second.socketType == ICluster::ACCEPT_SOCKET)
			this->readSocket(eventList[eventListIndex]);
		else if (eventList[eventListIndex].events & EPOLLIN && socketInfo->second.socketType == ICluster::PIPE_SOCKET)
			this->readPipe(eventList[eventListIndex]);
		else
			this->writeOperations(eventList[eventListIndex]);
	}

	void Server::addClientPool(int fd, ARequest *req)
	{
		this->_clientPool[fd] = req;
	}

	void Server::readSocket(const struct epoll_event &eventList)
	{
		// TO DO: Possibly will need to add a timer to listening to timeout connection
		char buffer[1024];
		std::size_t bodySize;
		std::size_t expectedSize;
		memset(buffer, '\0', sizeof(buffer));
		ssize_t bufRead = recv(eventList.data.fd, buffer, sizeof(buffer) - 1, 0);
		if (bufRead <= 0)
		{
			AuxFunc::handleRecvError(ICluster::cluster->getEvent(), eventList, bufRead, ICluster::cluster->getEpollFd());
			ICluster::cluster->deleteAcceptSocket(eventList.data.fd);
			return;
		}
		buffer[bufRead] = '\0';
		Request *req = this->_clientPool.find(eventList.data.fd) == this->_clientPool.end() ? new Request(eventList.data.fd) : dynamic_cast<Request *>(this->_clientPool[eventList.data.fd]);
		try
		{
			req->readReq(buffer, bufRead);
		}
		catch(const Webserv::Request::InvalidReqException& e)
		{
			req->send400ErrorCode(this->_configurations);
			this->_clientPool[eventList.data.fd] = req;
			if (!AuxFunc::handle_ctl(ICluster::cluster->getEpollFd(), EPOLL_CTL_MOD, EPOLLOUT, eventList.data.fd, ICluster::cluster->getEvent()))
				throw Webserv::Server::ServerException();
			return;
		}
		bodySize = req->getReqBody().size();
		expectedSize = this->findExpectedSize(req);
		if (bufRead == sizeof(buffer) - 1 || bodySize < expectedSize)
		{
			while (bodySize < expectedSize)
			{
				memset(buffer, '\0', sizeof(buffer));
				bufRead = recv(eventList.data.fd, buffer, sizeof(buffer) - 1, 0);
				if (bufRead <= 0)
				{
					AuxFunc::handleRecvError(ICluster::cluster->getEvent(), eventList, bufRead, ICluster::cluster->getEpollFd());
					return;
				}
				buffer[bufRead] = '\0';
				std::string body(buffer, bufRead);
				bodySize = req->setReqBody(body);
			}
		}
		this->_clientPool[eventList.data.fd] = req;
		if (!req->isReady())
			return;
		req->handleReq(this->_configurations, this->_sessions);
		this->_sessions[req->getCookie()._id] = req->getCookie();
		if (!AuxFunc::handle_ctl(ICluster::cluster->getEpollFd(), EPOLL_CTL_MOD, EPOLLOUT, eventList.data.fd, ICluster::cluster->getEvent()))
			throw Webserv::Server::ServerException();
	}

	void Server::readPipe(const struct epoll_event &eventList)
	{
		char buffer[1024];
		int bytesRead = 0;
		int status = 0;
		ARequest *cgiReq = this->_clientPool.find(eventList.data.fd)->second;
		int ogReqFd = dynamic_cast<const CgiReq *>(cgiReq)->getOgReqSock();
		Request *ogReq = dynamic_cast<Request *>(this->_clientPool.find(ogReqFd)->second);

		(void)status;
		memset(buffer, '\0', sizeof(buffer));
		while ((bytesRead = read(eventList.data.fd, buffer, sizeof(buffer))) > 0)
		{
			ogReq->setResourceContent(std::string(buffer));
			memset(buffer, '\0', sizeof(buffer));
		}
		// TO DO: Think about failures in Cgi execution and send 500 errors
		//if (bytesRead == -1)
		// {
		// 	close(pipeFd[PIPE_READ]);
		// 	throw Webserv::Cgi::CgiErrorException();
		// }
		// if (close(eventList.data.fd) == -1)
		// 	throw Webserv::Cgi::CgiErrorException();
		// if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		// 	throw Webserv::Cgi::CgiErrorException();
		// if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_DEL, EPOLLIN, eventList.data.fd, Cluster::cluster->getEvent()))
		// 	throw Webserv::Server::ServerException();
		// if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_ADD, EPOLLIN, cgiReq->getSocketFd(), Cluster::cluster->getEvent()))
		// 	throw Webserv::Server::ServerException();
		close(eventList.data.fd);
		AuxFunc::handle_ctl(ICluster::cluster->getEpollFd(), EPOLL_CTL_DEL, EPOLLIN, eventList.data.fd, ICluster::cluster->getEvent());
		AuxFunc::handle_ctl(ICluster::cluster->getEpollFd(), EPOLL_CTL_ADD, EPOLLOUT, ogReq->getSocketFd(), ICluster::cluster->getEvent());
		this->_clientPool.erase(cgiReq->getSocketFd());
		delete cgiReq;
	}

	void Server::writeOperations(const struct epoll_event &eventList)
	{
		const Request *req = dynamic_cast<const Request *>(this->_clientPool.find(eventList.data.fd)->second);
		// *----CAMBIO----//
		HttpResponse Hresp;
		std::string response;
		Director director;
		ConcreteBuilder *builder = new ConcreteBuilder(&Hresp); 
		director.setBuilder(builder);

		director.buildDefaultResponse(req->getResCode(), req->getResHeaders());
		response = Hresp.returnResponse(req->getResourceContent());
	
		if (send(eventList.data.fd, response.c_str(), response.size(), 0) == -1)
			Webserv::Logger::errorLog(errno, strerror, false);
		delete builder;
		// *----------//

		// The lines below are comented because we now don't close the connection on server. Later on we might need to change this
		// if (!AuxFunc::handle_ctl(Cluster::cluster->getEpollFd(), EPOLL_CTL_DEL, EPOLLOUT, eventList.data.fd, Cluster::cluster->getEvent()))
		// 	throw Server::ServerException();
		if (!AuxFunc::handle_ctl(ICluster::cluster->getEpollFd(), EPOLL_CTL_MOD, EPOLLIN, eventList.data.fd, ICluster::cluster->getEvent()))
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

	void Server::deleteExpiredSessions(void)
	{
		std::map<std::string, struct CookieData>::iterator it;
		std::time_t currTime = std::time(NULL);
		std::stack<std::string> expiredSessId;
		
		for  (it = this->_sessions.begin(); it != this->_sessions.end(); it++)
		{
			if (currTime >= it->second._expirationDate)
				expiredSessId.push(it->first);
		}
		while (expiredSessId.size() != 0)
		{
			this->_sessions.erase(expiredSessId.top());
			expiredSessId.pop();
		}
	}

	// const char *Server::ServerException::what(void) const throw()
	// {
		
	// }

	Server::~Server() {}
}
