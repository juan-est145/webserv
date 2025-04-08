/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuxFunc.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:10:20 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/08 17:10:18 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUXFUNC_HPP
#define AUXFUNC_HPP

#ifndef MAX_CONTENT_LENGTH
#define MAX_CONTENT_LENGTH 35000000
#endif

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

	public:
		static bool handle_ctl(int epollFd, int op, int event, int watchFd, struct epoll_event &eventConf);
	};

}

#endif