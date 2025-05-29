/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:20:31 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 11:29:50 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVER_HPP
#define ISERVER_HPP

#include "ARequest.hpp"

namespace Webserv
{
	class IServer
	{
	public:
		virtual void processClientConn(int eventListIndex) = 0;
		virtual void addClientPool(int fd, ARequest *req) = 0;
		virtual ~IServer();

		class ServerException : public std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
	};

}

#endif