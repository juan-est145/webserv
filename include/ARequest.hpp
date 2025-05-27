/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:22:13 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/27 19:06:13 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AREQUEST_HPP
#define AREQUEST_HPP

#include <string>
#include <iostream>

namespace Webserv
{
    class ARequest
    {
    protected:
        int _socketFd;

    public:
        ARequest(void);
        ARequest(int socketFd);
        ARequest(const ARequest &toCopy);
        ARequest &operator=(const ARequest &toCopy);

        virtual void readReq(const char *buffer, size_t bufSize) = 0;

        int getSocketFd(void) const;
        ~ARequest();
    };

}

#endif