/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICluster.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:29:36 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 11:21:41 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICLUSTER_HPP
#define ICLUSTER_HPP

#include "IServer.hpp"

namespace Webserv
{

	class ICluster
	{
	public:
		enum SocketType
		{
			LISTEN_SOCKET,
			ACCEPT_SOCKET,
			PIPE_SOCKET,
		};
		typedef struct SocketData
		{
			enum SocketType socketType;
			IServer *server;
		} t_SocketData;

		static ICluster *cluster;

		virtual void initVirtualServers(void) = 0;
		virtual void deleteAcceptSocket(int fd) = 0;
		virtual void addPipeSocket(int fd, IServer *server) = 0;
		virtual IServer *findServer(int fd) = 0;
		virtual struct epoll_event &getEvent(void) const = 0;
		virtual int getEpollFd(void) const = 0;
		virtual const struct epoll_event *getEventList(void) const = 0;
		virtual const std::map<int, SocketData> &getSockets(void) const = 0;
		virtual ~ICluster();
	};
}

#endif