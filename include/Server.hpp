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
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <netdb.h>
#include <sys/epoll.h>


#include <cstdio>
#include <cstdlib>

namespace Webserv
{
	class Server
	{
	private:
		int _listenFd;
		struct addrinfo *_address;
		const std::string _host;
		const std::string _port;
		void listenConnection(void);
		void addConnectionToQueue(int epollFd, struct epoll_event &event);
		void processClientConn(int epollFd, struct epoll_event &eventList, struct epoll_event &eventConf);

	public:
		Server(void);
		Server(const std::string &host, const std::string &port);
		Server(const Server &copy);
		Server &operator=(const Server &assign);
		void initServer(void);
		~Server();
	};
} // namespace Webserv

#endif