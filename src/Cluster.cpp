/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/12 18:54:43 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cluster.hpp"

namespace Webserv
{
	Cluster *Cluster::cluster = NULL;

	Cluster::Cluster(void): _configurations(std::vector<ConfigServer>())
	{
		this->_epollFd = -1;
	}

	Cluster::Cluster(const std::vector<ConfigServer> &configurations): _configurations(std::vector<ConfigServer>(configurations)) 
	{
		this->_epollFd = -1;
	}

	Cluster::Cluster(const Cluster &toCopy): _configurations(toCopy.getConfigurations())
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
		this->obtainAddrInfo();
	}

	void Cluster::obtainAddrInfo(void)
	{
		struct addrinfo hints;
		std::map<std::string, struct addrinfo *> serverList;
	
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		for (configurationIter it = this->_configurations.begin(); it != this->_configurations.end(); it++)
		{
			struct addrinfo *address = new struct addrinfo;
			if (int errorCode = getaddrinfo(NULL, AuxFunc::ft_itoa(it->getPort()).c_str(), &hints, &address) != 0)
			{
				delete (address);
				Webserv::Logger::errorLog(errorCode, gai_strerror, false);
				throw Cluster::ClusterException();
			}
		}
		
	}

	const std::vector<ConfigServer> &Cluster::getConfigurations(void) const
	{
		return (Cluster::cluster->_configurations);	
	}

	int Cluster::getEpollFd(void) const
	{
		return (Cluster::cluster->_epollFd);
	}
	
	const char *Cluster::ClusterException::what(void) const throw ()
	{
		return ("There was an error starting the virtual servers and the program must stop");
	}

	Cluster::~Cluster() {}
}