/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angcampo <angcampo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/03/19 18:16:42 by angcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/AuxFunc.hpp"

namespace Webserv
{
	AuxFunc::AuxFunc(void) {}

	AuxFunc::AuxFunc(AuxFunc &toCopy)
	{
		*this = toCopy;
	}

	AuxFunc &AuxFunc::operator=(const AuxFunc &assign)
	{
		(void)assign;
		return (*this);
	}

	bool AuxFunc::handle_ctl(int epollFd, int op, int event, int watchFd, struct epoll_event &eventConf)
	{
		eventConf.events = event;
		eventConf.data.fd = watchFd;
		if (epoll_ctl(epollFd, op, watchFd, &eventConf) == -1)
		{
			close(epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			return (false);
		}
		return (true);
	}

	AuxFunc::~AuxFunc(void) {}
}
