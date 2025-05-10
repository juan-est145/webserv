/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:16 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/10 11:14:21 by juestrel         ###   ########.fr       */
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

	void AuxFunc::handleRecvError(struct epoll_event &eventConf, const struct epoll_event &eventList, ssize_t bufRead, int epollFd)
	{
		eventConf.events = EPOLLIN;
		eventConf.data.fd = eventList.data.fd;
		if (bufRead == -1 || epoll_ctl(epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
		{
			close(epollFd);
			Webserv::Logger::errorLog(errno, strerror, false);
			throw std::exception();
		}
		close(eventList.data.fd);
	}

	bool AuxFunc::isDigit(char c) {
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))  {
			return true;
		}
		return false;
	}
	
	std::string AuxFunc::urldecode(const char *url)
	{
		char a, b;
		std::string dst;
		while (*url) {
				if ((*url == '%') &&
					((a = url[1]) && (b = url[2])) &&
					(AuxFunc::isDigit(a) && AuxFunc::isDigit(b))) {
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

	std::string AuxFunc::ft_itoa(int number)
	{
		std::string result = "";

		if (number == INT32_MIN)
			result = "-2" + ft_itoa(147483648);
		else if (number < 0)
			result = "-" + ft_itoa(number * -1);
		else if (number > 9)
		{
			result += ft_itoa(number / 10);
			result += ft_itoa(number % 10);
		}
		else
			result = number + 48;
		return (result);
	}

	std::string AuxFunc::ft_itoa(unsigned int number)
	{
		std::string result = "";

		if (number > 9)
		{
			result += ft_itoa(number / 10);
			result += ft_itoa(number % 10);
		}
		else
			result = number + 48;
		return (result);
	}

	std::string AuxFunc::mapPathToResource(const Location &locationFile, std::string reqPath)
	{
		std::string path = locationFile.getPath().size() <= 1 ? "" : locationFile.getPath().substr(1);
		std::string resourcePath;
		unsigned int breakIndex = 0;

		for (; breakIndex < std::min(reqPath.size(), locationFile.getPath().size()); breakIndex++)
		{
			if (reqPath[breakIndex] != locationFile.getPath()[breakIndex])
				break;
		}
		reqPath.erase(0, breakIndex);
		resourcePath = AuxFunc::urldecode((locationFile.getRootLocation() + path + reqPath).c_str());
		return (resourcePath);
	}

	std::string AuxFunc::getHumanTime(time_t rawTime)
	{
		struct tm *timeInfo = std::localtime(&rawTime);
		char buffer[80];

		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
		return (std::string(buffer));
	}

	AuxFunc::~AuxFunc(void) {}
}
