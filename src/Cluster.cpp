/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/13 10:53:15 by juestrel         ###   ########.fr       */
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
		this->bindSocket();
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

	void Cluster::bindSocket(void)
	{
		int optVal = 1;
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