/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:04:19 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 00:07:49 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ARequest.hpp"

namespace Webserv
{
    ARequest::ARequest(void) 
    {
        this->_socketFd = -1;
    }

    ARequest::ARequest(int socketFd)
    {
        this->_socketFd = socketFd;
    }

    ARequest::ARequest(const ARequest &toCopy)
    {
        *this = toCopy;
    }

    ARequest &ARequest::operator=(const ARequest &toCopy)
    {
        if (this != &toCopy)
            this->_socketFd = toCopy._socketFd;
        return (*this);
    }

    int ARequest::getSocketFd(void) const
    {
        return (this->_socketFd);
    }

    ARequest::~ARequest() {}
}