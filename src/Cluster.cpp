/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/12 16:56:24 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cluster.hpp"

namespace Webserv
{
	Cluster *Cluster::cluster = NULL;

	Cluster::Cluster(void) 
	{
		this->_epollFd = -1;
	}

	Cluster::Cluster(const Cluster &toCopy)
	{
		(void)toCopy;
	}

	Cluster &Cluster::operator=(const Cluster &toCopy)
	{
		(void)toCopy;
		return (*this);
	}

	Cluster::~Cluster() {}
}