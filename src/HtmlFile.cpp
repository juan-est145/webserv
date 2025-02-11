/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:30:15 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/11 18:11:22 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HtmlFile.hpp"

namespace Webserv
{
    HtmlFile::HtmlFile(): rq(Request())
    {
        
        this->_socketFd = -1;
        this->_size = -1;
        this->_content = -1;
    }

    HtmlFile::HtmlFile(const HtmlFile &copy)
    {
        *this = copy;
    }

    HtmlFile::HtmlFile(const Request &rq): rq(rq)
    {
        this->_socketFd = -1;
        this->_size = -1;
        this->_content = -1;
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
    std::string obtainHtmlPath(const std::string &url)
    {
        if (url == "/")
            return ("./html/index.html");
        if (url == "/upload")
            return ("./html/upload.html");
        return ("./html/error404.html");
    }

    int HtmlFile::obtainFileFd(int epollFd, struct epoll_event &eventList, struct epoll_event &eventConf)
    {
        int pipeFd[2];
        std::string path = obtainHtmlPath(rq.getPath());
        std::cout << rq.getPath() << std::endl;
        if (!this->fileExits(path))
        {
            // TO DO: Later mark this as a 404 response
            std::cerr << "Failed to open html" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        this->_socketFd = eventList.data.fd;
        if (pipe(pipeFd) == -1)
        {
            close(epollFd);
            Logger::errorLog(errno, strerror, false);
            throw HtmlFile::HtmlFileException();
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            close(epollFd);
            close(pipeFd[PIPE_READ]);
            close(pipeFd[PIPE_WRITE]);
            Logger::errorLog(errno, strerror, false);
            throw HtmlFile::HtmlFileException();
        }
        else if (pid == 0)
            this->execPy(pipeFd, path);//CHANGED
        if (close(pipeFd[PIPE_WRITE]) == -1)
        {
            close(epollFd);
            close(pipeFd[PIPE_READ]);
            Logger::errorLog(errno, strerror, false);
            throw HtmlFile::HtmlFileException();
        }
        if (!AuxFunc::handle_ctl(epollFd, EPOLL_CTL_ADD, EPOLLIN, pipeFd[PIPE_READ], eventConf))
            throw HtmlFile::HtmlFileException();
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
    /*-----------------------CHANGED-------------------------*/
    void HtmlFile::execPy(int pipeFd[2], std::string &path)
    {
        char *args[] = {
            (char *)"python3",
            (char *)"./cgi/readHtml.py",
            (char *) path.c_str(),
            NULL,
        };
        if (close(pipeFd[PIPE_READ]) == -1)
        {
            close(pipeFd[PIPE_WRITE]);
            Logger::errorLog(errno, strerror, true);
        }
        if (dup2(pipeFd[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            close(pipeFd[PIPE_READ]);
            close(pipeFd[PIPE_WRITE]);
            Logger::errorLog(errno, strerror, true);
        }
        if (close(pipeFd[PIPE_WRITE]))
        {
            close(pipeFd[PIPE_READ]);
            Logger::errorLog(errno, strerror, true);
        }
        execve((const char *)"/usr/bin/python3", args, NULL);
        exit(EXIT_FAILURE);
    }
    /*---------------------------------------------------------*/
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
        if (buffer == NULL)
            throw HtmlFile::HtmlFileException();
        this->_content = buffer;
    }

    int HtmlFile::getSocketFd(void) const
    {
        return (this->_socketFd);
    }

    const char *HtmlFile::HtmlFileException::what(void) const throw()
    {
        return ("HtmlFile class found a problem and the server must stop now");
    }

    HtmlFile::~HtmlFile() {}

}
