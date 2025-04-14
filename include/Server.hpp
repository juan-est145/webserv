/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:05 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/14 13:06:52 by juestrel         ###   ########.fr       */
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
#include <sys/epoll.h>
#include <sys/socket.h>
#include <exception>
#include <cstdio>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include "Logger.hpp"
#include "AuxFunc.hpp"
#include "Request.hpp"
#include "Director.hpp"
#include "ConcreteBuilder.hpp"
#include "HttpResponse.hpp"
#include "ConfigServer.hpp"
#include "Cluster.hpp"

namespace Webserv
{
	class Server
	{
	private:
		int _listenFd;
		const std::vector<ConfigServer> _configurations;
		std::map<int, Request *> _clientPool;
		// void listenConnection(void);
		void addConnectionToQueue(struct epoll_event &event) const;
		void readOperations(int socketFd);
		void readSocket(struct epoll_event &eventList, struct epoll_event &eventConf);
		void readFile(struct epoll_event &eventList, struct epoll_event &eventConf);
		void writeOperations(struct epoll_event &eventList, struct epoll_event &eventConf);

	public:
		Server(void);
		Server(const std::vector<ConfigServer> &configurations);
		Server(const Server &copy);
		Server &operator=(const Server &assign);
		void processClientConn(int socketFd);
		//void initServer(void);

		class ServerException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
		~Server();
	};
}

#endif