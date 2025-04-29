/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:11:46 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/29 11:19:23 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#ifndef NUMBER_EPOLL
#define NUMBER_EPOLL 1
#endif
#ifndef E_WAIT_TIMEOUT
#define E_WAIT_TIMEOUT 100
#endif

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <bitset>
#include <sstream>
#include "Server.hpp"
#include "ConfigServer.hpp"

namespace Webserv
{
	class Server;
	class ConfigServer;

	class Cluster
	{
	public:
		enum SocketType
		{
			LISTEN_SOCKET,
			ACCEPT_SOCKET,
		};
		typedef struct AddressData
		{
			struct addrinfo *addrinfo;
			std::vector<ConfigServer> _configurations;
		} t_AddressData;
		typedef struct SocketData
		{
			enum SocketType socketType;
			Server *server;
		} t_SocketData;

		static Cluster *cluster;

		static Cluster *getInstance(const std::vector<ConfigServer> &configurations);
		static Cluster *getInstance(void);
		const std::vector<ConfigServer> &getConfigurations(void) const;
		int getEpollFd(void) const;
		const std::map<int, SocketData> &getSockets(void) const;
		const struct epoll_event *getEventList(void) const;
		struct epoll_event &getEvent(void) const;

		void initVirtualServers(void);
		void deleteAcceptSocket(int fd);

		class ClusterException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
		~Cluster();

	private:
		typedef std::vector<ConfigServer>::const_iterator configurationIter;
		typedef std::map<std::string, t_AddressData>::iterator addressDataIter;
		typedef std::map<int, SocketData>::iterator socketIter;

		int _epollFd;
		std::map<int, SocketData> _sockets;
		const std::vector<ConfigServer> _configurations;
		struct epoll_event _eventList[50];
		struct epoll_event _event;

		Cluster(void);
		Cluster(const std::vector<ConfigServer> &configurations);
		Cluster(const Cluster &toCopy);
		Cluster &operator=(const Cluster &toCopy);
		std::map<std::string, t_AddressData> obtainAddrInfo(void);
		void addressKey(t_AddressData address, std::map<std::string, t_AddressData> &serverList);
		void bindSocket(std::map<std::string, t_AddressData> &serverList);
		void listenConnection(void);
		void addConnectionToQueue(int listenSocket);
	};
}

#endif