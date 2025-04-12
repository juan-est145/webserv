/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:11:46 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/12 16:57:21 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <string>
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <sys/socket.h>
#include "Server.hpp"

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
		static Cluster *getInstance();
		~Cluster();

	private:
		int _epollFd;
		struct addrinfo *_address;
		std::map<int, SocketData> _sockets;
		Cluster(void);
		Cluster(const Cluster &toCopy);
		Cluster &operator=(const Cluster &toCopy);
	};
}

#endif