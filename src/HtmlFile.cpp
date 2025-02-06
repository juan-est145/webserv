/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:30:15 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/06 18:14:35 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HtmlFile.hpp"
#include "../include/Server.hpp"
namespace Webserv
{
    HtmlFile::HtmlFile()
    {
        this->content = "";
        this->size = 0;
    }
    HtmlFile::HtmlFile(const HtmlFile &copy)
    {
        *this = copy;
    }
    HtmlFile::~HtmlFile()
    {
    }
    HtmlFile &HtmlFile::operator=(const HtmlFile &other)
    {
        this->content = other.getContent();
        this->size = other.getSize();
        return *this;
    }

    void HtmlFile::readFile(std::string &filePath,int epollFd, struct epoll_event &eventList)
    {
        // Open the file
        int fd = open(filePath.c_str(), O_RDONLY);
        if (fd == -1)
        {
            std::cerr << "Failed to open " << filePath << std::endl;
            exit(EXIT_FAILURE);
        }
        // Get the information of the file(the size)
        struct stat fileStat;
        if (stat(filePath.c_str(), &fileStat) == -1)
        {
            std::cerr << "Failed to get file size" << std::endl;
            close(fd);
            exit(EXIT_FAILURE);
        }
        // Save the size
        this->size = fileStat.st_size;
        // Book the memory and +1 for the null
        char *buffer = (char *)malloc(this->size + 1);
        if (buffer == NULL)
        {
            std::cerr << "Failed to allocate memory" << std::endl;
            close(fd);
            exit(EXIT_FAILURE);
        }
        eventList.events = EPOLLIN;
        eventList.data.fd = fd;
        /********************************************************* */
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd , &eventList) == -1)
        {
            close(epollFd);
            Webserv::Logger::errorLog(errno, strerror, false);
            throw Server::ServerException();
        }          

        
        /********************************************************* */
        // save the content in buffer
        if (read(fd, buffer, this->size) != this->size)
        {
            std::cerr << "Failed to read file" << std::endl;
            free(buffer);
            close(fd);
            exit(EXIT_FAILURE);
        }
        buffer[this->size] = '\0';
        // Save the content
        this->content = std::string(buffer);
        free(buffer);
        close(fd);
    }
    // GETTERS
    std::string HtmlFile::getContent() const
    {
        return this->content;
    }

    long HtmlFile::getSize() const
    {
        return this->size;
    }
}
