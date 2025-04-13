/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:11:46 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/13 10:19:22 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <bitset>
#include <sstream>
#include "Server.hpp"
#include "ConfigServer.hpp"

namespace Webserv
{
	class Cluster
	{
	public:
		enum SocketType
		{
			LISTEN_SOCKET,
			ACCEPT_SOCKET,
		};
		typedef struct SocketData
		{
			enum SocketType socketType;
			Server *server;
		} t_SocketData;

		static Cluster *cluster;

		static Cluster *getInstance(const std::vector<ConfigServer> &configurations);
		const std::vector<ConfigServer> &getConfigurations(void) const;
		int getEpollFd(void) const;
		void initVirtualServers(void);
		class ClusterException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
		~Cluster();

	private:
		typedef std::vector<ConfigServer>::const_iterator configurationIter;
		int _epollFd;
		std::map<int, SocketData> _sockets;
		const std::vector<ConfigServer> _configurations;

		Cluster(void);
		Cluster(const std::vector<ConfigServer> &configurations);
		Cluster(const Cluster &toCopy);
		void obtainAddrInfo(void);
		void addressKey(struct addrinfo *address, std::map<std::string, struct addrinfo *> &serverList);
		void bindSocket(void);
		Cluster &operator=(const Cluster &toCopy);
	};
}

#endif