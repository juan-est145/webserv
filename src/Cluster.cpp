/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/12 17:47:37 by juestrel         ###   ########.fr       */
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
	}

	const std::vector<ConfigServer> &Cluster::getConfigurations(void) const
	{
		return (Cluster::cluster->_configurations);	
	}

	int Cluster::getEpollFd(void) const
	{
		return (Cluster::cluster->_epollFd);
	}

	Cluster::~Cluster() {}
}