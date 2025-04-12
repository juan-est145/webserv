/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/12 17:10:47 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cluster.hpp"

namespace Webserv
{
	Cluster *Cluster::cluster = NULL;

	Cluster::Cluster(void): _configurations(std::vector<ConfigServer>())
	{
		this->_epollFd = -1;
		memset(&this->_address, 0, sizeof(this->_address));
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

	Cluster *Cluster::getInstance(void)
	{
		if (cluster == NULL)
			cluster = new Cluster();
		return (cluster);
	}

	const std::vector<ConfigServer> &Cluster::getConfigurations(void) const
	{
		return (this->_configurations);	
	}

	Cluster::~Cluster() 
	{
		delete (cluster);
	}
}