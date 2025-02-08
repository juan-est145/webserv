/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:30:15 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/08 11:18:01 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HtmlFile.hpp"
#include "../include/Server.hpp"
namespace Webserv
{
    HtmlFile::HtmlFile() 
    {
        this->_socketFd = -1;
        this->_size = -1;
        this->_content = -1;
    }

    HtmlFile::HtmlFile(const HtmlFile &copy)
    {
        *this = copy;
    }

    HtmlFile &HtmlFile::operator=(const HtmlFile &other)
    {
        if (this != &other)
        {
            this->_socketFd = other._socketFd;
            this->_size = other._size;
            this->_content = other._content;
        }
        return *this;
    }

    int HtmlFile::obtainFileFd(std::string &filePath, int epollFd, struct epoll_event &eventConf)
    {
        int pipeFd[2];
        if (!this->fileExits(filePath))
        {
            // TO DO: Later mark this as a 404 response
            std::cerr << "Failed to open file" << std::endl;
            exit(EXIT_FAILURE);
        }
        this->_socketFd = eventConf.data.fd;

        // TO DO: Check return value of pipe, close, dup2 and fork
        pipe(pipeFd);
        pid_t pid = fork();
        if (pid == 0)
            this->execPy(pipeFd);
        close(pipeFd[PIPE_WRITE]);
        eventConf.events = EPOLLIN;
        eventConf.data.fd = pipeFd[PIPE_READ];
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, pipeFd[PIPE_READ], &eventConf) == -1)
        {
            close(epollFd);
            Webserv::Logger::errorLog(errno, strerror, false);
            throw Server::ServerException();
        }
        return (pipeFd[PIPE_READ]);
    }

    bool HtmlFile::fileExits(const std::string &path)
    {
        // TO DO: Check if I can open a file or not
        struct stat fileStat;

        if (stat(path.c_str(), &fileStat) == -1)
            return (false);
        if (!S_ISREG(fileStat.st_mode))
            return (false);
        this->_size = fileStat.st_size;
        return (true); 
    }

    void HtmlFile::execPy(int pipeFd[2])
    {
        char *args[] = {
            (char *)"python3",
            (char *)"./cgi/readHtml.py",
            NULL,
        };
        close(pipeFd[PIPE_READ]);
        dup2(pipeFd[PIPE_WRITE], STDOUT_FILENO);
        close(pipeFd[PIPE_WRITE]);
        execve((const char *)"/usr/bin/python3", args, NULL);
        exit(0);
    }

    long HtmlFile::getSize(void) const
    {
        return (this->_size);
    }

    const std::string &HtmlFile::getContent(void) const
    {
        return (this->_content);
    }

    void HtmlFile::setContent(const char *buffer)
    {
        // TO DO: Check for null values in buffer
        this->_content = buffer;
    }

    int HtmlFile::getSocketFd(void) const
    {
        return (this->_socketFd);
    }

    HtmlFile::~HtmlFile() {}

}
