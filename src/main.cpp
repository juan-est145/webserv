#include "../include/main.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <cstring>

int main(void)
{
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0)
        return (-1);

        
    struct sockaddr_in address;
    int addrLen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(80);
    
    
    int bindResult = bind(serverFd, (sockaddr *)&address, addrLen);
    if (bindResult < 0)
        return (-1);


    if (listen(serverFd, 20) < 0)
        return (-1);
    while (1)
    {
        int newSocket = accept(serverFd, (sockaddr *)&address, (socklen_t *)&addrLen);
        if (newSocket < 0)
            return (-1);
        std::string response = "Hola caracola";
        write(newSocket, response.c_str(), response.size());
        close(newSocket);
    }
    close(serverFd);
    return (0);
}