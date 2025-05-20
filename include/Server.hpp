/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:05 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/20 08:45:54 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <exception>
#include <cstdio>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include "Logger.hpp"
#include "Request.hpp"
#include "Director.hpp"
#include "ConcreteBuilder.hpp"
#include "HttpResponse.hpp"
#include "Cluster.hpp"
#include "ConfigServer.hpp"
#include "Cookie.hpp"

namespace Webserv
{
	class ConfigServer;
	class Request;
	class AuxFunc;

	class Server
	{
	private:
		int _listenFd;
		const std::vector<ConfigServer> _configurations;
		std::map<int, Request *> _clientPool;
		std::map<std::string, struct CookieData> _sessions;

		void addConnectionToQueue(struct epoll_event &event) const;
		void readSocket(const struct epoll_event &eventList);
		void readFile(struct epoll_event &eventList, struct epoll_event &eventConf);
		void writeOperations(const struct epoll_event &eventList);
		std::size_t findExpectedSize(Request *req) const;
		void deleteExpiredSessions(void);

	public:
		Server(void);
		Server(const std::vector<ConfigServer> &configurations, int listenFd);
		Server(const Server &copy);
		Server &operator=(const Server &assign);
		void processClientConn(int eventListIndex);

		class ServerException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
		~Server();
	};
}

#endif