/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:20 by juestrel          #+#    #+#             */
/*   Updated: 2025/02/21 15:17:49 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUXFUNC_HPP
#define AUXFUNC_HPP

#include <sys/epoll.h>
#include <unistd.h>
#include <cerrno>
#include "Logger.hpp"

namespace Webserv
{
	class AuxFunc
	{
	private:
		AuxFunc(void);
		AuxFunc(AuxFunc &toCopy);
		AuxFunc &operator=(const AuxFunc &assign);
		~AuxFunc();

		bool isxdigit(char c);
	public:
		static bool handle_ctl(int epollFd, int op, int event, int watchFd, struct epoll_event &eventConf);
		std::string urldecode2(const char *url);
	};

}

#endif