/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/01 16:24:10 by mfuente-         ###   ########.fr       */
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

	bool isxdigit(char c) {
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))  {
			return true;
		}
		return false;
	}
	
	std::string urldecode(const char *url)
	{
		char a, b;
		std::string dst;
		while (*url) {
				if ((*url == '%') &&
					((a = url[1]) && (b = url[2])) &&
					(isxdigit(a) && isxdigit(b))) {
						//Convert the firts character in hexadecimal
						if (a >= 'a')
							a -= 32;  
						//Transform in decimal
						if (a >= 'A')
							a -= ('A' - 10);
						else
							a -= '0';
						//The same with the second character
						if (b >= 'a')
							b -= 'a' - 'A';
						if (b >= 'A')
							b -= ('A' - 10);
						else
							b -= '0';
						//Convert the number in hexadecimal
						dst += 16 * a + b;
						url += 3;
				}
				else if (*url == '+') {
					dst += ' ';
					url++;
				} else {
					dst += *url++;
				}
			}
			return dst;
	}
}
