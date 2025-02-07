/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:30:15 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/07 18:19:33 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HtmlFile.hpp"
#include "../include/Server.hpp"
namespace Webserv
{
    HtmlFile::HtmlFile() {}

    HtmlFile::HtmlFile(const HtmlFile &copy)
    {
        *this = copy;
    }

    HtmlFile &HtmlFile::operator=(const HtmlFile &other)
    {
        if (this != &other)
        {
            this->_pipeFd[PIPE_READ] = other._pipeFd[PIPE_WRITE];
            this->_pipeFd[PIPE_WRITE] = other._pipeFd[PIPE_WRITE];
        }
        return *this;
    }

    int HtmlFile::readFile(std::string &filePath, int epollFd, struct epoll_event &eventList)
    {
        if (!this->fileExits(filePath))
        {
            // TO DO: Later mark this as a 404 response
            std::cerr << "Failed to open file" << std::endl;
            exit(EXIT_FAILURE);
        }

        // TO DO: Check return value of pipe, close, dup2 and fork
        pipe(this->_pipeFd);
        pid_t pid = fork();
        if (pid == 0)
            this->execPy();
        close(this->_pipeFd[PIPE_WRITE]);
        eventList.events = EPOLLIN;
        eventList.data.fd = this->_pipeFd[PIPE_READ];
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, this->_pipeFd[PIPE_READ], &eventList) == -1)
        {
            close(epollFd);
            Webserv::Logger::errorLog(errno, strerror, false);
            throw Server::ServerException();
        }
        return (this->_pipeFd[PIPE_READ]);
    }

    bool HtmlFile::fileExits(const std::string &path) const
    {
        // TO DO: Check if I can open a file or not
        struct stat fileStat;

        if (stat(path.c_str(), &fileStat) == -1)
            return (false);
        if (!S_ISREG(fileStat.st_mode))
            return (false);
        return (true);
    }

    void HtmlFile::execPy()
    {
        char *args[] = {
            (char *)"python3",
            (char *)"pyTest.py",
            NULL,
        };
        close(this->_pipeFd[PIPE_READ]);
        dup2(this->_pipeFd[PIPE_WRITE], STDOUT_FILENO);
        close(this->_pipeFd[PIPE_WRITE]);
        execve((const char *)"/usr/bin/python3", args, NULL);
        exit(0);
    }

    HtmlFile::~HtmlFile() {}
}
