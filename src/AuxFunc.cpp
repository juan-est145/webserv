/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/02/28 13:22:15 by juestrel         ###   ########.fr       */
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

	void AuxFunc::handleRecvError(struct epoll_event &eventConf, struct epoll_event &eventList, ssize_t bufRead, int epollFd)
	{
		eventConf.events = EPOLLIN;
		eventConf.data.fd = eventList.data.fd;
		if (bufRead == -1 || epoll_ctl(epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
		{
			close(epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw Webserv::Server::ServerException();
		}
		close(eventList.data.fd);
	}

	AuxFunc::~AuxFunc(void) {}
}
