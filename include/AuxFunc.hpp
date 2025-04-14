/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:20 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/14 13:20:57 by juestrel         ###   ########.fr       */
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
#include "Logger.hpp"
#include "Server.hpp"

namespace Webserv
{
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
	};

}

#endif