/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:20 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/28 23:50:39 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUXFUNC_HPP
#define AUXFUNC_HPP

#ifndef MAX_CONTENT_LENGTH
#define MAX_CONTENT_LENGTH 35000000
#endif

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cerrno>
#include <sstream>
#include <ctime>
#include <cctype>
#include "Logger.hpp"
#include "Location.hpp"

namespace Webserv
{
	class Server;
	class Location;
	
	class AuxFunc
	{
	private:
		AuxFunc(void);
		AuxFunc(AuxFunc &toCopy);
		AuxFunc &operator=(const AuxFunc &assign);
		~AuxFunc();

	public:
		static bool handle_ctl(int epollFd, int op, int event, int watchFd, struct epoll_event &eventConf);
		static void handleRecvError(struct epoll_event &eventConf, const struct epoll_event &eventList, ssize_t bufRead, int epollFd);
		static std::string urldecode(const char *url);
		static bool isDigit(char c);
		static std::string ft_itoa(int number);
		static std::string ft_itoa(unsigned int number);
		static std::string mapPathToResource(const Location &locationFile, std::string reqPath);
		static std::string getHumanTime(time_t rawTime);
		static std::string getGmtTime(void);
		static std::string getGmtTime(std::time_t time);
		static unsigned long hexToDecimal(std::string hexVal);
	};

}

#endif