/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:09:48 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/07 18:16:09 by juestrel         ###   ########.fr       */
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

namespace Webserv
{
    class HtmlFile
    {
    private:
        int _pipeFd[2];
        void execPy(); // This is a temporal name
        bool fileExits(const std:: string &path) const;

    public:
        HtmlFile();
        HtmlFile(const HtmlFile &copy);
        ~HtmlFile();
        HtmlFile &operator=(const HtmlFile &otro);
        std::string getContent() const;
        long getSize() const;
        int readFile(std::string &filePath, int epollFd, struct epoll_event &eventList);
    };
}

#endif