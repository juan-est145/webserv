/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:22:13 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/27 17:25:14 by juestrel         ###   ########.fr       */
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
    private:
    public:
        ARequest(void);
        ARequest(const ARequest &toCopy);
        ARequest &operator=(const ARequest &toCopy);

        virtual void readReq(const char *buffer, size_t bufSize) = 0;

        ~ARequest();
    };

}

#endif