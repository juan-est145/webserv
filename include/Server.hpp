#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <netdb.h>

namespace Webserv
{
	class Server
	{
	private:
		int _listenFd;
		struct addrinfo* _address;
		const unsigned short int _port;
		const std::string _host;
		int _sizeAddress;
		void listenConnection(void);

	public:
		Server(void);
		Server(std::string &host ,const unsigned short int port);
		Server(const Server &copy);
		Server &operator=(const Server &assign);
		void initServer(void);
		~Server();
	};
} // namespace Webserv

#endif