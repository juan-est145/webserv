/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:09:48 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/11 17:53:10 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTMLFILE_H
#define HTMLFILE_H

#ifndef PIPE_READ
#define PIPE_READ 0
#endif

#ifndef PIPE_WRITE
#define PIPE_WRITE 1
#endif

#include <string>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <exception>
#include "AuxFunc.hpp"
#include "Request.hpp"

namespace Webserv
{
    class HtmlFile
    {
    private:
        int _socketFd;
        long _size;
        const Request rq;
        std::string _content;
        void execPy(int pipeFd[2], std::string &path); // This is a temporal name
        bool fileExits(const std::string &path);

    public:
        HtmlFile();
        HtmlFile(const Request &rq);
        HtmlFile(const HtmlFile &copy);
        ~HtmlFile();
        HtmlFile &operator=(const HtmlFile &otro);
        int obtainFileFd(int epollFd, struct epoll_event &eventList, struct epoll_event &eventConf);
        const std::string &getContent(void) const;
        void setContent(const char *buffer);
        long getSize(void) const;
        int getSocketFd(void) const;
        class HtmlFileException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
    };
}

#endif