/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/08 19:09:56 by juestrel         ###   ########.fr       */
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

	int AuxFunc::itoa(std::string str)
	{
		std::stringstream ss(str);
		if (str.length() > 10)
			throw std::exception();
		for (size_t i = 0; i < str.length(); ++i)
		{
			if (!isdigit(str[i]))
				throw std::exception();
		}
		int res;
		ss >> res;
		return (res);
	}

	AuxFunc::~AuxFunc(void) {}
}
