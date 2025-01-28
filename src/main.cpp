#include "../include/main.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <cstring>

// TO DO: Check if port number is under 1024 and the bind process fails, send a message that says that with that port number, the server must run with root privileges.
// 65535 is the maximum port number

int main(void)
{
    std::string host = "127.0.0.1";
    std::string port = "3000";
    Webserv::Server *server = new Webserv::Server(host, port);    
    server->initServer();
    delete server;
    return (0);
}