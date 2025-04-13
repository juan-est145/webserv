/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/13 11:42:56 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cluster.hpp"

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

	void Cluster::initVirtualServers(void)
	{
		std::map<std::string, t_AddressData> serverList = this->obtainAddrInfo();
		this->bindSocket(serverList);
		this->listenConnection();
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

			if (listenFd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol) < 0)
			{
				freeaddrinfo(addrinfo);
				Webserv::Logger::errorLog(errno, strerror, false);
				throw Cluster::ClusterException();
			}
			socketData.socketType = LISTEN_SOCKET;
			// TO DO: Change the type of constructor
			socketData.server = new Server();
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
		
	}

	const std::vector<ConfigServer> &Cluster::getConfigurations(void) const
	{
		return (Cluster::cluster->_configurations);
	}

	int Cluster::getEpollFd(void) const
	{
		return (Cluster::cluster->_epollFd);
	}

	const char *Cluster::ClusterException::what(void) const throw()
	{
		return ("There was an error starting the virtual servers and the program must stop");
	}

	Cluster::~Cluster() {}
}