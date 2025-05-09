/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:30:15 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/04 20:23:03 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HtmlFile.hpp"

namespace Webserv
{
    HtmlFile::HtmlFile(): rq(Request())
    {
        
        this->_socketFd = -1;
        this->_content = -1;
    }

    HtmlFile::HtmlFile(const HtmlFile &copy)
    {
        *this = copy;
    }

    HtmlFile::HtmlFile(const Request &rq): rq(rq)
    {
        this->_socketFd = -1;
        this->_content = -1;
    }

    HtmlFile &HtmlFile::operator=(const HtmlFile &other)
    {
        if (this != &other)
        {
            this->_socketFd = other._socketFd;
            this->_content = other._content;
        }
        return *this;
    }

    int HtmlFile::obtainFileFd(int epollFd, struct epoll_event &eventList, struct epoll_event &eventConf)
    {
        int pipeFd[2];
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
            this->execPy(pipeFd, this->rq.getResourceData().path);//CHANGED
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
    
    /*-----------------------CHANGED-------------------------*/
    void HtmlFile::execPy(int pipeFd[2], const std::string &path)
    {
        int result_py;
        char *args[] = {
            (char *)"python3",
            (char *)"./cgi/readHtml.py",
            (char *) path.c_str(),
            NULL,
        };

        //setenv("env_python", "valor", 1); VARIABLE PARA EL ARCHIVO PYTHON
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
        result_py = execve((const char *)"/usr/bin/python3", args, NULL);
        // TO DO: Later erase this.
        std::cout << result_py << std::endl;
        exit(EXIT_FAILURE);
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

    const Request &HtmlFile::getRequest(void) const
    {
        return (this->rq);
    }

    const char *HtmlFile::HtmlFileException::what(void) const throw()
    {
        return ("HtmlFile class found a problem and the server must stop now");
    }

    HtmlFile::~HtmlFile() {}

}
