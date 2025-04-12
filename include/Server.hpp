/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:05 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/12 16:24:09 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#ifndef NUMBER_EPOLL
#define NUMBER_EPOLL 1
#endif
#ifndef E_WAIT_TIMEOUT
#define E_WAIT_TIMEOUT 100
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <netdb.h>
#include <sys/epoll.h>
#include <exception>
#include <cstdio>
#include <sstream>
#include <map>
#include <algorithm>
#include "Logger.hpp"
#include "AuxFunc.hpp"
#include "Request.hpp"
#include "Director.hpp"
#include "ConcreteBuilder.hpp"
#include "HttpResponse.hpp"

namespace Webserv
{
	class Server
	{
	private:
		int _listenFd;
		int _epollFd;
		std::map<int, Request *> _clientPool;
		struct addrinfo *_address;
		const std::string _host;
		const std::string _port;
		void listenConnection(void);
		void addConnectionToQueue(struct epoll_event &event) const;
		void processClientConn(struct epoll_event &eventList, struct epoll_event &eventConf);
		void readOperations(struct epoll_event &eventList, struct epoll_event &eventConf);
		void readSocket(struct epoll_event &eventList, struct epoll_event &eventConf);
		void readFile(struct epoll_event &eventList, struct epoll_event &eventConf);
		void writeOperations(struct epoll_event &eventList, struct epoll_event &eventConf);

	public:
		Server(void);
		Server(const std::string &host, const std::string &port);
		Server(const Server &copy);
		Server &operator=(const Server &assign);
		void initServer(void);
		class ServerException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
		~Server();
	};
}

#endif