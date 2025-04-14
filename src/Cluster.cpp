/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/14 14:20:17 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cluster.hpp"

extern bool g_stop;

namespace Webserv
{
	Cluster *Cluster::cluster = NULL;

	Cluster::Cluster(void) : _configurations(std::vector<ConfigServer>())
	{
		this->_epollFd = -1;
	}

	Cluster::Cluster(const std::vector<ConfigServer> &configurations) : _configurations(std::vector<ConfigServer>(configurations))
	{
		this->_epollFd = -1;
	}

	Cluster::Cluster(const Cluster &toCopy) : _configurations(toCopy.getConfigurations())
	{
		(void)toCopy;
	}

	Cluster &Cluster::operator=(const Cluster &toCopy)
	{
		(void)toCopy;
		return (*this);
	}

	Cluster *Cluster::getInstance(const std::vector<ConfigServer> &configurations)
	{
		if (cluster == NULL)
			cluster = new Cluster(configurations);
		return (cluster);
	}

	Cluster *Cluster::getInstance(void)
	{
		if (cluster == NULL)
			throw Cluster::ClusterException();
		return (cluster);
	}

	void Cluster::initVirtualServers(void)
	{
		std::map<std::string, t_AddressData> serverList = this->obtainAddrInfo();
		this->bindSocket(serverList);
		this->listenConnection();
		// TO DO: Implement cleanup function for each LISTEN_SOCKET and it's server class
	}

	std::map<std::string, Cluster::t_AddressData> Cluster::obtainAddrInfo(void)
	{
		struct addrinfo hints;
		std::map<std::string, t_AddressData> serverList;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		for (configurationIter it = this->_configurations.begin(); it != this->_configurations.end(); it++)
		{
			t_AddressData address;
			address._configurations.push_back(*it);
			if (int errorCode = getaddrinfo(NULL, AuxFunc::ft_itoa(it->getPort()).c_str(), &hints, &address.addrinfo) != 0)
			{
				if (address.addrinfo != NULL)
					freeaddrinfo(address.addrinfo);
				Webserv::Logger::errorLog(errorCode, gai_strerror, false);
				throw Cluster::ClusterException();
			}
			this->addressKey(address, serverList);
		}
		return (serverList);
	}

	void Cluster::addressKey(t_AddressData address, std::map<std::string, t_AddressData> &serverList)
	{
		std::string key = "";
		if (address.addrinfo->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(address.addrinfo->ai_addr);
			key = std::bitset<32>(ipv4->sin_addr.s_addr).to_string() + std::bitset<16>(ipv4->sin_port).to_string();
		}
		else
		{
			struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6 *>(address.addrinfo->ai_addr);
			std::ostringstream oss;
			for (int i = 0; i < 4; i++)
			{
				std::bitset<32> bits(ipv6->sin6_addr.__in6_u.__u6_addr32[i]);
				oss << bits.to_string();
			}
			key = oss.str() + std::bitset<16>(ipv6->sin6_port).to_string();
		}
		if (serverList.find(key) != serverList.end())
		{
			serverList[key]._configurations.push_back(address._configurations[0]);
			freeaddrinfo(address.addrinfo);
			return;
		}
		serverList[key] = address;
	}

	void Cluster::bindSocket(std::map<std::string, t_AddressData> &serverList)
	{
		int optVal = 1;
		for (addressDataIter it = serverList.begin(); it != serverList.end(); it++)
		{
			// TO DO: Later on, we need to store the socket somewhere and associate it to
			// server instance
			struct addrinfo *addrinfo = it->second.addrinfo;
			t_SocketData socketData;
			int listenFd;

			if ((listenFd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) < 0)
			{
				freeaddrinfo(addrinfo);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Cluster::ClusterException();
			}
			socketData.socketType = LISTEN_SOCKET;
			socketData.server = new Server(it->second._configurations);
			this->_sockets[listenFd] = socketData;
			if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optVal, sizeof(optVal) == -1))
			{
				freeaddrinfo(addrinfo);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Cluster::ClusterException();
			}
			if (bind(listenFd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
			{
				freeaddrinfo(addrinfo);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Cluster::ClusterException();
			}
			freeaddrinfo(it->second.addrinfo);
			addrinfo = NULL;
			if (listen(listenFd, 20) < 0)
			{
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Server::ServerException();
			}
		}
	}

	void Cluster::listenConnection(void)
	{
		this->_epollFd = epoll_create(NUMBER_EPOLL);
		if (this->_epollFd == -1)
		{
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Server::ServerException();
		}
		for (socketIter it = this->_sockets.begin(); it != this->_sockets.end(); it++)
		{
			if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_ADD, EPOLLIN, it->first, this->_event))
				throw Cluster::ClusterException();
		}
		while (!g_stop)
		{
			int eventCount = epoll_wait(this->_epollFd, this->_eventList, sizeof(this->_eventList), E_WAIT_TIMEOUT);
			if (eventCount == -1)
			{
				if (g_stop)
					break;
				std::cout << "Failed here" << std::endl;
				close(this->_epollFd);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Cluster::ClusterException();
			}
			for (int i = 0; i < eventCount; i++)
			{
				int socketFd = this->_eventList[i].data.fd;
				if (this->_sockets[socketFd].socketType == LISTEN_SOCKET)
					this->addConnectionToQueue(socketFd);
				else
					this->_sockets[socketFd].server->processClientConn(socketFd, i);
			}
		}
	}

	void Cluster::addConnectionToQueue(int listenSocket)
	{
		struct sockaddr_storage clientAddr;
		t_SocketData socketDesc;
		socklen_t addrSize = sizeof(clientAddr);
		std::cout << "We have a connection" << std::endl;
		int newSocket = accept(listenSocket, (sockaddr *)&clientAddr, &addrSize);
		if (newSocket < 0)
		{
			close(this->_epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Cluster::ClusterException();
		}
		if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_ADD, EPOLLIN, newSocket, this->_event))
			throw Cluster::ClusterException();
		socketDesc.socketType = ACCEPT_SOCKET;
		socketDesc.server = this->_sockets[listenSocket].server;
		this->_sockets[newSocket] = socketDesc;
	}

	const std::vector<ConfigServer> &Cluster::getConfigurations(void) const
	{
		return (Cluster::cluster->_configurations);
	}

	int Cluster::getEpollFd(void) const
	{
		return (Cluster::cluster->_epollFd);
	}

	const std::map<int, Cluster::SocketData> &Cluster::getSockets(void) const
	{
		return (Cluster::cluster->_sockets);
	}

	const struct epoll_event *Cluster::getEventList(void) const
	{
		return (Cluster::cluster->_eventList);
	}

	struct epoll_event &Cluster::getEvent(void) const
	{
		return (Cluster::cluster->_event);
	}

	const char *Cluster::ClusterException::what(void) const throw()
	{
		return ("There was an error starting the virtual servers and the program must stop");
	}

	Cluster::~Cluster() {}
}